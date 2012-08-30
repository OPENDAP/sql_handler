/*
 * SQLConnector.h
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
 *  Created on: Oct 8, 2008
 *      Author: carlo cancellieri
 *      Updated on May 2010
 *      -> building SQL bridge
 */

#ifndef SQLCONNECTOR_H_
#define SQLCONNECTOR_H_

#include "SQLErrorConnector.h"
#include "SQLSimpleConnector.h"

/**
 * @brief This is the main Component used to retrieve
 * data from the database.
 *
 */
template <class SQL_TYPE, class ODBC_TYPE, class ERROR_TYPE, class MSG_TYPE=ERROR_TYPE>
class SQLConnector :
	public SQLSimpleConnector<SQL_TYPE,ODBC_TYPE>,
	public SQLErrorConnector<ERROR_TYPE,MSG_TYPE> {
protected:
	/**
	 * @brief You should set READY status
	 * when the connector is ready to retrieve
	 * values using getNext() (or getType).
	 * @param state true if connector is ready
	 * (default), false otherwise.
	 *
	 */
	virtual void setReady(bool state=true){
		SQLHandleConnector::setReady(state);

		/**
		 * If you implement separately Type and Error
		 * connector remember to set also their state
		 * @code
		 * SQLTypeConnector<SQL_TYPE,ODBC_TYPE>::setReady(state);
		 * SQLErrorConnector<ERROR_TYPE,MSG_TYPE>::setReady(state);
		 * @endcode
		 */
	}
public:
	/**
	 * @brief Tell the ActionFactory
	 * (or any other caller) that
	 * the connector is ready to be
	 * used (to read data).
	 * @return true if connector is
	 * ready, false otherwise
	 */
	virtual const bool & isReady()const{
		return SQLHandleConnector::isReady();
		/**
		 * If you implement separately Type and Error
		 * connector remember to check also their state
		 * @code
		 * if (SQLHandleConnector::isReady() &&
		 * 		SQLTypeConnector<SQL_TYPE,ODBC_TYPE>::isReady() &&
		 * 		SQLErrorConnector<ERROR_TYPE,MSG_TYPE>::isReady())
		 * 	return true;
		 * else
		 * 	return false;
		 * @endcode
		 */
	}
	virtual ~SQLConnector(){
TESTDEBUG(SQL_NAME_TEST,"DELETING: SQLConnector"<<endl);
	};
	/**
	 * @brief Constructor
	 * @param an SQLContainer containing
	 * ALL the information needed to establish
	 * connections and to build the SQL query.
	 */
	SQLConnector():
		SQLSimpleConnector<SQL_TYPE,ODBC_TYPE>(),
		SQLErrorConnector<ERROR_TYPE,MSG_TYPE>(){
TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLConnector"<<endl);
	};
	/**
	 * @brief Constructor
	 * @param an SQLContainer containing
	 * ALL the information needed to establish
	 * connections and to build the SQL query.
	 */
	SQLConnector(SQLContainer *c):
		SQLSimpleConnector<SQL_TYPE,ODBC_TYPE>(c),
		SQLErrorConnector<ERROR_TYPE,MSG_TYPE>(){
TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLConnector"<<endl);
	}
};
#endif /* SQLCONNECTOR_H_ */
