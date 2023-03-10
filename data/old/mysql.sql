
# Test variable definitions

define $DSN$=test

[section]

api=odbc
server=$DSN$
# The user and pass keywords are REQUIRED for MySQL 8
user=mysql
pass=paswordywords

[select]
a, b, c

[from]
sqlh_table
