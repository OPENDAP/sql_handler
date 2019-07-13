

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
  * **_How to load database tables in install.dir into DBMS?_**
  * **_how to configure odbc.ini etc._**



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

