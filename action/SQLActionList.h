/*
 * SQLActionList.h
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
 *  Created on: 07/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLACTIONLIST_H_
#define SQLACTIONLIST_H_

// DEBUG
#include "SQLDefinitions.h"

/**
 * @brief Interface which defines methods to
 * implement to build a working ActionList.
 * An ActionList is a collection of objects
 * which implements a method defined as:
 *  OUT_TYPE * action(ARGS_TYPE*)
 * <br>Note: This is typically defined in a
 * SQLAction.
 * This interface defines typical methods of
 * a list such as:
 * - hasNext
 * - getSize
 * - reset
 * <br>define also a doNext method instead of
 * a getNext to free the implementing class
 * to define its own action() storage type.
 *
 */
template<class ARGS_TYPE, class OUT_TYPE=void>
class SQLActionList {
public:
    SQLActionList() {};
    //SQLActionList(SQLActionList<IN,OUT> *ptr){};

    /**
     * UPDATE: 16 Jun 2010
     * commented out:
     * 		virtual SQLAction<IN,OUT>& getNext()=0;
     * Substituted by:
     * 		virtual OUT * doNext(IN * in)=0;
     * This is done to free the list storage type:
     * Static way -> list of ACTION
     * Dynamic way -> list of SQLAction
     */
    /**
     * @brief run next action in the list.
     * @return an OUT value (default is 'void*')
     */
    virtual OUT_TYPE *doNext(ARGS_TYPE *in) = 0;


    /**
     * @brief check if there is an SQLAction<IN,OUT>.
     * @return true if getNext() can return next action
     */
    virtual bool hasNext() = 0;

    /**
     * @brief calculate the size of the ActionList.
     * @return size_t representing the size
     */
    virtual size_t getSize() = 0;

    /**
     * @brief initialize/reset the next element to the
     * first element in the list.
     * @return false if something goes wrong.
     */
    virtual bool reset() = 0;

    virtual ~SQLActionList() {
    }

};


#endif /* SQLACTIONLIST_H_ */
