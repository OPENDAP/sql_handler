/*
 * SQLStaticActionList.h
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
 *  Created on: 17/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLSTATICACTIONLIST_H_
#define SQLSTATICACTIONLIST_H_
#include "SQLStaticAction.h"
#include "SQLActionList.h"

#include "DEM/SQLInternalFatalError.h"

/**
 * @brief Encode an ActionList of StaticAction(s)
 *
 * @todo this code should be merged with the
 * SQLObjectActionList code since they only
 * differ in storage type (see ACTION) and
 * in constructor and destructor.
 */
template <	class ARGS_TYPE,
			class OUT_TYPE=void >
class SQLStaticActionList : public SQLActionList<ARGS_TYPE,OUT_TYPE>{
	typedef typename SQLStaticAction<ARGS_TYPE,OUT_TYPE>::ACTION ACTION;
private:
	// not permitted
	SQLStaticActionList(){};

	/**
	 * The list
	 */
	ACTION *_list;

	// lists size
	size_t _size;

	// iterator list
	size_t _iterator;

public:
#if 0
// actually no need to implement the clone interface
//#if __CLONE__==1
	// clone pattern implementation
	SQLStaticActionList<ARGS_TYPE,OUT_TYPE>* clone(){
		return new SQLStaticActionList<ARGS_TYPE,OUT_TYPE>(this);
	}
	// clone pattern implementation
	SQLStaticActionList<ARGS_TYPE,OUT_TYPE>* create()throw (std::bad_alloc){
		return new SQLStaticActionList<ARGS_TYPE,OUT_TYPE>(this);
	}
#endif

	// copy
	SQLStaticActionList<ARGS_TYPE,OUT_TYPE>(
		SQLStaticActionList<ARGS_TYPE,OUT_TYPE> * ptr):
			_list(ptr->list),_size(ptr->_size),_iterator(ptr->_iterator){};

	/**
	 * @brief Execute the Next action in the list.
	 * Next is indexed by '_iterator'
	 * @see reset()
	 */
	virtual OUT_TYPE* doNext(ARGS_TYPE *in){
TESTDEBUG(SQL_NAME_TEST,"StaticActionList: doNext: "<<_iterator<<endl);
TESTDEBUG(SQL_NAME_TEST,"StaticActionList: args: "<<*in<<endl);
		return (_list[_iterator++])(in);
	}

	/**
	 * @brief check if there is an SQLAction<IN,OUT>.
	 * @return true if doNext() can do next action
	 */
	virtual bool hasNext(){
		if (_iterator<_size)
			return true;
		else
			return false;
	}

	/**
	 * @brief calculate the size of the ActionList.
	 * @return size_t representing the size
	 */
	virtual size_t getSize(){
		return _size;
	}

	/**
	 * @brief initialize/reset the next element to the
	 * first element in the list.
	 * @return false if something goes wrong, true otherwise.
	 */
	virtual bool reset(){
		_iterator=0;
		return true;
	}

	/**
	 * @brief default Constructor.
	 * @param list - array of ACTIONs function pointer
	 * @param size - array size
	 * @note: action array deletion (if needed) should
	 * be handled externally
	 */
	SQLStaticActionList(ACTION *list, size_t size):
		_list(list),_size(size),_iterator(0)
	{
		if (!list)
			throw SQLInternalFatalError("Unable to store a NULL ActionList",
					__FILE__,__LINE__);
TESTDEBUG(SQL_NAME_TEST,"CREATED: SQLStaticActionList"<<endl);
	};

	/**
	 * @brief Constructor for a single action list.
	 * @param list - reference to an ACTION function pointer
	 */
	SQLStaticActionList(ACTION list):
		_size(1),_iterator(0)
	{
		if (!list)
			throw SQLInternalFatalError("Unable to store a NULL ActionList",
					__FILE__,__LINE__);
		else{
			_list=new ACTION[1];
			_list[0]=list;
		}
	};


	virtual ~SQLStaticActionList(){

		if (_list)
			// handling special case
			if (_size == 1)
				delete [] _list;

TESTDEBUG(SQL_NAME_TEST,"DELETED: SQLStaticActionList"<<endl);
	};

};

#endif /* SQLSTATICACTIONLIST_H_ */
