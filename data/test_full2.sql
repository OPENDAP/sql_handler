define $USER$=postgres
define $FROM$=table1 join table2

# If you have a service configured
# with name 'MySQL_DSN' user 'user'
# and a pass stored into ODBC
# configuration you only need:

[section]
api=odbc
#note: ' will not filtered
server='PostgreSQL'
user=$USER$

[select]
nome

[from]
$FROM$

[where]
id<10
