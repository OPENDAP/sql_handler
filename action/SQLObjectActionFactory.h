/*
 * SQLObjectActionFactory.h
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
 *  Created on: 04/ago/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLOBJECTACTIONFACTORY_H_
#define SQLOBJECTACTIONFACTORY_H_

#include "action/SQLAction.h"
#include "action/SQLActionFactory.h"
#include "action/SQLObjectActionList.h"

#include "connector/SQLTypeConnector.h"

#include "DEM/SQLInternalError.h"

/**
 * @brief SPECIALIZATION class which inherit SQLActionFactory specializing
 * its template parameter  <CODE_TYPE, ARGS_TYPE, OUT_TYPE> as
 * following:
 * - CODE_TYPE = SQL_TYPE
 * - ARGS_TYPE = SQL_TYPE
 * - DAP_TYPE = libdap::BaseType
 * <br>This will keep mapping between SQL_TYPE and corresponding BaseType
 * into the same method that keep a SQL_TYPE* as argument (ARGS_TYPE).
 * <br>The method should be an implementation of
 * SQLFactoryComponent::action() which use Component name instead of
 * Action due to its API dependent nature.
 * It is called 'SQLObjectTypeFactory':
 * - 'Object' since its ActionList storage type is SQLAction
 * which are object based action (stateful functors).
 * - 'Type' its main (only) action is to map Types building BaseTypes
 * starting from SQL_TYPEs.
 * - 'Factory' it inherit from an SQLActionFactory so its basically a
 * factory of actions.
 *
 * @see ODBCFactoryComponent
 * @see SQLFactoryComponent::action()
 * @see SQLActionFactory
 */
template <class CODE_TYPE,				// connector && ActionFactory
			class ARGS_TYPE			// connector && ActionFactory
			class OUT_TYPE	= void >
class SQLObjectActionFactory :
	public SQLActionFactory<CODE_TYPE,ARGS_TYPE,OUT_TYPE> {
public:

	/**
	 * @brief get the list of actions to do
	 * corresponding to the passed error code.
	 * @param the code
	 * @return the list of SQLActions to do
	 */
	virtual SQLActionList<CODE_TYPE,ARGS_TYPE> &
		getActions(CODE_TYPE * code) throw (SQLInternalError){
		return (*_getActions.action(code));

	}

	/**
	 * @brief no argument mapping needed since this is
	 * statically coded into the getDapType action.
	 */
	virtual ARGS_TYPE * getArgs(CODE_TYPE *code){
		return _getArgs.action(code);
	};

	/**
	 * @brief returns the actual column type as SQL_TYPE.
	 */
	virtual CODE_TYPE * getCode(){
		return _getCode.action(NULL);
	};

	/**
	 * @brief Stop condition.
	 * Actually objects are requested once at time
	 */
	virtual bool stop(CODE_TYPE * code){
		return _stop.action(code);
	}

	/**
	 * @brief constructor
	 * @param a reference to a SQLObjectAction
	 * 	implementing the getArgs function
	 * @param a reference to a SQLObjectAction
	 * 	implementing the getCode function
	 * @param a reference to a SQLObjectAction
	 * 	implementing the getActions function
	 * @param a reference to a SQLObjectAction
	 * 	implementing the stop function
	 *
	 * SQLTypeFactoryComponent.
	 */
	SQLObjectActionFactory(
			SQLObjectAction<CODE_TYPE,ARGS_TYPE> &_getArgs,
			SQLObjectAction<void,CODE_TYPE> &getCode,
			SQLObjectAction<CODE_TYPE,SQLActionList<CODE_TYPE,ARGS_TYPE> > &getActions,
			SQLObjectAction<CODE_TYPE,bool> &stop):
		SQLActionFactory<CODE_TYPE,ARGS_TYPE,OUT_TYPE>(),
		_getArgs(getArgs),
		_getCode(getCode),
		_getActions(getActions),
		_stop(stop)
	{
TESTDEBUG(SQL_NAME_TEST,"CREATED: OBJECT TYPE FACTORY"<<endl);
	}

	virtual ~SQLObjectTypeFactory(){
TESTDEBUG(SQL_NAME_TEST,"DELETED: OBJECT TYPE FACTORY"<<endl);
	}

	SQLConnector<CODE_TYPE,ARGS_TYPE,OUT_TYPE> & getConnector(){
		return _connector;
	}

private:
	SQLAction<CODE_TYPE,ARGS_TYPE> &_getArgs;
	SQLAction<void,CODE_TYPE> &_getCode;
	SQLAction<CODE_TYPE,size_t> &_getActions;
	SQLAction<CODE_TYPE,bool> &_stop;

	SQLObjectActionFactory(){};
};



#endif /* SQLOBJECTACTIONFACTORY_H_ */
