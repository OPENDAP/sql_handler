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

#include <string>
#include <sstream>

#include "ODBCConnector.h"

using namespace std;

bool
ODBCConnector::connect() {
    BESDEBUG(ODBC_NAME, "ODBCConnector: Starting connection" << endl);

// WARNING -> DB password in printed out to DEBUG
    BESDEBUG(ODBC_NAME, "ODBCConnector Parameters" <<
                                                   " Server=\"" << getParams().getServer() << "\"" <<
                                                   " Username=\"" << getParams().getUser() << "\"" <<
                                                   " Password=\"" << getParams().getPass() << "\"" << endl);
    // clean previous buffer
    if (isReady()) {
        clean();
    }

    // Allocate an environment handle
    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);

    //@todo check this on MS ODBC
    rc = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

    // Allocate a connection handle
    rc = SQLAllocHandle(SQL_HANDLE_DBC, env, &conn);

    /**
     *  @todo Connect using DSN
     *  SQLDriverConnect(conn, NULL, "DSN=PostgreSQL;", SQL_NTS,
     *  	NULL, 0, NULL, SQL_DRIVER_COMPLETE);
     *
     */
    //rc = SQLConnect(conn, ServerName, SQL_NTS, User, SQL_NTS, Password, SQL_NTS);
#if 1
    // I turned this back on in order to get MySQL-8.x to work as this code would
    // not correctly authenticate without it. After doing so, and thus disabling James' change below
    // the PostgreSQL tests continued to work and the handler was able to use the "user" and "pass"
    // sections of the data/mysql.sql definition to successfully authenticate with MySQL.
    rc = SQLConnect(conn, (SQLCHAR *) getParams().getServer().c_str(), SQL_NTS,
                    (SQLCHAR *) getParams().getUser().c_str(), SQL_NTS,
                    (SQLCHAR *) getParams().getPass().c_str(), SQL_NTS);
#else
    // I'm not sure this is any better than the above code, but suggests
    // a way to cut down on the duplication of information that's held
    // in the odbc.ini file (that has username and password info too).
    // I added extra parameters to get the connection string because I'm
    // chasing down memory errors and one is reported involoving this
    // call. It neither helped nor hurt... The pointers can be replaced
    // with NULL and the length with 0. jhrg 9/7/12
    string dsn = "DSN=";
    dsn.append(getParams().getServer());
    std::vector<SQLCHAR> OutConnectionString(SQL_MAX_OPTION_STRING_LENGTH + 1);
    SQLSMALLINT OutConnectionStringLength;
    rc = SQLDriverConnect(conn, NULL,
                  (SQLCHAR*)dsn.c_str(), SQL_NTS,
                  OutConnectionString.data(), SQL_MAX_OPTION_STRING_LENGTH,
                  &OutConnectionStringLength, SQL_DRIVER_NOPROMPT);
#endif
    BESDEBUG(ODBC_NAME, "ODBCConnector: Connection done with status: " << rc << endl);

    return true;
}


/**
 * return the maximum size of a column data type
 *
 * if column parameter is 0 returns the row size;
 */
SQLLEN &
ODBCConnector::getColSize(size_t column) {
    if (sizes) {
        if (column == getCols())
            return row_size;
        if (column < getCols()) {
            return sizes[column];
        }
        else
            throw SQLInternalError("Index provided exceeds the array limit!",
                                   __FILE__, __LINE__);
    }
    else {

        if (getCols() > 0) {
            sizes = new SQLLEN[getCols()]();
            //(SQLSMALLINT*)calloc(getCols(),sizeof(SQLSMALLINT));
        }
        else
            throw SQLInternalError(
                    "Unable to calculate size for a ResultSet of 0 columns",
                    __FILE__, __LINE__);

        for (size_t col = 0; col < getCols(); col++) {
            SQLLEN actual_column_size;
            rc = SQLColAttribute(stmt, ((SQLSMALLINT) col + 1),
                                 SQL_DESC_LENGTH, NULL, 0, NULL, &actual_column_size);

            /**
             *  force check for errors here
             *  since this is a critical operation
             */
            if (sef)
                SQLErrorManager<void>::trigger(_SQLH_ON_ALWAYS, *sef);
            sizes[col] = actual_column_size;
            row_size += actual_column_size;
        }
        return getColSize(column);
    }
}

