/*
 * SQLCastAction.h
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
 *  Created on: 31/mag/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLCASTACTION_H_
#define SQLCASTACTION_H_

#include "action/SQLObjectAction.h"
// TESTDEBUG
#if __TESTS__==1
#include "SQLDefinitions.h"
#endif
// free
#include <cstdlib>

/**
 * @brief class which extends an SQLAction to implement
 * a stateful action.
 * This is useful when you want to build a stateful SQLAction
 * starting from a static function.
 * Here the function is a CAST function used by the SQLBaseType
 * to make possible casting from SQL_TYPE to the DAP_TYPE storage
 * type (string,int,float,...).
 * The state variable is a pointer which store the result of the
 * cast until freeValue() is called or the class dtor is called.
 * <br>
 * @note: CLONE is re-implemented to keep the CAST method
 */
template <class IN, class OUT>
class SQLCastAction : public SQLObjectAction<IN,OUT>{
public :
#if __CLONE__==1
	/**
	 * @brief This is the implementation of the
	 * Clone interface.
	 * @return a pointer to a clone of this object
	 */
	virtual SQLCastAction<IN,OUT>* create()throw (std::bad_alloc){
		return this->clone();
	}

	/**
	 * @brief This is the implementation of the
	 * Clone interface.
	 * @return a pointer to a clone of this object
	 */
	virtual SQLCastAction<IN,OUT> *clone(){
		return new SQLCastAction<IN,OUT>(*this);
	}
#endif

	/**
	 * definition of the CAS action function
	 */
	typedef typename SQLAction<IN,OUT>::ACTION CAST;

	/**
	 * @brief Constructor
	 * @param cast_function a function pointer to the
	 * cast function to use
	 * @param the reuse flag telling if the the casted
	 * pointer can be deleted (false) or is reused
	 * (true as default)
	 */
	SQLCastAction(CAST cast_function,
					bool reuse=true):
		SQLObjectAction<IN,OUT>(reuse),
		_cast(cast_function)
	{
	if (!cast_function)
		throw SQLInternalFatalError("SQLCastAction: CAST function is null",
			__FILE__,__LINE__);
	}

	/**
	 * the action to take.
	 * This is only a wrapper for the CAST action
	 * passed to the constructor
	 * @param a pointer to the value to cast
	 * @return a OUT* representing the casted value
	 */
	OUT * action (IN * val){
TESTDEBUG(SQL_NAME_TEST,"SQLCastAction: casting object"<<endl);
		setVal((*_cast)(val));
		return this->getVal();
	}

	/**
	 * @brief COPY constructor
	 * @param a reference to SQLCastAction
	 */
	SQLCastAction(SQLCastAction<IN,OUT> & ca):
		SQLObjectAction<IN,OUT>(ca),
		_cast(ca._cast)
	{}

	/**
	 * @brief dtor which delete the casted pointer
	 * only if the _reuse flag is set to 1
	 */
	virtual ~SQLCastAction(){
TESTDEBUG(SQL_NAME_TEST,"SQLCastAction: deleting object"<<endl);
	}

private:
	CAST _cast;
	SQLCastAction(){};
};
#endif /* SQLCASTACTION_H_ */
