import itertools
from dataclasses import dataclass, replace
from typing import Sequence, Optional

from yql.essentials.providers.common.proto.gateways_config_pb2 import EGenericDataSourceKind, EGenericProtocol
from ydb.public.api.protos.ydb_value_pb2 import Type

from ydb.library.yql.providers.generic.connector.tests.utils.settings import Settings
from ydb.library.yql.providers.generic.connector.tests.utils.generate import generate_table_data
import ydb.library.yql.providers.generic.connector.tests.utils.types.clickhouse as clickhouse
import ydb.library.yql.providers.generic.connector.tests.utils.types.mysql as mysql
import ydb.library.yql.providers.generic.connector.tests.utils.types.postgresql as postgresql
import ydb.library.yql.providers.generic.connector.tests.utils.types.ydb as Ydb
from ydb.library.yql.providers.generic.connector.tests.utils.schema import (
    Schema,
    Column,
    ColumnList,
    DataSourceType,
    SelectWhat,
    SelectWhere,
    makeYdbTypeFromTypeID,
)

from ydb.library.yql.providers.generic.connector.tests.common_test_cases.base import BaseTestCase
from ydb.library.yql.providers.generic.connector.tests.utils.settings import GenericSettings


@dataclass
class TestCase(BaseTestCase):
    schema: Schema
    data_in: Sequence
    select_what: SelectWhat
    select_where: Optional[SelectWhere]
    data_out_: Optional[Sequence]
    check_output_schema: bool = False

    @property
    def data_out(self) -> Sequence:
        return self.data_out_ if self.data_out_ else self.data_in

    @property
    def generic_settings(self) -> GenericSettings:
        gs = super().generic_settings
        for cluster in gs.clickhouse_clusters:
            cluster.protocol = self.protocol
        return gs


