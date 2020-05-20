
# Test variable definitions

# Set DSN to the name in square brackets in odbc.init.
# For the sqlite example bundled with the handler, that is [sqlite_test].
define $DSN$=sqlite_test

[section]

api=odbc
server=$DSN$
# The user and pass keywords are not needed 
# user=root
# pass=opendap

[select]
a, b, c

[from]
sqlh_table
