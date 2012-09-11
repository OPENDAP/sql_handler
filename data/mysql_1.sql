
# Test variable definitions

define $DSN$=test

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
