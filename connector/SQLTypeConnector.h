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
	// actual cursors position
	size_t _column_position;
	size_t _row_position;

	// number of columns (Fields)
	size_t _columns;
	// number of rows (Tuples)
	size_t _rows;
	// flag indicating the end condition
	bool _not_end;

	/**
	 * @brief add 'increment' to the ACTUAL row position
	 * if increment is greater to the maximum value,
	 * last row will be indexed and _not_end is set
	 * to false.
	 * @param how much rows to skip
	 * @return number of skipped rows or getRows() if
	 * actual row+increment put cursor to the end
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
			// set the end condition
			_not_end=false; //!< GLOBAL ENDING CONDITION

TESTDEBUG(SQL_NAME_TEST,"--ENDING------> getRow: "<<getRow()
<<" getCol:"<<getCol()<<std::endl);

			//returning maximum for skipped rows
			return getRows(); //!< FUNCTION ENDING CONDITION
		}
	}
protected:

	/**
	 * @brief Set the row size resulting
	 * from of the QUERY
	 */
	inline void setRows(size_t rows){_rows=rows;}

	/**
	 * @brief Set the column size resulting
	 * from of the QUERY
	 */
	inline void setCols(size_t columns){_columns=columns;}

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
	 * can throw an BESInternalException or an
	 * SQLInternalException
	 * @note use notEnd to check end condition
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
	virtual const std::string & getColName(const size_t & column)=0;

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
	virtual const std::string & getColDesc(const size_t & column)=0;

	/**
	 * @brief return the status of this connector
	 * indicating if it is ready to getNext().
	 * @note this method has its implementation
	 * in the SQLHandleConnector sister abstract class.
	 * Anyway you may implement this method if you want
	 * to implement separately Type and Handle Connector
	 */
	virtual const bool & isReady()const=0;

	/**
	 * @brief indicate the status of the cursors
	 * @return false if the end is reached and no
	 * more element are available.
	 */
	const bool & notEnd(){return _not_end;}

	/**
	 * returns the number of rows of the result set
	 */
	const size_t & getRows(){return _rows;}

	/**
	 * @brief returns the position (column) of the getNext()
	 * return value.
	 * interval 0 to (getRows()-1)
	 */
	const size_t & getRow()const{return _row_position;}

	/**
	 * @brief reset cursor and notEnd status
	 * @note should be used only if you don't have
	 * used the getNext() since ODBC cursors (usually)
	 * can't go back.
	 */
	void reset(){
		_row_position=0;
		_column_position=0;
		// check for end condition
		if (getRows()==0 && getCols()==0)
			_not_end=false;
		else
			_not_end=true;
	}

	/**
	 * @brief returns the number of columns of the result set
	 *
	 */
	const size_t & getCols()const{return _columns;}

	/**
	 * @brief returns the position (row) of the getNext()
	 * return value;
	 * @note interval 0 to (getCols()-1)
	 */
	const size_t & getCol()const{return _column_position;}

#if 0
	/** DEPRECATED
	 * @brief reset the ACUTAL column position to '0'
	 * and notEnd status to true
	 * @note should be used only if connector is
	 * buffered or if you don't have used getNext()
	 * since ODBC cursors usually can't go back.
	 */
	void resetCol(){ _column_position=0; _not_end=true; }
#endif

	/**
	 * @brief add 'increment' to the
	 * ACTUAL column position
	 * Setting number of column and rows
	 * corresponding to the current
	 * position + next.
	 * If the getCols and getRows limit are
	 * reached, sets the not_end flag to false.
	 * @return number of skipped rows or getRows()
	 * if limits are reached
	 * @note column start from 0 to getCols()-1
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
			if (setRow(1)==getRows()) // !_not_end
				return getRows();	// ENDING CONDITION
			else
				rows++;
			// remaining
			// NOTE: increment is less than getCols()
			_column_position=sum-getCols();
		}

TESTDEBUG(SQL_NAME_TEST,"-------------> getRow: "<<getRow()
<<" getCol:"<<getCol()<<std::endl);
TESTDEBUG(SQL_NAME_TEST,"-------------> rows: "<<rows
<<" increment:"<<increment<<std::endl);

		return rows;
	}



	virtual ~SQLTypeConnector(){
TESTDEBUG(SQL_NAME_TEST,"DELETING: SQLTypeConnector"<<std::endl);
	}

	/**
	 * @brief Constructor
	 */
	SQLTypeConnector():
		_column_position(0),
		_row_position(0),
		_columns(0),
		_rows(0),
		_not_end(true){
TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLTypeConnector"<<std::endl);
	}

};


#endif /* SQLTYPECONNECTOR_H_ */
