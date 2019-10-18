/*
 * SQLConnectAction.h
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
 *  Created on: 11/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLCONNECTACTION_H_
#define SQLCONNECTACTION_H_

#include "container/SQLContainer.h"
#include "connector/SQLHandleConnector.h"
#include "DEM/SQLErrorManager.h"
#include "DEM/SQLInternalError.h"
#include <BESError.h>
#include <exception>

/**
 * @todo add Manager template arguments and wrapper to pass
 * merge and join functions.
 *
 * @brief Implements the connect action for the
 * connector.
 * <br>'ON_EVENT' actions are performed using the
 * SQLErrorManager and the passed ErrorFactory.
 * Set this variable to NULL to skip this phase.
 * <br>You can also use the static method connect(...)
 * to make manual connection.
 * <br>NOTE: We haven't fixed the MSG_TYPE and OUT
 * types since they are unused. Set those
 * parameter accordingly with the ActionList
 * you want to use as container.
 */
template<class ERROR_TYPE, class MSG_TYPE, class OUT>
class SQLConnectAction : public SQLAction<MSG_TYPE, OUT> {
private:
    SQLConnectAction() {};
    SQLHandleConnector &hc;
    /**
     * ErrorFactory is stored as pointer because it can be NULL
     * (which mean to run connection without checking errors)
     * anyway it comes from a reference so no deletion is provided.
     * If needed deletion must be handled externally
     */
    SQLActionFactory<ERROR_TYPE, MSG_TYPE, OUT> *ef;
    SQLContainer *c; //deletion will be handled by SQLHandleConnector
    bool _force;
public:
#if __CLONE__ == 1
    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLAction<MSG_TYPE,OUT>* create()throw (std::bad_alloc){
        return this->clone();
    };

    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLConnectAction<ERROR_TYPE,MSG_TYPE,OUT> *clone(){
        return new SQLConnectAction<ERROR_TYPE,MSG_TYPE,OUT>(
            this->hc,this->c,this->ef,this->_force);
    }
#endif

    virtual ~SQLConnectAction() {}

    /**
     * @brief Default constructor
     * @param connector the SQLHandleConnector to use
     * @param container the container to use to get connection string
     * @param error_factory the ActionFactory used to take actions on
     * errors (or any code returned of ERROR_TYPE). Can be NULL which
     * means that no actions are performed on error code.
     * @param force a bool to force check point activation (default is
     * 'false' which mean that is active only if you have set it into
     * configuration).
     * @see SQLActionFactory
     * @see SQLContainer
     * @see SQLHandlerConnector
     */
    SQLConnectAction(
            SQLHandleConnector &connector,
            SQLContainer *container,
            SQLActionFactory<ERROR_TYPE, MSG_TYPE, OUT> *error_factory,
            bool force = false) :
            c(container),
            ef(error_factory),
            hc(connector),
            _force(force),
            SQLAction<MSG_TYPE, OUT>() {};

    /**
     * @brief Manual connecting
     *
     * This simply wrap action method passing a NULL
     * pointer as required.
     */
    OUT *connect() {
        return connect(hc, c, ef, _force);
    }

    /**
     * This is the action which will be called by the
     * ActionFactory.
     */
    OUT *action(MSG_TYPE *) throw(SQLInternalFatalError) {
        return connect(hc, c, ef, _force);
    }

    /**
     * @brief Static method to try to connect
     * @param connector a reference to the SQLHandleConnector to use
     * @param container a pointer to the container to use to connect
     * @param error_factory an ActionFactory used to take actions on
     * errors (if _SQLH_ON_CONNECT check point is active or 'force'
     * is true). Can be NULL which mean that no actions are performed
     * on any error code.
     * @param force a bool to force check point activation (default is
     * 'false' which mean that is active only if you have set it into
     * configuration).
     * @see SQLActionFactory
     * @see SQLErrorFactory
     * @see SQLContainer
     * @see SQLHandleConnector
     */
    static OUT *
    connect(SQLHandleConnector &connector,
            SQLContainer *container,
            SQLActionFactory<ERROR_TYPE, MSG_TYPE, OUT> *error_factory,
            bool force = false) {
        BESDEBUG(SQL_NAME, "SQLConnectAction: Connecting" << std::endl);
        try {
            bool ret = connector.connect(container);
            if (error_factory) {
                if (force)
                    return SQLErrorManager<OUT>::trigger(_SQLH_ON_ALWAYS,
                                                         *error_factory);
                else
                    return SQLErrorManager<OUT>::trigger(_SQLH_ON_CONNECT,
                                                         *error_factory);
            }
            else if (!ret)
                throw SQLInternalFatalError(
                        "SQLConnectAction: Connect execution fails (returning false)",
                        __FILE__, __LINE__);

            BESDEBUG(SQL_NAME, "SQLConnectAction: Connected" << std::endl);
        }
        catch (SQLInternalError &ie) {
            // don't forget to close the connector
            connector.close();
            /**
             *  This exception here (applied to connect) should be fatal.
             *  It will be thrown as fatal.
             */
            throw SQLInternalFatalError(ie.get_message(),
                                        ie.get_file(), ie.get_line());
        }
        catch (BESError &e) {
            // don't forget to close the connector
            connector.close();
            throw SQLInternalFatalError(e.get_message(),
                                        e.get_file(), e.get_line());
        }
        catch (std::exception &e) {
            // don't forget to close the connector
            connector.close();
            throw SQLInternalFatalError(e.what(),
                                        __FILE__, __LINE__);
        }
        catch (...) {
            // don't forget to close the connector
            connector.close();
            throw SQLInternalFatalError(
                    "SQLConnectAction: Unknown error while connecting",
                    __FILE__, __LINE__);
        }
        return NULL; // to avoid warning
    }

};

#endif /* SQLOPENACTION_H_ */
