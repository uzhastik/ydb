LIBRARY()

SRCS(
    modification.cpp
)

PEERDIR(
    ydb/core/tx/columnshard/data_sharing/initiator/controller
    ydb/core/tx/columnshard/data_sharing/common/session
    ydb/core/tx/columnshard/tablet
    ydb/core/tx/columnshard/data_sharing/destination/transactions
)

END()
