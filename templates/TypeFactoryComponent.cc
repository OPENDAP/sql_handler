/*
 * OPENDAP_CLASSTypeFactoryComponent.cpp
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

#include "OPENDAP_CLASSTypeFactoryComponent.h"

#include <BESInternalError.h>

/**
 * THIS IS ONLY AN EXAMPLE
 * YOU MAY WANT TO DELETE THIS
 * FUNCTION IF NOT USED
 *
 * @note: YOU DON'T HAVE TO
 * READ THE VALUE FROM THE
 * CONNECTOR HERE.
 * THIS FUNCTION ONLY PROVIDES
 * CAST FUNCTIONALITY.
 *
 * @brief The CAST function used to
 * read from the Connector and
 * convert in the right format.
 */
static void *
cast_func(void *val) {

    BESDEBUG(OPENDAP_CLASS_NAME, "casting a VOID" << endl);
    /**
     * if OPENDAP_CLASS connector directly
     * bind fields to variables and no
     * conversion is needed.
     *
     * @note: if val is reused for example
     * if it is the connector buffer the
     * SimpleType reuse constructor flag
     * should be set to true (as default).
     *
     * @see ODBCConnector
     * @see ODBCTypeFactoryComponent
     */
    return val;
}

/**
 * THIS IS ONLY AN EXAMPLE
 * YOU MAY WANT TO DELETE THIS
 * FUNCTION IF NOT USED
 *
 * @note: YOU DON'T HAVE TO
 * READ THE VALUE FROM THE
 * CONNECTOR HERE.
 * THIS FUNCTION ONLY PROVIDES
 * CAST FUNCTIONALITY.
 *
 * @brief The CAST function used to
 * read from the Connector and
 * convert to the right format.
 *
 */
static string *
cast_string(void *val) {
    string *ret = new string();
    ret->assign(((char *) val));
    /**
     *  val is reused so we don't delete it
     *  instead 'ret' is a new copy which can
     *  be safely deleted (once used) so we call
     *  constructor with reuse==false;
     */
    return ret;
}

/**
 *
 * @brief Provide action method implementation using DTM components.
 *
 */
libdap::BaseType *
OPENDAP_CLASSTypeFactoryComponent::action(SQL_TYPE *type)
throw(SQLInternalError, SQLInternalFatalError) {
    /**
     * THIS IS ONLY AN EXAMPLE YOU MAY WANT TO DELETE THIS CONSTRUCT IF NOT USED
     *
     * @note: YOU DON'T HAVE TO READ THE VALUE FROM THE CONNECTOR HERE.
     *
     * THIS FUNCTION ONLY PROVIDES CAST FACTORY FUNCTIONALITY.
     */
    switch (*type) {
        case 1:    // assume 1 represent a string
            /**
             * the template parameter 'string' indicates the
             * output type of the cast function and should be
             * set to the internal libdap::BaseType derived object
             * type.
             * F.e.: Here libdap::Str uses a buffer of string type
             */
            return new SQLSimpleType<SQL_TYPE, ODBC_TYPE, Str, string>(getConnector(), &cast_string, false);
        case 2: // assume 2 represent an integer
            /**
             * the default template parameter void (here not added
             * since it is the default one) indicates the
             * output type of the cast function and should be
             * set to the internal libdap::BaseType derived object
             * type.
             * F.e.: Here libdap::Int uses a buffer of int type
             * So the cast function should return an int* casted
             * to a void*.
             * The SQLSimpleType will use the val2buf(void*)
             * function to set the internal buffer
             */
            return new SQLSimpleType<SQL_TYPE, ODBC_TYPE, Int32>(getConnector(), &cast_func, true);
            /**
             * ...
             */
        default:
            throw SQLInternalFatalError(
                    "SQLTypeFactory not recognized the SQL_TYPE is searching for",
                    __FILE__, __LINE__);
    }
    return NULL; // to avoid warning
}
