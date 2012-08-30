/*
 * SQLErrorFactory.h
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
 *  Created on: 28/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLERRORFACTORY_H_
#define SQLERRORFACTORY_H_

#include "action/SQLAction.h"
#include "action/SQLActionFactory.h"
#include "action/SQLStaticActionList.h"

#include "connector/SQLErrorConnector.h"

/**
 * @brief Implementation of an ActionFactory used to
 * easily build an {Static|Object|Dynamic}ErrorFactory,
 * depending from the generated Actions type
 * (by getActions()).
 *
 * <br>To build this class you have to:
 * - implement some methods defined by an ActionFactory:
 * - 	getActions
 * - 	stop
 * - implement an SQLErrorConnector
 * - know that SQLErrorFactory is used by standard
 * "/handler/..." actions with an implementation called
 * SQLErrorManager which specialize the SQLActionManager.
 * - know that SQLErrorFactory is defined using static
 * actions.
 * @see SQLActionFactory
 * @see SQLActionManager
 * @see SQLErrorManager
 *
 * @note If you want to implement an ErrorFactoryComponent
 * which uses stateful SQLAction of fixed size try to
 * specialize ObjectActionFactory
 * @see SQLObjectActionList
 * @see SQLObjectActionFactory
 * @see SQLAction
 * @see SQLObjectAction (general purpose implementation)
 * @see SQLCastAction
 * @see SQLStaticAction (general purpose implementation)
 *
 * @note If you want to implement an ErrorFactoryComponent
 * which uses dynamically allocated SQLAction try to
 * specialize DynamicActionFactory.
 * @see SQLDynamicActionList
 * @see SQLDynamicActionFactory
 *
 * @see ODBCErrorFactoryComponent
 */
template <	class ERROR_TYPE,			//< connector && ErrorFactory
			class MSG_TYPE=ERROR_TYPE,	//< connector && ErrorFactory
			class OUT=void>				//< ErrorFactory
class SQLErrorFactory :
	public SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT> {
public:

	typedef SQLActionList<MSG_TYPE,OUT> &(*GETACTIONS)(ERROR_TYPE *);
	/**
	 * @brief get the list of actions to do
	 * corresponding to the passed error code.
	 * @param the error code
	 * @return the list of SQLActions to do
	 */
	virtual SQLActionList<MSG_TYPE,OUT> &
		getActions(ERROR_TYPE * code) throw (SQLInternalError){
		SQLActionList<MSG_TYPE,OUT> *_al=&_getActions(code);
		_al->reset();
		return *_al;
	}

	/**
	 * @brief no argument mapping needed since this is
	 * statically coded into the getDapType action.
	 */
	virtual MSG_TYPE * getArgs(ERROR_TYPE *code){
		return _connector.getMsg(code);
	};

	/**
	 * @brief returns the actual error type as SQL_TYPE.
	 */
	virtual ERROR_TYPE * getCode(){
		return _connector.getError();
	};

	typedef bool(*STOP)(ERROR_TYPE *);
	/**
	 * @brief Stop condition.
	 */
	virtual bool stop(ERROR_TYPE * code){
		if (_stop)
			return _stop(code);
		else
			return true;
	}

	/**
	 * @brief copy constructor
	 * @param const reference to an SQLErrorFactory type object
	 */
	SQLErrorFactory(const SQLErrorFactory<ERROR_TYPE,MSG_TYPE,OUT> &ef):
		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT>(),
		_getActions(ef._getActions),
		_stop(ef._stop),
		_connector(ef._connector)
	{
TESTDEBUG(SQL_NAME_TEST,"COPIED: SQLErrorFactory"<<endl);
	}

	/**
	 * @brief constructor
	 * @param connector is the connector used to get data
	 * @param getErrors_function is a GETACTIONS function pointer used
	 * to bind getActionList to a user defined method.
	 * @param stop_function is a STOP function pointer used to bind stop()
	 * to a user defined method. If NULL stop will return always true;
	 *
	 */
	SQLErrorFactory(SQLErrorConnector<ERROR_TYPE,MSG_TYPE> &connector,
						GETACTIONS getActions_function, STOP stop_function=NULL):
		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT>(),
		_getActions(getActions_function),
		_stop(stop_function),
		_connector(connector)
	{
		if (!_getActions)
			throw SQLInternalFatalError(
				"Could not build an ErrorFactory using a NULL pointer for GETACTION",
				__FILE__,__LINE__);
TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLErrorFactory"<<endl);
	}

	virtual ~SQLErrorFactory(){
TESTDEBUG(SQL_NAME_TEST,"DELETING: SQLErrorFactory"<<endl);
	}

	SQLErrorConnector<ERROR_TYPE,MSG_TYPE> & getConnector(){
		return _connector;
	}
private:
	GETACTIONS _getActions;
	STOP _stop;
	// connector (See SQLSequence)
	SQLErrorConnector<ERROR_TYPE,MSG_TYPE> &_connector;
};

#endif /* SQLSTATICERRORFACTORY_H_ */
