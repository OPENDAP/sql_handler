#-----------------------DATASET v0.4-------------------------
# 0
# rows starting with '#' are comments
# blank lines are accepted

# 1
# The sections order is used to perform
# query so if you have a preferred source
# put its section up into the dataset.

# 2
# Variables are permitted and must be
# specified before their use as following:
# define $NAMEVAR$ = value of 'the' variable
# They are delimited with $VARIABLE$
# and could be used in all the document
# parts excepting for Tags and comments
# regex used to parse a variable definition:
#"[ ]*define[ ]+(\\$\\w+\\$)[ ]*=[ ]*([A-z,0-9,\\',\\\", ,\\,]+)"

# 3
# sections are delimited by the Tag
# [section]
# Tags are delimited with '[' and ']'
# After a Tag a semicolon '\n' should
# be found.
# Tags are:
# - [section]
# - [select]
# - [from]
# - [where]
# - [other]
# reserved keys are:
# - api
# - dbname
# - server
# - port
# - user
# - pass
# reserved api values are:
# - sql (should never be used)
# - odbc (indicate the default
#    plugin)
# If you want to use the bundled
# unixODBC handler you have to set
# odbc as api value into the section:
#	[section]
#	api=odbc
#	server=DSN_test
#	user=user
#	pass=pass
# Note that all the lines into the
# [section] part are in the form:
# RESERVED_KEY = VALUE
# VALUE can contain:
# - A-z
# - 0-9
# - '
# - "
# -   (white spaces)
# - , (comma)
# - :

# 4
# since each driver has its own
# connection string
# the content of this section's
# part is not checked.
# WRONG STRING CONNECTION WILL
# BE CHECKED AT RUNTIME BY THE
# CHOSEN DRIVER
# The connection string will be copied
# and merged, variables will be replaced
# and then it will be passed to the
# specified driver as specified by
# the driver in use.

# 5
# Each section should contain at least:
# [section]
# api=APINAME
# server=SERVER_NAME
# [select]
# sql string for select clause
# [from]
# valid and complete SQL 'from' clause

# 6
# Note that you DO NOT have to specify:
# - 'SELECT' in select clause since it's
# handled by the driver.
# - 'FROM' in from clause
# - 'WHERE' in where predicate
# - ';' at the end of the query
#
# This is to avoid dangerous SQL injection
# like 'insert' or 'delete'.
#
# Each column name HAVE TO BE UNIQUE so
# you are encouraged to USE ALIAS like:
# table1.attribute attr1
# table.attribute as attr2
# attribute AS attr3
#
# USE SAME ALIAS NAME for all of the
# 'section' of the dataset
# F.e.:
# [section]
# table.wind_c as wc
# ...
# [section]
# table3.wind_chill as wc
#

# 7
# Optionally each section may contain:
# [WHERE]
# sql string for where predicate
# [OTHER]
# all the entries in this part should
# be in this form:
# key = value
# They are inserted into a map which
# the driver may handle.
# This can be also used to override
# default behaviour (standard feature):
# DAP != is changed in <>
# DAP ~= is changed in LIKE
# You can specify a different comparator
# substitution for each section since
# each API may handle different
# SQL dialect.
# Here you can specify connection 
# parameters or sql query string. 
# See API documentation.

# ---------- EXAMPLE ---------------
# Declaring variables:
# the following declared variables
# can be used somewhere in this file

define $USER$=postgres
define $PASS$=postgres
define $HOST$=SHAREDHOST
define $FROM$=table1 join table2

#------------EXAMPLE---------------
# USING unixODBC default SQLPlugin:
# If you have a service configured
# with name 'MySQL_DSN' user 'user'
# and a pass stored into ODBC
# configuration you only need:

[section]
# API indicate the SQLPlugin to use
# 'odbc' is a reserved key indicating
# to use the unixODBC bundled SQLPlugin
api=odbc

# This is the name of the 
# configured DSN
# (http://www.unixodbc.org)
server=MySQL_DSN
user=user

[select]
# The attribute list to query
# NOTE: the order used here
# will be keep in the results
name, address, phone

[from]
# The table to use
# can be a complex FROM clause
actor

[where]
# this is optional constraint
# which will be applied to
# ALL the query of this section
# can be used to limit the
# shared data
id_actor<100

[other]
# No need since this is a standard
# comparator substitution
~==LIKE
# No need since this is a standard
# comparator substitution
!==<>

#------------EXAMPLE---------------
[section]
api=mysql
user=$USER$
pass=$PASS$
server=$HOST$:5000
dbname=ProGM_2

[select]
#this wildcard is permitted but
# you should prefer to specify
# all the attributes by hand since
# using '*' attribute list can't be 
# checked before executing the query.
# So it's impossible to check
# attributes passed to the BES,
# this may cause uncheckable 
# query errors.
*

[from]
$FROM$

#------------EXAMPLE---------------
[section]
api=pg
dbname=ProGM
server=localhost
port=5432
user=$USER$
pass=$PASS$

[select]
column_1 as i, table_1.column_2 as r,
table_2.column_2 as v

[from]
# natural join
table_1 join table_2
# plus a cartesian product
, table_3

[where]
# omitting AND will break
# the SQL predicate list!
# This should be a valid where
# predicate.
# NOTE: USE ALIAS FOR CONSTRAINT
i < '1000' AND
r <> '0'

[other]
order=order by 'i'
# NOTE SQL syntax (dialect) can be
# relative to the driver in use!

#------------EXAMPLE---------------
[section]
api=odbc
server=SQLH
user=postgres
pass=postgres

[select]
int i,real r,varchar v
[from]
sqlh_table

[section]
api=odbc
server=SQLH
user=postgres
pass=postgres

[select]
integer as i,float as r ,string as v
[from]
sqlh_table

#-----------------------DATASET v0.4--(END)---------------------
