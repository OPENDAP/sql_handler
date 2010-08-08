/*
 * SQLDefaultErrorAction.h
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
 *  Created on: 16/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLDEFAULTERRORACTION_H_
#define SQLDEFAULTERRORACTION_H_
#include <BESDebug.h>

#include "action/SQLStaticAction.h"
#include "SQLDefinitions.h"

#include "SQLInternalFatalError.h"
#include "SQLInternalError.h"

#include "SQLResponseNames.h"
/**
 *
 * 1. Internal Error
 * The error is internal, for example, unable to create an
 * XML document for a response
 *
 * 2. Internal Fatal Error
 * The error is unrecoverable. A memory error, for example.
 * Something that the BES MUST exit from
 *
 * 3. Syntax User Error
 * The user has made an error, such as a constraint expression
 * with syntax errors in it
 *
 * 4. Forbidden Error
 * The user is not allowed to access the resource, directory
 * permissions, dataset permissions, etc...
 *
 * 5. Not Found Error
 * The user has requested a resource that doesn't exist.
 *
 * The BES catches a BESError, sets the appropriate flags and
 * returns the error as an XML document. If it is an Internal
 * Fatal Error, then after it returns the XML error document,
 * it exits.
 *
 * [Patrick West - 06-01-2010]
 *
 */

/**
 *
 * This method is used to map Errors to ACTIONS.
 * @params ERROR_TYPE error the error returned by the
 * ODBC driver in use.
 *
 * READY TO USE ACTIONS:
 *	SQLDebugAction
 *		-> error will be sent to DEBUG in context 'sql'
 * 	SQLFatalAction
 * 		-> an SQLInternalFatalException will be thrown
 * 		-> and sent to 'sql' DEBUG context.
 *	SQLExceptionAction
 *		-> an SQLInternalException will be thrown
 *
 */


/**
 * @brief Define default ACTIONs.
 * It provide public methods to get SQLAction instances
 * or ACTION methods to build your own static ActionLists
 */
template <class IN>
class SQLDefaultErrorAction {
#if 0
#define _SQLH_DEBUG(args); \
	BESDEBUG( SQL_NAME, _SQLH_DEBUG_PREFIX<<args<< endl );
#define _SQLH_EXCEPT(args); \
	throw SQLInternalError(_SQLH_EXCEPTION_PREFIX+args,__FILE__,__LINE__);
#define _SQLH_EXCEPT(args); \
	throw SQLInternalError(_SQLH_EXCEPTION_PREFIX+args,__FILE__,__LINE__);
#endif
public:
	/**
	 * @brief Throws an SQLInternalFatalError Exception.
	 * @param args is a template argument appended to
	 * SQLDefinitions::_SQLH_FATAL_PREFIX using
	 * operator<<
	 */
	static void *fatalMethod(IN *args){
		BESDEBUG(SQL_NAME,_SQLH_FATAL_PREFIX<<*args<< std::endl);
		throw SQLInternalFatalError(*args,__FILE__,__LINE__);
		return NULL; // to avoid warning
	}

	/**
	 * @brief Instantiate an SQLAction initialized using
	 * fatalMethod
	 * @return A new instance of SQLAction<IN,void>
	 */
	static SQLStaticAction<IN,void> *fatalAction(){
		return new SQLStaticAction<IN>(&fatalMethod);
	}

	/**
	 * @brief Throws an SQLInternalError Exception
  	 * @param args is a template argument appended to
	 * SQLDefinitions::_SQLH_EXCEPTION_PREFIX using
	 * operator+
	 */
	static void *exceptionMethod(IN *args){
		BESDEBUG(SQL_NAME,_SQLH_EXCEPTION_PREFIX<<*args<< std::endl);
		throw SQLInternalError(*args,__FILE__,__LINE__);
		return NULL; // to avoid warning
	}

	/**
	 * @brief Instantiate an SQLAction initialized using
	 * exceptionMethod
	 * @return A new instance of SQLAction<IN,void>
	 */
	static SQLStaticAction<IN,void> *exceptionAction(){
		return new SQLStaticAction<IN>(&exceptionMethod);
	}

	/**
	 * @brief Run BESDEBUG in 'sql' context
  	 * @param args is a template argument type
  	 * which is appended to the
  	 * SQLDefinitions::_SQLH_DEBUG_PREFIX using
  	 * 'operator<<'
	 */
	static void *debugMethod(IN *args) {
		BESDEBUG( SQL_NAME, _SQLH_DEBUG_PREFIX<<*args<< std::endl);
		return NULL; // to avoid warning
	}

	/**
	 * @brief Instantiate an SQLAction initialized using
	 * debugMethod
	 * @return A new instance of SQLAction<IN,void>
	 */
	static SQLStaticAction<IN,void> *debugAction(){
		return new SQLStaticAction<IN>(&debugMethod);
	}

};


#endif /* SQLDEFAULTERRORACTION_H_ */
