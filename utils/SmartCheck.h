/*
 * WhatIs.h
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
 *  Created on: 09/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SMARTCHECK_H_
#define SMARTCHECK_H_
#include <list>
#include <vector>
#include <map>

#include <iostream>

namespace smart {
	/**
	 * @brief Class which implements methods to check
	 * STATICALLY (at compile time) the template
	 * parameter T (and K for maps). This is useful
	 * to specialize actions if functions and
	 * constructors which use template should know
	 * if the passed object is a pointer or not
	 * (for example).
	 * Supported checks are applied on a passed object
	 * of T{,K} type:
	 * - Is T a pointer
	 * - Is T a null pointer ()
	 * - Is T{,K} a Structure (list,vector,map)
	 * - Is T an array[]
	 */
	class SmartCheck{
	private:

		template<class T, class K=void>
		struct str { static const bool is = false; };

		template<class T>
		struct str<std::list<T> > { static const bool is = true; };

		template<class T>
		struct str<std::vector<T> >{ static const bool is = true; };

		template<class T,class K>
		struct str<std::map<T,K> >{ static const bool is = true; };

		template<class T>
		struct str<T[]> { static const bool is = false; };

		// checking for array type
		template<class T>
		struct array { static const bool is = false; };

		template<class T>
		struct array<T[]> { static const bool is = true; };

	public:

		/**
		 * IS TEMPLATE 'T' A POINTER?
		 */
		template<class T>
		static bool isPtr(T ptr) {
			return false;
		}

		template<class T>
		static bool isPtr(T* ptr) {
			return true;
		}

		template<class T>
		static bool isPtr(const T* ptr) {
			return true;
		}

		/**
		 * IS IT A NULL PINTER?
		 */
		template<class T>
		static bool isNull(T &p) {
			if (isPtr(p))
				return isNull(p);
			else
				return false;
		}

		template<class T>
		static bool isNull(T *p) {
				if (*p)
					return false;
				else
					return true;
		}

		/**
		 * IS TEMPLATE 'T' or 'T','K' A STRUCTURE?
		 */
		template<class T,class K>
		static bool isStr() {
			return str<T,K>::is;
		}

		template<class T>
		static bool isStr() {
			return str<T>::is;
		}
		/**
		 * IS TEMPLATE 'T' AN ARRAY?
		 */
		template<class T>
		static bool isArray() {
			return array<T>::is;
		}

	};
}
#endif /* SMARTCHECK_H_ */
