/*
 * SQLITETypes.h
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
 *  Created on: 06/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLITETYPES_H_
#define SQLITETYPES_H_
/**
 * GUIDELINES:
 * DO NOT USE:
 *  - & (reference)
 *  - * (pointer)
 *  for types.
 *
 *  Some exception are accepted
 *  such as char* is accepted for
 *  MSG_TYPE as string.
 *
 */

/**
 * @todo change this type accordingly
 * with you ODBC API design
 */
typedef int SQL_TYPE;
/**
 * @todo change this type accordingly
 * with you ODBC API design
 *
 * @note this void will be used as
 * void*
 */
typedef void ODBC_TYPE;

#endif /* SQLITETYPES_H_ */
