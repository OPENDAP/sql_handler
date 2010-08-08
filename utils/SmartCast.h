/*
 * SmartCast.h
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
 *  Created on: 11/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SMARTCAST_H_
#define SMARTCAST_H_

// DEBUG
#include "SQLDefinitions.h"

#include <exception>

#include "Clone.h"

namespace smart {

	/**
	 * @brief Structure to provide a getPtr() function
	 * which is able to return a pointer copy
	 * starting from a unknown type object.
	 * <br>NOTE:if __CLONE__ flag is active
	 * (set it to 1) the specialization
	 * structure will be able to provide a
	 * copy also for T* see comments why this
	 * is not possible without CLONE.
	 */
	template <class T, class _CLONE=void>
	struct SmartCast {
		static T* getPtr(){
TESTDEBUG(SQL_NAME, "SmartCast: &-CONSTRUCTOR:----"<<std::endl);
			return new T();
		}
		/**
		 * @brief constructor: will NOT make a copy of the passed pointer.
		 * This is done since we cannot allocate abstract T classes.
		 * You may use this constructor passing new copy of the variable.
		 * Use this constructor carefully since 'k' may not be stored
		 * into the heap!
		 * const int *i=new const int();
		 * const int &r=5;
		 * i=&r;
		 * delete i;
		 */
		static T* getPtr(T* k){
TESTDEBUG(SQL_NAME,"SmartCast: *-CONSTRUCTOR----"<<std::endl);
			/**
			 *  does T implement copy constructor?
			 *  If so, which type?
			 *  T n=new T(*K);
			 *  T n=new T(K);
			 *  use CLONE
			 */
			return k;

		}

		/*
		 * build a new object using
		 * copy constructor
		 */
		static T* getPtr(T k){
			return new T(k);
		}

		/**
		 * @brief constructor: will NOT make a copy of the passed pointer.
		 * This is done since we cannot allocate abstract T classes.
		 * You may use this constructor passing new copy of the variable.
		 * Use this constructor carefully since 'k' may not be stored
		 * into the heap!
		 * const int *i=new const int();
		 * const int &r=5;
		 * i=&r;
		 * delete i;
		 */
		static const T* getPtr(const T* k){
TESTDEBUG(SQL_NAME,"SmartCast: *-CONST-CONSTRUCTOR----"<<std::endl);
			return k;
		}

		/*
		 * build a new object using
		 * copy constructor
		 */
		static const T* getPtr(const T& k){
TESTDEBUG(SQL_NAME,"SmartCast: &-CONST-CONSTRUCTOR----"<<std::endl);
			return new const T(k);
		}

	};

	/**
	 * @brief if clone.h is defined we could provide safe
	 * clone layer allocation functionalities.
	 */
	template <class T >
	struct SmartCast < T, smart::Clone<T> > {

		static T* getPtr(T* k){
TESTDEBUG(SQL_NAME,"SmartCast: *-CLONE----"<<std::endl);
			return k->clone();
		}

		static T* getPtr(T& k){
TESTDEBUG(SQL_NAME,"SmartCast: &-CLONE----"<<std::endl);
			return k.clone();
		}

		static const T* getPtr(const T* k){
TESTDEBUG(SQL_NAME,"SmartCast: *-CLONE----"<<std::endl);
			return const_cast<const T*>(k->clone());
		}
		static const T* getPtr(const T& k){
TESTDEBUG(SQL_NAME,"SmartCast: &-CLONE----"<<std::endl);
			return const_cast<const T*>(k->clone());
		}

	};
}


#endif /* SMARTCAST_H_ */
