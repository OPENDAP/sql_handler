/*
 * SQLErrorConnector.h
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

#ifndef SQLERRORCONNECTOR_H_
#define SQLERRORCONNECTOR_H_
// debug
#include "SQLDefinitions.h"

/**
 * @brief This is the error part of a SQLConnector.
 *
 * This interface should be implemented if you want to
 * use a complete SQLConnector which uses the
 * DynamiErrorManager part of the SQLHandler.
 * Using DEM you'll be able to use standard actions
 * provided by the platform and or add your own.
 * @see SQLAction
 * @see SQLErrorManager
 * @see SQLCheckPoint
 * @see SQLErrorFactory
 * @see SQLDefaultErrorFactory
 *
 */
template <class ERROR_TYPE, class MSG_TYPE=ERROR_TYPE>
class SQLErrorConnector {
	public:
		/**
		 * @brief set the actual connection status
		 * and return the related code.
		 * @return an object representing status.
		 * NOTE: consider that you can make
		 * ERROR_TYPE==MSG_TYPE if the ODBC driver
		 * in use don't use coded error status.
		 */
		virtual ERROR_TYPE * getError()=0;

		/**
		 * @brief Given an error code this method
		 * should return a MSG_TYPE object which
		 * represent a decoded informative object;
		 * Typically this object is a string.
		 * @param a coded error given by getError()
		 * @return a decoded error message which will
		 * be passed as argument to the ErrorList
		 * actions.
		 * @see SQLErrorManager
		 */
		virtual MSG_TYPE * getMsg(ERROR_TYPE * error_code)=0;

		/**
		 * NOTE: this method has its implementation
		 * in the SQLHandleConnector sister abstract class.
		 * Anyway you may implement this method if you want
		 * to implement separated Type and Handle Connector
		 * @see SQLHandleConnector::setReady()
		 * @see SQLHandleConnector::isReady()
		 */
		virtual const bool & isReady()const=0;

		/**
		 * @brief dtor
		 */
		virtual ~SQLErrorConnector(){
TESTDEBUG(SQL_NAME_TEST,"DELETING: SQLErrorConnector"<<endl);
		}

		/**
		 * @brief Constructor
		 */
		SQLErrorConnector(){
TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLErrorConnector"<<endl);
		}
	};

#endif /* SQLERRORCONNECTOR_H_ */
