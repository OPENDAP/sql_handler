/*
 * ODBCConnector.h
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

#ifndef ODBCCONNECTOR_H_
#define ODBCCONNECTOR_H_

// atoi
#include <stdlib.h>

// unixODBC
#include <sql.h>
#include <sqlext.h>

#include "connector/SQLConnector.h"

#include "ODBCTypes.h"
#include "SQLDefinitions.h"

#include "DEM/SQLErrorManager.h"
#include "DEM/SQLErrorFactory.h"

using std::string;

/**
 * @brief ODBCConnector implementing the SQLConnector
 *
 * @see SQLHandleConnector
 * @see SQLTypeConnector
 * @see SQLErrorConnector
 *
 */
class ODBCConnector :public SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>{

#define _buf_size 1024

	/**
	 * @brief Connect to DB
	 * @returns true if d_status can be settled to isReady()
	 */
	bool connect();

	MSG_TYPE * getMsg(ERROR_TYPE * error_code);

	/**
	 * return the maximum size of a column data type
	 *
	 * if column paramether is 0 returns the row size;
	 */
	SQLLEN & getColSize(size_t column);

public:
#if 0
	/**
	 * this method could be called by a BES Command
	 * to get available drivers to use in dataset
	 */
	void getSQLDrivers();

	/**
	 * this method could be called by a BES Command
	 * to get available datasources for a dataset
	 */
	void getSQLDataSources();
#endif

	SQL_TYPE getCType(SQLLEN /* jhrg &*/ sql_type);

	/**
	 * should be usable with getCol() which returns:
	 * 0 - (n-1) 	column index	n=getCols()
	 * ODBC uses
	 * 1 - n 		column index
	 */
	virtual SQL_TYPE * getType(size_t column);

	virtual const string & getColName(const size_t &column);

	virtual const string & getColDesc(const size_t& column);


	virtual ERROR_TYPE * getError();

	/**
	 * @brief Query
	 * NOTE: this is also a good place to
	 * update QUERY size limit using
	 * setCols(nFiled) and setRows(nRows)
	 */
	bool query();

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
	ODBC_TYPE * getNext(size_t next=1);

	void fetch();

	/**
	 * @brief Set error factory used by this connector.
	 */
	void setErrorFactory(SQLErrorFactory<ERROR_TYPE,MSG_TYPE> &ef);

	/**
	 * @brief Close connection
	 */
	bool close();

	ODBCConnector():
		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>(),
		stmt(NULL),		//!< statement handle
		env(NULL),		//!< environment handle
		conn(NULL),		//!< connection handle
		rc(0),			//!< d_status
		toFetchRows(0),	//!< number of rows to fetch
		strMsg(""), 	//!< error message
		msgEnvSeq(1),		//!< Env error sequence number // read API documentation
		msgConnSeq(1),		//!< Conn error sequence number // read API documentation
		msgStmtSeq(1),		//!< Stmt error sequence number // read API documentation
		sef(NULL),		//!< error factory
		d_buf(NULL),		//!< buffer
//SQLLEN d_status[_buf_size];//[bufferRows];
		types(NULL),	//!< buffer
		sizes(NULL),	//!< buffer
		row_size(0),	//!< buffer
		descs(NULL),	//!< buffer (column descriptions)
		names(NULL)	//!< buffer
	{
        TESTDEBUG(ODBC_NAME,"CREATING: ODBCConnector"<<endl);
        strMsg = new char[_buf_size];
	};

	virtual ~ODBCConnector() {

        // NONEDED
        //		close();

        // called by close()
        //clean();

        // see clean()
#if 0
        if (strMsg) {
            delete[] strMsg;
            strMsg = 0;
        }
#endif

	};

private:
	void clean() {

        // set d_status to NOT READY
        setReady(false);

        reset();

        if (types) {
            delete[] types;
            types = 0;
        }
        if (sizes) {
            delete[] sizes;
            sizes = 0;
        }
        row_size = 0;
        if (descs) {
            delete[] descs;
            descs = 0;
        }
        if (names) {
            delete[] names;
            names = 0;
        }
        if (d_buf) {
            free(d_buf);
            d_buf = 0;
        }
#if 0
        /**
         * we have an action 'close()' which
         * may clear the error message.
         * This is done into the
         * destructor.
         */
        if (strMsg) {
            delete [] strMsg;
            strMsg=0;
        }
#endif

		msgEnvSeq=1;	//!< Env error sequence number // read API documentation
		msgStmtSeq=1;	//!< Stmt error sequence number // read API documentation
		msgConnSeq=1;	//!< Conn error sequence number // read API documentation

#if 0
		/**
		 *  handled by ODBCPlugin
		 */
		if (sef)
			delete sef;
		sef=0;
#endif
#if 0
		/**
		 * handled by close()
		 */
		// statement handle
		stmt;
		// environment handle
		env;
		// connection handle
		conn;
#endif
	}

#if 0
	// QUERY column size
	//  getCols()
	// QUERY row size
	//  getRows()
	// BUFFER max column size
	//  getCols()
	// BUFFER max row size
	int bufferRows;
	// BUFFER actual row
	int bufferRow;
	// BUFFER actual col
	int bufferCol;
#endif


	SQLHSTMT stmt;//!< statement handle

	SQLHENV env;//!< environment handle

	SQLHDBC conn;//!< connection handle

	SQLRETURN rc;//!< global d_status
	size_t toFetchRows; //!<number of rows to fetch or skip

	// errors
	MSG_TYPE strMsg; //!< error message (argument)
	SQLSMALLINT msgEnvSeq,msgConnSeq,msgStmtSeq; //!< error index sequence

	SQLErrorFactory<ERROR_TYPE,MSG_TYPE> *sef; //!<this is passed not rebuilt


	SQLCHAR **d_buf;	//!< buffer
	SQLLEN d_status[_buf_size];	//!< buffer d_status

	// temporary vars
	SQL_TYPE * types;//!< types buffer

	SQLLEN * sizes, row_size; //!< sizes buffer

	string * descs; //!< descriptions buffer

	string * names; //!< column names buffer
};

#endif /* ODBCCONNECTOR_H_ */
