/*
 * SQLHandleConnector.h
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

#ifndef SQLHANDLECONNECTOR_H_
#define SQLHANDLECONNECTOR_H_
#include "container/SQLContainer.h"

/**
 * @brief This is the Connector Handle part which should
 * be used to open and close the connection, it also
 * implement isReady() and setReady() methods
 */
class SQLHandleConnector {
private:
	/**
	 * Connector status:
	 * -true if (connect() && query())
	 * -false otherwise
	 * This mean that isReady() return
	 * true when the connector is ready
	 * to return data via SQLTypeConnector
	 * or return errors via SQLErrorConnector
	 */
	bool _isReady;
	/**
	 * Dataset container which implements
	 * methods used to get connection
	 * and query parameters.
	 */
	SQLContainer *_container;

protected:
	/**
	 * @brief can be used to set the actual
	 * container to use, this method should
	 * never be used if connect() is correctly
	 * coded.
	 * @note: the container should be deleted
	 * outside of this class
	 * @param SQLContainer *
	 */
	inline void setParams(SQLContainer *c){
#if 0
		// connector don't have to handle SQLContainer
		if (_container)
			delete _container;
#endif
		_container=c;
	}

	/**
	 * @brief You should set READY status
	 * when the connector is ready to retrieve
	 * values using getNext() (or getType).
	 *
	 */
	void setReady(bool state=true){
		_isReady=state;
	}

	/**
	 * @brief Here we have to establish the
	 * DB connection using parameters given
	 * by the getParams().
	 *
	 * note: this may be never called directly.
	 * To connect use connect(SQLContainer) instead.
	 *
	 * @see connect(SQLContainer)
	 */
	virtual bool connect()=0;

public:

	/**
	 * @brief Close connection
	 */
	virtual bool close()=0;

	/**
	 * @brief Uses informations obtained by
	 * connect(SQLContainer) to build and
	 * execute a valid (for the driver in use)
	 * query.
	 * @note should set:
	 * - isReady() flag
	 * - size limits using setCols(nFiled)
	 * and setRows(nRows)
	 * @see SQLTypeConnector::setCols()
	 * @see SQLTypeConnector::setRows()
	 */
	virtual bool query()=0;


	/**
	 * @brief Tell the ActionFactory
	 * (or any other caller) that
	 * the connector is ready to be
	 * used.
	 * @return true if connector is
	 * ready, false otherwise
	 */
	virtual const bool & isReady()const{
		return _isReady;
	}

	/**
	 * @return a constant reference to
	 * the SQLContainer passed to
	 * connect(SQLContainer &)
	 */
	inline SQLContainer & getParams(){
#if 1
		if (_container)
			return *_container;
		else
			throw BESInternalError(
				"Unable to get NULL reference to paramethers",
					__FILE__,__LINE__);
#endif
	}

	/**
	 * @brief Connect to DB
	 */
	bool connect(SQLContainer *c){
#if 0
		// connector don't have to handle SQLContainer
		if (_container)
			delete _container;
#endif
		_container=c;
		return connect();
	}

	virtual ~SQLHandleConnector(){
#if 0
		// connector don't have to handle SQLContainer
		if (_container)
			delete _container;
		_container=0;
#endif
TESTDEBUG(SQL_NAME_TEST,"DELETING: SQLHandleConnector"<<endl);
	};

	/**
	 * @brief Constructor
	 * @param an SQLContainer containing
	 * ALL the information needed to establish
	 * connections and to build the SQL query.
	 */
	SQLHandleConnector():
		_isReady(false),
		_container(NULL){
TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLHandleConnector"<<endl);
	};

	/**
	 * @brief Constructor
	 * @param an SQLContainer containing
	 * ALL the information needed to establish
	 * connections and to build the SQL query.
	 */
	SQLHandleConnector(SQLContainer *c):
		_isReady(false),
		_container(c){
TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLHandleConnector"<<endl);
	}
};

#endif /* SQLHANDLECONNECTOR_H_ */
