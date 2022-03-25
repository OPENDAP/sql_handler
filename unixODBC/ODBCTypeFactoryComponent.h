/*
 * ODBCTypeFactoryComponent.h
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
 *  Created on: 08/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef ODBCTYPEFACTORYCOMPONENT_H_
#define ODBCTYPEFACTORYCOMPONENT_H_

// libdap
#include <libdap/Byte.h>
#include <libdap/Int16.h>
#include <libdap/UInt16.h>
#include <libdap/Int32.h>
#include <libdap/UInt32.h>
#include <libdap/Float32.h>
#include <libdap/Float64.h>
#include <libdap/Str.h>
#include <libdap/Url.h>

#include <libdap/Array.h>
#include <libdap/Vector.h>

#include <libdap/Structure.h>
#include <libdap/Sequence.h>
#include <libdap/Grid.h>

#include "DTM/SQLSimpleType.h"
#include "DTM/SQLTypeFactoryComponent.h"

#include "SQLResponseNames.h"
#include "DEM/SQLInternalFatalError.h"

#include "ODBCTypes.h"

#include <sqlext.h>


// FIXME Removed jhrg 10/1/14 using namespace libdap;
/**
 * @brief unixODBC implementation of the SQLTypeFactoryComponent
 */
class ODBCTypeFactoryComponent : public SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE> {
public:
#if __CLONE__ == 1

    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLAction<SQL_TYPE,libdap::BaseType> *clone(){
        return new ODBCTypeFactoryComponent(*this);
    }
#endif

    /**
     * @brief default constructor
     * @param conn a reference to a SimpleConnector
     */
    ODBCTypeFactoryComponent(SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> &conn) :
            SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE>(conn) {};

    /**
     * @brief copy constructor
     * @param t is a reference to a constant instance of this class
     */
    ODBCTypeFactoryComponent(const ODBCTypeFactoryComponent &t) :
            SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE>(t) {};


    virtual ~ODBCTypeFactoryComponent() {};

    /**
     * @brief Provide action method implementation using DTM components.
     */
    libdap::BaseType *action(SQL_TYPE *type);

#if 0
    // throw (SQLInternalError,SQLInternalFatalError);
#endif
};


#endif /* ODBCTYPEFACTORYCOMPONENT_H_ */
