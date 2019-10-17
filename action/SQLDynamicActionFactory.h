/*
 * SQLDynamicActionFactory.h
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

#ifndef SQLDYNAMICACTIONFACTORY_H_
#define SQLDYNAMICACTIONFACTORY_H_

#include <memory>

#include "utils/SmartMap.h"
#include "utils/Clone.h"

#if 0
#include "utils/SharedPtr.h"
#endif

#include "SQLActionFactory.h"
#include "SQLDynamicActionList.h"
#include "SQLAction.h"
#include "SQLActionList.h"

#include <BESInternalError.h>
#include <BESInternalFatalError.h>

/**
 * @brief This is a DynamicActionFactory useful to dynamically add
 * Actions to a map.
 */
template<class CODE_TYPE, class ARGS_TYPE, class OUT_TYPE=void>
class SQLDynamicActionFactory : public SQLActionFactory<CODE_TYPE, ARGS_TYPE, OUT_TYPE> {

protected:
#if __CLONE__ == 1

    // using SmartValueMap (to speed up search keys)
    typedef smart::SmartValueMap<
                        // key type
                        CODE_TYPE ,
                        // Value type
                        SQLDynamicActionList<ARGS_TYPE,OUT_TYPE>,
                        // override default SMART layer forcing to use the CLONE_LAYER
                        // using SMART LAYER:
//@todo check why SmartValueMap don't use clone to instantiate SQLDynamicActionList
                        std::shared_ptr<
                            // ON CONTAINER
                        SQLDynamicActionList<ARGS_TYPE,OUT_TYPE>,
                            // WITH CLONE LAYER
                            typename SmartValueMap<
                                CODE_TYPE,
                                SQLDynamicActionList<ARGS_TYPE,OUT_TYPE> >::CLONE_VMAP_INT >
        > action_map;
private:

    std::shared_ptr<
        action_map,
        typename SmartValueMap<
            CODE_TYPE,
            action_map >::CLONE_VMAP_INT> _action_map;
#else
private:

    typedef smart::SmartValueMap<CODE_TYPE,
            SQLDynamicActionList<ARGS_TYPE, OUT_TYPE> > action_map;

    std::shared_ptr<action_map> _action_map;

#endif

    SQLActionList<ARGS_TYPE, OUT_TYPE> &
    getActions(CODE_TYPE *code)
    throw(SQLInternalError) {
        if (_action_map) {
            typename action_map::iterator i = (_action_map)->find(*code);

            if (i != (_action_map)->end()) {
                (*(i->second)).reset();
                return (*(i->second));
                // I can't print the error since I don't know its type!
            }
            else
                throw SQLInternalError(
                        "SQLDynamicActionFactory: Cannot get the action."
                        " Be sure to map this code correctly.",
                        __FILE__, __LINE__);
        }
        else
            throw SQLInternalError(
                    "SQLDynamicActionFactory: Cannot get an action from a "
                    "not initialized SQLDynamicActionRunner map.",
                    __FILE__, __LINE__);
    }

public:


    void addAction(CODE_TYPE code, SQLAction<ARGS_TYPE, OUT_TYPE> *action)
    throw(BESInternalError) {
        if (!action)
            throw SQLInternalError(
                    "SQLDynamicActionFactory: Cannot store a NULL object"
                    " into the action map.", __FILE__, __LINE__);

        typename action_map::iterator i = _action_map->find(code);
        if (i != _action_map->end()) {
            BESDEBUG(SQL_NAME, "SQLDynamicActionFactory: Pushing "
                               "into an existing ActionLIST" << endl);
            ((*i).second)->push_back(action);
            // I can't print the error since I don't know its type!
        }
        else {
            SQLDynamicActionList<ARGS_TYPE, OUT_TYPE> *action_list =
                    new SQLDynamicActionList<ARGS_TYPE, OUT_TYPE>();

            action_list->push_back(action);

            // store a NEW Smart[list]
            _action_map->insert(code, action_list);
        }
    }

    /**
     * @brief Default constructor
     */
    SQLDynamicActionFactory<CODE_TYPE, ARGS_TYPE, OUT_TYPE>() :
            _action_map(new action_map()),
            SQLActionFactory<CODE_TYPE, ARGS_TYPE, OUT_TYPE>() {
    }


    virtual ~SQLDynamicActionFactory() {
    }
};

#endif /* SQLDYNAMICACTIONFACTORY_H_ */
