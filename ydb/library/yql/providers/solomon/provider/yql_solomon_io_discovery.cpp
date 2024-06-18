#include "yql_solomon_provider_impl.h"

#include <ydb/library/yql/providers/common/provider/yql_provider.h>
#include <ydb/library/yql/providers/solomon/expr_nodes/yql_solomon_expr_nodes.h>
#include <ydb/library/yql/providers/solomon/scheme/yql_solomon_scheme.h>

#include <util/string/split.h>
#include <util/string/strip.h>

namespace NYql {

namespace {
using namespace NNodes;

std::array<TExprNode::TPtr, 2U> ExtractSchema(TExprNode::TListType& settings) {
    for (auto it = settings.cbegin(); settings.cend() != it; ++it) {
        if (const auto item = *it; item->Head().IsAtom("userschema")) {
            settings.erase(it);
            return {item->ChildPtr(1), item->ChildrenSize() > 2 ? item->TailPtr() : TExprNode::TPtr()};
        }
    }

    return {};
}

}

class TSolomonIODiscoveryTransformer : public TSyncTransformerBase {
public:
    TSolomonIODiscoveryTransformer(TSolomonState::TPtr state)
        : State_(state)
    {
    }

    TStatus DoTransform(TExprNode::TPtr input, TExprNode::TPtr& output, TExprContext& ctx) override {
        output = input;

        if (ctx.Step.IsDone(ctx.Step.DiscoveryIO)) {
            return TStatus::Ok;
        }

        auto status = OptimizeExpr(input, output, [this] (const TExprNode::TPtr& node, TExprContext& ctx) -> TExprNode::TPtr {
            if (auto maybeWrite = TMaybeNode<TSoWrite>(node)) {
                if (!maybeWrite.DataSink()) {
                    return node;
                }
                auto write = maybeWrite.Cast();

                if (!EnsureArgsCount(write.Ref(), 5, ctx)) {
                    return {};
                }

                return Build<TSoWrite>(ctx, write.Pos())
                    .World(write.World())
                    .DataSink(write.DataSink())
                    .FreeArgs()
                        .Add<TCoAtom>()
                            .Value("")
                        .Build()
                        .Add(write.Arg(3))
                    .Build()
                    .Done().Ptr();
            }

            if (auto maybeRead = TMaybeNode<TSoRead>(node)) {
                auto read = maybeRead.Cast();
                if (read.DataSource().Category().Value() != SolomonProviderName) {
                    return node;
                }

                const auto& object = read.Arg(2).Ref();
                YQL_ENSURE(object.IsCallable("MrTableConcat"));

                auto settings = read.Ref().Child(4);
                auto settingsList = read.Ref().Child(4)->ChildrenList();
                auto userSchema = ExtractSchema(settingsList);

                auto soObject = Build<TSoObject>(ctx, read.Pos())
                                  .Settings(settings)
                                .Done();

                return userSchema.back()
                    ? Build<TSoReadObject>(ctx, read.Pos())
                        .World(read.World())
                        .DataSource(read.DataSource())
                        .RowType(std::move(userSchema.front()))
                        .Object(soObject)
                        .ColumnOrder(std::move(userSchema.back()))
                      .Done().Ptr()
                    : Build<TSoReadObject>(ctx, read.Pos())
                        .World(read.World())
                        .DataSource(read.DataSource())
                        .RowType(std::move(userSchema.front()))
                        .Object(soObject)
                      .Done().Ptr();
            }

            return node;
        }, ctx, TOptimizeExprSettings {nullptr});

        return status;
    }

    void Rewind() final {
    }

private:
    TSolomonState::TPtr State_;
};

THolder<IGraphTransformer> CreateSolomonIODiscoveryTransformer(TSolomonState::TPtr state) {
    return THolder(new TSolomonIODiscoveryTransformer(state));
}

} // namespace NYql
