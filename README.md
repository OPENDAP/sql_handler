

# Welcome to the Hyrax sql_handler
This handler makes the happytimes with the RDBMS data. w00t.

## Additional Required Software
In addition to the Hyrax server, you will need a database that can be accessed using
ODBC and an ODBC driver for that database that is compatible with the unixODBC 
software package. This includes most Relational Database Management Systems (RDBMS)
as well as many other interesting tools, including NoSQL databases like MongoDB.

If your computer does not have the unixODBC driver software installed, see 
'Installing ODBC on your Hyrax server' below. To build the Hyrax sql handler, you
only need the generic components of unixODBC to be installed. However, to actually
serve data from a database, you will need the appropriate (specific) driver.

## Building and Installing the sql_handler
Unlike most of the BES modules, this handler for SQL data is a separate project.
It can be added to the bes/modules directory or built outside of the bes software.

To build the code:

* `autoreconf -fiv`
* `./configure --prefix=$prefix`
* `make`
* `make check` (See more below)
* `make install`

_Build Notes_
* The `./configure` script must find both libdap and the BES. It will use the 
pkgconfig package manager and, if that fails, look for dap-config and bes-config
on the PATH.
* **configure or make doesn't work**
* This will happen if unixODBC is not installed. Only the generic part of unixODBC 
is needed to configure and build the handler. We suggest you use a binary package
from yum, apt-get, or brew.
* **make check doesn't work**
* In order to run `make check` you will need to install the test database located 
in `install.dir` 
  * Install DBMS as needed (See _Installing DBMS software on your server_ below).
  * How to load database tables in install.dir into DBMS? (See _Creating the test database_ below)
  * **_how to configure odbc.ini etc._**

