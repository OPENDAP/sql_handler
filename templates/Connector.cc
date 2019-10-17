/*
 #COMPLETE#
 * OPENDAP_CLASSConnector.cpp
 #COMPLETE#
#SIMPLE#
 * OPENDAP_CLASSSimpleConnector.cpp
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

#include "OPENDAP_CLASSConnector.h"

#COMPLETE#
#SIMPLE#

#include "OPENDAP_CLASSSimpleConnector.h"

#SIMPLE#

bool
#COMPLETE#
OPENDAP_CLASSConnector::connect()

#COMPLETE#
#SIMPLE#

OPENDAP_CLASSSimpleConnector::connect()
#SIMPLE#
{
    BESDEBUG(OPENDAP_CLASS_NAME, "Starting connection" << endl);

    // clean previous buffer status
    if (isReady()) {
        clean();
    }

    /**
     * @todo add your connection code here
     * @note remember to set return status
     * accordingly
     */

#COMPLETE#
    /**
     * @todo remember to set the status of this
     * connector accordingly to the result of this
     * operation.
     * This can be stored into a member variable
     * or can be returned by an ODBC specific Api.
     * Anyway remember that this status should be
     * used into the ERROR_TYPE * getError()
     * member function to check the connector status.
     */
#COMPLETE#

    BESDEBUG(OPENDAP_CLASS_NAME, "Connection successful" << endl);
    return true;
}

/**
 * @brief Cached version of getType
 *
 * should be usable with getCol() which returns:
 * 0 - (n-1) 	column index	n=getCols()
 * OPENDAP_CLASS uses
 * 1 - n 		column index
 */
