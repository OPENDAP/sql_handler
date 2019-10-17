/*
 * SmartList.h
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

#ifndef SMARTLIST_H_
#define SMARTLIST_H_

#include <memory>

#if 0
#include "SharedPtr.h"
#endif

#include "SmartList.h"
#include "Clone.h"

#include <list>

namespace smart {

/**
 * @brief SmartList implements all the main methods
 * of an ordinary stl list<T> using shared pointer.
 * Doing so you can safely store pointers into the
 * list without take care of their deletion.
 * @implements if __CLONE__ is set to 1 smart::Clone
 * @see std::shared_ptr
 * @see smart::Clone
 * @see smart::SmartMap
 * @see smart::SmartValueMap
 */
    template<class K, class T = std::shared_ptr<K> >
    class SmartList
#if __CLONE__ == 1
        :public smart::Clone< smart::SmartList<K,T> > {
        public:
            typedef smart::Clone<K > CLONE_LIST_INT;

            SmartList<K,T>* clone() {
                return new SmartList<K,T>(this);
            }

            SmartList<K,T>* create()throw (std::bad_alloc) {
                return new SmartList<K,T>();
            }
#else
    {
    public:
#endif
        typedef typename std::list<T>::const_iterator const_iterator;
        typedef typename std::list<T>::iterator iterator;

        SmartList(smart::SmartList<K, T> *the_list) :
                _list(the_list->getList()) {
            //if (SmartCheck::isPtr<K>())
            //	throw new std::exception();
        };

        SmartList(smart::SmartList<K, T> &the_list) :
                _list(the_list.getList()) {
        };

        SmartList() :
                _list(std::list<T>()) {
        };

#if 1

        inline void push_back(K *k) {
            _list.push_back(T(k));
        }

#endif
#if 1

        inline void push_back(K &k) {
            _list.push_back(T(k));
        }            //T(k)); }
#endif

        inline void push_front(K *k) {
            _list.push_front(T(k));
        }

        inline void push_front(K &k) {
            _list.push_front(T(k));
        }

        inline void pop_back() {
            _list.pop_back();
        }

        inline void pop_front() {
            _list.pop_front();
        }

#if 0
        inline void push_back(K &k)
        {   _list.push_back(T(WhatIs::getPtr<K>(k)));}
#endif

#if 0
        void push_front(K &k) {
            _list.push_front(T(WhatIs::getPtr<K>(k)));
        }
#endif

        inline iterator begin() {
            return _list.begin();
        }

        inline const_iterator begin() const {
            return _list.begin();
        }

        inline iterator end() {
            return _list.end();
        }

        inline const_iterator end() const {
            return _list.end();
        }

        inline bool empty() {
            return _list.empty();
        }

        inline size_t size() {
            return _list.size();
        }

        virtual ~SmartList() {
        };

        inline const std::list<T> &getList() {
            return _list;
        }

    protected:
        std::list<T> _list;
    };

#if 1

/**
 * @brief Please avoid using templates with & or *
 */
    template<class K>
    class SmartList<K &, std::shared_ptr<K &> > {
        SmartList();

    public:
        virtual ~SmartList() {
        };
    };

#endif

#if 1

/**
 * @brief Please avoid using templates with & or *
 */
    template<class K>
    class SmartList<const K *, std::shared_ptr<const K *> > {
        SmartList() {
        };
    public:
        virtual ~SmartList() {
        };
    };

#endif

} // namespace smart

#endif /* SMARTLIST_H_ */
