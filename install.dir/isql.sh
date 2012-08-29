#!/bin/sh

#############TODO REMOVE ME#################
#PATH="/opt/unixodbc/bin/:/opt/bes/bin/:$PATH"
############################################

############################################
# PARAM: A FILE NAME
# RETURN: THE FILE NAME AS ABSOLUTE PATH OR
# VALUE 1 IF FILE NOT EXISTS AND IS NOT EXEC
############################################
getCmd(){
CMD=`type -P $1`;
if [[ -x $CMD && -n "$CMD" ]]; then
        echo "$CMD"; #command
else
        echo "1"; #error
fi;
}

###########################################
# PARAM: NO
# RETURN: 'Y' if Y or y  or enter is pressed
# or 'N' if N or n is pressed
###########################################
confirm(){
res=0;
while [[ $res != "Y" && $res != "N" ]]; do
	read -p "[Y/n]:" res
	if [[ $res == "y" || $res == "Y" ]]; then
		res="Y"
	elif [[ $res == "n" || $res == "N" ]]; then
		res="N"
	elif [[ -z $res ]]; then
		res="Y"
	fi
done
echo $res;
}

########################FILES
OUT="/dev/stdout"
ERR="/dev/stderr"
######### SQLcommands to create and insert data into database
SQL="sqlh.sql"
######### relative path for SQLH dataset
SQLH_DATASET="data/sql/test_full.sql"
######### relative path for bes.conf
BES_CONF="etc/bes/bes.conf"
#########TODO command file
BES_CMD_FILE="bes.cmd"
#########loaded after BESCMD
PREFIX=""
#########TODO read data path from bes.conf
DATASET=""

echo "LOADING BES ENVIRONMENT"
WHAT="bescmdln"
echo "SEARCHING FOR: $WHAT"
CMD=`getCmd $WHAT`;
if [[ $CMD == 1 ]]; then 
	echo "[NOT OK]	$WHAT	Check your PATH"; 
	exit 1;
else
	echo "[OK]	$WHAT 	->$CMD<-";
	BESCMDLN="$CMD";
fi;

WHAT="besctl"
echo "SEARCHING FOR: $WHAT"
CMD=`getCmd $WHAT`;
if [[ $CMD == 1 ]]; then 
	echo "[NOT OK]	$WHAT	Check your PATH"; 
	exit 2;
else
	echo "[OK]	$WHAT 	->$CMD<-";
	BESCMD="$CMD";
fi;

#get 'prefix' from the BES command:
PREFIX=${CMD%/bin/besctl};
echo "PREFIX: 				$PREFIX"
BES_CONF="$PREFIX/$BES_CONF";
echo "BES configuration file: 		$BES_CONF"
#1### Get data from bes.conf
BES_DATA="`cat $BES_CONF|grep BES.Catalog.catalog.RootDirectory=`";
#2### Trim variable
BES_DATA="${BES_DATA#BES.Catalog.catalog.RootDirectory=}";
##### Set SQLH_DATASET
echo "BES data file: 				$BES_DATA"
SQLH_DATASET="$BES_DATA/$SQLH_DATASET";
echo "SQLH dataset test file: 		$SQLH_DATASET"

#############unixODBC##############

WHAT="isql"
echo "SEARCHING FOR: $WHAT"
CMD=`getCmd $WHAT`;
if [[ $CMD == 1 ]]; then 
	echo "[NOT OK]	$WHAT	Check your PATH"; 
	exit 3;
else
	echo "[OK]	$WHAT 	->$CMD<-";
	ISQL="$CMD";
fi;

WHAT="odbc_config"
echo "SEARCHING FOR: $WHAT"
CMD=`getCmd $WHAT`;
if [[ $CMD == 1 ]]; then 
	echo "[NOT OK]	$WHAT	Check your PATH"; 
	exit 4;
else
	echo "[OK]	$WHAT 	->$CMD<-";
	ODBC_CONFIG="$CMD";
fi;

ODBC_INI=`$CMD --odbcini`
ODBCINST_INI=`$CMD --odbcinstini`
if [[ ! -w $ODBC_INI ]]; then
	echo "You don't have write permission on $ODBC_INI"
	exit 5
elif [[ ! -w $ODBCINST_INI ]]; then
	echo "You don't have write permission on $ODBCINST_INI"
	exit 5
fi

######THIS IS NOT POSSIBLE######
#echo -n "BUILDING DB 	... "
#isql -v PostgreSQL postgres -n <<EOF go
#CREATE DATABASE SQLH2;
#go
#\commit
#\quit;
#EOF
#echo -e "[DONE]\n"
################################

