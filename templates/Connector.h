/*
 #COMPLETE#
 * OPENDAP_CLASSConnector.h
 #COMPLETE#
#SIMPLE#
 * OPENDAP_CLASSSimpleConnector.h
#SIMPLE#
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
 *  Created on: 22/giu/2010
 *      Author: carlo cancellieri
 */
#COMPLETE#
#ifndef OPENDAP_CLASSCONNECTOR_H_
#define OPENDAP_CLASSCONNECTOR_H_
#COMPLETE#
#SIMPLE#
#ifndef OPENDAP_CLASSSIMPLECONNECTOR_H_
#define OPENDAP_CLASSSIMPLECONNECTOR_H_
#SIMPLE#

#include "OPENDAP_CLASSResponseNames.h"
#include "OPENDAP_CLASSTypes.h"

#include "DEM/SQLInternalFatalError.h"
#include "DEM/SQLInternalError.h"
#include <BESInternalError.h>

#COMPLETE#

#include "DEM/SQLErrorManager.h"
#include "DEM/SQLErrorFactory.h"
#include "connector/SQLConnector.h"

#COMPLETE#
#SIMPLE#

#include "connector/SQLSimpleConnector.h"

#SIMPLE#

#include "SQLDefinitions.h"


#include <BESDebug.h>

#COMPLETE#

/**
 * @brief OPENDAP_CLASSConnector implementing the SQLConnector
 *
 * @see SQLHandleConnector
 * @see SQLTypeConnector
 * @see SQLErrorConnector
 *
 */
class OPENDAP_CLASSConnector : public SQLConnector<SQL_TYPE, ODBC_TYPE, ERROR_TYPE, MSG_TYPE> {
#COMPLETE#
#SIMPLE#

/**
 * @brief OPENDAP_CLASSSimpleConnector implementing the SQLSimpleConnector
 *
 * @see SQLHandleConnector
 * @see SQLTypeConnector
 *
 */
    class OPENDAP_CLASSSimpleConnector : public SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> {
#SIMPLE#

        /**
         * @brief Connect to DB
         * @returns true if status can be settled to isReady()
         */
        bool connect();

    public:
        /**
         * should be usable with getCol() which returns:
         * 0 - (n-1) 	column index	n=getCols()
         * ODBC uses
         * 1 - n 		column index
         */
        virtual SQL_TYPE *getType(size_t column);

        virtual const string &getColName(const size_t &column);

        virtual const string &getColDesc(const size_t &column);

#COMPLETE#

        /**
         * @brief Set error factory used by this connector.
         */
        void setErrorFactory(SQLErrorFactory<ERROR_TYPE, MSG_TYPE> &ef);

        virtual ERROR_TYPE *getError();

        MSG_TYPE *getMsg(ERROR_TYPE *error_code);

#COMPLETE#

        /**
         * @brief Query
         * NOTE: this is also a good place to
         * update QUERY size limit using
         * setCols(nFiled) and setRows(nRows)
         */
        bool query();

        /**
         * @brief Returns a value of ODBC_TYPE type containing
         * the next value in this result set.
         * <br>It is pointed by the ACTUAL position:
         * - column 	-> getCol()
         * - row		-> getRow()
         * <br>In a Sequence the NEXT value should be:
         * - The NEXT COLUMN value of the actual row
         * <br>OR
         * - The first value of the NEXT ROW
         * @note This method should also update cursors to the
         * next position using setNext:
         * - setNext(size_t increment)
         * @note The first time it is called should return object in
         * position 0,0 and set cursors to col:1 and row:0 (or
         * row:1 if only 1 column), if limits are reached
         * (col==getCols() && row==getRows()) this method
         * can throw an exception or an
         * SQLInternalException
         * @note use notEnd to check end condition
         */
        ODBC_TYPE *getNext(size_t next = 1);

        /**
         * @brief Close connection
         */
        bool close();

#COMPLETE#

        OPENDAP_CLASSConnector() :
                SQLConnector<SQL_TYPE, ODBC_TYPE, ERROR_TYPE, MSG_TYPE>(),
#COMPLETE#
#SIMPLE#
                OPENDAP_CLASSSimpleConnector()

        :

        SQLSimpleConnector<SQL_TYPE, ODBC_TYPE>(),
#SIMPLE#
                toFetchRows(

        0),
#COMPLETE#
        strMsg(new

        MSG_TYPE()

        ),    //!< error message type
        sef(NULL),        //!< error factory
#COMPLETE#
        types(NULL),    //!< buffer (column type)
        descs(NULL),    //!< buffer (column descriptions)
        names(NULL)    //!< buffer (column name)
        {
        };
#COMPLETE#

        virtual ~OPENDAP_CLASSConnector() {
#COMPLETE#
#SIMPLE#
            virtual ~OPENDAP_CLASSSimpleConnector()
            {
#SIMPLE#

                /**
                 *  There's no need to call close()
                 *  here, it is handled before
                 *  Connector dtor is called.
                 *
                 *  clean() is called by close()
                 */
#COMPLETE#
                /**
                 * @todo double check if this
                 * or other deletions are needed!
                 */
                if (strMsg)
                    delete strMsg;
                strMsg = 0;
#COMPLETE#
            };

            private:
            void clean() {
                setReady(false); //!< set status to NOT READY

                /**
                 * @todo: add your clear code here
                 * renew buffers
                 * close connection
                 * clear variable status
                 * ...
                 */

                reset();

                if (types)
                    delete[] types;
                types = 0;
                if (descs)
                    delete[] descs;
                descs = 0;
                if (names)
                    delete[] names;
                names = 0;
#COMPLETE#
#if 0
                /**
                 * NEVER DO THIS
                 * we have an action 'SQLCloseAction()' which
                 * may use the error message.
                 * This is done into the
                 * destructor.
                 */
                if (strMsg)
                    delete [] strMsg;
                strMsg=0;
                /**
                 * NEVER DO THIS
                 *  This is handled by OPENDAP_CLASSPlugin
                 */
                if (sef)
                    delete sef;
                sef=0;
#endif
#COMPLETE#
            }
            // status
            /**
             * this is optional and can be deleted if
             * your connector do not use it.
             */
            size_t toFetchRows; //!<number of rows to fetch or skip
            // errors
#COMPLETE#
            MSG_TYPE * strMsg;
            SQLErrorFactory<ERROR_TYPE, MSG_TYPE> *sef; //!<this is passed not rebuilt
#COMPLETE#

            // buffers
            SQL_TYPE *types;
            // descriptions
            string *descs;
            // column names
            string *names;
        };

#endif /* OPENDAP_CLASSCONNECTOR_H_ */
