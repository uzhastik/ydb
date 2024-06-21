# Перезапуск кластеров {{ ydb-short-name }}, развёрнутых с помощью Ansible

Кластеры {{ ydb-short-name }} обеспечивают высокие гарантии доступности; поэтому необходимо учитывать модель отказоустойчивости кластера во время любого технического обслуживания, включая перезапуск кластера. Существует два типа узлов, которые могут потребовать перезапуска:

* Database узлы (также известные как динамические) не сохраняют состояние; поэтому основное внимание стоит уделять тому, чтобы их было достаточно для обработки нагрузки каждой базы данных. Обычно для динамических узлов достаточно простого поэтапного перезапуска с небольшими задержками.
* Storage узлы (также известные как статические) имеют персистентное состояние и отвечают за безопасное хранение данных. Поэтому они требуют особого обращения для обеспечения доступности данных. Каждый кластер {{ ydb-short-name }} имеет выделенный компонент, который отслеживает все сбои и обслуживание и может определить, безопасно ли сейчас остановить или перезапустить конкретный узел. Поэтому важно запрашивать у него разрешение на каждую операцию, а полный перезапуск узлов хранения часто занимает некоторое время.

## Перезапуск через Ansible playbook

Репозиторий [ydb-ansible](https://github.com/ydb-platform/ydb-ansible) содержит playbook под названием `ydb_platform.ydb.restart`, который можно использовать для перезапуска кластера {{ ydb-short-name }}. Запускайте его из той же директории, которая использовалась для [изначального развёртывания](initial-deployment.md).

### Перезапуск всех узлов

По умолчанию `ydb_platform.ydb.restart` перезапускает все узлы кластера. Сначала идут статические узлы, затем динамические. Команда для его запуска:

```bash
ansible-playbook ydb_platform.ydb.restart
```

### Фильтр по типу узлов

Задачи в playbook `ydb_platform.ydb.restart` помечены типами узлов, поэтому вы можете использовать функциональность тегов Ansible для фильтрации узлов по их типу.

Эти две команды эквивалентны и перезапустят все узлы хранения:

```bash
ansible-playbook ydb_platform.ydb.restart --tags storage
ansible-playbook ydb_platform.ydb.restart --tags static
```

Эти две команды эквивалентны и перезапустят все узлы баз данных:
```bash
ansible-playbook ydb_platform.ydb.restart --tags database
ansible-playbook ydb_platform.ydb.restart --tags dynamic
```

### Фильтр по имени хоста

Чтобы перезапустить конкретный хост или подмножество хостов, используйте аргумент `--limit`:

```bash
ansible-playbook ydb_platform.ydb.restart --limit='<hostname>'
ansible-playbook ydb_platform.ydb.restart --limit='<hostname-1,hostname-2>'
```

Его можно использовать и вместе с тегами:
```bash
ansible-playbook ydb_platform.ydb.restart --tags database --limit='<hostname>'
```

## Перезапуск узлов вручную

Инструмент [ydbops](https://github.com/ydb-platform/ydbops) реализует различные манипуляции с кластерами {{ ydb-short-name }}, включая перезапуск. Описанный выше playbook `ydb_platform.ydb.restart` использует его за сценой, но использование его вручную тоже допустимо.

Больше рекомендаций и информации о том, как это работает, находится в статье [{#T}](../manual/maintenance-without-downtime.md).