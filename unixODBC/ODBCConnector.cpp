 /*
 * ODBCConnector.cpp
 *
 * This file is part of the SQLHandler a C++ library to add relational
 * database access to the OPeNDAP Hyrax back-end server.
 * Copyright (C) 2010  Carlo Cancellieri <ccancellieri@hotmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 *
 *  Created on: 22/giu/2010
 *      Author: carlo cancellieri
 */

#include "ODBCConnector.h"

bool
ODBCConnector::connect()
{
	BESDEBUG(ODBC_NAME,"ODBCConnector: Starting connection"<<endl);

// WARNING -> DB password in printed out to DEBUG
TESTDEBUG(ODBC_NAME,"ODBCConnector: Parameters:"
		"\n    Server: "<<getParams().getServer()<<
		"\n    Username: "<<getParams().getUser()<<
		"\n    Password: "<<getParams().getPass()<<endl);
	// clean previous buffer
	if (isReady()){
		clean();
	}

	// Allocate an environment handle
	rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);

	//@todo check this on MS ODBC
	rc = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0);

	// Allocate a connection handle
	rc = SQLAllocHandle(SQL_HANDLE_DBC, env, &conn);

	/**
	 *  @todo Connect using DSN
	 *  SQLDriverConnect(conn, NULL, "DSN=PostgreSQL;", SQL_NTS,
	 *  	NULL, 0, NULL, SQL_DRIVER_COMPLETE);
	 *
	 */
	//rc = SQLConnect(conn, ServerName, SQL_NTS, User, SQL_NTS, Password, SQL_NTS);
	rc = SQLConnect(conn,(SQLCHAR*)getParams().getServer().c_str(),
			SQL_NTS,(SQLCHAR*)getParams().getUser().c_str(),
			SQL_NTS,(SQLCHAR*)getParams().getPass().c_str(),SQL_NTS);

TESTDEBUG(ODBC_NAME,"ODBCConnector: Connection done with status: "<<rc<<endl);
	return true;
}


/**
 * return the maximum size of a column data type
 *
 * if column parameter is 0 returns the row size;
 */
SQLLEN &
ODBCConnector::getColSize(size_t column){
	if (sizes){
		if (column==getCols())
			return row_size;
		if(column<getCols()){
			return sizes[column];
		}
		else
			throw SQLInternalError("Index provided exceeds the array limit!",
					__FILE__,__LINE__);
	}
	else {

		if (getCols()>0){
			sizes=new SQLLEN[getCols()]();
			//(SQLSMALLINT*)calloc(getCols(),sizeof(SQLSMALLINT));
		}
		else
			throw SQLInternalError(
					"Unable to calculate size for a ResultSet of 0 columns",
						__FILE__,__LINE__);

		for (size_t col=0; col<getCols(); col++)
		{
			SQLLEN actual_column_size;
			rc=SQLColAttribute(stmt ,((SQLSMALLINT)col+1),
				SQL_DESC_LENGTH,NULL, 0, NULL, &actual_column_size);
TESTDEBUG(ODBC_NAME,"ODBCConnector: Getting actual_col_size: "<<
	actual_column_size<<" for column: "<<col<<" exit status: "<<rc<<endl);
			/**
			 *  force check for errors here
			 *  since this is a critical operation
			 */
			if (sef)
				SQLErrorManager<void>::trigger(_SQLH_ON_ALWAYS,*sef);
			sizes[col]=actual_column_size;
			row_size+=actual_column_size;
		}
		return getColSize(column);
	}
}

SQL_TYPE
ODBCConnector::getCType(SQLLEN & sql_type)
{
	switch(sql_type){
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_LONGVARCHAR:
		return SQL_C_CHAR;
		break;
	case SQL_WCHAR:
	case SQL_WVARCHAR:
	case SQL_WLONGVARCHAR:
		return SQL_C_WCHAR;
		break;
	case SQL_DECIMAL:
	case SQL_NUMERIC:
		return SQL_C_LONG; // not default
		break;
	case SQL_BIT:
		return SQL_C_BIT;
		break;
	case SQL_TINYINT:
		return SQL_C_TINYINT;
		break;
	case SQL_SMALLINT:
		return SQL_C_SHORT;
		break;
	case SQL_INTEGER:
		return SQL_C_LONG; //todo check LONG or SLONG?
		break;
	case SQL_BIGINT:
		return SQL_C_SBIGINT;
		break;
	case SQL_REAL:
		return SQL_C_FLOAT;
		break;
	case SQL_FLOAT:
		return SQL_C_DOUBLE;
		break;
	case SQL_DOUBLE:
		return SQL_C_DOUBLE;
		break;
	case SQL_BINARY:
	case SQL_VARBINARY:
	case SQL_LONGVARBINARY:
		return SQL_C_BINARY;
		break;
	case SQL_TYPE_DATE:
		return SQL_C_TYPE_DATE;
		break;
	case SQL_TYPE_TIME:
		return SQL_C_TYPE_TIME;
		break;
		/* todo: TIME
	case SQL_INTERVAL_...:
		return SQL_C_TYPE_DATE;
		break;
		*/
	case SQL_GUID:
		return SQL_C_CHAR;
		break;
	default:
		return SQL_C_CHAR;
		break;
	}
}

