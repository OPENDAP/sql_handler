/*
 * SQLSimpleConnector.h
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
 *  Created on: 11/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLSIMPLECONNECTOR_H_
#define SQLSIMPLECONNECTOR_H_
//debug
#include "SQLDefinitions.h"

#include "SQLHandleConnector.h"
#include "SQLTypeConnector.h"
/**
 * @brief this interface do not use the
 * Error Part of the Connector.
 * This should be implemented if you want to make
 * error checks internally.
 * If you want to produce a complete driver use
 * the main SQLConnector prototype.
 *
 */
template <class SQL_TYPE, class ODBC_TYPE>
class SQLSimpleConnector:
	public SQLHandleConnector,
	public SQLTypeConnector<SQL_TYPE,ODBC_TYPE>{
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
	virtual bool isReady(){
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
	virtual ~SQLSimpleConnector(){
TESTDEBUG(SQL_NAME,"DELETING: SQLSimpleConnector"<<endl);
	};
	/**
	 * @brief Constructor
	 * @param an SQLContainer containing
	 * ALL the information needed to establish
	 * connections and to build the SQL query.
	 */
	SQLSimpleConnector():
		SQLHandleConnector(),
		SQLTypeConnector<SQL_TYPE,ODBC_TYPE>(){
TESTDEBUG(SQL_NAME,"CREATING: SQLSimpleConnector"<<endl);
	};
	/**
	 * @brief Constructor
	 * @param an SQLContainer containing
	 * ALL the information needed to establish
	 * connections and to build the SQL query.
	 */
	SQLSimpleConnector(SQLContainer *c):
		SQLHandleConnector(c),
		SQLTypeConnector<SQL_TYPE,ODBC_TYPE>(){
TESTDEBUG(SQL_NAME,"CREATING: SQLSimpleConnector"<<endl);
	}
};

#endif /* SQLSIMPLECONNECTOR_H_ */
