/*
 * ODBCTypeFactoryComponent.cpp
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
 *  Created on: 23/lug/2010
 *      Author: carlo cancellieri
 */

#include "ODBCTypeFactoryComponent.h"

#include <BESInternalError.h>
/**
 * @brief The READ function used to
 * read from the Connector and
 * convert in the right format.
 */
static void*
cast_func(void*val){
TESTDEBUG(SQL_NAME,"ODBCFactoryComponent: casting a VOID"<<endl);
	/**
	 * ODBC connector directly bind fields to
	 * variables so no conversion is needed.
	 * @note: val is reused since it is the
	 * connector buffer so the SimpleType
	 * constructor should be set to true
	 * (as default).
	 * Connector dtor will delete it when it
	 * is no more required
	 *
	 * @see ODBCConnector
	 */
	return val;
}

/**
 * @brief The READ function used to
 * read from the Connector and
 * convert to the right format.
 */
static string*
cast_string(void*val){
TESTDEBUG(SQL_NAME,"ODBCFactoryComponent: casting a STRING"<<endl);

	string *ret=new string();
	ret->assign(((char*)val));
	/**
	 *  val is reused so we don't delete it
	 *  instead 'ret' is a new copy which can
	 *  be safely deleted (once used) so we call
	 *  constructor with reuse==false;
	 */
	return ret;
}

/**
 * @todo:check matching conversions here!
 *
 * @brief Provide action method implementation using DTM components.
 *
 */
libdap::BaseType *
ODBCTypeFactoryComponent::action(SQL_TYPE *type)
		throw (SQLInternalError,SQLInternalFatalError)
{
TESTDEBUG(ODBC_NAME,"Getting new object"<<endl);
	switch(*type){
//#define SQL_C_CHAR    SQL_CHAR
	case SQL_C_CHAR:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Str,string>(getConnector(),&cast_string,false);
//#define SQL_C_SLONG      (SQL_C_LONG+SQL_SIGNED_OFFSET)
	case SQL_C_SLONG:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int32>(getConnector(),&cast_func,true);
//#define SQL_C_LONG    SQL_INTEGER
	case SQL_C_LONG:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int32>(getConnector(),&cast_func,true);
		//return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int32,long>(getConnector(),&cast_long,false);
//#define SQL_C_SHORT   SQL_SMALLINT
	case SQL_C_SHORT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int16>(getConnector(),&cast_func,true);
//#define SQL_C_FLOAT   SQL_REAL
	case SQL_C_FLOAT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Float32>(getConnector(),&cast_func,true);
//#define SQL_C_DOUBLE  SQL_DOUBLE
	case SQL_C_DOUBLE:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Float64>(getConnector(),&cast_func,true);
//#define	SQL_C_NUMERIC		SQL_NUMERIC
	case SQL_C_NUMERIC:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int32>(getConnector(),&cast_func,true);
//#define SQL_C_DEFAULT 99
	case SQL_C_DEFAULT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Str,string>(getConnector(),&cast_string,false);
#if 0
//#define SQL_SIGNED_OFFSET       (-20)
//#define SQL_UNSIGNED_OFFSET     (-22)
//#define SQL_C_DATE       SQL_DATE
#endif
	case SQL_C_DATE:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Str>(getConnector(),&cast_func,true);
//#define SQL_C_TIME       SQL_TIME
	case SQL_C_TIME:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Str>(getConnector(),&cast_func,true);
//#define SQL_C_TIMESTAMP  SQL_TIMESTAMP
	case SQL_C_TIMESTAMP:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Str>(getConnector(),&cast_func,true);
//#define SQL_C_TYPE_DATE					SQL_TYPE_DATE
	case SQL_C_TYPE_DATE:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Str>(getConnector(),&cast_func,true);
//#define SQL_C_TYPE_TIME					SQL_TYPE_TIME
	case SQL_C_TYPE_TIME:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Str>(getConnector(),&cast_func,true);
//#define SQL_C_TYPE_TIMESTAMP			SQL_TYPE_TIMESTAMP
	case SQL_C_TYPE_TIMESTAMP:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Str>(getConnector(),&cast_func,true);
#if 0
//#define SQL_C_INTERVAL_YEAR				SQL_INTERVAL_YEAR
//#define SQL_C_INTERVAL_MONTH			SQL_INTERVAL_MONTH
//#define SQL_C_INTERVAL_DAY				SQL_INTERVAL_DAY
//#define SQL_C_INTERVAL_HOUR				SQL_INTERVAL_HOUR
//#define SQL_C_INTERVAL_MINUTE			SQL_INTERVAL_MINUTE
//#define SQL_C_INTERVAL_SECOND			SQL_INTERVAL_SECOND
//#define SQL_C_INTERVAL_YEAR_TO_MONTH	SQL_INTERVAL_YEAR_TO_MONTH
//#define SQL_C_INTERVAL_DAY_TO_HOUR		SQL_INTERVAL_DAY_TO_HOUR
//#define SQL_C_INTERVAL_DAY_TO_MINUTE	SQL_INTERVAL_DAY_TO_MINUTE
//#define SQL_C_INTERVAL_DAY_TO_SECOND	SQL_INTERVAL_DAY_TO_SECOND
//#define SQL_C_INTERVAL_HOUR_TO_MINUTE	SQL_INTERVAL_HOUR_TO_MINUTE
//#define SQL_C_INTERVAL_HOUR_TO_SECOND	SQL_INTERVAL_HOUR_TO_SECOND
//#define SQL_C_INTERVAL_MINUTE_TO_SECOND	SQL_INTERVAL_MINUTE_TO_SECOND
//#define SQL_C_BINARY     SQL_BINARY
#endif
	case SQL_C_BINARY:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int16>(getConnector(),&cast_func,true);
//#define SQL_C_BIT        SQL_BIT
	case SQL_C_BIT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int16>(getConnector(),&cast_func,true);
//#define SQL_C_SBIGINT	(SQL_BIGINT+SQL_SIGNED_OFFSET)
	case SQL_C_SBIGINT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int32>(getConnector(),&cast_func,true);
//#define SQL_C_UBIGINT	(SQL_BIGINT+SQL_UNSIGNED_OFFSET)
	case SQL_C_UBIGINT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int32>(getConnector(),&cast_func,true);
//#define SQL_C_TINYINT    SQL_TINYINT
	case SQL_C_TINYINT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int16>(getConnector(),&cast_func,true);

//#define SQL_C_SSHORT     (SQL_C_SHORT+SQL_SIGNED_OFFSET)
	case SQL_C_SSHORT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int16>(getConnector(),&cast_func,true);
//#define SQL_C_STINYINT   (SQL_TINYINT+SQL_SIGNED_OFFSET)
	case SQL_C_STINYINT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int16>(getConnector(),&cast_func,true);
//#define SQL_C_ULONG      (SQL_C_LONG+SQL_UNSIGNED_OFFSET)
	case SQL_C_ULONG:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int32>(getConnector(),&cast_func,true);
//#define SQL_C_USHORT     (SQL_C_SHORT+SQL_UNSIGNED_OFFSET)
	case SQL_C_USHORT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int32>(getConnector(),&cast_func,true);
//#define SQL_C_UTINYINT   (SQL_TINYINT+SQL_UNSIGNED_OFFSET)
	case SQL_C_UTINYINT:
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,Int32>(getConnector(),&cast_func,true);
	default:
		throw SQLInternalFatalError(
			"SQLTypeFactory not recognized the SQL_TYPE is searching for",
				__FILE__,__LINE__);
	}
	return NULL; // to avoid warning
}