SQL_TYPE
ODBCConnector::getCType(SQLLEN /* jhrg &*/ sql_type) {
    switch (sql_type) {
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
ODBCConnector::getType(size_t column) {
    if (types) {
        if (column < getCols()) {
            return &types[column];
        }
        else
            throw SQLInternalError("Index provided exceeds the array limit!",
                                   __FILE__, __LINE__);
    }
    else {

        if (getCols() > 0) {
            types = new SQL_TYPE[getCols()]();
        }
        else
            throw SQLInternalError(
                    "Unable to check type for a ResultSet of 0 columns",
                    __FILE__, __LINE__);

        for (size_t col = 0; col < getCols(); col++) {
            // Added initialization; this quiets an 'uninitialized access' error
            // reported by valgrind. jhrg 9/7/12
            SQLLEN sql_type = 0;
            rc = SQLColAttribute(stmt, col + 1, SQL_DESC_CONCISE_TYPE, 0, 0, 0, &sql_type);
            BESDEBUG(ODBC_NAME, "ODBCConnector: Getting column type: " << sql_type <<
                                                                       " for column: " << col << " exit status: " << rc
                                                                       << endl);
            /**
             *  force check for errors here
             *  since this is a critical operation
             */
            if (sef)
                SQLErrorManager<void>::trigger(_SQLH_ON_ALWAYS, *sef);

            types[col] = getCType(sql_type);
        }

        return getType(column);
    }
}

const string &
ODBCConnector::getColName(const size_t &column) {
    if (names) {
        if (column < getCols()) {
            return names[column];
        }
        else {
            throw SQLInternalFatalError("Index provided exceeds the array limit!",
                                        __FILE__, __LINE__);
        }
    }
    else {

        if (getCols() > 0) {
            names = new string[getCols()]();
            //(SQLLEN*)calloc(getCols(),sizeof(SQLLEN));
        }
        else
            throw SQLInternalFatalError(
                    "Unable to check name for a ResultSet of 0 columns",
                    __FILE__, __LINE__);

        for (size_t col = 0; col < getCols(); col++) {
            char name[_buf_size];
            rc = SQLColAttribute(stmt, col + 1, SQL_DESC_NAME, &name, _buf_size, 0, 0);
            BESDEBUG(ODBC_NAME, "ODBCConnector: Getting column name: " << name <<
                                                                       " for column: " << col << " exit status: " << rc
                                                                       << endl);
            /**
             *  force check for errors here
             *  since this is a critical operation
             */
            if (sef)
                SQLErrorManager<void>::trigger(_SQLH_ON_ALWAYS, *sef);

            names[col].assign(name);
        }
        return getColName(column);
    }
}

// to get portable INT -> to -> STRING
#include <sstream>

const string &
ODBCConnector::getColDesc(const size_t &column) {
    if (descs) {
        if (column < getCols()) {
            return descs[column];
        }
        else
            throw SQLInternalFatalError("Index provided exceeds the array limit!",
                                        __FILE__, __LINE__);
    }
    else {

        if (getCols() > 0) {
            descs = new string[getCols()]();
            //(SQLSMALLINT*)calloc(getCols(),sizeof(SQLSMALLINT));
        }
        else
            throw SQLInternalFatalError(
                    "Unable to calculate size for a ResultSet of 0 columns",
                    __FILE__, __LINE__);

        for (size_t col = 0; col < getCols(); col++) {
            /**
             * @todo MORE FANCY DESCRIPTION
             * probably using column type we may want to show:
             * precision	SQL_DESC_PRECISION
             * time type
             * type of column SQL_DESC_TYPE_NAME
             * name of column
             * table name		SQL_DESC_TABLE_NAME
             */
            SQLLEN len = 0;
            rc = SQLColAttribute(stmt, ((SQLSMALLINT) col + 1),
                                 SQL_DESC_PRECISION, NULL, 0, NULL, &len);
            /**
             *  force check for errors here
             */
            try {
                if (sef)
                    SQLErrorManager<void>::trigger(_SQLH_ON_GET_NEXT, *sef);

                //@todo something better of the following:
                std::stringstream o;
                o << len;
                descs[col] = "Using precision: " + o.str();
            }
            catch (SQLInternalError &e) {
                /**
                 * This is not a critical op. so
                 * if not fatal error is thrown
                 * set to empty string.
                 */
                descs[col] = "";
            }
        }
        return getColDesc(column);
    }
}

error_type *
ODBCConnector::getError() {
    return &rc;
}

message_t *
ODBCConnector::getMsg(error_type *error_code) {
    char strState[_buf_size];
    SQLINTEGER errNum = 0;
    SQLSMALLINT msgLen = 0;

    if (SQL_SUCCEEDED(*error_code)) {
        // QUERY success no error
        msgEnvSeq = 1; //reset error sequence index
        msgStmtSeq = 1; //reset error sequence index
        msgConnSeq = 1; //reset error sequence index
        strMsg = "No errors";
        return &strMsg;
    }
    else if (*error_code == SQL_NO_DATA) {
        // QUERY result gives no data into the result
        msgEnvSeq = 1; //reset error sequence index
        msgStmtSeq = 1; //reset error sequence index
        msgConnSeq = 1; //reset error sequence index
        ostringstream oss;
        oss << errNum << " " << strState << " No data";
        strMsg = oss.str();
        return &strMsg;
    }
    else {
        SQLRETURN rc2;
        char msg[_buf_size];
        // check connection statement
        if (SQL_SUCCESS == (rc2 = SQLGetDiagRec(SQL_HANDLE_DBC, conn,
                                                msgConnSeq, (SQLCHAR *) strState, &errNum,
                                                (SQLCHAR *) msg, _buf_size, &msgLen))) {
            msgConnSeq++;
            ostringstream oss;
            oss << errNum << " " << strState << " " << msg;
            strMsg = oss.str();
            return &strMsg;
        }
        else
            BESDEBUG(ODBC_NAME, "ODBCConnector: GetError (conn status): " << rc2 << endl);

        // check statement statement
        if (SQL_SUCCESS == (rc2 = SQLGetDiagRec(SQL_HANDLE_STMT, stmt,
                                                msgStmtSeq, (SQLCHAR *) strState, &errNum,
                                                (SQLCHAR *) msg, _buf_size, &msgLen))) {
            msgStmtSeq++;
            ostringstream oss;
            oss << errNum << " " << strState << " " << msg;
            strMsg = oss.str();
            return &strMsg;
        }
        else
            BESDEBUG(ODBC_NAME, "ODBCConnector: GetError (stmt status): " << rc2 << endl);

        // check environment statement
        if (SQL_SUCCESS == (rc2 = SQLGetDiagRec(SQL_HANDLE_ENV, env,
                                                msgEnvSeq, (SQLCHAR *) strState, &errNum,
                                                (SQLCHAR *) msg, _buf_size, &msgLen))) {
            msgEnvSeq++;
            ostringstream oss;
            oss << errNum << " " << strState << " " << msg;
            strMsg = oss.str();
            return &strMsg;
        }
        else {
            BESDEBUG(ODBC_NAME, "ODBCConnector: GetError (env status): " << rc2 << endl);
            strMsg = "Unable to get error message";
            return &strMsg;
        }
    }
}

unsigned long
ODBCConnector::getRowCount() {
#if 1
    SQLHSTMT statement;
    // FIXME Hackery. I used this ti get the number of rows. The SQLRowCount()
    // Does not reliably return that for anything other than UPDATE, INSERT,
    // or DELETE statements. jhrg 10/16/19
    // Allocate a statement handle
    SQLAllocHandle(SQL_HANDLE_STMT, conn, &statement);

    string count_stmt = getParams().buildCountQuery();
    rc = SQLExecDirect(statement, (SQLCHAR *) count_stmt.c_str(), SQL_NTS);
    BESDEBUG(ODBC_NAME, "ODBCConnector: Query \"" << count_stmt << "\" executed; rc=" << rc << endl);

    /* create a cursor and execute a statement */

    if (!SQL_SUCCEEDED(rc))
        throw BESInternalError("ODBC SQL Error: Couldn't exec query", __FILE__, __LINE__);

    /* fetch the result: here we expect single row with single column */
    rc = SQLFetch(statement);
    if (!SQL_SUCCEEDED(rc))
        throw BESInternalError("ODBC SQL Error: Couldn't fetch row", __FILE__, __LINE__);

    /* extract data from the fetched row */
    SQLLEN indicator;
    long count = 0;
    SQLLEN cbValueMax = sizeof(count);
    rc = SQLGetData(statement, 1, SQL_C_LONG, &count, cbValueMax, &indicator);
    if (!SQL_SUCCEEDED(rc))
        throw BESInternalError("ODBC SQL Error: Couldn't extract row count", __FILE__, __LINE__);
    if (indicator == SQL_NULL_DATA)
        throw BESInternalError("ODBC SQL Error: Unexpected null value returned by the database", __FILE__, __LINE__);

    BESDEBUG(ODBC_NAME, "Rows in table: " << hex << count << dec << ", indicator: " << indicator << endl);

    /* free resources: close the cursor */
    SQLFreeHandle(SQL_HANDLE_STMT, statement);

    return count;
#endif
}

/**
 * @brief Query
 * @note this is also a good place to
 * update QUERY size limit using
 * setCols(nFiled) and setRows(nRows)
 * @note should set isReady() flag
 */
bool
ODBCConnector::query() {
    /**
     *  ready should be settled to true by query
     *  so if this happen here we have to clean
     *  buffers.
     */
    if (isReady()) {
        clean(); //!< clean members
    }

    // rows
    // getRowCount() uses the same query logic as buildQuery(), but gets the row count
    // instead of the data. This works reliably (with a variety of ODBC drivers) while
    // the SQLRowCount() function does not. That is, SQLRowCount is guaranteed for
    // UPDATE, INSERT, or DELETE statements only.
    unsigned long rows = getRowCount();
    setRows(rows);

    // Allocate a statement handle
    SQLAllocHandle(SQL_HANDLE_STMT, conn, &stmt);

    string query = getParams().buildQuery();
    rc = SQLExecDirect(stmt, (SQLCHAR *) query.c_str(), SQL_NTS);
    BESDEBUG(ODBC_NAME, "ODBCConnector: Query \"" << query << "\" executed; rc=" << rc << endl);
    if (!SQL_SUCCEEDED(rc))
        throw BESInternalError("ODBC SQL Error: Couldn't exec query", __FILE__, __LINE__);

#if 0
    //size_t rows;
    // This is a bug. The SQLRowCount() function returns the number of rows affected by an
    // UPDATE, INSERT, or DELETE statement; an SQL_ADD, SQL_UPDATE_BY_BOOKMARK, or
    // SQL_DELETE_BY_BOOKMARK operation in SQLBulkOperations; or an SQL_UPDATE or
    // SQL_DELETE operation in SQLSetPos. Some drivers might set it for other operations, apparently.
    // See https://docs.microsoft.com/en-us/sql/odbc/reference/syntax/sqlrowcount-function?view=sql-server-ver15
    // For this handler's code to work, we need to know the number of rows in the database!
    // jhrg 10/16/19
    SQLLEN rows = 0;
    SQLRowCount(stmt, &rows);
    BESDEBUG(ODBC_NAME, "ODBCConnector: Setting rows number: " << rows << endl);
    //rows = 10;    // HACK jhrg 10/16/19
    setRows(rows);
#endif

    // columns
    SQLSMALLINT cols = 0;
    rc = SQLNumResultCols(stmt, &cols);
    BESDEBUG(ODBC_NAME, "ODBCConnector: Setting columns number: " << cols << endl);
    if (!SQL_SUCCEEDED(rc))
        throw BESInternalError("ODBC SQL Error: Couldn't get column number", __FILE__, __LINE__);

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

    BESDEBUG(ODBC_NAME, "ODBCConnector: Calculating columns size" << endl);

    // NOTE if(d_buf) check is done by clear()
    d_buf = (SQLCHAR **) calloc(cols, sizeof(SQLCHAR *));
    for (int c = 0; c < cols; c++) {
        // check maximum sizeof
        d_buf[c] = (SQLCHAR *) calloc(getColSize(c), sizeof(SQLCHAR));
    }

    // Loop through the rows in the result-set binding to local variables
    for (int i = 0; i < cols; i++) {
        rc = SQLBindCol(stmt, i + 1, *getType(i),
                        d_buf[i], getColSize(i), &d_status[i]);
        if (!SQL_SUCCEEDED(rc))
            throw BESInternalError("ODBC SQL Error: Couldn't bind result set to local variables", __FILE__, __LINE__);

    }

    // if resulting d_status SUCCEEDED set READY d_status.
    if (sef)
        SQLErrorManager<void>::trigger(_SQLH_ON_ALWAYS, *sef);

    // set number of rows to fetch.
    toFetchRows = 1;
    // set cursors to start and check notEnd condition
    reset();
    // set d_status
    setReady(true);
    // return the d_status
    return true;
}

/**
 * @brief Returns a value of ODBC_TYPE type containing
 * the next value in this result set.
 * <br>It is pointed by the ACTUAL position:
 * - column 	-> getCol()
 * - row		-> getRow()
 * <br>In a Sequence the NEXT value should be:
 * - The NEXT COLUMN value of the actual row
 * <br>OR
 * - The first value of the NEXT ROW
 * @note This method should also update cursors to the
 * next position using setNext:
 * - setNext(size_t increment)
 * @note The first time it is called should return object in
 * position 0,0 and set cursors to col:1 and row:0 (or
 * row:1 if only 1 column), if limits are reached
 * (col==getCols() && row==getRows()) this method
 * can throw an exception or an
 * SQLInternalException
 * @note use notEnd to check end condition
 */
ODBC_TYPE *
ODBCConnector::getNext(size_t next) {
    if (!isReady() || !notEnd())
        throw SQLInternalError(
                "Unable to getNext() element, connector: !isReady() or end is reached",
                __FILE__, __LINE__);

    //@todo may we want to try to reconnect?

    /**
     * number of rows to fetch (skip or extract)
     */
    while (toFetchRows > 0) { // while skip
        fetch(); // fetch next row
        toFetchRows--;
    }

    // get the pointer
    SQLCHAR *ret = d_buf[getCol()];

    /**
     * Set number of column and rows
     * corresponding to the current
     * position + next.
     * Return the number of skipped rows
     * which is stored for the next fetch
     * operation
     */
    toFetchRows = setNext(next);

    return ret;
}

void
ODBCConnector::fetch() {
    /**
     * fetch row
     */
    rc = SQLFetch(stmt);

    /**
     *  if some fetched data is null
     *  set it to '0'
     */
    for (size_t c = 0; c < getCols(); c++) {
        if (d_status[c] == SQL_NULL_DATA)
            d_buf[c] = (SQLCHAR *) '0';
    }
}

/**
 * @brief Set error factory used by this connector.
 */
void
ODBCConnector::setErrorFactory(SQLErrorFactory<error_type, message_t> &ef) {
    sef = &ef;    //!<DO NOT DELETE!
}

/**
 * @brief Close connection
 */
bool
ODBCConnector::close() {
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

    clean();//!< clean temporary members;

    BESDEBUG(ODBC_NAME, "ODBCConnector: Connection closed" << endl);
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