/**
 * should be usable with getCol() which returns:
 * 0 - (n-1) 	column index	n=getCols()
 * ODBC uses
 * 1 - n 		column index
 */
SQL_TYPE *
ODBCConnector::getType(size_t column){
	if (types){
		if(column<getCols()){
			return &types[column];
		}
		else
			throw SQLInternalError("Index provided exceeds the array limit!",
					__FILE__,__LINE__);
	}
	else {

		if (getCols()>0){
			types=new SQL_TYPE[getCols()]();
			//(SQLLEN*)calloc(getCols(),sizeof(SQLLEN));
		}
		else
			throw SQLInternalError(
					"Unable to check type for a ResultSet of 0 columns",
						__FILE__,__LINE__);

		for (size_t col=0; col<getCols(); col++)
		{
			SQLLEN sql_type;
			SQLColAttribute(stmt , col+1, SQL_DESC_CONCISE_TYPE,0, 0, 0, &sql_type);
			BESDEBUG(ODBC_NAME,"ODBCConnector: Getting column type: "<<sql_type<<
					" for column: "<<col<<" exit status: "<<rc<<endl);
			/**
			 *  force check for errors here
			 *  since this is a critical operation
			 */
			if (sef)
				SQLErrorManager<void>::trigger(_SQLH_ON_ALWAYS,*sef);

			types[col]=getCType(sql_type);
		}
		return getType(column);
	}
}

const string &
ODBCConnector::getColName(const size_t &column){
	if (names){
		if(column<getCols()){
			return names[column];
		}
		else {
TESTDEBUG(SQL_NAME,"ODBCConnector::getColName() "
		"\n on column: "<<column<<endl);
			throw SQLInternalFatalError("Index provided exceeds the array limit!",
					__FILE__,__LINE__);
		}
	}
	else {

		if (getCols()>0){
			names=new string[getCols()]();
			//(SQLLEN*)calloc(getCols(),sizeof(SQLLEN));
		}
		else
			throw SQLInternalFatalError(
					"Unable to check name for a ResultSet of 0 columns",
						__FILE__,__LINE__);

		for (size_t col=0; col<getCols(); col++)
		{
			char name[_buf_size];
			SQLColAttribute(stmt , col+1, SQL_DESC_NAME,&name, _buf_size, 0, 0);
			BESDEBUG(ODBC_NAME,"ODBCConnector: Getting column name: "<<name<<
					" for column: "<<col<<" exit status: "<<rc<<endl);
			/**
			 *  force check for errors here
			 *  since this is a critical operation
			 */
			if (sef)
				SQLErrorManager<void>::trigger(_SQLH_ON_ALWAYS,*sef);

			names[col].assign(name);
		}
		return getColName(column);
	}
}

// to get portable INT -> to -> STRING
#include <sstream>
const string &
ODBCConnector::getColDesc(const size_t& column){
	if (descs){
		if(column<getCols()){
			return descs[column];
		}
		else
			throw SQLInternalFatalError("Index provided exceeds the array limit!",
					__FILE__,__LINE__);
	}
	else {

		if (getCols()>0){
			descs=new string[getCols()]();
			//(SQLSMALLINT*)calloc(getCols(),sizeof(SQLSMALLINT));
		}
		else
			throw SQLInternalFatalError(
					"Unable to calculate size for a ResultSet of 0 columns",
						__FILE__,__LINE__);

		for (size_t col=0; col<getCols(); col++)
		{
			/**
			 * @todo MORE FANCY DESCRIPTION
			 * probably using column type we may want to show:
			 * precision	SQL_DESC_PRECISION
			 * time type
			 * type of column SQL_DESC_TYPE_NAME
			 * name of column
			 * table name		SQL_DESC_TABLE_NAME
			 */
			SQLLEN len=0;
			rc=SQLColAttribute(stmt ,((SQLSMALLINT)col+1),
				SQL_DESC_PRECISION,NULL, 0,NULL, &len);
TESTDEBUG(ODBC_NAME,"ODBCConnector: Getting actual_col_desc: "<<len<<
	" for column: "<<col<<" exit status: "<<rc<<endl);
			/**
			 *  force check for errors here
			 */
			try{
				if (sef)
					SQLErrorManager<void>::trigger(_SQLH_ON_GET_NEXT,*sef);
				//@todo something better of the following:
				std::stringstream o;
				o<<len;
				descs[col]="Using precision: "+o.str();
			}catch (SQLInternalError &e){
				/**
				 * This is not a critical op. so
				 * if not fatal error is thrown
				 * set to empty string.
				 */
				descs[col]="";
			}
		}
		return getColDesc(column);
	}
}

