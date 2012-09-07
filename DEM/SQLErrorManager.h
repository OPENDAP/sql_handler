/*
 * SQLErrorManager.h
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
 *  Created on: 28/mag/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLERRORMANAGER_H_
#define SQLERRORMANAGER_H_

#include "SQLDefinitions.h"
#include "SQLCheckPoint.h"

#include "action/SQLActionFactory.h"
#include "action/SQLActionManager.h"

#include <BESInternalFatalError.h>

/**
 * @brief SPECIALIZATION Implements an ActionManager
 * to build an ErrorManager which fix
 * - JOIN==MERGE==OUT_TYPE
 * so the returning type (OUT_TYPE) will be the
 * same type of the SQLAction composing
 * SQLActionList managed by this SQLActionManager.
 *
 */
template<class JOIN = void>
class SQLErrorManager: protected SQLActionManager<JOIN, JOIN, JOIN> {
public:

    SQLErrorManager() :
            SQLActionManager<JOIN, JOIN, JOIN>()
    {
#if 0
        /**
         *  This is done by the SQLModule::initialize
         *
         *  initialize check point reading
         *  configuration file.
         */
        //SQLCheckPoint::init();
#endif
    }
    ;

    /**
     * @brief Act as a trigger starting Actions only
     * if corresponding CheckPoint is active.
     * @param check @see SQLCheckPoint
     * @param error_factory
     * @return latest action result of JOIN type
     * @throw @see SQLActionManager::doActions
     */
    template<class ERROR_TYPE, class ARGS_TYPE>
    static JOIN *trigger(_SQLH_CHECKS check, SQLActionFactory<ERROR_TYPE, ARGS_TYPE, JOIN> &error_factory)
    {
        // trigger activation
        if (check == _SQLH_ON_ALWAYS) {
            BESDEBUG(SQL_NAME, "SQLErrorManager: This check point is forced, running error checks."<<endl);
            /*
             *  run using:
             *  - passed error_factory
             *  - implemented merge result function
             *  - implemented join result function
             *  - do not call getArgs for each action of the list
             *  @see SQLActionManager doc
             */
            return doActions(error_factory, &merge, &join, false);
        }
        else if (SQLCheckPoint::check(check)) {
            BESDEBUG(SQL_NAME, "SQLErrorManager: This check point is active, running error checks."<<endl);
            /*
             *  run using:
             *  - passed error_factory
             *  - implemented merge result function
             *  - implemented join result function
             *  - do not call getArgs for each action of the list
             *  @see SQLActionManager doc
             */
            return doActions(error_factory, &merge, &join, false);
        }
        else
            BESDEBUG(SQL_NAME, "SQLErrorManager: This check point isn't active"<<endl);
        return NULL;
    }

    /**
     * @brief SQLActionManager<...>::JOIN type function
     */
    static JOIN * join(JOIN * prev, JOIN * actual)
    {
#if __TESTS__==1
        if (actual)
        BESDEBUG(SQL_NAME,"_JOIN: "<<std::endl);
        else
        BESDEBUG(SQL_NAME,"!_JOIN: "<<std::endl);
#endif
        return actual;
        // no delete
    }

    /**
     * @brief SQLActionManager<libdap::BaseType>::MERGE type function
     */
    static JOIN * merge(JOIN * prev, JOIN * actual)
    {
#if __TESTS__==1
        if (actual)
        BESDEBUG(SQL_NAME,"_MERGE: "<<std::endl);
        else
        BESDEBUG(SQL_NAME,"!_MERGE: "<<std::endl);
#endif
        return actual;
        // no delete
    }
};

#endif /* SQLERRORMANAGER_H_ */
