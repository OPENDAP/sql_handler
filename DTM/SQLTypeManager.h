/*
 * SQLTypeManager.h
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
 *  Created on: 22/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLTYPEMANAGER_H_
#define SQLTYPEMANAGER_H_

#include "action/SQLActionManager.h"

#include <BaseType.h>

/**
 * @brief This is an implementation of the SQLActionManager
 * which defines OUT_TYPE==_MERGE_TYPE==_JOIN_TYPE
 * and sets the final status to the last returned
 * object of the last executed ActionList.
 * <br>This is useful when:
 * ActionFactory is a
 *  	SQL{Static|Object|Dynamic}TypeFactory
 * which has:
 *  - ActionLists size == 1
 *  - stop() returns always true
 *
 * @see ODBCStaticTypeFactory
 * <br>
 * If you need more complex returning type implement
 * your own Manager starting from an
 * SQLActionManager<OUT_TYPE,MERGE_TYPE,JOIN_TYPE>
 */

class SQLTypeManager : public SQLActionManager<libdap::BaseType> {
private:
    // abstract  class
    SQLTypeManager() : SQLActionManager<libdap::BaseType>() {};
public:

    // wrap the doActions method to pass JOIN and MERGE functions
    template<class CODE_TYPE, class ARGS_TYPE>
    static libdap::BaseType *doActions(
            SQLActionFactory<CODE_TYPE, ARGS_TYPE, libdap::BaseType> &_af) {
        /*
         *  run using:
         *  - passed action_factory
         *  - implemented merge result function
         *  - implemented join result function
         *  - do not call getArgs for each action of the list
         *  @see SQLActionManager class
         */
        return SQLActionManager<libdap::BaseType>::doActions(_af,
                                                             &merge,
                                                             &join,
                                                             false);
    }

    // SQLActionManager<...>::JOIN type function
    static libdap::BaseType *join(libdap::BaseType *prev, libdap::BaseType *actual) {
#if __TESTS__ == 1
        if (actual)
            BESDEBUG(SQL_NAME,"_JOIN: "<<std::endl);
        else
            BESDEBUG(SQL_NAME,"!_JOIN: "<<std::endl);
#endif
        return actual;
//		no delete since address pointer is used
//			smart::Delete::smartDelete<libdap::BaseType*>(actual);
    }

    // SQLActionManager<libdap::BaseType>::MERGE type function
    static libdap::BaseType *merge(libdap::BaseType *prev, libdap::BaseType *actual) {
#if __TESTS__ == 1
        if (actual)
            BESDEBUG(SQL_NAME,"_MERGE: "<<std::endl);
        else
            BESDEBUG(SQL_NAME,"!_MERGE: "<<std::endl);
#endif
        return actual;
//		no delete since address pointer is used
//			smart::Delete::smartDelete<libdap::BaseType*>(actual);
    }

};

#endif /* SQLTYPEMANAGER_H_ */
