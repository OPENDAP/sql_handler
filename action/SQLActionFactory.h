/*
 * SQLActionFactory.h
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

#ifndef SQLACTIONFACTORY_H_
#define SQLACTIONFACTORY_H_
//debug
#include "SQLDefinitions.h"

#include "DEM/SQLInternalError.h"
#include "DEM/SQLInternalFatalError.h"

#include "SQLAction.h"
#include "SQLActionList.h"


/**
 * @brief Interface to provide a methods
 * to interact with an ActionManager.
 *
 * An ActionFactory is an object which is able to:
 * - return codes representing a status or a
 * 		function return or whatever you want.
 * - decide which of those code represent a
 * 		stop condition (don't do other actions)
 * Using that code:
 * - return an argument which represent a string,
 * 		a vector or whatever you want.
 * - return list of SQLActions of the same type
 * 		(which mean same input type argument and
 * 		same output type) which are executed
 * 		using those argument.
 * This object is projected to be passed to an
 * ActionManager which executes its methods
 * 'storing' and 'working' with the returning
 * results of each action.
 * @see SQLAction
 * @see SQLActionList
 * @see SQLDynamicActionFactory
 * @see SQLActionManager
 */
template <class CODE_TYPE, class ARGS_TYPE, class OUT_TYPE=void>
class SQLActionFactory {

public:
	/**
	 * @brief get the list of actions to do
	 * corresponding to the passed error code.
	 * @param the error code
	 * @return the list of SQLActions to do
	 */
	virtual SQLActionList<ARGS_TYPE,OUT_TYPE> &
		getActions(CODE_TYPE * code)
			throw (SQLInternalError)=0;

	/**
	 * @brief This method have to be implemented
	 * using the specific ODBC API in use.
	 * This should return a result (typically a
	 * string) from an error code (typically an
	 * int).
	 * @param the error code
	 * @return the message object returned by
	 * the DB corresponding to the error code
	 * (passed in 'err').
	 *
	 * The resulting MSG_TYPE will be passed to
	 * the ActionList as argument so if you want
	 * to implement actions by your self, be sure
	 * to set:
	 * SQLAction<IN == MSG_TYPE,
	 * 						[OUT is optional]>
	 * Note also that for each MSG_TYPE returned
	 * an entire ActionList is executed using the
	 * same MSG_TYPE message.
	 *
	 * You could also use your own MSG_TYPE
	 * type passing to Actions an object to work
	 * on. In this case you can implement
	 * your own Actions starting from SQLAction.
	 * NOTE: this is not a need since
	 * SQLxxxAction accept MSG_TYPE.
	 *
	 * NOTE: if your ODBC API don't use this
	 * method you could use an ERROR_TYPE
	 * equals to the MSG_TYPE implementing
	 * this method simply returning the input
	 * 'err' variable.
	 */
	virtual ARGS_TYPE * getArgs(CODE_TYPE *code) =0;

	/**
	 * Pointer to the method used to get
	 * codes.
	 * This is probably a function coming from
	 * your ODBC API implementation of the
	 * ERROR_TYPE SQLConnector::getError()
	 *
	 * NOTE:
	 * Never return a NULL pointer.
	 * NOTE:
	 * ERROR_TYPE shouldn't contain *
	 *
	 *
	 * We can change this behavior but we will
	 * lost the possibility to run the 'last'
	 * application_list associated to the stop
	 * error code.
	 * @see STOP_ACTION() method for details.
	 * carlo cancellieri 06-06-2010
	 *
	 */
	virtual CODE_TYPE *getCode() =0;

	/**
	 * @brief This method is used by the
	 * SQLExceptionManager to stop requesting
	 * for next error to the DB server.
	 *
	 * Note that you can still apply an
	 * action_list to this error code.
	 * Actions execution will stop at the and
	 * of that list.
	 *
	 * @param the error code corresponding
	 * to the normal status (or equivalent).
	 * @return true if next call of getCode
	 * shouldn't be done, stopping actions.
	 *
	 */
	virtual bool stop(CODE_TYPE * code) =0;

	/**
	 * @brief Dtor
	 */
	virtual ~SQLActionFactory(){
TESTDEBUG(SQL_NAME_TEST,"DELETED: SQLActionFactory"<<std::endl);
	};

	/**
	 * @brief Constructor
	 */
	SQLActionFactory<CODE_TYPE,ARGS_TYPE,OUT_TYPE>(){
TESTDEBUG(SQL_NAME_TEST,"CREATED: SQLActionFactory"<<std::endl);
	};

};

#endif /* SQLACTIONFACTORY_H_ */