#######################DSN VARS########################
res=0;
DSN="0"
while [[ $res != "Y" ]]; do
	echo "Setting Data-Source-Name (free):"
	read DSN
	echo "DSN is $DSN confirm?"
	res=`confirm`
done

res=0;
DRIVER="0"
while [[ $res != "Y" ]]; do
	echo "Setting ODBC driver PATH (The library to use to connect DB):"
	echo "F.E.:/usr/local/lib/libodbcpsql.so (for postgres)"
	read DRIVER
	echo "Driver PATH is $DRIVER confirm?"
	res=`confirm`
done

res=0;
DB_TYPE="0"
while [[ $res != "Y" ]]; do
while [[ $res != "OK" ]]; do
	echo "Setting database type:"
	echo "1 - PostgreSQL"
	echo "2 - MySQL"
	read -p "[1-2]:" DB_TYPE
	if [[ $DB_TYPE == 1 ]]; then
		DB_TYPE="POSTGRES"
		res="OK";
	elif [[ $DB_TYPE == 2 ]]; then
		DB_TYPE="MYSQL"
		res="OK";
	else				#TODO EXPAND THIS LIST
		res="NOTOK";
	fi
done
	echo "Database type is $DB_TYPE confirm?"
	res=`confirm`
done

########################DB VARS########################
res=0;
DB_HOST="0"
while [[ $res != "Y" ]]; do
	echo "Setting DBMS HOST address:"
	read DB_HOST
	echo "HOST is $DB_HOST confirm?"
	res=`confirm`
done

res=0;
DB_PORT="0"
while [[ $res != "Y" ]]; do
	echo "Setting DBMS PORT number:"
	read DB_PORT
	echo "DBMS PORT is $DB_PORT confirm?"
	res=`confirm`
done

res=0;
DB="0"
while [[ $res != "Y" ]]; do
	echo "Setting (existing) database name:"
	read DB
	echo "DATABASE is $DB confirm?"
	res=`confirm`
done

res=0;
USER="0"
while [[ $res != "Y" ]]; do
	echo "Setting (existing) user name:"
	read USER
	echo "USER is $USER confirm?"
	res=`confirm`
done

res=0;
PASS="0"
while [[ $res != "Y" ]]; do
	echo "Setting (existing) password:"
	read PASS
	echo "PASS is $PASS confirm?"
	res=`confirm`
done


READ_ONLY="No"
##############################################################CALL THE TEMPLATE SCRIPT
CMD="sh ./template.sh $DSN $DRIVER $DB_TYPE $DB_HOST $DB_PORT $DB $USER $PASS $READ_ONLY $ODBC_INI"
echo "RUNNING: $CMD"
# CREATE CONFIGURATION AND APPEND TO ODBC_INI FILE
$CMD
echo "---------CREATED CONFIGURATION:--------"
cat $ODBC_INI
echo "-----------------END-------------------"


# RUN SQL COMMANDS USING ISQL ON ODBC_INI CONFIGURED FILE
echo "ADDING TABLE  	... "
$ISQL -v $DSN $USER $PASS -n < $SQL 2&>$ERR &>$OUT;

# TODO NO WAY TO GET SUCCESS OR ERRO STATUS!
#if [[ res==0 ]]; then
#	echo "[DONE]"
#else
#	echo -e "\n\n\tWARNING: Unable to build database, check the error log.\n"
#	exit 6;
#fi

########################BES VARS
BES_PORT="10002"          ####################### TODO
BES_HOST=$DB_HOST #"localhost" TODO

res=0;
while [[ $res != "N" ]]; do
	echo "Start or restart the BES test?"
	res=`confirm`
	if [[ $res == "Y" ]]; then
		echo -n "STARTING THE BES 	... "
		$BESCMD restart -d $ERR,odbc,sql &>$OUT;
		echo -e "[DONE]\n"
		res="N"
	fi
done

res=0;
while [[ $res != "Y" ]]; do
	echo "Run SQLHandler BES test?"
	res=`confirm`
	if [[ $res == "N" ]]; then
		exit
	fi
done

echo -n "USING BES 		... "
cat "$BES_CMD_FILE" | $BESCMDLN -p $BES_PORT -h $BES_HOST 2&>$ERR;
echo -e "[DONE]\n"

echo -n "STOPPING THE BES 	... "
CMD="$BESCMD stop 2&>$ERR";
$CMD;
echo -e "[DONE]\n"