ERROR_TYPE *
ODBCConnector::getError(){
TESTDEBUG(ODBC_NAME,"ODBCConnector: running GetError() on status: "<<rc<<endl);
	return &rc;
}

MSG_TYPE *
ODBCConnector::getMsg(ERROR_TYPE * error_code){
#if 1
	char strState[_buf_size];
	SQLINTEGER errNum=0;
	SQLSMALLINT msgLen=0;
#if __TESTS__==1
	if (error_code)
		BESDEBUG(ODBC_NAME,
			"ODBCConnector: running GetMsg() on status: "<<*error_code<<endl);
	else
		BESDEBUG(ODBC_NAME,
			"ODBCConnector: running GetMsg() on status: NULL"<<endl);
#endif

	if (SQL_SUCCEEDED(*error_code)){
		// QUERY success no error
		msgEnvSeq=1; //reset error sequence index
		msgStmtSeq=1; //reset error sequence index
		msgConnSeq=1; //reset error sequence index
		sprintf(strMsg,"No errors");
		return &strMsg;
	}
	else if (*error_code==SQL_NO_DATA){
		// QUERY result gives no data into the result
		msgEnvSeq=1; //reset error sequence index
		msgStmtSeq=1; //reset error sequence index
		msgConnSeq=1; //reset error sequence index
		sprintf(strMsg,"%d %s %s\n", errNum, strState, "No data");
		return &strMsg;
	}
	else {
		SQLRETURN rc2;
		char msg[_buf_size];
// check connection statement
		if (SQL_SUCCESS==(rc2=
				SQLGetDiagRec(SQL_HANDLE_DBC, conn,
				msgConnSeq, (SQLCHAR*)strState, &errNum,
				(SQLCHAR*)msg, _buf_size, &msgLen))){
			msgConnSeq++;
			sprintf(strMsg,"%d %s %s\n", errNum, strState, msg);
			return &strMsg;
		}
		else
			BESDEBUG(ODBC_NAME,"ODBCConnector: GetError (conn status): "<<rc2<<endl);
// check statement statement
		if (SQL_SUCCESS==(rc2=
				SQLGetDiagRec(SQL_HANDLE_STMT, stmt,
				msgStmtSeq, (SQLCHAR*)strState, &errNum,
				(SQLCHAR*)msg, _buf_size, &msgLen))){
			msgStmtSeq++;
			sprintf(strMsg,"%d %s %s\n", errNum, strState, msg);
			return &strMsg;
		}
		else
			BESDEBUG(ODBC_NAME,"ODBCConnector: GetError (stmt status): "<<rc2<<endl);
// check environment statement
		if (SQL_SUCCESS==(rc2=
				SQLGetDiagRec(SQL_HANDLE_ENV, env,
				msgEnvSeq, (SQLCHAR*)strState, &errNum,
				(SQLCHAR*)msg, _buf_size, &msgLen))){
			msgEnvSeq++;
			sprintf(strMsg,"%d %s %s\n", errNum, strState, msg);
			return &strMsg;
		}
		else
		{
			BESDEBUG(ODBC_NAME,"ODBCConnector: GetError (env status): "<<rc2<<endl);
			sprintf(strMsg,"Unable to get error message");
			return &strMsg;
		}
	}
#endif
}

/**
 * @brief Query
 * NOTE: this is also a good place to
 * update QUERY size limit using
 * setCols(nFiled) and setRows(nRows)
 */