SQL_TYPE *
#COMPLETE#
OPENDAP_CLASSConnector::getType(size_t column) {
#COMPLETE#
#SIMPLE#
    OPENDAP_CLASSSimpleConnector::getType(size_t
    column){
#SIMPLE#
        if (types) {
            if (column < getCols()) {
                return &types[column];
            }
            else
                throw SQLInternalFatalError("Index provided exceeds the array limit!",
                                            __FILE__, __LINE__);
        }
        else {
            if (getCols() > 0) {
                types = new SQL_TYPE[getCols()]();
            }
            else
                throw SQLInternalFatalError(
                        "Unable to check type for a ResultSet of 0 columns",
                        __FILE__, __LINE__);

            for (size_t col = 0; col < getCols(); col++) {
                /**
                 * @todo Here you have to provide a function
                 * returning the type of the passed column and
                 * store its result into the provided buffer.
                 * Do something like this:
                 *  SQL_TYPE type;
                 *  type=this->getType(column)
                 *  types[col]=type;
                 *
                 *  @note check that the SQL_TYPE supports the
                 *  << operator otherwise the following debug
                 *  print will not compile.
                 */
                SQL_TYPE type;
                types[col] = type;
#COMPLETE#
                /**
                 * @todo remember to set the status of this
                 * connector accordingly to the result of this
                 * operation.
                 * This can be stored into a member variable
                 * or can be returned by an ODBC specific Api.
                 * Anyway remember that this status should be
                 * used into the ERROR_TYPE * getError()
                 * member function to check the connector status.
                 */
#COMPLETE#
                BESDEBUG(OPENDAP_CLASS_NAME, "Getting column type: " << types[col] <<
                                                                     " for column: " << col << endl);

#COMPLETE#
                /**
                 * This is optional:
                 *  force check for errors here
                 *  since this is a critical operation
                 */
                if (sef)
                    SQLErrorManager<void>::trigger(_SQLH_ON_ALWAYS, *sef);
#COMPLETE#
            }
            return getType(column);
        }
    }

    const string &
#COMPLETE#
    OPENDAP_CLASSConnector::getColName(const size_t &column) {
#COMPLETE#
#SIMPLE#
        OPENDAP_CLASSSimpleConnector::getColName(
        const size_t &column){
#SIMPLE#
            if (names) {
                if (column < getCols()) {
                    return names[column];
                }
                else {
                    throw SQLInternalFatalError("Index provided exceeds the array limit!",
                                                __FILE__, __LINE__);
                }
            }
            else {
                if (getCols() > 0) {
                    names = new string[getCols()]();
                }
                else
                    throw SQLInternalFatalError(
                            "Unable to check name for a ResultSet of 0 columns",
                            __FILE__, __LINE__);

                for (size_t col = 0; col < getCols(); col++) {
                    /**
                     * @todo Here you have to provide a function
                     * returning the name of the passed column and
                     * set the provided buffer using that value:
                     * Do something like this:
                     *  char name[_buf_size];
                     *  name=this->getName(column)
                     *  names[col].assign(name);
                     */
                    names[col].assign("EMPTY_NAME");
#COMPLETE#
                    /**
                     * @todo remember to set the status of this
                     * connector accordingly to the result of this
                     * operation.
                     * This can be stored into a member variable
                     * or can be returned by an ODBC specific Api.
                     * Anyway remember that this status should be
                     * used into the ERROR_TYPE * getError()
                     * member function to check the connector status.
                     */
#COMPLETE#
                    BESDEBUG(OPENDAP_CLASS_NAME, "Getting column name: " << names[col] <<
                                                                         " for column: " << col << endl);
#COMPLETE#
                    /**
                     * This is optional:
                     *  force check for errors here
                     *  since this is a critical operation
                     */
                    if (sef)
                        SQLErrorManager<void>::trigger(_SQLH_ON_ALWAYS, *sef);
#COMPLETE#
                }
                return getColName(column);
            }
        }

        const string &
#COMPLETE#
        OPENDAP_CLASSConnector::getColDesc(const size_t &column) {
#COMPLETE#
#SIMPLE#
            OPENDAP_CLASSSimpleConnector::getColDesc(
            const size_t &column){
#SIMPLE#
                if (descs) {
                    if (column < getCols()) {
                        return descs[column];
                    }
                    else
                        throw SQLInternalFatalError("Index provided exceeds the array limit!",
                                                    __FILE__, __LINE__);
                }
                else {
                    if (getCols() > 0) {
                        descs = new string[getCols()]();
                    }
                    else
                        throw SQLInternalFatalError(
                                "Unable to calculate size for a ResultSet of 0 columns",
                                __FILE__, __LINE__);

                    for (size_t col = 0; col < getCols(); col++) {
                        /**
                         * @todo Here you have to provide a function
                         * returning a description of the column and
                         * set the provided buffer using that value:
                         * Do something like this:
                         *  char desc[_buf_size];
                         *  desc=this->getDescription(column)
                         *  descs[col].assign(desc);
                         * You may also consider to use a different
                         * function for each column type to get, f.e.:
                         *  for an integer -> precision
                         *  for a string -> size
                         *  for time -> localization
                         */
                        descs[col] = "EMPTY_DESCRIPTION";
#COMPLETE#
                        /**
                         * @todo remember to set the status of this
                         * connector accordingly to the result of this
                         * operation.
                         * This can be stored into a member variable
                         * or can be returned by an ODBC specific Api.
                         * Anyway remember that this status should be
                         * used into the ERROR_TYPE * getError()
                         * member function to check the connector status.
                         */
#COMPLETE#
                        BESDEBUG(OPENDAP_CLASS_NAME, "Getting column description: " << descs[col]
                                                                                    << " for column: " << col << endl);
                        /**
                         *  force check for errors here
                         */
                        try {
#COMPLETE#
                            /**
                             * This is optional:
                             *  force check for errors here
                             *  since this is a critical operation
                             */
                            if (sef)
                                SQLErrorManager<void>::trigger(_SQLH_ON_GET_NEXT, *sef);
#COMPLETE#

                        }
                        catch (SQLInternalError &e) {
                            /**
                             * This is not a critical op. so
                             * if not fatal error is thrown
                             * set to empty string.
                             */
                            descs[col] = "";
                        }
                    }
                    return getColDesc(column);
                }
            }

#COMPLETE#
            ERROR_TYPE *
            OPENDAP_CLASSConnector::getError() {
                BESDEBUG(OPENDAP_CLASS_NAME, "running GetError() " << endl);
                /**
                 * @todo: This member should return the pointer to the
                 * actual connector status (of ERROR_TYPE type), this
                 * can represent:
                 * - error status
                 * - normal status
                 * - status you may want to take some action
                 * @note: you could safely use a variable member since
                 * this pointer is NEVER DELETED.
                 * Do something like:
                 *  return &status;
                 * Or return a new pointer handling its deletion.
                 */
                return NULL;
            }
#COMPLETE#

#COMPLETE#
            MSG_TYPE *
            OPENDAP_CLASSConnector::getMsg(ERROR_TYPE *error_code) {
                /**
                 * @todo: Here you have to decode the error code
                 * to get an instance of the MSG_TYPE type.
                 * This is usually a string but you may want to
                 * get different object(s) as parameter of your
                 * SQLAction<IN,OUT>.
                 * This can be usefull to get a complete stack trace
                 * or anything you may consider usefull to take action
                 * on an error (or specific connector status).
                 * F.E.: You can pass the connector itself to run
                 * specific administration task (vacuum, ...)
                 * @note SQLDefaultErrorAction are of type:
                 * SQLAction<IN,void> anyway IN is used with
                 * << operator which mean that IN can be a
                 * stream, a string or anything like that.
                 * Do not return anything.
                 * @see SQLErrorManager
                 * @see SQLDefaultErrorAction
                 *
                 * @note please keep nomenclature consistent:
                 * Each SQLAction which code ODBC API dependent
                 * Action should be called Something{Component}
                 * Api indipendent SQLAction can be called
                 * Something{Action}
                 */
                return NULL;
            }
#COMPLETE#

/**
 * @brief Query
 * NOTE: this is also a good place to
 * update QUERY size limit using
 * setCols(nFiled) and setRows(nRows)
 */
            bool
#COMPLETE#
            OPENDAP_CLASSConnector::query() {
#COMPLETE#
#SIMPLE#
                OPENDAP_CLASSSimpleConnector::query()
                {
#SIMPLE#
                    bool status = true;//!< the return status
                    /**
                     *  ready should be settled to true by query
                     *  so if this happen here we have to clean
                     *  buffers.
                     */
                    if (isReady()) {
                        clean();
                    }

                    string query = getParams().buildQuery();

                    BESDEBUG(OPENDAP_CLASS_NAME, "Executing query" << endl);

                    /**
                     * @todo: Here you have to execute the query.
                     * Use your ODBC API to perform a query execution.
                     * the query is found in:
                     * string query=getParams().buildQuery();
                     * If you have added some string substitution to the
                     * dataset you can get all the extracted variables
                     * using getParams().getOther(&key_to_val)
                     * @see SQLContainer
                     * @see SQLTextContainer
                     */
#COMPLETE#
                    /**
                     * @todo remember to set the status of this
                     * connector accordingly to the result of this
                     * operation.
                     * This can be stored into a member variable
                     * or can be returned by an ODBC specific Api.
                     * Anyway remember that this status should be
                     * used into the ERROR_TYPE * getError()
                     * member function to check the connector status.
                     */
#COMPLETE#
                    BESDEBUG(OPENDAP_CLASS_NAME, "Query \"" << query << "\" executed." << endl);

                    // rows
                    size_t rows;
                    /**
                     * @todo Here you have to set the number of
                     * rows returned by the executed query.
                     * If it's impossible to know this here
                     * set it to >0 and remember to update
                     * this value in the getNext() member function.
                     * Do something like this:
                     *  rows=connector->getRowNumber();
                     */
                    BESDEBUG(OPENDAP_CLASS_NAME, "Setting rows number: " << rows << endl);
                    setRows(rows);

                    // columns
                    size_t cols;
                    /**
                     * @todo Here you have to set the number of
                     * columns returned by the executed query.
                     * Do something like this:
                     *  cols=connector->getColumnNumber();
                     */
                    BESDEBUG(OPENDAP_CLASS_NAME, "Setting columns number: " << cols << endl);
                    setCols(cols);


                    /**
                     *  Query SUCCEDED set READY status.
                     *  @todo: Here you have to set the isReady
                     *  status of the connector which mean that
                     *  the query is succesfully executed and
                     *  connector is ready to return the first
                     *  element using getNext()
                     *  Do something like this:
                     *   if (status==GOOD_STATUS())
                     */

                    toFetchRows = 1;//!< set number of rows to fetch.

                    reset();//!< reset cursors to start and check notEnd condition
                    setReady(status);//! set status
                    return status;//! return the status
                }

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
                ODBC_TYPE *
#COMPLETE#
                OPENDAP_CLASSConnector::getNext(size_t next) {
#COMPLETE#
#SIMPLE#
                    OPENDAP_CLASSSimpleConnector::getNext(size_t
                    next){
#SIMPLE#
                        if (!isReady() || !notEnd())
                            throw SQLInternalError(
                                    "Unable to getNext() element, connector: !isReady() or end is reached",
                                    __FILE__, __LINE__);
                        //@todo may we want to try to reconnect?

                        BESDEBUG(OPENDAP_CLASS_NAME, "Getting next element" << endl);

                        while (toFetchRows > 0) { // while skip
                            /**
                             * todo: If we are here a new
                             * row should be read so prepare
                             * your cursor to read next row
                             * (if it is needed).
                             *
                             * @note formally you have to
                             * do this for each skipped row
                             * (usually 1).
                             *
                             * @note setNext automatically
                             * set indexes (getRow() and
                             * getCol()) to the right position
                             * so you don't have to reset
                             * manually columns.
                             * No need to use setCol or setRow.
                             */
                            toFetchRows--;
                        }

                        /**
                         * todo: read the value using getCol() as index
                         * and prepare the return pointer.
                         */
                        ODBC_TYPE *ret = NULL;

                        /**
                         * Set number of column and row
                         * corresponding to the current
                         * position + next.
                         * Return the number of skipped rows
                         * which is stored for the next fetch
                         * operation (if needed)
                         */
                        toFetchRows = setNext(next);
#COMPLETE#
                        /**
                         * @todo remember to set the status of this
                         * connector accordingly to the result of this
                         * operation.
                         * This can be stored into a member variable
                         * or can be returned by an ODBC specific Api.
                         * Anyway remember that this status should be
                         * used into the ERROR_TYPE * getError()
                         * member function to check the connector status.
                         */
#COMPLETE#
                        /**
                         * @todo: use you ODBC API to return
                         * the first element from the cursor
                         * or (better) the buffer.
                         * Do something like:
                         *   return buf[getCol()];
                         *  or
                         *   return connector->getFromCursor(getCol());
                         *
                         *  @note: deletion of this pointer object can
                         *  be handled externally using DTM/SQLBaseType
                         *  action called cast.
                         *  You can return a local address or a new pointer
                         *  and decide externally if perform delete (or free)
                         *  or not.
                         *  @see SQLCastAction
                         *  @see SQLBaseType
                         *  @see SQLTypeFactoryComponent
                         */
                        return ret;
                    }

#COMPLETE#
/**
 * @brief Set error factory used by this connector.
 *
 * @note: This is optional, you can use this method to set
 * its ErrorFactory to force some check.
 */
                    void
                    OPENDAP_CLASSConnector::setErrorFactory(SQLErrorFactory <ERROR_TYPE, MSG_TYPE> &ef) {
                        sef = &ef;    // DO NOT DELETE!
                    }
#COMPLETE#


/**
 * @brief Close connection
 */
                    bool
#COMPLETE#
                    OPENDAP_CLASSConnector::close() {
#COMPLETE#
#SIMPLE#
                        OPENDAP_CLASSSimpleConnector::close()
                        {
#SIMPLE#

                            /**
                             * @todo Use your ODBC API to take
                             * all the needed actions to implement
                             * a clean close:
                             * close cursors
                             * close connection
                             * close statements
                             * clean environment
                             * free conn,stat,env handlers
                             * ...
                             */
#COMPLETE#
                            /**
                             * @todo remember to set the status of this
                             * connector accordingly to the result of this
                             * operation.
                             * This can be stored into a member variable
                             * or can be returned by an ODBC specific Api.
                             * Anyway remember that this status should be
                             * used into the ERROR_TYPE * getError()
                             * member function to check the connector status.
                             */
#COMPLETE#

                            /**
                             * Clean temporary members;
                             * free buffers and clean
                             * status member variables.
                             */
                            clean();

                            BESDEBUG(OPENDAP_CLASS_NAME, "Connection closed" << endl);
                            /**
                             * return status.
                             * @note if something goes wrong you
                             * may return false.
                             */
                            return true;
                        }

