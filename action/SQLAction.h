/*
 * SQLAction.h
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
 *  Created on: 08/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLACTION_H_
#define SQLACTION_H_

// debug
#include "SQLDefinitions.h"

#include "DEM/SQLInternalError.h"
#include "DEM/SQLInternalFatalError.h"

#if __CLONE__ == 1
#include "utils/Clone.h"
#endif

/**
 * @brief This class is used to:
 * - Define the ACTION function prototype
 * - Is used as container instance for 'one' ACTION method
 * - override operator() to implement functor functionalities
 * - Can be inherited to create a stateful functor
 * @implements Clone (if __CLONE__ == 1)
 *
 * @see SQLCastAction (stateful action)
 * @see SQLFactoryComponent (stateful action)
 * @see ./handler/connector/{actions}
 * @see SQLStaticAction (stores static action)
 */
template<class ARGS_TYPE, class OUT_TYPE=void>
struct SQLAction
#if __CLONE__ == 1
    : smart::Clone<SQLAction<ARGS_TYPE,OUT_TYPE> > {
public:
    /**
     * @brief The clone interface implemented by this class.
     */
    typedef typename smart::Clone<SQLAction<ARGS_TYPE,OUT_TYPE> > CLONE_ACTION_INT ;
#if 0
    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLAction<ARGS_TYPE,OUT_TYPE>* create()throw (std::bad_alloc){
        return this->clone();
    };

    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLAction<ARGS_TYPE,OUT_TYPE> *clone(){
        return new SQLAction<ARGS_TYPE,OUT_TYPE>(this);
    }
#endif
#else
{
public:
#endif

    // DEFINITION
    typedef OUT_TYPE *(*ACTION)(ARGS_TYPE *);

    /**
     * @brief define the action represented by this instance
     * @return a pointer of OUT type (default is 'void*').
     */
    virtual OUT_TYPE *action(ARGS_TYPE *args) = 0;

    /**
     * @brief operator() enable an SQLAction to run
     * like an SQLAction<IN,OUT>::ACTION
     * (action)(argument)
     */
    virtual OUT_TYPE *operator()(ARGS_TYPE *args) {
        return ((this->action)(args));
    }

    /**
     * @brief constructor.
     */
    SQLAction() {}

    virtual ~SQLAction() {}
};


#endif /* SQLAction_H_ */
