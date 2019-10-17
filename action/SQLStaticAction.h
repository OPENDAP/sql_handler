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
 *  Created on: 03/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLSTATICACTION_H_
#define SQLSTATICACTION_H_

#include "DEM/SQLInternalError.h"
#include "DEM/SQLInternalFatalError.h"

#include "SQLAction.h"

#if __CLONE__ == 1
#include "utils/Clone.h"
#endif

/**
 * @brief This class is used to:
 * - Provide the ACTIONs definition
 * - Is used as container instance for 'one' STATIC ACTION function.
 * @implements Clone
 */
template<class ARGS_TYPE, class OUT_TYPE=void>
struct SQLStaticAction : public SQLAction<ARGS_TYPE, OUT_TYPE>
#if __CLONE__ == 1
    , smart::Clone<SQLStaticAction<ARGS_TYPE,OUT_TYPE> > {
public:
    /**
     * @brief The clone interface implemented by this class.
     */
    typedef typename smart::Clone<SQLStaticAction<ARGS_TYPE,OUT_TYPE> > CLONE_ACTION_INT ;

    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLStaticAction<ARGS_TYPE,OUT_TYPE>* create()throw (std::bad_alloc){
        return this->clone();
    };

    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLStaticAction<ARGS_TYPE,OUT_TYPE> *clone(){
        return new SQLStaticAction<ARGS_TYPE,OUT_TYPE>(this);
    }

#else
{
public:
#endif


    /**
     *  @brief Define the ACTION function
     *  NOTE: using typedef we actually cannot use 'throw'
     */
    typedef OUT_TYPE *(*ACTION)(ARGS_TYPE *);//throw (SQLInternalError,SQLInternalFatalError)

    /**
     * @brief execute the action passed to the constructor.
     * @return a pointer of OUT type (default is 'void*').
     */
    virtual OUT_TYPE *action(ARGS_TYPE *args) throw(SQLInternalError, SQLInternalFatalError) {
        return (*_action)(args);
    }

    /**
     * @brief copy constructor.
     * @throw SQLInternalError if 'sql_action.action' is a NULL pointer
     */
    SQLStaticAction(SQLStaticAction<ARGS_TYPE, OUT_TYPE> *sql_action)
    throw(SQLInternalError):
            _action(sql_action->getAction()),
            SQLAction<ARGS_TYPE, OUT_TYPE>() {
        if (!_action)
            throw SQLInternalError("Unable to build an SQLAction using a NULL "
                                   "pointer method", __FILE__, __LINE__);
    };

    /**
     * @brief Main constructor
     * @param the ACTION method to use as action().
     * Action is defined as:
     * 	typedef OUT_TYPE*(*ACTION)(IN);
     * @throw SQLInternalError if 'action' is a NULL pointer
     */
    SQLStaticAction(ACTION action)
    throw(SQLInternalError):
            _action(action),
            SQLAction<ARGS_TYPE, OUT_TYPE>() {
        if (!_action)
            throw SQLInternalError("Unable to build an SQLAction using a NULL "
                                   "pointer method", __FILE__, __LINE__);
    };

    void resetAction(ACTION action) throw(SQLInternalError) {
        if (!action)
            throw SQLInternalError("Unable to update an SQLAction using a NULL "
                                   "pointer method", __FILE__, __LINE__);
        else
            _action = action;
    }

    inline ACTION getAction() {
        return _action;
    }

    virtual ~SQLStaticAction() {};

protected:

private:
    // action to do
    ACTION _action;

    /**
     * not public, argument should be set
     */
    SQLStaticAction() {};

};

#endif /* SQLSTATICACTION_H_ */
