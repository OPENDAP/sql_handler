/*
 * SQLComplexType.h
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
 *  Created on: 27/mag/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLCOMPLEXTYPE_H_
#define SQLCOMPLEXTYPE_H_

#include <Vector.h>
#include <Array.h>
#include <libdap/BaseType.h>
#include <DEM/SQLInternalFatalError.h>

#include "SQLBaseType.h"

// FIXME Removed jhrg 10/1/14 using namespace libdap;

/**
 * @todo: make tests
 *
 * @deprecated Until test phase ends
 *
 * @brief This class is used as BaseType for complex types
 * such us Array or Vector and provide the methods to
 * runtime check these types.
 * If you need to implement complex type you can
 * extend this class implementing the read() method
 * If you need to implement a compound type you could
 * also use this class but you have to implement:
 * - read()
 *  To read sub-objects of your compound structure
 *  (you can do this also outside of your class using
 * - check()
 */
template<class SQL_TYPE,
        class ODBC_TYPE = void,
        class DAP_TYPE = Array>
class SQLComplexType : public SQLBaseType<SQL_TYPE, ODBC_TYPE, DAP_TYPE> {

private:

    // do not permit the empty constructor
    SQLComplexType() {
        throw SQLInternalFatalError(
                "You cannot use this constructor for complex type",
                __FILE__, __LINE__);
    };

    // do not permit the SQLSimpleType constructor
    SQLComplexType(const std::string &name) {
        throw SQLInternalFatalError(
                "You cannot use this constructor for complex type",
                __FILE__, __LINE__);
    };


    /**
     * this should be re-implemented using
     * something like smart::SmartCheck::isStr
     *
     * carlo cancellieri 15 jun 2010
     */
    /**
     * This method is intended to check for complex type
     * so the only checks we operate are to recognize
     * the Vector and Array type.
     */
    virtual void check() throw(BESError) {
        try {
            // NOTE:
            // DAP_TYPE is NOT 'Array'
            // and
            // DAP_TYPE is NOT 'Vector'
            // THROW ERROR
            if (typeid(libdap::Array) != typeid(DAP_TYPE)
                || typeid(libdap::Vector) != typeid(DAP_TYPE))
                // throw ERROR
                throw SQLInternalFatalError(
                        "Trying to instantiate a not Vector derived object!",
                        __FILE__, __LINE__);

        }
        catch (bad_typeid) {
            BESDEBUG(SQL_NAME, "BadTypeID");
            throw SQLInternalFatalError(
                    "Trying to instantiate a BadTypeID pointer!",
                    __FILE__, __LINE__);
        }
        catch (...) {
            BESDEBUG(SQL_NAME, "Generic exception");
            throw SQLInternalFatalError("Generic exception!",
                                        __FILE__, __LINE__);
        }
    }


public:

    SQLComplexType(const std::string &name,
                   BaseType *bt,
                   typename SQLCastAction<ODBC_TYPE, void>::CAST cast_function) :
            SQLBaseType<DAP_TYPE, ODBC_TYPE>(name, bt, cast_function/*=0*/) {};

    virtual ~SQLComplexType() {};

#if 1
    /**
     * Commented out since this could be a bad Idea
     * I think you have to implement your own SQLComplexType
     * implementation.
     * Carlo Cancellieri May 31 2010
     *
     */
    // see OpeNDAP documentation
    // http://docs.opendap.org/index.php/ProgrammerGuideChapter3
    /*
     * @todo: this method has to be implemented
     */
    virtual bool read() {
        /**
         * UPDATED:
         * this method can be passed using constructor.
         */
        //return this->action(this);


        throw SQLInternalFatalError(
                "SQLComplexType: Unimplemented SQLComplexType::read() method called.",
                __FILE__, __LINE__);

    }

#endif


    virtual BaseType *ptr_duplicate() {
        return new SQLComplexType<DAP_TYPE, ODBC_TYPE>(*this);
    };

    /**
     * @brief copy constructor
     * @note called by ptr_dup
     */
    SQLComplexType(SQLComplexType<DAP_TYPE, ODBC_TYPE> &obj) :
            SQLBaseType<SQL_TYPE, ODBC_TYPE, DAP_TYPE>(obj) {};

    /**
     * CANGES:
     * We don't need this method since this kind of operation
     * should be applied by the method referenced by the _read_function
     * pointer.
     *
     * Here you have to implement the conversion between
     * the returned database value this->getConnector().getValue()
     * type (ODBC_TYPE) and the void* required by the val2buf() method
     * coming from the extended object.
     * See OpeNDAP documentation
     * http://docs.opendap.org/index.php/ProgrammerGuideChapter3
     *
     * NOTE: this could also useful to initialize member variables
     * of the extending object like, F.E.: the size of *val (if
     * it's a vector)
     */
    //virtual void *cast(ODBC_TYPE * val){ return static_cast<void*>(val); };

};

#endif /* SQLCOMPLEXTYPE_H_ */
