/*
 * SmartMap.h
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

#ifndef SMARTMAP_H_
#define SMARTMAP_H_

#include "SharedPtr.h"
#include "SmartCast.h"

#include <map>

namespace smart {

	/**
	 * @brief this is the Smart comparator.
	 * NOTE: it use of the operator*() so be sure that
	 * used SmartPtr implement or inherit this one.
	 */
	template<typename _St>
	struct less : public binary_function<_St, _St, bool>
	{
		bool
		operator()(const _St& __x, const _St& __y) const
			{return (*(__x) < *(__y)); }
	};

	/**
	 * @brief Map to store object using smart pointers.
	 * Note that this class uses smart pointers for
	 * key and values.
	 * If you want to use smart management only for
	 * values, use SmartValueMap.
	 * @implements if __CLONE__ is set to 1 smart::Clone
	 * @see smart::SharedPtr
	 * @see smart::Clone
	 * @see smart::SmartList
	 * @see smart::SmartValueMap
	 */
	template <
		class K,
		class V,
		class T1=smart::SharedPtr<K>,
		class T2=smart::SharedPtr<V> >
	class SmartMap
#if __CLONE__==1
	:public smart::Clone< smart::SmartMap<K,V,T1,T2> > {
	public:
		typedef smart::Clone<smart::SmartMap<K,V,T1,T2> > CLONE_MAP_INT;

		SmartMap<K,V,T1,T2>* clone(){
			return new SmartMap<K,V,T1,T2>(this);
		}

		SmartMap<K,V,T1,T2>* create()throw (std::bad_alloc){
			return new SmartMap<K,V,T1,T2>();
		}

#else
	{
	public:
#endif

		typedef typename std::map<T1,T2>::const_iterator const_iterator;
		typedef typename std::map<T1,T2>::iterator iterator;

	#if 0
		// @todo: CHECK
		V& operator[](const K& __k)
		{ return *(_map[T1(__k)]); }

		size_t size()
		{ return _map.size(); }

		// todo: CHECK
		const V&
		at(const K& __k) const
		{ const T1 &key=T1(__k);
			const_iterator __i = _map.lower_bound(key);
			if (__i == _map.end() || _map.key_comp()(key, (*__i).first))
				__throw_out_of_range(__N("SmartMap::at"));
			return *((*__i).second);
		};
	#endif

	#if 0

		/**
		 * Commented out since these insert function aren't
		 *  'safe' in this context
		 * we may check if the V parameter is passed as pointer
		 * or not.
		 *
		 * UPDATE: 14 Jun 2010 carlo cancellieri
		 * Check is done buy SharedPtr using constructor T(ptr)
		 * Still commented out since actually not used/tested
		 */
		std::pair<iterator, bool>
		insert(std::pair<K,V> &__x)
		{ return _map.insert(
						std::make_pair(
								T1(__x.first),
								T2(__x.second)));
		};
	#endif

		/**
		 * @brief insert into the map.
		 * Please be sure to allocate V and K arguments
		 * into the HEAP (using 'new' operator).
		 *
 		 * UPDATE: 14 Jun 2010 carlo cancellieri
		 * Check is done by SharedPtr using constructor T(ptr)
		 * @see SharedPtr, SmartCast
		 */
		inline std::pair<iterator, bool>
		insert(K &k, V &v)
		{ return _map.insert(
					std::make_pair(T1(k),T2(v))); }

		inline std::pair<iterator, bool>
		insert(K *k, V *v)
		{ return _map.insert(
					std::make_pair(T1(k),T2(v))); }

		inline std::pair<iterator, bool>
		insert(K &k, V *v)
		{ return _map.insert(
					std::make_pair(T1(k),T2(v))); }

		inline std::pair<iterator, bool>
		insert(K *k, V &v)
		{ return _map.insert(
					std::make_pair(T1(k),T2(v))); }
		/**
		 * @return number of erased object
		 */
		inline size_t
		erase(K &k)
		{ return _map.erase(T1(k)); }

		inline void
		erase(SmartMap::iterator k)
		{ return _map.erase(k); }

		inline iterator
		find(K *k)
		{ return _map.find(T1(k)); }

		inline iterator
		find(K &k)
		{ return _map.find(T1(k)); }

		inline const_iterator
		find(K *k) const
		{ return _map.find(T1(k)); }

		inline const_iterator
		find(K &k) const
		{ return _map.find(T1(k)); }

		inline iterator
		end()
		{ return _map.end(); }

		inline iterator
		begin()
		{ return _map.begin(); }

		bool
		empty()
		{ return _map.empty(); }

		size_t
		size()
		{ return _map.size(); }

		explicit SmartMap()
		{};

		explicit SmartMap(smart::SmartMap<T1,T2> *the_map):
				_map(the_map->getMap())
		{};

		virtual ~SmartMap(){};

		inline const std::map<T1, T2 >& getMap()
		{ return &_map; };

	protected:
		std::map<T1, T2 ,smart::less<T1> > _map;
	};


#if 1

	/**
	 * @brief Please avoid using templates with & or *
	 */
	template <
		class K,
		class V >
	class SmartMap< K& , V&,
					smart::SharedPtr<K&>,
					smart::SharedPtr<V&> > {
		explicit SmartMap(){};
	public:
		virtual ~SmartMap(){};
	};

	/**
	 * @brief Please avoid using templates with & or *
	 */
	template <
			class K,
			class V >
		class SmartMap< K& , V,
						smart::SharedPtr<K&>,
						smart::SharedPtr<V> > {
			explicit SmartMap()
		//		if (WhatIs::isPtr<V>())
		//			throw new std::exception();
			{};
		public:
			virtual ~SmartMap(){};
		};

	/**
	 * @brief Please avoid using templates with & or *
	 */
	template <
			class K,
			class V >
		class SmartMap< K , V&,
						smart::SharedPtr<K>,
						smart::SharedPtr<V&> > {
			explicit SmartMap()
			{};
		public:
			virtual ~SmartMap(){};
		};
#endif

#if 1
	/**
	 * @brief This map stores object using smart pointers.
	 * Note that keys are NOT stored as smart object. This
	 * speed up find searches and the resulting size of the
	 * map is smaller than the SmartMap one.
	 * @implements if __CLONE__ is set to 1 smart::Clone
	 * @see smart::SharedPtr
	 * @see smart::Clone
	 * @see smart::SmartList
	 * @see smart::SmartMap
	 */
	template <
		class K,
		class V,
		class T=smart::SharedPtr<V> >
	class SmartValueMap
#if __CLONE__==1
	:public smart::Clone< smart::SmartValueMap<K,V,T> > {
	public:
		typedef smart::Clone< smart::SmartValueMap<K,V,T> > CLONE_VMAP_INT;

		SmartValueMap<K,V,T>* clone(){
			return new SmartValueMap<K,V,T>(this);
		}

		SmartValueMap<K,V,T>* create()throw (std::bad_alloc){
			return new SmartValueMap<K,V,T>();
		}

#else
	{
	public:
#endif

		typedef typename std::map<K,T>::const_iterator const_iterator;
		typedef typename std::map<K,T>::iterator iterator;

		inline std::pair<iterator, bool>
		insert(std::pair<K,V> &__x)
		{ return _map.insert(
					std::make_pair(__x.first,T(__x.second))); }

		inline std::pair<iterator, bool>
		insert(K &k, V &v)
		{ return _map.insert(
					std::make_pair(k,T(v))); }

		inline std::pair<iterator, bool>
		insert(K &k, V *v)
		{ return _map.insert(
					std::make_pair(k,T(v))); }

		/**
		 * @return number of erased object
		 */
		inline size_t
		erase(K &k)
		{ return _map.erase(T1(k)); }

		inline void
		erase(SmartValueMap::iterator k)
		{ return _map.erase(k); }

		inline iterator
		find(K &key)
		{ return _map.find(key); }

		inline const_iterator
		find(K &key) const
		{ return _map.find(key); }

		inline iterator
		end()
		{ return _map.end(); }

		inline iterator
		begin()
		{ return _map.begin(); }

		bool
		empty()
		{ return _map.empty(); }

		size_t
		size()
		{ return _map.size(); }

		explicit SmartValueMap():_map(std::map<K, T >())
		{}

		explicit SmartValueMap(SmartValueMap<K,V,T> *the_map):
			_map(the_map->getMap())
		{}

		explicit SmartValueMap(SmartValueMap<K,V,T> &the_map):
			_map(the_map.getMap())
		{}

		virtual ~SmartValueMap(){}

		inline const std::map<K, T > &getMap()
		{ return _map; }

	protected:
		std::map<K, T > _map;
	};
#endif

}

#endif /* SMARTMAP_H_ */
