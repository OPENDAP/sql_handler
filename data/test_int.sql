# If you have a service configured
# with name 'MySQL_DSN' user 'user'
# and a pass stored into ODBC
# configuration you only need:
[section]
api=odbc
server=PostgreSQL
user=postgres

[select]
lastcron
[from]
mdl_block
# [where]
# id_attore<10
[other]
# No need since this is default
~==LIKE
# No need since this is default
!==<>

#-----------------------DATASET v0.4--(END)---------------------
