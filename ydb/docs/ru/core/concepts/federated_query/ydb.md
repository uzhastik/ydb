# Работа с базами данных {{ ydb-short-name }}

{{ ydb-full-name }} может выступать в качестве внешнего источника данных для другой базы {{ ydb-full-name }}. В данном разделе рассматривается организация совместной работы двух независимых баз данных {{ ydb-short-name }} в режиме обработки федеративных запросов.

Для подключения к внешней базе {{ ydb-short-name }} со стороны другой базы {{ ydb-short-name }}, выступающей в роли движка обработки федеративных запросов, на последней требуется выполнить следующие шаги:

1. Подготовить аутентификационные данные для доступа к удалённой базе {{ ydb-short-name }}. В настоящее время в федеративных запросах к {{ ydb-short-name }} доступен метод аутентификации по [логину и паролю](../../security/authentication.md#static-credentials) (остальные методы не поддерживаются). Пароль к внешней базе сохраняется в виде [секрета](../datamodel/secrets.md):

   ```yql
    CREATE OBJECT ydb_datasource_user_password (TYPE SECRET) WITH (value = "<password>");
    ```

1. Создать [внешний источник данных](../datamodel/external_data_source.md), описывающий стороннюю базу {{ ydb-short-name }}. Параметр `LOCATION` содержит сетевой адрес экземпляра {{ ydb-short-name }}, к которому осуществляется сетевое подключение. В `DATABASE_NAME` указывается имя базы данных (например, `local`). Для аутентификации во внешнюю базу используются значения параметров `LOGIN` и `PASSWORD_SECRET_NAME`. Включить шифрование соединений к внешней базе данных можно с помощью параметра `USE_TLS="TRUE"`. Если шифрование включено, то в поле `<port>` параметра `LOCATION` необходимо указать порт gRPCs внешней {{ ydb-short-name }}, в противном случае - порт gRPC.

    ```yql
    CREATE EXTERNAL DATA SOURCE ydb_datasource WITH (
        SOURCE_TYPE="Ydb",
        LOCATION="<host>:<port>",
        DATABASE_NAME="<database>",
        AUTH_METHOD="BASIC",
        LOGIN="user",
        PASSWORD_SECRET_NAME="ydb_datasource_user_password",
        USE_TLS="TRUE"
    );
    ```

1. {% include [!](_includes/connector_deployment.md) %}
1. [Выполнить запрос](#query) к внешнему источнику данных.

## Синтаксис запросов {#query}

Для извлечения данных из таблиц внешней базы {{ ydb-short-name }} используется следующая форма SQL-запроса:

```yql
SELECT * FROM ydb_datasource.`<table_name>`
```

где:

- `ydb_datasource` - идентификатор внешнего источника данных;
- `<table_name>` - полное имя таблицы внутри [иерархии](../../concepts/index.html#ydb-hierarchy) каталогов в базе данных {{ ydb-short-name }}, например, `table`, `dir1/table1` или `dir1/dir2/table3`.

В случае, если таблица находится на верхнем уровне иерархии (не принадлежит ни одному из каталогов), допускается не заключать имя таблицы в обратные апострофы "\`":

```yql
SELECT * FROM ydb_datasource.<table_name>
```

## Ограничения

При работе с внешними источниками данных {{ ydb-short-name }} существует ряд ограничений:

1. {% include [!](_includes/supported_requests.md) %}
1. {% include [!](_includes/predicate_pushdown.md) %}

    |Тип данных {{ ydb-short-name }}|
    |----|
    |`Bool`|
    |`Int8`|
    |`Uint8`|
    |`Int16`|
    |`Uint16`|
    |`Int32`|
    |`Uint32`|
    |`Int64`|
    |`Uint64`|
    |`Float`|
    |`Double`|
    |`String`|
    |`Utf8`|


## Поддерживаемые типы данных

При работе с таблицами, размещёнными во внешней базе {{ ydb-short-name }}, пользователям доступен ограниченный набор типов данных. Все остальные типы, за исключением перечисленных ниже, не поддерживаются. В некоторых случаях производится преобразование типов, то есть колонки таблицы из внешней базы {{ ydb-short-name }} меняют свой тип после вычитывания этой таблицы базой {{ ydb-short-name }}, обрабатывающей федеративный запрос.

|Тип данных внешнего источника {{ ydb-short-name }}|Тип данных в федеративной {{ ydb-short-name }}|
|---------|---------|
|`Bool`|`Bool`|
|`Int8`|`Int8`|
|`Int16`|`Int16`|
|`Int32`|`Int32`|
|`Int64`|`Int64`|
|`Uint8`|`Uint8`|
|`Uint16`|`Uint16`|
|`Uint32`|`Uint32`|
|`Uint64`|`Uint64`|
|`Float`|`Float`|
|`Double`|`Double`|
|`String`|`String`|
|`Utf8`|`Utf8`|
|`Date`|`Date`|
|`Datetime`|`Datetime`|
|`Timestamp`|`Timestamp`|
|`Json`|`Json`|
|`JsonDocument`|`Json`|
