# Digi-Journal

## Resource

**Log**

Attributes:

* heading (string)
* rating (integer)
* entry (string)
* date (string)
* place (string)

## Schema

```sql
CREATE TABLE logs (
id INTEGER PRIMARY KEY,
heading TEXT,
date TEXT,
entry TEXT,
rating INTEGER,
place TEXT);
```

## REST Endpoints

Name                    | Method | Path
------------------------|--------|------------------
Retrieve log collection | GET    | /logs
Retrieve log member     | GET    | /logs/*\<id\>*
Create log member       | POST   | /logs
Update log member       | PUT    | /logs/*\<id\>*
Delete log member       | DELETE | /logs/*\<id\>*