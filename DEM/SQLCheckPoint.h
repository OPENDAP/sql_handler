/*
 * SQLCheckPoint.h
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
 *  Created on: 03/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLCHECKPOINT_H_
#define SQLCHECKPOINT_H_

#include <bitset>
#include <vector>
#include <string.h>

#include <BESInternalError.h>

// for configuration keys
#include "SQLDefinitions.h"

#define _SQLH_CHECK_SIZE 6 // form CONNECT to CUSTOM

typedef enum {
_SQLH_ON_ALWAYS=0,
_SQLH_ON_CONNECT=1,
_SQLH_ON_CLOSE=2,
_SQLH_ON_QUERY=3,
_SQLH_ON_GET_NEXT=4,
_SQLH_ON_NEXT_ROW=5,
_SQLH_ON_CUSTOM=6
} _SQLH_CHECKS;

// FIXME Removed jhrg 10/1/14 using namespace std;

/**
 * @brief There are 6 check points that you can activate:
 * _SQLH_ON_CONNECT	=	1
 * _SQLH_ON_CLOSE	=	2
 * _SQLH_ON_QUERY	=	3
 * _SQLH_ON_GET_NEXT	=	4
 * _SQLH_ON_NEXT_ROW	=	5
 * _SQLH_ON_CUSTOM	=	6
 * <br>Each check point corresponds to a SQL phase in which handlers can perform
 * Actions.
 * <br>Common actions are (used by unixODBC):
 * - print out debug info
 * - throw a Fatal error if the connection status is bad
 * - throw a (recoverable) InternalError on get next object
 * <br>Others actions could be:
 * - mail me on error
 * - dump a file
 * - whatever you want
 * <br>Each registered SQLPlugin module can register and use its list
 * of actions which will be activated or not depending on this option:
 * SQL.CheckPoint=1,2,3,4
 * <br>You could also concatenate this setup to force deeper check detail
 * in your SQLPlugin configuration file.
 * <br>SQL.CheckPoint+=5
 * <br>Note: This list affects ALL the registered SQL plugin modules.
 * <br>The _SQLH_ON_ALWAYS is used to force the CheckPoint to always start.
 */
class SQLCheckPoint{
private:
	static bool isSet;

	// we use bitset instead of vector<bool>
	// this should be faster and smaller
	static std::bitset<_SQLH_CHECK_SIZE> function;

protected:
	// used to cache results
	static unsigned long last_type_checked;
	static bool last_status;

public:
	/**
	 * @brief initialize the class setting
	 * its static member reading from the
	 * configuration file.
	 * @return true
	 * @throws BESError if something goes
	 * wrong
	 */
	static bool init() throw (BESError);

	/**
	 * @brief check if the passed (type) argument
	 * represent an active check point,
	 * if so return true; false otherwise
	 */
	static bool check(unsigned long type);
#if 0
	/**
	 * THIS SHOULD NOT BE NECESSARY:
	 * -> USE ON_CUSTOM
	 * carlo cancellieri 06-03-2010
	 *
	 *
	 * @DEPRECATED
	 *
	 * implement this method to add
	 * more custom check points.
	 * NOTE: you need to use customCheck()
	 * in your code.
	 * Before creating a new CHECK POINT:
	 * use your own check points only if
	 * the standard ones don't logically fit
	 * your check case
	 */
	virtual bool customCheck(){
		throw BESInternalError("Custom check method not implemented",
					__FILE__,__LINE__);
	};
#endif
};
#endif /* SQLCHECKPOINT_H_ */
