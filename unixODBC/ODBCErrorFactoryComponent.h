/*
 * ODBCErrorFactoryComponent.h
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
 *  Created on: 23/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef ODBCERRORFACTORYCOMPONENT_H_
#define ODBCERRORFACTORYCOMPONENT_H_


#include "action/SQLStaticActionList.h"
#include "DEM/SQLErrorFactory.h"
#include "DEM/SQLDefaultErrorAction.h"
#include "ODBCConnector.h"
#include "ODBCTypes.h"

/**
 * ERROR COMPONENTS
 * @brief This class is a container for static error
 * components of the unixODBC SQLPlugin.
 * <br>Note: it is called Component as it could not
 * be reused by other drivers.
 * <br>We inherit from an SQLErrorFactory to make things
 * clear:
 * - implements methods to build an ActionFactory.
 * - the ActionFactory implemented is an implementation
 * called SQLErrorFactory
 * - SQLErrorFactory is used by standard "/handler/..."
 * actions with an implementation called SQLErrorManager
 * which specialize the SQLActionManager.
 * - SQLErrorFactory can be defined using static actions
 * @see SQLActionFactory
 * @see SQLErrorFactory
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
 * @note We use SQLErrorFactory which do not need to
 * be inherited since it uses static methods.
 * @see getErrorFactory
 *
 */
class ODBCErrorFactoryComponent : public SQLErrorFactory<error_type, message_t> {
public:
#if 0
    /**
     * @brief utility function which return a copy of an ErrorFactory
     * @param conn a reference to the ODBCConnector
     * @return a copy of a SQLErrorFactory of this driver (unixODBC)
     */
    static SQLErrorFactory<error_type,message_t>
    getErrorFactory(ODBCConnector &conn){
        return SQLErrorFactory<error_type,message_t>(conn,&_getActions,&_stop);
    }
#endif

    /**
     * @brief constructor
     * @param a reference to a SQLErrorConnector or a
     * SQLConnector
     */
    ODBCErrorFactoryComponent(ODBCConnector &conn) :
            SQLErrorFactory<error_type, message_t>(conn, &_getActions, &_stop) {}

    /**
     * @brief copy constructor
     * @param a reference to an ODBCErrorFactoryComponent
     */
    ODBCErrorFactoryComponent(ODBCErrorFactoryComponent &c) :
            SQLErrorFactory<error_type, message_t>(c.getConnector(),
                                                  &c._getActions,
                                                  &c._stop) {}

    virtual ~ODBCErrorFactoryComponent() {};
private:


    /**
     * @brief This is the getActions method implementation
     *
     * This method will be passed to the SQLErrorFactory
     * constructor.
     * It returns the statically defined list of actions
     * (ActionsLists) to do on the incoming error code.
     * Defined template parameters indicates the types used
     * by the actions in the lists:
     * - message_t is the type passed as IN (as input argument
     * of the action)
     * - void is the returned type of the action
     * <br>NOTE: default actions defined in SQLDefaultErrorAction
     * returns void
     *
     * @see SQLDefaultErrorAction
     * @see OUT* SQLAction<IN,OUT>::action(IN*)
     *
     * @param error the error code representing the actual
     * status.
     * @return SQLActionList
     *
     */
    static SQLActionList<message_t, void> &_getActions(error_type *error);

    /**
     * @brief The method which represent the stop condition
     * of the ActionFactory execution flow it usually return
     * true when the input error code represents a no_error
     * condition.
     *
     * @param error the actual status code
     * @return bool 'true' if ActionFactory should stop to
     * call ERROR_CODE getCode(), this will result in:
     * At the end of the actual executing ActionList exit.
     */
    static bool _stop(error_type *error);

    /**
     * defines the type of ACTION which will be used to
     * build static action lists.
     * <br>Note: this defines a function pointer.
     */
    typedef SQLStaticAction<message_t>::ACTION _list_type;

    /**
     * From SQLGetEnvAttr API DOC we could have 5
     * different return status:
     * 1 SQL_SUCCESS,
     * 2 SQL_SUCCESS_WITH_INFO,
     * 3 SQL_NO_DATA,
     * 4 SQL_ERROR,
     * 5 SQL_INVALID_HANDLE.
     */
#if 0
    // fatalAction also print to debug

    // common error ActionList
    static _list_type _error[];
#endif
    // 1 SQL_SUCCESS,
    static SQLStaticActionList<message_t, void> success;

    // 2 SQL_SUCCESS_WITH_INFO,
    static SQLStaticActionList<message_t, void> info;

    // 3 SQL_NO_DATA,
    static SQLStaticActionList<message_t, void> no_data;

    // 4 SQL_ERROR,
    static SQLStaticActionList<message_t, void> error;

    // 5 SQL_INVALID_HANDLE.
    static SQLStaticActionList<message_t, void> invalid;
};

#endif /* ODBCERRORFACTORYCOMPONENT_H_ */
