#include "skeleton_block_and_get.h"
#include "blobstorage_skeletonerr.h"

#include <ydb/core/blobstorage/vdisk/common/vdisk_events.h>
#include <ydb/core/blobstorage/vdisk/common/vdisk_response.h>

#include <ydb/library/actors/core/actor.h>
#include <ydb/library/actors/core/actor_bootstrapped.h>

#include <memory>

namespace NKikimr {

class TBlockAndGetActor : public TActorBootstrapped<TBlockAndGetActor> {
public:
    TBlockAndGetActor() = delete;
    explicit TBlockAndGetActor(
        TEvBlobStorage::TEvVGet::TPtr ev,
        NActors::TActorId skeletonId,
        TIntrusivePtr<TVDiskContext> vCtx,
        TActorIDPtr skeletonFrontIDPtr,
        TVDiskID selfVDiskId,
        TVDiskIncarnationGuid vDiskIncarnationGuid,
        TIntrusivePtr<NKikimr::TBlobStorageGroupInfo> gInfo)
        : SenderId(ev->Sender)
        , SkeletonId(skeletonId)
        , VCtx(vCtx)
        , SkeletonFrontIDPtr(skeletonFrontIDPtr)
        , SelfVDiskId(selfVDiskId)
        , VDiskIncarnationGuid(vDiskIncarnationGuid)
        , GInfo(gInfo)
    {
        Y_VERIFY_S(ev->Get()->Record.HasForceBlockTabletData(), VCtx->VDiskLogPrefix);
        Y_VERIFY_S(ev->Get()->Record.GetForceBlockTabletData().HasId(), VCtx->VDiskLogPrefix);
        Y_VERIFY_S(ev->Get()->Record.GetForceBlockTabletData().HasGeneration(), VCtx->VDiskLogPrefix);
        Request = std::move(ev);
    }

    void Bootstrap() {
        // create TEvVBlock request
        const TInstant deadline = Request->Get()->Record.GetMsgQoS().HasDeadlineSeconds()
            ? TInstant::Seconds(Request->Get()->Record.GetMsgQoS().GetDeadlineSeconds())
            : TInstant::Max();

        auto request = std::make_unique<TEvBlobStorage::TEvVBlock>(
            Request->Get()->Record.GetForceBlockTabletData().GetId(),
            Request->Get()->Record.GetForceBlockTabletData().GetGeneration(),
            VDiskIDFromVDiskID(Request->Get()->Record.GetVDiskID()),
            deadline
        );

        // send TEvVBlock request
        Send(SkeletonId, request.release());

        Become(&TThis::StateWait);
    }

    STRICT_STFUNC(StateWait,
        hFunc(TEvBlobStorage::TEvVBlockResult, Handle);
        cFunc(TEvents::TSystem::Poison, PassAway);
    )

    void Handle(TEvBlobStorage::TEvVBlockResult::TPtr &ev) {
        switch (ev->Get()->Record.GetStatus()) {
            case NKikimrProto::OK:
            case NKikimrProto::ALREADY:
                break;
            default: {
                // we failed to block required generation, so return failure
                auto response = NErrBuilder::ErroneousResult(
                    VCtx,
                    // return BLOCKED so that dsproxy returns right away and doesn't try remaining vdisks
                    NKikimrProto::BLOCKED,
                    "failed to block required generation",
                    Request,
                    TAppData::TimeProvider->Now(),
                    SkeletonFrontIDPtr,
                    SelfVDiskId,
                    VDiskIncarnationGuid,
                    GInfo
                );
                SendVDiskResponse(TActivationContext::AsActorContext(), SenderId, response.release(), Request->Cookie, VCtx, {});
                return PassAway();
            }
        }

        // send TEvVGet request, the reply will go directly to sender.
        // clear ForceBlockTabletData to make sure we never ever have a cycle.
        Request->Get()->Record.ClearForceBlockTabletData();
        TActivationContext::Send(Request.Release());
        PassAway();
    }

private:
    NActors::TActorId SenderId;
    NActors::TActorId SkeletonId;
    TIntrusivePtr<TVDiskContext> VCtx;
    TActorIDPtr SkeletonFrontIDPtr;
    TVDiskID SelfVDiskId;
    TVDiskIncarnationGuid VDiskIncarnationGuid;
    TIntrusivePtr<NKikimr::TBlobStorageGroupInfo> GInfo;
    TEvBlobStorage::TEvVGet::TPtr Request;
};

std::unique_ptr<NActors::IActor> CreateBlockAndGetActor(
    TEvBlobStorage::TEvVGet::TPtr ev,
    NActors::TActorId skeletonId,
    TIntrusivePtr<TVDiskContext> vCtx,
    TActorIDPtr skeletonFrontIDPtr,
    TVDiskID selfVDiskId,
    TVDiskIncarnationGuid vDiskIncarnationGuid,
    TIntrusivePtr<NKikimr::TBlobStorageGroupInfo> gInfo)
{
    return std::make_unique<TBlockAndGetActor>(ev, skeletonId, vCtx, skeletonFrontIDPtr, selfVDiskId, vDiskIncarnationGuid, gInfo);
}

} // NKikimr
