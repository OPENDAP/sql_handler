/*
 * SQLNextTypeAction.h
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

#ifndef SQLNEXTTYPEACTION_H_
#define SQLNEXTTYPEACTION_H_

#include "connector/SQLSimpleConnector.h"

#include "DTM/SQLTypeManager.h"
#include <BaseType.h>

#include "DEM/SQLErrorManager.h"
#include "DEM/SQLInternalError.h"
#include <BESError.h>
#include <exception>

/**
 * @todo add Manager template arguments and wrapper to pass
 * merge and join functions.
 *
 * @brief Implements the nextType action which
 * returns a BaseType* corresponding to the actual
 * (connector.getCol()) position.
 * <br>'ON_EVENT' actions are performed using the
 * SQLErrorManager and the passed ErrorFactory.
 * Set this variable to NULL to skip this phase.
 * <br>You can also use the static method nextType(...)
 * to make manual call.
 * <br>NOTE: ARGS_TYPE and OUT are not  fixed since
 * they regard ErrorFactorys. Set those
 * parameter accordingly with the ErrorList
 * you want to use to check this Action.
 * <br>NOTE:
 * Output type of this action is fixed to
 * libdap::BaseType due to the Action scope.
 */
template<class SQL_TYPE, class ODBC_TYPE,    // TypeConnector && FactoryComponent
        class ERROR_TYPE, class ARGS_TYPE, class OUT>    // ErrorFactory
class SQLNextTypeAction : public SQLAction<SQL_TYPE, libdap::BaseType> {
private:
    SQLNextTypeAction() {
    };
    SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> &tc;
    SQLActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType> &tf;
    /**
     * ErrorFactory is stored as pointer because it can be NULL
     * (which mean to run connection without checking errors)
     * anyway it comes from a reference so no deletion is provided.
     * If needed deletion must be handled externally
     */
    SQLActionFactory<ERROR_TYPE, ARGS_TYPE, OUT> *ef;

    bool _force;
public:
#if __CLONE__ == 1
    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLAction<SQL_TYPE,libdap::BaseType>* create()throw (std::bad_alloc) {
        return this->clone();
    };

    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLAction<SQL_TYPE,libdap::BaseType> *clone() {
        return new SQLNextTypeAction<
        SQL_TYPE,ODBC_TYPE,
        ERROR_TYPE, ARGS_TYPE, OUT>(
                this->tc,this->tf,this->ef,this->_force);
    }
#endif

    virtual ~SQLNextTypeAction() {
    }

    /**
     * @brief Default constructor
     * @param connector the SQLTypeConnector to use
     * @param tf the SQLActionFactory used to build object
     * @param error_factory the ActionFactory used to take actions on
     * errors (or any code returned of ERROR_TYPE). Can be NULL which
     * mean that no actions are performed on any error code.
     * @param force a bool to force check point activation (default is
     * 'false' which mean that is active only if you have set it into
     * configuration).
     * @see SQLActionFactory
     * @see SQLFactoryComponent
     * @see SQLHandleConnector
     */
    SQLNextTypeAction(SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> &connector,
                      SQLActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType> &tf,
                      SQLActionFactory<ERROR_TYPE, ARGS_TYPE, OUT> *error_factory, bool force = false) :
            tc(connector), tf(tf), ef(error_factory), _force(force), SQLAction<ARGS_TYPE, OUT>() {
    };

    /**
     * @brief Manual connecting
     *
     * This simply wrap action method passing a NULL
     * pointer as required.
     */
    libdap::BaseType *nextType() {
        return nextType(tc, tf, ef, _force);

    }

    /**
     * This is the action which will be called by the
     * ActionFactory.
     */
    libdap::BaseType *action(ARGS_TYPE *) throw(SQLInternalFatalError) {
        return nextType(tc, tf, ef, _force);
    }

    /**
     * @brief Static method to try to connect
     * @param connector the SQLTypeConnector to use
     * @param type_factory the SQLActionFactory used to build object
     * @param error_factory the ActionFactory used to take actions on
     * errors (or any code returned of ERROR_TYPE) Can be NULL which
     * mean that no actions are performed on any error code.
     * @param force a bool to force check point activation (default is
     * 'false' which mean that is active only if you have set it into
     * configuration).
     * @see SQLActionFactory
     * @see SQLFactoryComponent
     * @see SQLHandleConnector
     */
    static libdap::BaseType *
    nextType(SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> &connector,
             SQLActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType> &type_factory,
             SQLActionFactory<ERROR_TYPE, ARGS_TYPE, OUT> *error_factory, bool force = false) {
        try {
            libdap::BaseType *bt = SQLTypeManager::doActions(type_factory);

            /**
             *  set index to the next column setNext() returns number
             *  of skipped rows so if returns >0 && <getRows() we are
             *  on _SQLH_ON_NEX_ROW
             */
            size_t rows = connector.setNext();

            // check errors (if active)
            if (error_factory) {
                if (force)
                    SQLErrorManager<OUT>::trigger(_SQLH_ON_ALWAYS, *error_factory);
                else {
                    if (connector.notEnd()) { // check end condition
                        // for each passed row
                        while (rows-- > 0) {
                            // test ON_NEXT_ROW trigger and do actions
                            SQLErrorManager<OUT>::trigger(_SQLH_ON_NEXT_ROW, *error_factory);
                        }
                    }
                    // test ON_GET_NEXT trigger
                    SQLErrorManager<OUT>::trigger(_SQLH_ON_GET_NEXT, *error_factory);
                }
            }

            return bt;
        }
        catch (SQLInternalError ie) {
            /**
             *  if ErrorManager throws an SQLInternalException
             *  we have to 'continue' the loop
             *  @see SQLActionManager::tryNext
             */
            BESDEBUG(SQL_NAME, ie.get_message() << " file:" << ie.get_file() << " row:" << ie.get_line() << endl);
            BESDEBUG(SQL_NAME, "SQLNextTypeAction: Element skipped" << endl);
            return NULL;
        }
        catch (BESError &e) {
            connector.close();
            throw SQLInternalFatalError(e.get_message(), e.get_file(), e.get_line());
        }
        catch (exception &e) {
            connector.close();
            throw SQLInternalFatalError(e.what(),
                                        __FILE__, __LINE__);
        }
        catch (...) {
            connector.close();
            throw SQLInternalFatalError("SQLNextTypeAction: Unknow error while connecting",
                                        __FILE__, __LINE__);
        }
        return NULL; // to avoid warning
    }

};

#endif /* SQLNEXTTYPEACTION_H_ */
