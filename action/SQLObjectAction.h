/*
 * SQLObjectAction.h
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
 *  Created on: 04/ago/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLOBJECTACTION_H_
#define SQLOBJECTACTION_H_

#include "action/SQLAction.h"

// free
#include <cstdlib>

/**
 * @brief class which extends an SQLAction to implement
 * a stateful action.
 * The state variable is a pointer which store the result of the
 * action until freeValue() is called or the class dtor is called.
 * @note: implementing the action you have to set the
 * buffer calling setVal(IN*) method before return.
 * <br>
 * @note: CLONE is re-implemented to keep the status of this
 * SQLAction
 */
template<class IN, class OUT>
class SQLObjectAction : public SQLAction<IN, OUT> {
public:
#if __CLONE__ == 1
    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLObjectAction<IN,OUT>* create()throw (std::bad_alloc) {
        return this->clone();
    };

    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLObjectAction<IN,OUT> *clone() {
        return new SQLObjectAction<IN,OUT>(*this);
    }
#endif

    /**
     * @brief Constructor
     * @param the reuse flag telling if the the
     * pointer can be deleted (false) or is reused
     * (true as default)
     */
    SQLObjectAction(bool reuse = true) :
            SQLAction<IN, OUT>(), _val(NULL), _reuse(reuse) {
    };

    /**
     * @brief COPY constructor
     * @param a reference to SQLObjectAction
     */
    SQLObjectAction(const SQLObjectAction<IN, OUT> &oa) :
            SQLAction<IN, OUT>(), _val(oa._val), _reuse(oa._reuse) {
    };

    /**
     * @brief dtor which delete the casted pointer
     * only if the _reuse flag is set to 1
     */
    virtual ~SQLObjectAction() {
        BESDEBUG(SQL_NAME, "DELETING: SQLObjectAction" << endl);
        if (!_reuse) freeValue();
    };

    /**
     * @brief get a copy of the casted value
     * @return  the pointer (by value) if not null
     * @throws SQLInternalError is casted value is NULL
     */
    OUT getCopy() {
        if (_val)
            return *_val;
        else
            throw SQLInternalError("SQLObjectAction: Unable to return copy of NULL value",
                                   __FILE__, __LINE__);
    }

    /**
     * @brief get the pointer
     * @return the stored pointer
     * or NULL if it is NULL
     */
    OUT *getVal() {
        if (_val)
            return _val;
        else
            return NULL;
    }

    /**
     * @brief delete val without check reuse flag
     * @note caution do not check the reuse flag
     */
    void freeValue() {
        if (_val) {
            BESDEBUG(SQL_NAME, "SQLObjectAction: deleting val" << endl);
            delete (_val);
            _val = 0;
        }
    }

    /**
     * @brief return a reference to the reuse flag
     * @note caution you can modify function behavior
     */
    bool &reusable() {
        return _reuse;
    }

// protected:
    void setVal(OUT *val) {
        if (this->_val && !this->_reuse) delete (this->_val);
        this->_val = val;
    }

private:
    OUT *_val;
    bool _reuse;

    SQLObjectAction() {
    };
};

/**
 * @brief specialization used to handle void* deletion,
 * free() will be used.
 * Class which extends a SQLAction to implement a stateful
 * action.
 * @note: implementing the action you have to set the
 * buffer calling setVal(IN*) method before return.
 * The state variable is a pointer which store the result of the
 * action until freeValue() is called or the class dtor is called.
 * <br>Note: CLONE is re-implemented to keep the status of this
 * SQLAction
 */
template<class IN>
class SQLObjectAction<IN, void> : public SQLAction<IN, void> {
public:
#if __CLONE__ == 1
    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLObjectAction<IN,void>* create()throw (std::bad_alloc) {
        return this->clone();
    };

    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLObjectAction<IN,void> *clone() {
        return new SQLObjectAction<IN,void>(*this);
    }
#endif

    /**
     * @brief Constructor
     * @param the reuse flag telling if the the
     * pointer can be deleted (false) or is reused
     * (true as default)
     */
    SQLObjectAction(bool reuse = true) :
            SQLAction<IN, void>(), _val(NULL), _reuse(reuse) {
    };

    /**
     * @brief COPY constructor
     */
    SQLObjectAction(const SQLObjectAction<IN, void> &oa) :
            SQLAction<IN, void>(), _val(oa._val), _reuse(oa._reuse) {
    };

    /**
     * @brief dtor which delete the pointer
     * only if the _reuse flag is set to 1
     */
    virtual ~SQLObjectAction() {
        BESDEBUG(SQL_NAME, "DELETING: SQLObjectAction" << endl);
        if (!_reuse) freeValue();
    };

    /**
     * @brief get the casted pointer
     * @return the stored pointer
     * or NULL if it is NULL
     */
    void *getVal() {
        if (_val)
            return _val;
        else
            return NULL;
    }

    /**
     * @brief delete val without check reuse flag
     * @note caution do not check the reuse flag
     */
    void freeValue() {
        if (_val) {
            BESDEBUG(SQL_NAME, "SQLObjectAction: freeing val" << endl);
            free(_val);
            _val = 0;
        }
    }

    /**
     * @brief return a reference to the reuse flag
     * @note caution you can modify function behavior
     */
    bool &reusable() {
        return _reuse;
    }

    // FIXME removed here and above 10/1/14 jhrg protected:
    void setVal(void *val) {
        if (this->_val && !this->_reuse) free(this->_val);
        this->_val = val;
    }

private:
    void *_val;
    bool _reuse;

    SQLObjectAction() : _val(0), _reuse(0) {
    };
};

#endif /* SQLOBJECTACTION_H_ */
