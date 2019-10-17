/*
 * SQLDynamicTypeFactory.h
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

#ifndef SQLDYNAMICTYPEFACTORY_H_
#define SQLDYNAMICTYPEFACTORY_H_

#include "action/SQLAction.h"
#include "action/SQLDynamicActionFactory.h"
#include "action/SQLDynamicActionList.h"

#include "DTM/SQLTypeFactoryComponent.h"

#include "connector/SQLTypeConnector.h"

#include "DEM/SQLInternalError.h"

#include <BaseType.h>

/**
 * @brief SPECIALIZATION class which inherit SQLDynamicActionFactory
 * specializing its template parameter  <CODE_TYPE, ARGS_TYPE, OUT_TYPE> as
 * following:
 * - CODE_TYPE = SQL_TYPE
 * - ARGS_TYPE = SQL_TYPE
 * - DAP_TYPE = libdap::BaseType
 * <br>This will keep mapping between SQL_TYPE and corresponding BaseType
 * into the same method that keep an SQL_TYPE* as argument (ARGS_TYPE).
 * <br>The method should be an implementation of
 * SQLFactoryComponent::action() which use Component name instead of
 * Action due to its API dependent nature.
 * It is called 'SQLDynamicTypeFactory':
 * - 'Dynamic' since its ActionList storage type is a DynamicActionList
 * of (object based) actions (stateful).
 * - 'Type' its main action is to map Types building BaseTypes
 * starting from SQL_TYPEs.
 * - 'Factory' it inherit from an SQLActionFactory so its basically a
 * factory of actions.
 *
 * @see ODBCFactoryComponent
 * @see SQLDynamicActionList
 * @see SQLActionList
 * @see SQLFactoryComponent::action()
 * @see SQLDynamicActionFactory
 * @see SQLActionFactory
 */
template<class SQL_TYPE,                // connector && ActionFactory
        class ODBC_TYPE>            // connector && ActionFactory
//			class DAP_TYPE	= libdap::BaseType> // FIXED
class SQLDynamicTypeFactory :
        public SQLDynamicActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType> {
public:
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
     * @todo I'm planning to use this factory to build entire row/object.
     *  May we use the multiple request to automatically build DAS elements?
     *  getCode(){getType(col++)}
     */
    virtual bool stop(SQL_TYPE *code) {
        return true;
    }

    /**
     * @brief constructor
     * @param connector a reference to the SQLTypeConnector to use.
     */
    SQLDynamicTypeFactory(SQLTypeConnector<SQL_TYPE, ODBC_TYPE> &connector) :
            SQLDynamicActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType>(),
            _connector(connector) {
    }

    /**
     * @brief dtor
     */
    virtual ~SQLDynamicTypeFactory() {
    }

    /**
     * @brief return a reference to the connector
     */
    SQLTypeConnector<SQL_TYPE, ODBC_TYPE> &getConnector() {
        return _connector;
    }

private:
    SQLTypeConnector<SQL_TYPE, ODBC_TYPE> &_connector;

    SQLDynamicTypeFactory() {};
};

#endif /* SQLDYNAMICTYPEFACTORY_H_ */
