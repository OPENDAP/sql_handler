/*
 * SharedPtr.h
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
 *  Created on: 08/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SHAREDPTR_H_
#define SHAREDPTR_H_

// checks
#include <cstddef> // __GLIBCXX__, _HAS_TR1

// GNU C++ or Intel C++ using libstd++.
//
#if defined (__GNUC__) && __GNUC__ >= 4 &&  defined (__GLIBCXX__)
#  include <tr1/memory>
//
// IBM XL C++.
//
#elif defined (__xlC__)
#if __xlC__ >= 0�0900
#  define __IBMCPP_TR1__
#  include <memory>
#endif

// VC++ or Intel C++ using VC++ standard library.
//
#elif defined (_MSC_VER) && (_MSC_VER == 1500 && defined (_HAS_TR1) || _MSC_VER > 1500)
#  include <memory>
//
// Boost fall-back.
//
#else
#  include <memory>
#endif

//#include <new>

#include "SmartCheck.h"
#include "SmartCast.h"

#include "Clone.h"


#include <iostream>
// FIXME Removed jhrg 10/1/14 using namespace std;

namespace smart{

	/**
	 * @brief this is an alternative implementation
	 * of the std::tr1::enable_shared_from_this<T>
	 * template class.
	 * Used as layer between SQLHandler and desired
	 * shared_ptr implementation
	 */
	template <	// the Type to store (without '*' or '&'
				class T,
				// the clone interface if provided
				class CLONE=void,
						//smart::Clone<T>,
				// the shared_ptr impl
				class SMART=std::shared_ptr<T> >
	class SharedPtr : public SMART {
		private:
			/**
			 * to avoid array pointer creation
			 */
			static void* operator new[](size_t size){
				throw std::bad_alloc();
				/**
				 * we should inherit from array_ptr smart_array
				 */
			};

		public:
			//using SMART::operator==;
			//using SMART::operator<<;
			//using SMART::operator<=;
			//using SMART::operator>=;
			//using SMART::operator!=;
			//using SMART::operator->;
			//using SMART::operator*;
			using SMART::operator=;
			//using SMART::operator();
			//using SMART::operator new;
#if 0
			/**
			 * shouldn't be private?
			 * to avoid the pointer creation
			 *
			 * NO, Actually it is needed.
			 * carlo cancellieri, 10 giu 2010
			 *
			 */

			static void* operator new (size_t size){
				throw std::bad_alloc();
			};
#endif
#if 1
		SharedPtr():
			SMART(SmartCast<T,CLONE>::getPtr())
			{};
#endif
#if 1
		/**
		 * @brief constructor: will make a copy of the passed pointer.
		 */
		explicit SharedPtr(T &t):
			SMART(SmartCast<T,CLONE>::getPtr(t))
			{};
#endif
#if 1 // !__CLONE__
		/**
		 * @brief constructor: will NOT make a copy of the passed pointer.
		 * This is done since we cannot allocate abstract T classes.
		 * You may use this constructor passing new copy of the variable.
		 * Be careful since't' could not be into the heap!
		 */
		explicit SharedPtr(T *t):
			SMART(SmartCast<T,CLONE>::getPtr(t))
			{};
#endif
		virtual ~SharedPtr(){
TESTDEBUG(SQL_NAME_TEST,"SharedPtr: Use_count ->"<<this->use_count()<<endl);
TESTDEBUG(SQL_NAME_TEST,"DELETING: SharedPtr:"<<typeid(this).name()<<std::endl);
TESTDEBUG(SQL_NAME_TEST,"SharedPtr: Remain_count ->"<<this->use_count()-1<<endl);
		};
	};

#if 1
	/**
	 * @brief Please avoid using templates with & or *
	 */
	template <class T >
	class SharedPtr < T& , std::shared_ptr< T& > >{
	private:
		SharedPtr(){};
		explicit SharedPtr(T *t){};
		explicit SharedPtr(T &t){};
	private:
		virtual ~SharedPtr(){};
	};
#endif
#if 1
	/**
	 * @brief Please avoid using templates with & or *
	 */
	template <class T >
	class SharedPtr < T* , std::shared_ptr< T *> >{
	private:
		SharedPtr(){};
		explicit SharedPtr(T &t){};
	public:
		virtual ~SharedPtr(){};
	};
#endif

};

#endif /* SHAREDPTR_H_ */