## Installing ODBC on your Hyrax server.
The `sql_handler` (and by extension Hyrax) utilize 
[Open Database Connectivity (ODBC)](https://en.wikipedia.org/wiki/Open_Database_Connectivity) 
to access various DBMS systems. Drivers will need to be installed on the Hyrax host system. 

**[unixODBC](http://www.unixodbc.org)** - This open source ODBC driver is available:
  - As a source distribution (utilizes gnu autotools to build)
  - In the `yum` inventory for **CentOS-6** and  **CentOS-7** and 
  - In the `homebrew` inventory for **OSX**
  
Some DBMS producers provide their own ODBC drivers. _See the DBMS sections below._

## Installing DBMS software on your server.
In order to serve data from a database you need to have a database running to
hold the data and respond to queries. If you haven't got that sorted already, 
here's some info about how to go about it.

## [PostGreSQL](https://www.enterprisedb.com)

**Linux** 
- **[All Versions](https://www.enterprisedb.com/downloads/postgres-postgresql-downloads)** 
- Also available via **yum** (_Current available version on AWS CentOS-7 is 9.2.24_)

**OSX** 
- **[All Versions](https://www.enterprisedb.com/downloads/postgres-postgresql-downloads)**  (Package Installer)
- Also available via: **Homebrew** (Currently 11.2)
- There is also a **[PostGres Menubar App for OSX](https://postgresapp.com)** that creates menubar controls for the 
PostGres DBMS (conflicts with the package installer version of the DB)

**[PostGreSQL ODBC Implementation](https://odbc.postgresql.org)** - [All Versions](https://www.postgresql.org/ftp/odbc/versions/) 
_Caveat Emptor_: It would appear that this archive contains Windows binaries and source code for the rest (Linuix etc.).

## [MySQL](https://dev.mysql.com)

**Linux**

Available as a generic binary download.
* **[Version 5.7](https://dev.mysql.com/doc/refman/5.7/en/binary-installation.html)**
* **[Version 8.0](https://dev.mysql.com/doc/refman/8.0/en/binary-installation.html)**

Not currently available via **yum** on AWS CentOS-7

**OSX**

Available as either as either a package installer or tar archive download.
* **[Version 5.7](https://dev.mysql.com/doc/refman/5.7/en/osx-installation.html)**
* **[Version 8.0](https://dev.mysql.com/doc/refman/8.0/en/osx-installation.html)**

Also available via: **Homebrew** (Currently: 8.0.15)

## [SQLite](https://www.sqlite.org)
The SQLite binaries are described as: 
> A bundle of command-line tools for managing SQLite database files, including 
the command-line shell program, the sqldiff program, and the sqlite3_analyzer 
program.

**Linux** 
- **[Latest Binaries](https://www.sqlite.org/download.html)** (Current version 
3.28.0)
- Also available via: **yum** (_Current available version on CentOS-7 is 3.7.17_)

**OSX** 
- **[Latest Binaries](https://www.sqlite.org/download.html)** (Current version 
3.28.0)
- Also available via: **Homebrew** (Currently  3.27.2)

## Configuration

* The file `sql_handler/install.dir/odbc.TEMPLATES.ini` contains examples of 
 odbc_inst.ini files for various DBMS systems.

* `odbc.ini` - Defines connection options; it creates view into the database
and exposes it via the ODBC interface. Location: `/etc/odbc.ini`

* `odbc_inst.ini` - Defines driver options; it creates a associate between 
an odbc driver and a dbms. Location: `/etc/odbc_inst.ini`

## Notes

Mac OSX package installers:
- **Homebrew** (_Generally plays best with our production rules/ build system._)
- **MacPorts** (_ymmv_)
- **Fink** (_Please don't do this if you are building our software from source, 
it will make you sad._)

## Process

### PostGreSql 11.4 on OSX 10.13.6

- Installed using [the OSX package installer located here](https://www.enterprisedb.com/downloads/postgres-postgresql-downloads)
- If installing using homebrew on OSX, the rest of the Postgresql instructions worked _but_ 
manually adding the 'postgres' user was needed. To do this use the command:

```shell script
/usr/local/opt/postgres/bin/createuser -s postgres
```

WARNING: The [PostGres Menubar App for OSX](https://postgresapp.com) will not work with the software installed by the package installer. 

#### Creating the test database
- Created `test` database using the **pgAdmin** application. (or ``pg_ctl -D /usr/local/var/postgres start`` and then
``createdb test`` for homebrew installs)
- Opened the SQL Shell (`psql`) application and connected to the `test` database.
- Using the cut buffer I copied all but the first line (which selects which database to connect to, and accomplished 
in the previous step) from the `sglh.sql` file, and pasted those lines into the `psql` shell. This successfully created
the table and values for the tests.

```sql
CREATE TABLE sqlh_table 
(a integer NOT NULL DEFAULT 0,
b real NOT NULL DEFAULT 0,
c varchar(50),
CONSTRAINT pk PRIMARY KEY (a));

INSERT INTO sqlh_table(a, b, c) VALUES ('1', '81.0', 'string_a'), ('2', '61.1', 'string_b'), ('3', '51.0', 'string_c'), ('4', '2100.0', 'string_d'), ('5', '21.0', 'string_e'), ('6', '4133.0', 'string_f'), ('7', '31.4', 'string_g'), ('8', '21.3', 'string_h'), ('9', '11.6', 'string_i'), ('10', '22.2', 'string_j');
```

#### Setting up ODBC

##### Installed [unixODBC](http://www.unixodbc.org)
I used Homebrew to install unixODBC:
- `brew install unixodbc` 

##### Installed the PostgreSQL ODBC driver.
- Downloaded the [PostGreSQL ODBC Implementation](https://odbc.postgresql.org) source bundle.
- Followed the instructions [here](https://odbc.postgresql.org/docs/unix-compilation.html)

```shell
./configure
make
make install
```
##### Configuration
The ODBC configuration is held in two files:
- `/usr/local/etc/odbcinst.ini` which defines the dbms connection
- `/usr/local/etc/odbc.ini` which defines the view of a database to the system.

Example `/usr/local/etc/odbcinst.ini`:

```sql
[PostgreSQL]
Description=PostgreSQL ODBC driver
Driver=/usr/local/lib/psqlodbca.so
debug=0
Threading=2
UsageCount=1
```
Example `/usr/local/etc/odbc.ini`:

```sql
[test]
Driver              = PostgreSQL
Description         = LocalPostgres
Trace               = Yes
TraceFile           = sql.log
Database            = test
Servername          = localhost
UserName            = postgres
Password            = foobar
Port                = 5432
Protocol            = 8.4
ReadOnly            = No
RowVersioning       = No
ShowSystemTables    = No
ShowOidColumn       = No
FakeOidIndex        = No
ConnSettings        =
```

##### Test
At this point I was able to get the `isql` application to run and return the correct data:

```bash
[-bash: ~/OPeNDAP/hyrax/sql_handler] isql -v test
+---------------------------------------+
| Connected!                            |
|                                       |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
|                                       |
+---------------------------------------+
SQL> SELECT a,b,c FROM  sqlh_table;
+------------+----------------+---------------------------------------------------+
| a          | b              | c                                                 |
+------------+----------------+---------------------------------------------------+
| 1          | 81             | string_a                                          |
| 2          | 61.099998      | string_b                                          |
| 3          | 51             | string_c                                          |
| 4          | 2100           | string_d                                          |
| 5          | 21             | string_e                                          |
| 6          | 4133           | string_f                                          |
| 7          | 31.4           | string_g                                          |
| 8          | 21.299999      | string_h                                          |
| 9          | 11.6           | string_i                                          |
| 10         | 22.200001      | string_j                                          |
+------------+----------------+---------------------------------------------------+
SQLRowCount returns 10
10 rows fetched
SQL> 
```
NOTE: One can fiddle with the `odbc.ini` file and see the effects. For example, 
changing the name of the Datassource from `[test]` to `[foo]` should produce the following results:

```bash 
[-bash: ~/OPeNDAP/hyrax/sql_handler] isql -v test
[IM002][unixODBC][Driver Manager]Data source name not found and no default driver specified
[ISQL]ERROR: Could not SQLConnect
```

and

```bash
[-bash: ~/OPeNDAP/hyrax/sql_handler] isql -v foo
+---------------------------------------+
| Connected!                            |
|                                       |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
|                                       |
+---------------------------------------+
SQL> SELECT a,b,c FROM  sqlh_table;
+------------+----------------+---------------------------------------------------+
| a          | b              | c                                                 |
+------------+----------------+---------------------------------------------------+
| 1          | 81             | string_a                                          |
| 2          | 61.099998      | string_b                                          |
| 3          | 51             | string_c                                          |
| 4          | 2100           | string_d                                          |
| 5          | 21             | string_e                                          |
| 6          | 4133           | string_f                                          |
| 7          | 31.4           | string_g                                          |
| 8          | 21.299999      | string_h                                          |
| 9          | 11.6           | string_i                                          |
| 10         | 22.200001      | string_j                                          |
+------------+----------------+---------------------------------------------------+
SQLRowCount returns 10
10 rows fetched
SQL> 
```

After changing the Datasource source BACK to `[test]` I saw that at this point the 
first three tests began to return valid content so I made baselines:

```bash
[-bash: ~/OPeNDAP/hyrax/sql_handler/tests] make chec
[-bash: ~/OPeNDAP/hyrax/sql_handler/tests] ./testsuite 1 2 3
## ----------------------------------------------------------- ##
## sql_module 1.0.0 test suite: bes.conf besstandalone getdap. ##
## ----------------------------------------------------------- ##
  1: BESCMD mysql_1.das.bescmd                       ok
  2: BESCMD mysql_1.dds.bescmd                       ok
  3: BESCMD mysql_1.ddx.bescmd                       ok

## ------------- ##
## Test results. ##
## ------------- ##

All 3 tests were successful.
```

