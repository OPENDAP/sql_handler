/*
 * Delete.h
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

#ifndef DELETE_H_
#define DELETE_H_

#include "SmartCheck.h"

namespace smart {

/**
 * @todo: this class is never used into SQLHandler
 * so it has very poor test practice
 * @todo: make tests and fix some methods
 *
 * @deprecate use smart containers instead
 *
 * @brief Delete class contains usefull deletion
 * methods to erase pointer object from stl containers
 */
    class Delete {
    public:

        template<class D>
        static void smartDelete(D *ptr) {
            if (SmartCheck::isArray<D *>())
                delete[] ptr;
            else
                delete ptr;
        }

# if 1

        // unused
        template<class L>
        static void erase_list(list <L> &_list) {
            _list.erase(_list.begin(), _list.end());
        }


        /**
         *
         */
        template<class L>
        static void delete_list(list <L> &_list) {
            std::_List_node_base *actual = NULL;
            for (typename list<L>::iterator i = _list.begin(); i != _list.end(); i++) {
                if (actual != NULL) {
                    delete actual;
                    actual = i._M_node;
                }
                else
                    actual = i._M_node;
            }
            if (actual != NULL) {
                delete actual;
            }
        }

#endif

# if 1

        template<class P1, class P2>
        static void erase_map(map <P1, P2> &_map) {
            _map.erase(_map.begin(), _map.end());
        }

        /**
         *
         */
        template<class P1, class P2>
        static void delete_map(map <P1, P2> &_map) {
            std::_Rb_tree_node_base *actual = NULL;
            for (typename map<P1, P2>::iterator i = _map.begin(); i != _map.end(); i++) {
                if (actual != NULL) {
                    delete actual->_M_left;
                    delete actual->_M_right;
//				delete actual;
                    actual = i._M_node;
                }
                else
                    actual = i._M_node;
            }
            if (actual != NULL) {
                delete actual->_M_right;
                delete actual->_M_left;
            }
        }

#endif
    };

} // namespace smart

#endif /* DELETE_H_ */
