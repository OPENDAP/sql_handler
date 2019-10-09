/*
 * ODBCTypes.h
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

#ifndef ODBCTYPES_H_
#define ODBCTYPES_H_

#include <string>

/**
 *  ERRORS
 *
 *  unixODBC (form 'sqltypes.h')
 *
 * typedef signed short int   SQLSMALLINT;
 * typedef SQLSMALLINT     SQLRETURN;
 */
#include <sqltypes.h>
// SQLH
typedef SQLRETURN ERROR_TYPE;
//typedef SQLCHAR [] MSG_TYPE; // should be
typedef std::string MSG_TYPE; // workaround
//typedef char* MSG_TYPE; // workaround


/**
 *  TYPES
 *
 *  ODBC (from 'sql.h')
 *
 *  SQL data type codes
 *  #define	SQL_UNKNOWN_TYPE	0
 *  #define SQL_CHAR            1
 *  #define SQL_NUMERIC         2
 *  #define SQL_DECIMAL         3
 *  #define SQL_INTEGER         4
 *  #define SQL_SMALLINT        5
 *  #define SQL_FLOAT           6
 *  #define SQL_REAL            7
 *  #define SQL_DOUBLE          8
 *  #if (ODBCVER >= 0x0300)
 *  #define SQL_DATETIME        9
 *  #endif
 *  #define SQL_VARCHAR        12
 *
 *One-parameter shortcuts for date/time data types
 *#if (ODBCVER >= 0x0300)
 *#define SQL_TYPE_DATE      91
 *#define SQL_TYPE_TIME      92
 *#define SQL_TYPE_TIMESTAMP 93
 *#endif
 */

/**
*  'long'.
*  There are some exotic date type (actually not
*  used) which uses negative integer for mapping type.
*/
typedef SQLSMALLINT SQL_TYPE;
/**
 *  will be used column bind
 *  so each column will have a different
 *  type. Lets use void*
 */
typedef void ODBC_TYPE;

#endif /* ODBCTYPES_H_ */
