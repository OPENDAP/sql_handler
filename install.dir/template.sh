#!/bin/sh

#         template DSN DRIVER DB_TYPE DB_HOST DB_PORT DB USER PASS READ_ONLY OUT
#sh ./template.sh $DSN $DRIVER $DB_TYPE $DB_HOST $DB_PORT $DB $USER $PASS $READ_ONLY OUT
###########################
# WARNING NO ARGUMENT CHECK!
###########################
DSN=$1
DRIVER=$2
DB_TYPE=$3
DB_HOST=$4
DB_PORT=$5
DB=$6
USER=$7
PASS=$8
READ_ONLY=$9
OUT=${10}

if [[ $DB_TYPE == "MYSQL" ]]; then
echo "MYSQL -> $OUT"
echo "
[$DSN]
Description = SQLHandler test for $DB_TYPE
Trace       = Yes
TraceFile   = sql.log
Driver      = $DRIVER
SERVER      = $DB_HOST
USER        = $USER
PASSWORD    = $PASS
PORT        = $DB_PORT
DATABASE    = $DB
" >> "$OUT"
elif [[ $DB_TYPE == "POSTGRES" ]]; then
echo "POSTGRES ->$OUT"
echo "
[$DSN]
Description         = SQLHandler test for $DB_TYPE
Driver              = $DRIVER
Trace               = Yes
TraceFile           = sql.log
Database            = $DB
Servername          = $DB_HOST
UserName            = $USER
Password            = $PASS
Port                = $DB_PORT
Protocol            = 6.4
ReadOnly            = $READ_ONLY
RowVersioning       = No
ShowSystemTables    = No
ShowOidColumn       = No
FakeOidIndex        = No
ConnSettings        =
" >> "$OUT"
else
echo "
#SORRY: WORK IN PROGRESS!
">> "$OUT"
fi
