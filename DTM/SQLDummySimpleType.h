/*
 * SQLDummySimpleType.h
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
 *  Created on: 25/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLDUMMYSIMPLETYPE_H_
#define SQLDUMMYSIMPLETYPE_H_

#include "SQLCastAction.h"
#include <Str.h>

/**
 * @brief This is a implementation of a libdap::Str class
 * This is useful when connector won't work any substitute
 * object should be created.
 */
class SQLDummySimpleType:public libdap::Str{
	SQLCastAction<void,std::string> _cast;
public:

	/**
	 * @brief Constructor for dummy SIMPLE type.
	 * This is used to build dummy object when the connector is
	 * unavailable.
	 * @param name the name of the dummy SimpleType
	 * @param cast_action is pointer to the cast action to use
	 * @param reuse a boolean indicating if the casted pointer should
	 * be deleted or is reused elsewhere
	 */
	SQLDummySimpleType(	std::string name,
					SQLCastAction<void, std::string>::CAST cast_action,
					bool reuse):
		Str(name),
		_cast(cast_action, reuse){
TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLDummySimpleType"<<std::endl);
	};

	virtual ~SQLDummySimpleType(){};

	/**
	 * WARNING suppose DAP_TYPE is a BaseType inherited object
	 */
	virtual bool read()
	{
	try {
		if (this->read_p()){
TESTDEBUG(SQL_NAME_TEST,"SQLDummySimpleType: skipping object"<<std::endl);
			return false;
		}
		else
		{
TESTDEBUG(SQL_NAME_TEST,"SQLDummySimpleType: reading object"<<std::endl);
				this->set_value( *(this->_cast.action(NULL)) ); //read dummy value
				if (!_cast.reusable()) {
					this->_cast.freeValue(); // deleting casted value
				}
TESTDEBUG(SQL_NAME_TEST,"SQLDummySimpleType: object copied to the buffer"<<std::endl);
			return false;
		}
	}
	catch (BESError &e){
			BESDEBUG(SQL_NAME,
						"SQLDummySimpleType: Unable to read variable"<<std::endl);
			throw BESInternalFatalError(
				"SQLDummySimpleType: Unable to read variable: "+e.get_message(),
				e.get_file(),e.get_line());
		}
	};

	virtual BaseType *ptr_duplicate()
	{
		return new SQLDummySimpleType(this);
	};


	/**
	 * @brief copy constructor (used by ptr_duplicate())
	 * <br>WARNING suppose DAP_TYPE is a BaseType
	 */
	SQLDummySimpleType(SQLDummySimpleType *obj):
		Str(obj->name()),
		_cast(obj->_cast)
	{
TESTDEBUG(SQL_NAME_TEST,"COPING: SQLDummySimpleType"<<std::endl);
	};
};


#endif /* SQLDUMMYSIMPLETYPE_H_ */
