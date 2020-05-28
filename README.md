

# Welcome to the Hyrax sql_handler
This handler reads data from a database suing an ODBC driver. There are many
different database tools that support access using ODBC.

## Additional Required Software
In addition to the Hyrax server, you will need a database that can be accessed using
ODBC and an ODBC driver for that database that is compatible with the unixODBC 
software package. This includes most Relational Database Management Systems (RDBMS)
as well as many other interesting tools, including NoSQL databases like MongoDB.

If your computer does not have the unixODBC driver software installed, see 
'Installing ODBC on your Hyrax server' below. To build the Hyrax sql handler, you
only need the generic components of unixODBC to be installed. However, to actually
serve data from a database, you will need the appropriate (database specific) driver.

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
**configure or make doesn't work**
* This will happen if unixODBC is not installed. Only the generic part of unixODBC 
is needed to configure and build the handler. We suggest you use a binary package
from yum, apt-get, or brew (for CentOS, Ubuntu or OSX, respectively).
**make check doesn't work**
* In order to run `make check` you will need to install the test database located 
in `data` 
  * Install DBMS as needed (See _Installing DBMS software on your server_ below).
  * Load database tables in data into DBMS? (See _Creating the test database_ below)
  * **_how to configure odbc.ini etc._**

## Installing ODBC on your Hyrax server.
The `sql_handler` (and by extension Hyrax) uses 
[Open Database Connectivity (ODBC)](https://en.wikipedia.org/wiki/Open_Database_Connectivity) 
drivers to access various DBMS systems. Drivers will need to be installed on the Hyrax host system. 

**[unixODBC](http://www.unixodbc.org)** - This open source ODBC driver is available:
  - As a source distribution (utilizes gnu autotools to build)
  - In the `yum` inventory for **CentOS-6** and  **CentOS-7** and 
  - In the `homebrew` inventory for **OSX**
  
Some DBMS producers provide their own ODBC drivers. _See the DBMS sections below._

## Installing DBMS software on your server.
In order to serve data from a database you need to have a database running to
hold the data and respond to queries. If you haven't got that sorted already, 
here's some info about how to go about it. In the following, we describe how to
configure sqlite for use with ODBC drivers. This is overkill in many ways, but 
it is an almost universal example. In the appendix we include information about
PostGreSQL and MySQL.

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
- **[Latest Binaries](https://www.sqlite.org/download.html)** 
- Also available via: **Homebrew** 
- Use `brew install sqlite` but note that OSX already has sqlite3, which is an older 
version of the code, so you need to modify the PATH environment variable like this:
`export PATH="/usr/local/opt/sqlite/bin:$PATH"` and probably put that into `.bashrc`
as well.
- Install the sqliteodbc drivers using `brew install sqliteodbc`


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


## Configuration

* The file `sql_handler/data/odbc.TEMPLATES.ini` contains examples of 
 odbc_inst.ini files for various DBMS systems.

* `odbc.ini` - Defines connection options; it creates view into the database
and exposes it via the ODBC interface. Location: `/etc/odbc.ini`

* `odbc_inst.ini` - Defines driver options; it creates an association between 
an odbc driver and a dbms. Location: `/etc/odbc_inst.ini`

## Notes

Mac OSX package installers:
- **Homebrew** (_Generally plays best with our production rules/ build system._)
- **MacPorts** (_ymmv_)
- **Fink** (_Please don't do this if you are building our software from source, 
it will make you sad._)

## Examples

These examples include building a database that can be used with the tests. 
Following one of these will result in a configuration such that `make check`
should work.

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
  1: BESCMD postgresql.das.bescmd                       ok
  2: BESCMD postgresql.dds.bescmd                       ok
  3: BESCMD postgresql.ddx.bescmd                       ok

## ------------- ##
## Test results. ##
## ------------- ##

All 3 tests were successful.
```
### MySQL Ver 8.0.17 for Linux on x86_64 (MySQL Community Server - GPL) 
_Installed on CentOS-7 latest running in an AWS EC2 instance._
> NOTE: I disabled SELinux on this system to simplify my installation/configuration experience.

#### Installation
There are a bewildering number of ways to install MySQL on Linux. However, the most obvious one, 
**yum**, is not by default connected to the MySQL repository. The 
[MySQL Linux Installation Guide](https://dev.mysql.com/doc/refman/8.0/en/linux-installation.html)
directs one to connect the MySQL repo to your system. IN a case of misunderstanding I downloaded
the _Red Hat Enterprise Linux 7 / Oracle Linux 7 (x86, 64-bit), RPM Bundle_ from the **Red Hat Enterprise**
section of this page: https://dev.mysql.com/downloads/mysql/  The installation required that _all_
of the included rpms be named on the installation command:

```bash
tar -xvf mysql-8.0.17-1.el7.x86_64.rpm-bundle.tar;
sudo yum install *.rpm; # Note that the tar ball unpacks all the rpms into the CWD
```
This successfully install the software.

#### Start the MySQL daemon, _mysqld_
In order to start the service:

```bash
sudo service start mysqld
```

#### Deal with the mysql _root_ user password
After much trashing about I discover that the "root" password for the MySQL service is randomly 
generated and written to the error log. In order to recover it, perform the following:

```bash
% sudo grep 'temporary password' /var/log/mysqld.log
2019-10-07T19:06:18.810367Z 5 [Note] [MY-010454] [Server] A temporary password is generated for root@localhost: I_AOA_()d7e0
```
Changing the password goes like this:

```bash
% sudo mysql -uroot -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 24
Server version: 8.0.17

Copyright (c) 2000, 2019, Oracle and/or its affiliates. All rights reserved.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql> ALTER USER 'root'@'localhost' IDENTIFIED BY 'Your!Spiffy!New!Password123';
Query OK, 0 rows affected (0.00 sec)

mysql> quit
Bye
```

#### Create and populate the _test_ database
Login as the mysql _root_ user:

```bash
% mysql -uroot -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 26
Server version: 8.0.17 MySQL Community Server - GPL

Copyright (c) 2000, 2019, Oracle and/or its affiliates. All rights reserved.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
```
Create and select the the _test_ database:

```bash
mysql> CREATE DATABASE test;
Query OK, 1 row affected (0.00 sec)
mysql> USE test;
Database changed
```
Create the the table *sqlh_table*:

```bash
mysql> CREATE TABLE sqlh_table 
    -> (a integer NOT NULL DEFAULT 0,
    -> b real NOT NULL DEFAULT 0,
    -> c varchar(50),
    -> CONSTRAINT pk PRIMARY KEY (a));
Query OK, 0 rows affected (0.02 sec)
```
Populate the table *sqlh_table* with data:

```bash
mysql> INSERT INTO sqlh_table(a, b, c) VALUES ('1', '81.0', 'string_a'), ('2', '61.1', 'string_b'), ('3', '51.0', 'string_c'), ('4', '2100.0', 'string_d'), ('5', '21.0', 'string_e'), ('6', '4133.0', 'string_f'), ('7', '31.4', 'string_g'), ('8', '21.3', 'string_h'), ('9', '11.6', 'string_i'), ('10', '22.2', 'string_j');
Query OK, 10 rows affected (0.01 sec)
Records: 10  Duplicates: 0  Warnings: 0
```

Now we need a test user because doing everything as _root_ is probably ill advised:
From [Creating Accounts](https://dev.mysql.com/doc/refman/8.0/en/creating-accounts.html)

```bash
mysql -u root -p
Enter password: 
  ...
mysql> CREATE USER 'mysql'@'localhost' IDENTIFIED BY 'mysqlUserPasswordHere';
Query OK, 0 rows affected (0.01 sec)

mysql> GRANT SELECT ON *.* TO 'mysql'@'localhost' WITH GRANT OPTION;
Query OK, 0 rows affected (0.00 sec)
```

That should be enough to get the tests working, so quit the MySQL client for now:

```bash
mysql> quit;
```
#### Configure the ODBC faffery
I found ``odbcinst.ini`` like so:

```bash
[root@ip-172-31-9-74 etc]# find / -type f -name odbcinst.ini
/etc/odbcinst.ini
```
And this is its content:

```ini
[PostgreSQL]
Description=ODBC for PostgreSQL
Driver=/usr/lib/psqlodbcw.so
Setup=/usr/lib/libodbcpsqlS.so
Driver64=/usr/lib64/psqlodbcw.so
Setup64=/usr/lib64/libodbcpsqlS.so
FileUsage=1

[MySQL]
Description=ODBC for MySQL
Driver=/usr/lib/libmyodbc5.so
Setup=/usr/lib/libodbcmyS.so
Driver64=/usr/lib64/libmyodbc5.so
Setup64=/usr/lib64/libodbcmyS.so
FileUsage=1

[MySQL ODBC 8.0 Unicode Driver]
Driver=/usr/lib64/libmyodbc8w.so
SETUP=/usr/lib64/libmyodbc8S.so
UsageCount=1

[MySQL ODBC 8.0 ANSI Driver]
Driver=/usr/lib64/libmyodbc8a.so
SETUP=/usr/lib64/libmyodbc8S.so
UsageCount=1
```

So I added odbc.ini in the same spot with the same permissions:

```bash
[root@ip-172-31-9-74 etc]# touch /etc/odbc.ini
[root@ip-172-31-9-74 etc]# chmod 644 !$
chmod 644 /etc/odbc.ini
[root@ip-172-31-9-74 etc]# ls -l /etc/odbc*
-rw-r--r--. 1 root root   0 Oct  8 14:01 /etc/odbc.ini
-rw-r--r--. 1 root root 579 Jun 11 23:20 /etc/odbcinst.ini
```

#### Get and install the MySQL ODBC connector

Retrieve the appropriate **files** from: [MySQL ODBC Connector Downlods](https://dev.mysql.com/downloads/connector/odbc/)

This requires two separete RPM downloads:

- mysql-connector-odbc-8.0.17-1.el7.i686.rpm
- mysql-connector-odbc-setup-8.0.17-1.el7.i686.rpm

This page [MySQL ODC Connector Configuration](https://dev.mysql.com/doc/connector-odbc/en/connector-odbc-configuration-connection-parameters.html)
was of little use, and after a ridculaous amount of additonal thrashing I found that 
the ``odbcinst.ini`` file had to be modified to accomodate the new drivers:

OLD:

```ini
[MySQL]
Description=ODBC for MySQL
Driver=/usr/lib/libmyodbc5.so
Setup=/usr/lib/libodbcmyS.so
Driver64=/usr/lib64/libmyodbc5.so
Setup64=/usr/lib64/libodbcmyS.so
FileUsage=1
```
NEW:

```ini
[MySQL]
Description=ODBC for MySQL
Driver=/usr/lib/libmyodbc8w.so
Setup=/usr/lib/libodbcmyS.so
Driver64=/usr/lib64/libmyodbc8w.so
Setup64=/usr/lib64/libodbcmyS.so
FileUsage=1
```

Next I  edited the ``/etc/odbc.ini`` and added the test database like so:

```ini
[test]
Driver              = MySQL
Description         = LocalMySQL
Trace               = Yes
TraceFile           = sql.log
Database            = test
Servername          = localhost
UserName            = mysql
Password            = *************
Port                = 3306
```

After which I was able to connect to the DB using the  `isql` ODBC application:

Invoke `isql` on the "test" ODBC connection as the user "mysql" with password "passwordy"

```bash
% isql test mysql passwordy
+---------------------------------------+
| Connected!                            |
|                                       |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
|                                       |
+---------------------------------------+
SQL> help sqlh_table
+-----------------------------------------------------------------+-----------------------------------------------------------------+-----------------------------------------------------------------+-----------------------------------------------------------------+----------+---------------------+------------+--------------+---------------+---------------+---------+-----------------------------------------------------------------+-----------------------------------------------------------------+--------------+-----------------+------------------+-----------------+------------+
| TABLE_CAT                                                       | TABLE_SCHEM                                                     | TABLE_NAME                                                      | COLUMN_NAME                                                     | DATA_TYPE| TYPE_NAME           | COLUMN_SIZE| BUFFER_LENGTH| DECIMAL_DIGITS| NUM_PREC_RADIX| NULLABLE| REMARKS                                                         | COLUMN_DEF                                                      | SQL_DATA_TYPE| SQL_DATETIME_SUB| CHAR_OCTET_LENGTH| ORDINAL_POSITION| IS_NULLABLE|
+-----------------------------------------------------------------+-----------------------------------------------------------------+-----------------------------------------------------------------+-----------------------------------------------------------------+----------+---------------------+------------+--------------+---------------+---------------+---------+-----------------------------------------------------------------+-----------------------------------------------------------------+--------------+-----------------+------------------+-----------------+------------+
|                                                                 |                                                                 | sqlh_table                                                      | a                                                               | 4        | integer             | 10         | 4            | 0             | 10            | 0       |                                                                 | 0                                                               | 4            |                 |                  | 1               | NO         |
|                                                                 |                                                                 | sqlh_table                                                      | b                                                               | 8        | double              | 15         | 8            |               |               | 0       |                                                                 | 0                                                               | 8            |                 |                  | 2               | NO         |
|                                                                 |                                                                 | sqlh_table                                                      | c                                                               | -9       | varchar             | 50         | 200          |               |               | 1       |                                                                 |                                                                 | -9           |                 | 200              | 3               | YES        |
+-----------------------------------------------------------------+-----------------------------------------------------------------+-----------------------------------------------------------------+-----------------------------------------------------------------+----------+---------------------+------------+--------------+---------------+---------------+---------+-----------------------------------------------------------------+-----------------------------------------------------------------+--------------+-----------------+------------------+-----------------+------------+
SQLRowCount returns 3
3 rows fetched
SQL> select * from sqlh_table
+-----------+-------------------------+---------------------------------------------------+
| a         | b                       | c                                                 |
+-----------+-------------------------+---------------------------------------------------+
| 1         | 81                      | string_a                                          |
| 2         | 61.1                    | string_b                                          |
| 3         | 51                      | string_c                                          |
| 4         | 2100                    | string_d                                          |
| 5         | 21                      | string_e                                          |
| 6         | 4133                    | string_f                                          |
| 7         | 31.4                    | string_g                                          |
| 8         | 21.3                    | string_h                                          |
| 9         | 11.6                    | string_i                                          |
| 10        | 22.2                    | string_j                                          |
+-----------+-------------------------+---------------------------------------------------+
SQLRowCount returns 10
10 rows fetched
SQL> 
```
woot.

(That was ridculous, unneccessarily complex, and non-intuitive)

AT this point I had to revist the `sql_handler` code because there were issues with authentication.
I re-enabled the authentication (see ODBCConnecor.cc lines 70-94) and added the "data" file data/mysql.sql
With the correct user and passoword installed in the aforementioned file the handler was able to connect 
to the MySQL service.

At this point the code apears to be failing because of an unanticipated data type.

Running any of these commands in the tests directory:

```bash
besstandalone -c bes.conf -d "cerr,all" -i sql/mysql.dds.bescmd
besstandalone -c bes.conf -d "cerr,all" -i sql/mysql.das.bescmd
besstandalone -c bes.conf -d "cerr,all" -i sql/mysql.data.bescmd
```
Will produce this error:

```bash
SQLRequestHandler: Error: 7 Message: SQL Handler: The datatype read from the Data Source is not supported. The problem type code is: -8 File: unixODBC/ODBCTypeFactoryComponent.cc Line: 184
```
Hmmmmm

Also of note is that running besstandalone without the debugging enabled does not produce a meaningful messsage:

```xml
<Message>SQLRequestHandler: Error occurred, bad constraints specified or no active database server for this dataset. Please,try a less restrictive constraint or contact the administrator.</Message>
```
This should be corrected, in addition to correcting the underlying unanticipated data type bug.










