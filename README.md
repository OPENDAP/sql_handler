

# README - sql_handler

## Building and Installing the sql_handler
Unlike most of the BES modules, this handler for SQL data is a separate project.
It can be added to the bes/modules directory or built outside of the bes software.

To build the code:

* `autoreconf -fiv
./configure --prefix=<prefix>`
* `make`
* `make check` (See more below)
* `make install`

Notes:
The `./configure` script must find both libdap and the BES. It will use the pkgconfig package 
manager and, if that fails, look for dap-config and bes-config on the PATH.

**make check doesn't work**

## Installing DBMS software on your server.



## MySQL

### Linux
https://dev.mysql.com/doc/refman/5.7/en/binary-installation.html
https://dev.mysql.com/doc/refman/8.0/en/binary-installation.html

### OSX
https://dev.mysql.com/doc/refman/5.7/en/osx-installation.html
https://dev.mysql.com/doc/refman/8.0/en/osx-installation.html

## PostGreSQL

### Linux

### OSX

## SQLite

### Linux

### OSX


## Notes
* In order to run `make check` you will need to install the test database located 
in `install.dir` 

* `install.dir/odbc.TEMPLATES.ini` - Contains examples of various odbc_inst.ini 
files for various DBMS system.

* `odbc.ini` - Creates view into the database and exposes it via the ODBC interface.
* `odbc_inst.ini` - Creates a associate between an odbc driver and a dbms.




