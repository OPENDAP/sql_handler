/*
 * SQLDynamicActionList.h
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
 *  Created on: 12/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLDYNAMICACTIONLIST_H_
#define SQLDYNAMICACTIONLIST_H_

#include "utils/SmartList.h"
#include "utils/SharedPtr.h"
#include "SQLAction.h"

// FIXME Removed jhrg 10/1/14 using namespace smart;
/**
 * This is a DynamicActionList which implement methods
 * defined by SQLActionList interface.
 *
 */
template <class ARGS_TYPE,class OUT_TYPE=void>
class SQLDynamicActionList:
	public SQLActionList<ARGS_TYPE,OUT_TYPE>,
	/**
	 * @todo: here some #ifdef to check for the 'smart' layer
	 */
#if __CLONE__==1

	public smart::SmartList<
					SQLAction<ARGS_TYPE,OUT_TYPE>,
					// override default SMART layer forcing to use the CLONE_LAYER
					smart::SharedPtr<
						SQLAction<ARGS_TYPE,OUT_TYPE> ,
						typename SmartList<SQLAction<ARGS_TYPE,OUT_TYPE> >::CLONE_LIST_INT> >{
	public:

		// clone pattern implementation
		SQLDynamicActionList<ARGS_TYPE,OUT_TYPE>* clone(){
			return new SQLDynamicActionList<ARGS_TYPE,OUT_TYPE>(this);
		}
		// clone pattern implementation
		SQLDynamicActionList<ARGS_TYPE,OUT_TYPE>* create()throw (std::bad_alloc){
			return new SQLDynamicActionList<ARGS_TYPE,OUT_TYPE>();
		}

		SQLDynamicActionList():
			smart::SmartList<
				SQLAction<ARGS_TYPE,OUT_TYPE> ,
				smart::SharedPtr<
					SQLAction<ARGS_TYPE,OUT_TYPE> ,
					typename SmartList<	SQLAction<ARGS_TYPE,OUT_TYPE> >::CLONE_LIST_INT>  >(){};

		SQLDynamicActionList(SQLDynamicActionList<ARGS_TYPE,OUT_TYPE > &list):
						smart::SmartList<
							SQLAction<ARGS_TYPE,OUT_TYPE> ,
							smart::SharedPtr< SQLAction<ARGS_TYPE,OUT_TYPE> ,
							typename SmartList<SQLAction<ARGS_TYPE,OUT_TYPE> >::CLONE_LIST_INT>  >(list){};

		SQLDynamicActionList(SQLDynamicActionList<ARGS_TYPE,OUT_TYPE > *list):
						smart::SmartList<
							SQLAction<ARGS_TYPE,OUT_TYPE> ,
							smart::SharedPtr< SQLAction<ARGS_TYPE,OUT_TYPE> ,
							typename SmartList<SQLAction<ARGS_TYPE,OUT_TYPE> >::CLONE_LIST_INT> >(list){};
#else

	public smart::SmartList<SQLAction<ARGS_TYPE,OUT_TYPE> >{
	public:
		SQLDynamicActionList():
			smart::SmartList< SQLAction<ARGS_TYPE,OUT_TYPE> >(){};

		SQLDynamicActionList(SQLDynamicActionList<ARGS_TYPE,OUT_TYPE > &list):
						smart::SmartList< SQLAction<ARGS_TYPE,OUT_TYPE> >(list){};

		SQLDynamicActionList(SQLDynamicActionList<ARGS_TYPE,OUT_TYPE > *list):
						smart::SmartList< SQLAction<ARGS_TYPE,OUT_TYPE> >(list){};

#endif

	virtual OUT_TYPE * doNext(ARGS_TYPE * in){
		if (_iterator!=this->end()){
			return (*(_iterator++)->get()).action(in);
		}
		else
			if (this->reset())
				return (*(_iterator)->get()).action(in);
			else
				throw SQLInternalError("Unable to reset the ActionList",
						__FILE__,__LINE__);

	};

	virtual bool hasNext(){
		if (_iterator!=this->end())
			return true;
		else
			return false;
	};

	virtual bool reset(){
		_iterator=this->begin();
		return true;
	};

	virtual size_t getSize(){
		return this->size();
	};

	virtual ~SQLDynamicActionList(){
TESTDEBUG(SQL_NAME_TEST,"DELETING: SQLDynamicActionList"<<endl);
	};

private:
	typename SQLDynamicActionList<ARGS_TYPE,OUT_TYPE >::iterator _iterator;

};


#endif /* SQLDYNAMICACTIONLIST_H_ */
