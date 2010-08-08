/*
 * SQLTypeConnector.h
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
 *  Created on: 07/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLTYPECONNECTOR_H_
#define SQLTYPECONNECTOR_H_

//debug
#include "SQLDefinitions.h"

#include "DEM/SQLInternalError.h"

/**
 * @brief This is the main Component used to retrieve
 * data from the database.
 */
template <class SQL_TYPE, class ODBC_TYPE>
class SQLTypeConnector {
private:
	// actual position returned by getNext()
	size_t _column_position;
	size_t _row_position;

	// number of columns (Fields)
	size_t _columns;
	// number of rows (Tuples)
	size_t _rows;

protected:

	/**
	 * @brief Set the row size resulting
	 * from of the QUERY
	 */
	inline void setRows(size_t rows){ _rows=rows; }
	/**
	 * @brief Set the column size resulting
	 * from of the QUERY
	 */
	inline void setCols(size_t columns){ _columns=columns; }

	/**
	 * @brief You should set READY status
	 * when the connector is ready to retrieve
	 * values using getNext() (or getType).
	 * @param state true if connector is ready
	 * (default), false otherwise.
	 *
	 *
	 */
	virtual void setReady(bool state=true)=0;

public:
	/**
	 * @brief Returns a value of type SQL_TYPE representing
	 * the Type of the ACTUAL position (usually an entire column).
	 * This is usually a statically defined value.
	 * Refer to the your ODBC API documentation to know what's
	 * yours
	 * Should be usable with getCol() which returns:
	 * 0 - (n-1) 	column index	n=getCols()
	 * ODBC uses
	 * 1 - n 		column index
	 * @see ODBCConnector
	 */
	virtual SQL_TYPE * getType(size_t column)=0;

	/**
	 * @brief Returns a value of ODBC_TYPE type containing
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
	virtual ODBC_TYPE * getNext(size_t next=1)=0;


	/**
	 * @brief returns a string representation of the
	 * indexed column, it is used to build
	 * DAS
	 * Should be usable with getCol() which returns:
	 * 0 - (n-1) 	column index	n=getCols()
	 * ODBC uses
	 * 1 - n 		column index
	 * @see ODBCConnector
	 */
	virtual const string & getColName(const size_t & column)=0;

	/**
	 * @brief returns a string representation of the
	 * indexed column, it is used to build
	 * DAS
	 * Should be usable with getCol() which returns:
	 * 0 - (n-1) 	column index	n=getCols()
	 * ODBC uses
	 * 1 - n 		column index
	 * @see ODBCConnector
	 */
	virtual const string & getColDesc(const size_t & column)=0;

	/**
	 * NOTE: this method has its implementation
	 * in the SQLHandleConnector sister abstract class.
	 * Anyway you may implement this method if you want
	 * to implement separated Type and Handle Connector
	 */
	virtual bool isReady()=0;


	/**
	 * returns the number of rows of the result set
	 */
	inline size_t getRows(){return _rows;};

	/**
	 * @brief returns the position (column) of the getNext()
	 * return value.
	 * interval 0 to (getRows()-1)
	 */
	inline size_t getRow(){return _row_position;};

	/**
	 * @brief add 'increment' to the ACTUAL row position
	 * if increment is greater to the maximum value,
	 * last row will be indexed
	 */
	size_t setRow(size_t increment=1){
		size_t sum=_row_position+increment;
#if 0
		// size_t is unsigned
		if (increment<0)
			throw SQLInternalError(
				"WARNING: row increment should be >= 0",__FILE__,__LINE__);
#endif
		if (sum<getRows()){
			_row_position=sum;
			return increment;
		}
		else {
			// set to the last row +1
			_row_position=getRows();
			// set to the last col +1
			_column_position=getCols();

TESTDEBUG(SQL_NAME,"--ENDING------> getRow: "<<getRow()
<<" getCol:"<<getCol()<<endl);


			//ENDING CONDITION
			return getRows();
		}
	};

	/**
	 * @brief provided only for special cases;
	 * should be used only if connector is
	 * buffered since cursors usually can't go back.
	 */
	inline void resetRow(){ _row_position=0; };

	/**
	 * @brief returns the number of columns of the result set
	 *
	 */
	inline size_t getCols(){return _columns;};

	/**
	 * @brief returns the position (row) of the getNext()
	 * return value;
	 * interval 0 to (getCols()-1)
	 */
	inline size_t getCol(){return _column_position;};


	/**
	 * reset the ACUTAL column position to '0'
	 */
	inline void resetCol(){ _column_position=0; };

	/**
	 * @brief add 'increment' to the
	 * ACTUAL column position
	 * Setting number of column and rows
	 * corresponding to the current
	 * position + next.
	 * @return number of skipped rows or getRows()
	 * if limits are reached
	 * NOTE: column start from 0 to getCols()-1
	 */
	size_t setNext(size_t increment=1){
#if 0
		// column_position is unsigned
		// first element should be tracked using setNext(0)
		if (increment<1)
			throw SQLInternalError(
				"WARNING: column increment should be >= 1",__FILE__,__LINE__);
#endif

		size_t rows=0;
		// if 'increment' is bigger then a row
		if (increment > getCols() ) {
			// how many rows
			rows=setRow(increment/getCols());
			//_row_position+=rows;
			increment=increment - (getCols()*rows);
		}

		size_t sum=_column_position+increment;
		if (sum<getCols()) { // same row
			_column_position=sum;

		}
		else {
			//next row
			if (setRow(1)==getRows())
				return getRows();	// ENDING CONDITION
			else
				rows++;
			// remaining
			// NOTE: increment is less than getCols()
			_column_position=sum-getCols();
		}

TESTDEBUG(SQL_NAME,"-------------> getRow: "<<getRow()
<<" getCol:"<<getCol()<<endl);
TESTDEBUG(SQL_NAME,"-------------> rows: "<<rows
<<" increment:"<<increment<<endl);

		return rows;
	};



	virtual ~SQLTypeConnector(){
TESTDEBUG(SQL_NAME,"DELETING: SQLTypeConnector"<<endl);
	};

	/**
	 * @brief Constructor
	 * @param an SQLContainer containing
	 * ALL the information needed to establish
	 * connections and to build the SQL query.
	 */
	SQLTypeConnector():
		_column_position(0),
		_row_position(0),
		_columns(0),
		_rows(0){
TESTDEBUG(SQL_NAME,"CREATING: SQLTypeConnector"<<endl);
	};

};


#endif /* SQLTYPECONNECTOR_H_ */
