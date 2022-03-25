/*
 * OPENDAP_CLASSTypeFactoryComponent.h
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

#ifndef OPENDAP_CLASSTYPEFACTORYCOMPONENT_H_
#define OPENDAP_CLASSTYPEFACTORYCOMPONENT_H_


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

#include <libdap/BaseType.h>

#include "DTM/SQLSimpleType.h"
#include "DTM/SQLTypeFactoryComponent.h"

#include "DEM/SQLInternalFatalError.h"

#include "OPENDAP_CLASSTypes.h"
#include "OPENDAP_CLASSResponseNames.h"

#include <sqlext.h>

// FIXME Removed jhrg 10/1/14 using namespace libdap;
/**
 * @brief implementation of the SQLFactoryComponent
 */
class OPENDAP_CLASSTypeFactoryComponent : public SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE> {
public:
#if __CLONE__ == 1

    /**
     * @brief This is the implementation of the
     * Clone interface.
     * @return a pointer to a clone of this object
     */
    virtual SQLAction<SQL_TYPE,libdap::BaseType> *clone(){
        return new OPENDAP_CLASSTypeFactoryComponent(*this);
    }
#endif

    /**
     * @brief default constructor
     * @param conn a reference to a SimpleConnector
     */
    OPENDAP_CLASSTypeFactoryComponent(SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> &conn) :
            SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE>(conn) {};

    /**
     * @brief copy constructor
     * @param t is a reference to a constant instance of this class
     */
    OPENDAP_CLASSTypeFactoryComponent(const OPENDAP_CLASSTypeFactoryComponent &t) :
            SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE>(t) {
        /**
         * @todo if you add members update the copy constructor
         */
    };

    virtual ~OPENDAP_CLASSTypeFactoryComponent() {};

    /**
     * @brief Provide action method implementation using DTM components.
     */
    libdap::BaseType *
    action(SQL_TYPE *type) throw(SQLInternalError, SQLInternalFatalError);

};


#endif /* OPENDAP_CLASSTYPEFACTORYCOMPONENT_H_ */
