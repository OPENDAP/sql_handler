/*
 * ODBCErrorFactoryComponent.cpp
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

#include "ODBCErrorFactoryComponent.h"

using std::endl;
using std::string;

// SQL_SUCCESS
SQLStaticActionList<message_t, void> ODBCErrorFactoryComponent::success =
        SQLStaticActionList<message_t, void>(SQLDefaultErrorAction<message_t>::debugMethod);

// SQL_SUCCESS_WITH_INFO
SQLStaticActionList<message_t, void> ODBCErrorFactoryComponent::info =
        SQLStaticActionList<message_t, void>(SQLDefaultErrorAction<message_t>::debugMethod);

// SQL_NO_DATA
SQLStaticActionList<message_t, void> ODBCErrorFactoryComponent::no_data =
        SQLStaticActionList<message_t, void>(SQLDefaultErrorAction<message_t>::debugMethod);

#if 0
// fatalAction also print to debug
ODBCErrorFactoryComponent::_list_type ODBCErrorFactoryComponent::_error[2]=
    {	SQLDefaultErrorAction<MSG_TYPE>::debugMethod,
        SQLDefaultErrorAction<MSG_TYPE>::fatalMethod };
#endif

#if 0
// SQL_NO_DATA
SQLStaticActionList<MSG_TYPE,void> ODBCErrorFactoryComponent::no_data=
    SQLStaticActionList<MSG_TYPE,void>(ODBCErrorFactoryComponent::_error,2);
#endif

// SQL_ERROR
SQLStaticActionList<message_t, void> ODBCErrorFactoryComponent::error =
        SQLStaticActionList<message_t, void>(SQLDefaultErrorAction<message_t>::fatalMethod);

// SQL_INVALID_HANDLE
SQLStaticActionList<message_t, void> ODBCErrorFactoryComponent::invalid =
        SQLStaticActionList<message_t, void>(SQLDefaultErrorAction<message_t>::fatalMethod);

SQLActionList<message_t, void> &
ODBCErrorFactoryComponent::_getActions(error_t *error) {
    BESDEBUG(ODBC_NAME, "Checking for new errors.." << endl);
    if (!error)
        throw BESInternalFatalError(
                "ODBCPlugin: error code is NULL", __FILE__, __LINE__);
    switch (*error) {
        case SQL_SUCCESS:
            BESDEBUG(ODBC_NAME, "SUCCESS" << endl);
            return ODBCErrorFactoryComponent::success;
            break;
        case SQL_SUCCESS_WITH_INFO:
            BESDEBUG(ODBC_NAME, "INFO" << endl);
            return ODBCErrorFactoryComponent::info;
            break;
        case SQL_NO_DATA:
            BESDEBUG(ODBC_NAME, "SQL_NO_DATA" << endl);
            return ODBCErrorFactoryComponent::no_data;
            break;
        case SQL_ERROR:
            BESDEBUG(ODBC_NAME, "SQL_ERROR" << endl);
            return ODBCErrorFactoryComponent::error;
            break;
        case SQL_INVALID_HANDLE:
            BESDEBUG(ODBC_NAME, "SQL_INVALID_HANDLE" << endl);
            return ODBCErrorFactoryComponent::invalid;
            break;
        default:
            BESDEBUG(ODBC_NAME, "NOT MAPPED ERROR" << endl);
            throw BESInternalFatalError("No ErrorList found", __FILE__, __LINE__);
    }
}

bool
ODBCErrorFactoryComponent::_stop(error_t *error) {
    if (!error)
        throw BESInternalFatalError(
                "ODBCPlugin: error code is NULL", __FILE__, __LINE__);
    if (SQL_SUCCEEDED(*error))
        return true;
    if (SQL_NO_DATA == *error)
        return true;
    else
        return false;
}