bool
ODBCConnector::query(){
	/**
	 *  ready should be settled to true by query
	 *  so if this happen here we have to clean
	 *  buffers.
	 */
	if (isReady()) {//todo TEST (actually never happen)
		// close(); no, simply reset vars
		clean(); // clean the
		// connect(); no, simply execute new query
	}
	// Allocate a statement handle
	SQLAllocHandle(SQL_HANDLE_STMT, conn, &stmt);
	string query=getParams().buildQuery();
TESTDEBUG(ODBC_NAME,"ODBCConnector: Executing (REALLY) query"<<endl);
	rc=SQLExecDirect(stmt,(SQLCHAR *)query.c_str(),SQL_NTS);
	BESDEBUG(ODBC_NAME,"ODBCConnector: Query \""<<query
		<<"\" (REALLY) executed."<<endl);

	// rows
	//size_t rows;
	SQLLEN rows=0;
	SQLRowCount(stmt, &rows);
	BESDEBUG(ODBC_NAME,"ODBCConnector: Setting rows number: "<<rows<<endl);
	setRows(rows);

	// columns
	//size_t cols;
	SQLSMALLINT cols=0;
	SQLNumResultCols(stmt, &cols);
	BESDEBUG(ODBC_NAME,"ODBCConnector: Setting columns number: "<<cols<<endl);
	setCols(cols);
#if 0
	/**
	 * To specify the size of the rowset, the Statement
	 * attribute SQL_ROWSET_SIZE (SQLExtendedFetch) or
	 * SQL_ATTR_ROW_ARRAY_SIZE (SQLFetchScroll) must be
	 * set to the required value using the
	 * SQLSetStmtAttr call.
	 */
	SQLSetStmtAttr(stmt,SQL_ATTR_ROW_ARRAY_SIZE,&bufferRows,0);
#endif

	BESDEBUG(ODBC_NAME,"ODBCConnector: Calculating columns size"<<endl);

	// NOTE if(buf) check is done by clear()
	buf=(SQLCHAR**)calloc(cols,sizeof(SQLCHAR*));
	for (int c=0; c<cols; c++){
		// check maximum sizeof
		buf[c]=(SQLCHAR*)calloc(getColSize(c),sizeof(SQLCHAR));

TESTDEBUG(ODBC_NAME,"ODBCConnector: Calculated: "<<
	getColSize(c)<<" for column: "<<c<<endl);
	}

	//binding
TESTDEBUG(ODBC_NAME,"ODBCConnector: Binding"<<endl);

	/* Loop through the rows in the result-set binding to */
	/* local variables */
	for (int i = 0; i < cols; i++) {
		rc=SQLBindCol( stmt, i + 1, *getType(i),//SQL_C_CHAR,//bind to char
				buf[ i ], getColSize(i), &status[ i ] );//sizeof( buf[i] )

TESTDEBUG(ODBC_NAME,"ODBCConnector: Binding info-> Status:"<<rc<<
				" Type:"<<*getType(i)<<" size:"<<getColSize(i)<<
				" Col_status:"<<(status[i]==SQL_NULL_DATA)<<endl);
	}

TESTDEBUG(ODBC_NAME,"ODBCConnector: Bind done. Status: "<<rc<<endl);

	// fetch first row
	// if resulting status SUCCEDED set READY status.
	if (SQL_SUCCEEDED(rc=SQLFetch(stmt))){
		// set status to ready
		setReady();
		start=true; //set start status
		return true;

TESTDEBUG(ODBC_NAME,"ODBCConnector: First fetch exit status is: "<<rc<<endl);

	}
	else
		// return the status
		return false;
}

/**
 * Returns a value of ODBC_TYPE type containing
 * the NEXT value in this result set.
 * It is pointed by the ACTUAL position registered by:
 * - column 	-> getCol()
 * - row		-> getRow()
 * In a Sequence the NEXT value should be:
 * The NEXT COLUMN value of the actual row
 * OR
 * The first value of the NEXT ROW
 *
 * This method should also update the ACTUAL
 * position calling:
 * setCol(size_t increment)
 * OR
 * resetCol() && setRow(size_t increment)
 */
ODBC_TYPE *
ODBCConnector::getNext(size_t next){
	if (!isReady())
		throw SQLInternalError(
			"Unable to getNext() elemeny, connector: !isReady()",
			__FILE__,__LINE__);
	//@todo may we want to try to reconnect?

	if (start){
		BESDEBUG(ODBC_NAME,"ODBCConnector: Getting FIRST element"<<endl);
		start=false;
		return buf[getCol()];
	}
	BESDEBUG(ODBC_NAME,"ODBCConnector: Getting next element"<<endl);
	//if col or row limit reached
	size_t rows;
	/*
	 * Set number of column and rows
	 * corresponding to the current
	 * position + next.
	 * Return the number of skipped rows
	 */
	// get number of skipped rows
	// check ENDING CONDITION -> skipped==getRows()
	// check ENDING CONDITION -> getRow==getRows
	if ((rows=setNext(next))<getRows()){
TESTDEBUG(SQL_NAME,"--------setNext resulting-----> rows: "<<rows<<endl);
		while (rows>0){ // while skip
TESTDEBUG(SQL_NAME,"--------setNext starting fetch"<<endl);
			fetch(); // fetch next row
			rows--;
		}
		return buf[getCol()];
	}
	else
		// row limit reached (this may not happen)
		throw SQLInternalError("ResultSet limits reached!",
				__FILE__,__LINE__);
}

