/*
 * clone.h
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
 *  Created on: 13/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef CLONE_H_
#define CLONE_H_

#include <exception>

namespace smart {
    /**
     * @brief The template interface to implement
     * to make clone of objects.
     * @see SQLAction
     */
    template<class T>
    struct Clone {
        virtual T *clone() = 0;//throw (std::bad_alloc)=0;

        virtual T *create() throw(std::bad_alloc) = 0;
    };
} // namespace smart

#endif /* CLONE_H_ */
