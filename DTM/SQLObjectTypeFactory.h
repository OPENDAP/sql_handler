/*
 * SQLObjectTypeFactory.h
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
 *  Created on: 28/Jul/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLOBJECTTYPEFACTORY_H_
#define SQLOBJECTTYPEFACTORY_H_

#include "action/SQLAction.h"
#include "action/SQLActionFactory.h"
#include "action/SQLObjectActionList.h"

#include "DTM/SQLTypeFactoryComponent.h"

#include "connector/SQLTypeConnector.h"

#include "DEM/SQLInternalError.h"

#include <libdap/BaseType.h>

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
template<class SQL_TYPE,                // connector && ActionFactory
        class ODBC_TYPE>            // connector && ActionFactory
//			class DAP_TYPE	= libdap::BaseType> // FIXED
class SQLObjectTypeFactory :
        public SQLActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType> {
public:

    /**
     * @brief get the list of actions to do
     * corresponding to the passed error code.
     * @param the code
     * @return the list of SQLActions to do
     */
    virtual SQLActionList<SQL_TYPE, libdap::BaseType> &
    getActions(SQL_TYPE *code) throw(SQLInternalError) {
        /**
         * returns the same action for each error
         * since mapping is done into the action
         */
        _al.reset();
        return _al;

    }

    /**
     * @brief no argument mapping needed since this is
     * statically coded into the getDapType action.
     */
    virtual SQL_TYPE *getArgs(SQL_TYPE *code) {
        return code;
    };

    /**
     * @brief returns the actual column type as SQL_TYPE.
     */
    virtual SQL_TYPE *getCode() {
        return _connector.getType(_connector.getCol());
    }

    /**
     * @brief Stop condition.
     * Actually objects are requested once at time
     */
    virtual bool stop(SQL_TYPE *code) {
        return true;
    }

    /**
     * @brief constructor
     * @param factComp is a stateful SQLAction object which extends
     * SQLTypeFactoryComponent.
     */
    SQLObjectTypeFactory(SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE> &factComp) :
            SQLActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType>(),
            _action(factComp), // copy constructor
            _al(_action), // action list of 1 action
            _connector(factComp.getConnector()) // store & connector for local use
    {
    }

    /**
     * @brief copy constructor
     * @param a reference
     */
    SQLObjectTypeFactory(const SQLObjectTypeFactory<SQL_TYPE, ODBC_TYPE> &of) :
            SQLActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType>(),
            _action(of._action), // copy constructor
            _al(of._action), // action list of 1 action
            _connector(of._connector) // store & connector for local use
    {
    }

    virtual ~SQLObjectTypeFactory() {
    }

    SQLTypeConnector<SQL_TYPE, ODBC_TYPE> &getConnector() {
        return _connector;
    }

private:
    // single ojbect (stateful) action used...
    SQLAction<SQL_TYPE, libdap::BaseType> &_action;
    // to build an ActionList
    SQLObjectActionList<SQL_TYPE, libdap::BaseType> _al;
    SQLTypeConnector<SQL_TYPE, ODBC_TYPE> &_connector;

    SQLObjectTypeFactory() {};
};

#endif /* SQLOBJECTTYPEFACTORY_H_ */