void
ODBCConnector::fetch(){
	/**
	 * clean buffer
	 */
#if 0
	for (size_t c=0; c<getCols(); c++){
TESTDEBUG(ODBC_NAME,"ODBCConnector: buf["<<c<<"]: \""<<buf[c]<<"\""<<endl);
		//*buf[c]=0;
	}
#endif
	/**
	 * fetch row
	 */
	rc = SQLFetch( stmt);
	// if data is null set to '0'
	for (size_t c=0; c<getCols(); c++){
		if (status[c]==SQL_NULL_DATA)
			buf[c]=(SQLCHAR*)'0';
TESTDEBUG(SQL_NAME," Col_status:"<<(status[c]==SQL_NULL_DATA)<<endl);
	}
#if __TESTS__==1
	for (size_t c=0; c<getCols(); c++)
		BESDEBUG(ODBC_NAME,
				"ODBCConnector: new buffer value buf["<<c<<"]: ->\""<<buf[c]<<"\"<-"<<endl);
	BESDEBUG(ODBC_NAME,"ODBCConnector: Fetch status: "<<rc<<endl);
#endif
}

/**
 * @brief Set error factory used by this connector.
 */
void
ODBCConnector::setErrorFactory(SQLErrorFactory<ERROR_TYPE,MSG_TYPE> &ef){
	sef=&ef;	// DO NOT DELETE!
}

/**
 * @brief Close connection
 */
bool
ODBCConnector::close(){
	SQLCloseCursor(stmt);
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	SQLDisconnect(conn);
	SQLFreeHandle(SQL_HANDLE_DBC, conn);
	/**
	 * We end off the transaction using SQLEndTran,
	 * and deallocate the handles in reverse order
	 * compared with the way we allocated the handles.
	 * First we free the statement handle, then we
	 * disconnect from the database using SQLDisconnect,
	 * and then use SQLFreeHandle to free up all the
	 * other handles we allocated!
	 */
	SQLEndTran(SQL_HANDLE_ENV, env, SQL_ROLLBACK);
	SQLFreeHandle(SQL_HANDLE_ENV, env);

	// clean temporary members;
	clean();

	// set status to NOT READY
	// setReady(false); // done by clean

	BESDEBUG(ODBC_NAME,"ODBCConnector: Connection closed"<<endl);
	return true;
}


#if 0
/**
 * COMMENTED OUT UNTIL CHECKs
 * todo : CHECK THIS METHOD
 *
 * this method could be called by a BES Command
 * to get available drivers to use in dataset
 */
void ODBCConnector::getSQLDrivers(){
	SQLCHAR driver[256];
	SQLCHAR attr[256];
	SQLSMALLINT driver_ret;
	SQLSMALLINT attr_ret;
	SQLUSMALLINT direction;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

	direction = SQL_FETCH_FIRST;
	while(SQL_SUCCEEDED(rc = SQLDrivers(env, direction,
					   driver, sizeof(driver), &driver_ret,
					   attr, sizeof(attr), &attr_ret))) {
		direction = SQL_FETCH_NEXT;
		printf("%s - %s\n", driver, attr);
		if (rc == SQL_SUCCESS_WITH_INFO) printf("\tdata truncation\n");
	}
}

/**
 * todo : CHECK THIS METHOD
 *
 * this method could be called by a BES Command
 * to get available datasources for a dataset
 */
void ODBCConnector::getSQLDataSources(){
	SQLCHAR dsn[256];
	SQLCHAR desc[256];
	SQLSMALLINT dsn_ret;
	SQLSMALLINT desc_ret;
	SQLUSMALLINT direction;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

	direction = SQL_FETCH_FIRST;
	while(SQL_SUCCEEDED(rc = SQLDataSources(env, direction,
					   dsn, sizeof(dsn), &dsn_ret,
					   desc, sizeof(desc), &desc_ret))) {
		direction = SQL_FETCH_NEXT;
		printf("%s - %s\n", dsn, desc);
		if (rc == SQL_SUCCESS_WITH_INFO) printf("\tdata truncation\n");
	}
}

#endif
