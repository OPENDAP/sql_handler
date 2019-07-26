

# Welcome to the Hyrax sql_handler
This handler makes the happytimes with the RDBMS data. w00t.

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
* **make check doesn't work**
* In order to run `make check` you will need to install the test database located 
in `install.dir` 
  * Install DBMS as needed (See _Installing DBMS software on your server_ below
  ).
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
In order to server data from a database you need to have a database running to
hold the data and respond to queries. If you haven't got that sorted already, 
here's some info about how to go about it.



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

## [PostGreSQL](https://www.enterprisedb.com)

**Linux** 
- **[All Versions](https://www.enterprisedb.com/downloads/postgres-postgresql-downloads)** 
- Also available via **yum** (_Current available version on AWS CentOS-7 is 9.2.24_)

**OSX** 
- **[All Versions](https://www.enterprisedb.com/downloads/postgres-postgresql-downloads)**  (Package Installer)
- Also available via: **Homebrew** (Currently 11.2)
- There is also a **[PostGres Menubar App for OSX](https://postgresapp.com)** that creates menubar controls for the PostGres DBMS

**[PostGreSQL ODBC Implementation](https://odbc.postgresql.org)** - [All Versions](https://www.postgresql.org/ftp/odbc/versions/) _Caveat Emptor_: It would appear that this archive contains Windows binaries and source code for the rest (Linuix etc.).


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

* `odbc.ini` - Creates view into the database and exposes it via the ODBC 
interface. Location: `/etc/odbc.ini`

* `odbc_inst.ini` - Creates a associate between an odbc driver and a dbms. 
Location: `/etc/odbc_inst.ini`

## Notes

Mac OSX package installers:
- **Homebrew** (_Generally plays best with our production rules/ build system._)
- **MacPorts** (_ymmv_)
- **Fink** (_Please don't do this if you are building our software from source, 
it will make you sad._)


## Process

### PostGreSql 11.4 on OSX 10.13.6

- Installed using the OSX package installer.

#### Creating the test database
- Created `test` database using the **pgAdmin** application.
- Opened the SQL Shell (`psql`) application and connected to the `test` database.
- Using the cut buffer I copied all but the first line (which selects which database to connect to, and accomplished in the previous step) from the `sglh.sql` file, and pasted those lines into the `psql` shell. This successfully created the table and values for the tests.

```sql
CREATE TABLE sqlh_table 
(a integer NOT NULL DEFAULT 0,
b real NOT NULL DEFAULT 0,
c varchar(50),
CONSTRAINT pk PRIMARY KEY (a));

INSERT INTO sqlh_table(a, b, c) VALUES ('1', '81.0', 'string_a'), ('2', '61.1', 'string_b'), ('3', '51.0', 'string_c'), ('4', '2100.0', 'string_d'), ('5', '21.0', 'string_e'), ('6', '4133.0', 'string_f'), ('7', '31.4', 'string_g'), ('8', '21.3', 'string_h'), ('9', '11.6', 'string_i'), ('10', '22.2', 'string_j');
```

#### Setting up ODBC