class Factory:
    ss: Settings

    def __init__(self, ss: Settings):
        self.ss = ss

    def _column_selection(self) -> Sequence[TestCase]:
        '''
        In these test case set we check SELECT from a small table with various SELECT parameters,
        like `SELECT * FROM table` or `SELECT a, b, c FROM table`.

        It's quite important that the table has at least one column with lowercase name and
        at least one column with uppercase name.
        Within YQL, every arrow block gets one extra column with this name:
        https://a.yandex-team.ru/arcadia/ydb/library/yql/core/yql_expr_type_annotation.h?rev=r11978071#L333
        This extra column may be added into the arbitrary place in the list of columns
        and affect the data layout used by MiniKQL.
        '''

        # only two columns in a table
        schema = Schema(
            columns=ColumnList(
                Column(
                    name='COL1',
                    ydb_type=Type.INT32,
                    data_source_type=DataSourceType(
                        ch=clickhouse.Int32(), pg=postgresql.Int4(), ydb=Ydb.Int32(), my=mysql.Integer()
                    ),
                ),
                Column(
                    name='col2',
                    ydb_type=Type.INT32,
                    data_source_type=DataSourceType(
                        ch=clickhouse.Int32(), pg=postgresql.Int4(), ydb=Ydb.Int32(), my=mysql.Integer()
                    ),
                ),
            )
        )

        # only two rows
        data_in = [[1, 2], [10, 20]]

        # and many ways of performing the SELECT query
        params = (
            # SELECT * FROM table
            (
                SelectWhat.asterisk(column_list=schema.columns),
                data_in,
                (
                    EGenericDataSourceKind.CLICKHOUSE,
                    EGenericDataSourceKind.POSTGRESQL,
                    EGenericDataSourceKind.YDB,
                    EGenericDataSourceKind.MYSQL,
                    EGenericDataSourceKind.MS_SQL_SERVER,
                ),
            ),
            # SELECT COL1 FROM table
            (
                SelectWhat(SelectWhat.Item(name='COL1')),
                [
                    [1],
                    [10],
                ],
                (
                    EGenericDataSourceKind.CLICKHOUSE,
                    # NOTE: YQ-2264: doesn't work for PostgreSQL because of implicit cast to lowercase (COL1 -> col1)
                    EGenericDataSourceKind.YDB,
                    EGenericDataSourceKind.MYSQL,
                    EGenericDataSourceKind.MS_SQL_SERVER,
                ),
            ),
            # SELECT col1 FROM table
            (
                SelectWhat(SelectWhat.Item(name='col1')),
                [
                    [1],
                    [10],
                ],
                (EGenericDataSourceKind.POSTGRESQL,),  # works because of implicit cast to lowercase (COL1 -> col1)
            ),
            # SELECT col2 FROM table
            (
                SelectWhat(SelectWhat.Item(name='col2')),
                [
                    [2],
                    [20],
                ],
                (
                    EGenericDataSourceKind.CLICKHOUSE,
                    EGenericDataSourceKind.POSTGRESQL,
                    EGenericDataSourceKind.YDB,
                    EGenericDataSourceKind.MYSQL,
                    EGenericDataSourceKind.MS_SQL_SERVER,
                ),
            ),
            # SELECT col2, COL1 FROM table
            (
                SelectWhat(SelectWhat.Item(name='col2'), SelectWhat.Item(name='COL1')),
                [
                    [2, 1],
                    [20, 10],
                ],
                (
                    EGenericDataSourceKind.CLICKHOUSE,
                    # NOTE: YQ-2264: doesn't work for PostgreSQL because of implicit cast to lowercase (COL1 -> col1)
                    EGenericDataSourceKind.YDB,
                    EGenericDataSourceKind.MYSQL,
                    EGenericDataSourceKind.MS_SQL_SERVER,
                ),
            ),
            # SELECT col2, col1 FROM table
            (
                SelectWhat(SelectWhat.Item(name='col2'), SelectWhat.Item(name='col1')),
                [
                    [2, 1],
                    [20, 10],
                ],
                (EGenericDataSourceKind.POSTGRESQL,),  # works because of implicit cast to lowercase (COL1 -> col1)
            ),
            # Simple math computation:
            # SELECT COL1 + col2 AS col3 FROM table
            (
                SelectWhat(SelectWhat.Item(name='COL1 + col2', alias='col3')),
                [
                    [3],
                    [30],
                ],
                (
                    EGenericDataSourceKind.CLICKHOUSE,
                    # NOTE: YQ-2264: doesn't work for PostgreSQL because of implicit cast to lowercase (COL1 -> col1)
                    EGenericDataSourceKind.YDB,
                    EGenericDataSourceKind.MYSQL,
                    EGenericDataSourceKind.MS_SQL_SERVER,
                ),
            ),
            # Select the same column multiple times with different aliases
            # SELECT col2 AS A, col2 AS b, col2 AS C, col2 AS d, col2 AS E FROM table
            (
                SelectWhat(
                    SelectWhat.Item(name='col2', alias='A'),
                    SelectWhat.Item(name='col2', alias='b'),
                    SelectWhat.Item(name='col2', alias='C'),
                    SelectWhat.Item(name='col2', alias='d'),
                    SelectWhat.Item(name='col2', alias='E'),
                ),
                [
                    [2, 2, 2, 2, 2],
                    [20, 20, 20, 20, 20],
                ],
                (
                    EGenericDataSourceKind.CLICKHOUSE,
                    EGenericDataSourceKind.POSTGRESQL,
                    EGenericDataSourceKind.YDB,
                    EGenericDataSourceKind.MYSQL,
                    EGenericDataSourceKind.MS_SQL_SERVER,
                ),
            ),
        )

        test_cases = []
        for param in params:
            (select_what, data_out, data_source_kinds) = param

            test_case_name = f'column_selection_{select_what}'

            for data_source_kind in data_source_kinds:
                test_case = TestCase(
                    data_in=data_in,
                    data_source_kind=data_source_kind,
                    protocol=EGenericProtocol.NATIVE,
                    select_what=select_what,
                    select_where=None,
                    schema=schema,
                    data_out_=data_out,
                    name_=test_case_name,
                    pragmas=dict(),
                )

                test_cases.append(test_case)

        return test_cases

    def _large_table(self) -> Sequence[TestCase]:
        '''
        In this test the dataset is obviously larger than a single page
        (a single message of ReadSplits protocol), so it will take at least several protocol messages
        to transfer the table from Connector to the engine (dqrun/kqprun).

        Therefore, we will check:
        1. Connector's data prefetching logic
        2. Engine backpressure logic

        # TODO: assert connector stats when it will be accessible
        '''

        table_size = 2.5 * self.ss.connector.paging_bytes_per_page * self.ss.connector.paging_prefetch_queue_capacity
        # table_size = self.ss.connector.paging_bytes_per_page * self.ss.connector.paging_prefetch_queue_capacity / 1000

        schema = Schema(
            columns=ColumnList(
                Column(
                    name='col_00_int32',
                    ydb_type=makeYdbTypeFromTypeID(Type.INT32),
                    data_source_type=DataSourceType(ch=clickhouse.Int64(), pg=postgresql.Int8()),
                ),
                Column(
                    name='col_01_string',
                    ydb_type=makeYdbTypeFromTypeID(Type.STRING),
                    data_source_type=DataSourceType(ch=clickhouse.String(), pg=postgresql.Text()),
                ),
            )
        )

        data_source_kinds = (
            EGenericDataSourceKind.CLICKHOUSE,
            EGenericDataSourceKind.POSTGRESQL,
        )

        test_case_name = 'large'
        test_cases = []

        for data_source_kind in data_source_kinds:
            match data_source_kind:
                case EGenericDataSourceKind.CLICKHOUSE:
                    tc = TestCase(
                        name_=test_case_name,
                        data_source_kind=data_source_kind,
                        protocol=EGenericProtocol.NATIVE,
                        data_in=None,
                        data_out_=[[999999]],  # We put 1M of rows in the large table
                        select_what=SelectWhat(SelectWhat.Item(name='MAX(col_00_int32)', kind='expr')),
                        select_where=None,
                        schema=schema,
                        pragmas=dict(),
                    )

                case EGenericDataSourceKind.POSTGRESQL:
                    # Assuming that request will look something like:
                    # `SELECT * FROM table WHERE id = (SELECT MAX(id) FROM table)`
                    # We expect last line to be the answer
                    data_in = generate_table_data(schema=schema, bytes_soft_limit=table_size)
                    data_out = [data_in[-1]]
                    data_source_kinds = [EGenericDataSourceKind.CLICKHOUSE, EGenericDataSourceKind.POSTGRESQL]

                    tc = TestCase(
                        name_=test_case_name,
                        data_source_kind=data_source_kind,
                        protocol=EGenericProtocol.NATIVE,
                        data_in=data_in,
                        data_out_=data_out,
                        select_what=SelectWhat.asterisk(schema.columns),
                        select_where=SelectWhere(
                            expression_='col_00_int32 IN (SELECT MAX(col_00_int32) FROM {cluster_name}.{table_name})'
                        ),
                        schema=schema,
                        pragmas=dict(),
                    )

                case _:
                    raise ValueError(f'Unknown data source kind: {data_source_kind}')

            test_cases.append(tc)

        return test_cases

    def make_test_cases(self, data_source_kind: EGenericDataSourceKind) -> Sequence[TestCase]:
        protocols = {
            EGenericDataSourceKind.CLICKHOUSE: [EGenericProtocol.NATIVE, EGenericProtocol.HTTP],
            EGenericDataSourceKind.POSTGRESQL: [EGenericProtocol.NATIVE],
            EGenericDataSourceKind.YDB: [EGenericProtocol.NATIVE],
            EGenericDataSourceKind.MYSQL: [EGenericProtocol.NATIVE],
            EGenericDataSourceKind.MS_SQL_SERVER: [EGenericProtocol.NATIVE],
        }

        base_test_cases = None

        if data_source_kind in [
            EGenericDataSourceKind.YDB,
            EGenericDataSourceKind.MYSQL,
            EGenericDataSourceKind.MS_SQL_SERVER,
        ]:
            base_test_cases = self._column_selection()
        elif data_source_kind in [EGenericDataSourceKind.CLICKHOUSE, EGenericDataSourceKind.POSTGRESQL]:
            base_test_cases = list(
                itertools.chain(
                    self._column_selection(),
                    self._large_table(),
                )
            )
        elif data_source_kind == EGenericDataSourceKind.ORACLE:
            raise 'Common test cases are not supported by Oracle due to the lack of Int32 columns'
        else:
            raise f'Unexpected data source kind: {data_source_kind}'

        test_cases = []
        for base_tc in base_test_cases:
            if base_tc.data_source_kind != data_source_kind:
                continue
            for protocol in protocols[base_tc.data_source_kind]:
                tc = replace(base_tc)
                tc.protocol = protocol
                test_cases.append(tc)

        return test_cases
