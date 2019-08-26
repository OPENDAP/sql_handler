/*
 * SQLSequence.h
 *
 * This file is part of the SQLHandler a C++ library to add relational
 * database access to the OPeNDAP Hyrax back-end server.
 * Copyright (C) 2010  Carlo cancellieri <ccancellieri@hotmail.com>
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
 *  Created on: 06/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLSEQUENCE_H_
#define SQLSEQUENCE_H_

#include <BESDebug.h>

#include <Sequence.h>
//auto_ptr
#include <memory>
#if 0
#include "utils/SharedPtr.h"
#endif

#include "connector/SQLSimpleConnector.h"

/**
 * @brief SQLSequence is a libdap::Sequence implementation.
 */
template<class SQL_TYPE, //connector TypeFactory
        class ODBC_TYPE> //connector TypeFactory
class SQLSequence: public libdap::Sequence {
#if 0
    /**
     * We use auto_ptr to make possible transfer the connector
     * handle from this object to its copy.
     * Note: that once ptr_duplicate is called you lost the
     * connector handle from the cloned instance.
     *
     * If you need to control the connector in more than
     * one copy of the sequence you may use SharedPtr and
     * check if use_count()==1 then conn->close() into dtor;
     */
    //auto_ptr<SQLSimpleConnector<SQL_TYPE,ODBC_TYPE> > _conn;
#endif
    std::shared_ptr<SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> > _conn;

public:

    /**
     * @brief default constructor
     */
    SQLSequence(const string &name, const string &dataset, SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> * conn) :
            Sequence(name, dataset), _conn(conn)
    {
        if (_conn) {
            if (_conn->isReady()) {
                _conn->reset();
            }
            else {
                BESDEBUG(SQL_NAME, "SQLSequence: The passed connector should be ready to read."<<endl);
#if 0
                // we should return empty sequence, not fail
                throw BESInternalFatalError(
                        "SQLSequence: The passed connector should be ready to read.",
                        __FILE__,__LINE__);
#endif
            }
        }
        else
            BESDEBUG(SQL_NAME, "SQLSequence: The passed connector is NULL");

        BESDEBUG(SQL_NAME, "CREATING: SQLSequence"<< endl);
    }
    ;

    /**
     * @brief copy constructor
     */
    SQLSequence(SQLSequence & seq) :
            Sequence(seq), _conn(seq._conn)
    {
        if (_conn) {
            if (_conn->isReady()) {
                _conn->reset();
            }
            else {
                BESDEBUG(SQL_NAME, "SQLSequence: The passed connector should be ready to read."<<endl);
#if 0
                // we should return empty sequence, not fail
                throw BESInternalFatalError(
                        "SQLSequence: The passed connector should be ready to read.",
                        __FILE__,__LINE__);
#endif
            }
        }
        else
            BESDEBUG(SQL_NAME, "SQLSequence: The passed connector is NULL");

        BESDEBUG(SQL_NAME, "COPING: SQLSequence"<<endl);
    }
    /**
     *  @note For most of the types the default implementation of this method is
     fine. However, if you're building a server which must handle data
     represented using nested sequences, then you may need to provide a
     specialization of Sequence::set_read_p(). By default Sequence::set_read_()
     recursively sets the \e read_p property for all child variables to
     \e state. For servers where one Sequence reads an outer set of values
     and another reads an inner set, this is cumbersome. In such a case, it is
     easier to specialize Sequence::set_read_p() so that it does \e not
     recursively set the \e read_p property for the inner Sequence. Be sure
     to see the documentation for the read() method!

     @note For synthesized variables, this method does nothing. Thus, if a
     synthesized variable is added to a Sequence, the Sequence can iteratively
     reset the \e read_p property without affecting the value of that property
     for the synthesized variable. That's important since a synthesized
     variable's value is calculated, not read.

     @todo Look at making synthesized variables easier to implement and at
     making them more integrated into the overall CE evaluation process.
     Maybe the code that computes the synthesized var's value should be in the
     that variable's read() method? This might provide a way to get rid of the
     awkward 'projection functions' by replacing them with real children of
     BaseType. It would also provide a way to clean up the way the
     \e synthesized_p prop intrudes on the \e read_p prop.

     @see BaseType::read()
     @brief Sets the value of the \e read_p property.
     @param state Set the \e read_p property to this state.
     void
     BaseType::set_read_p(bool state)
     */
    /* FROM BASETYPE.cc
     * If you are writing Sequence::read(), be sure to check the
     documentation for Sequence::read_row() and Sequence::serialize()
     so you understand how Sequence::read() is being called.</li>
     <li> For Sequence::read(), your specialization must correctly manage the
     \c unsent_data property and row count in addition to the \c read_p
     property (handle the \c read_p property as describe above). For a
     Sequence to serialize correctly, once all data from the Sequence
     has been read, \c unsent_data property must be set to false (use
     Sequence::set_unsent_data()). Also, at that time the row number
     counter must be reset (use Sequence::reset_row_counter()). Typically
     the correct time to set \c unsent_data to false and reset the row
     counter is the time when Sequence::read() return false indicating
     that all the data for the Sequence have been read. Failure to
     handle these tasks will break serialization of nested Sequences. Note
     that when Sequence::read() returns with a result of true (indicating
     there is more data to send, the value of the \c unsent_data property
     should be true.

     Also, if you server must handle nested sequences, be sure to read
     about subclassing set_read_p().</li>
     </ul>

     @brief Read data into a local buffer.

     @return The return value of this method for all types except Sequence
     should always be false. Sequences should return true to indicate more
     values remain in the Sequence, false to indicate no more values remain.
     (see Sequence::serialize() and Sequence::read_row()).
     bool read()
     */
    /**
     * Implement inherited BaseType::read() method reading
     * all the element of this SQLSequence.
     */
    virtual bool read()
    {
        try {
            if (this->read_p()) // if already read
            { // Nothing to do
                BESDEBUG(SQL_NAME, "SQLSequence:read() SEQUENCE ALREADY READ"<< endl);
                return false;
            }

            if (!this->_conn->isReady()) {
                // not ready to read, return
                BESDEBUG(SQL_NAME, "SQLSequence:read() SEQUENCE EMPTY or not READY"<< endl);
                return false;
            }

            if (_conn->notEnd()) {
                BESDEBUG(SQL_NAME, "SQLSequence:read()  TotalRows " <<_conn->getRows()<<" TotalColumns: "<<_conn->getCols()<< endl);

                BESDEBUG(SQL_NAME,
                        "SQLSequence:read() READING SEQUENCE "<< name() << " ProcessingPosition: row: " << _conn->getRow()<<" col: "<<_conn->getCol()<< endl);

                for (Vars_iter p = var_begin(); p != var_end(); ++p) {
                    if ((*p)->send_p() || (*p)->is_in_selection()) {
                        (*p)->read();
#if 0
                        if(BESDebug::IsSet(SQL_NAME)){
                            BESDEBUG(SQL_NAME, "SQLSequence:  '"<< (*p)->name() << "'  value: '");
                            (*p)->print_val(*BESDebug::GetStrm(),"", false);
                            *BESDebug::GetStrm() <<  "'" << endl;
                        }
#endif
                    }
                }
                return false;
            }
            else { // End of sequence
                BESDEBUG(SQL_NAME, "SQLSequence:read() STOPPED.  Position row: " <<_conn->getRows()<<" col: "<<_conn->getCols()<< endl);
                //set_read_p(true); // done by read_row
                //set_send_p(true); // done by read_row

                set_unsent_data(false);
                reset_row_number();
                return true;
            }

        } //try
        catch (BESError &e) {
            _conn->close();
            throw SQLInternalError("SQLSequence:read() ERROR", __FILE__, __LINE__);
        }
        return true;
    }

    BaseType *
    ptr_duplicate()
    {
        return new SQLSequence<SQL_TYPE, ODBC_TYPE>(*this);
    }

    /**
     * @brief Dtor which close connection if this is
     * the last SQLSequence instance alive
     */
    virtual ~SQLSequence()
    {
        BESDEBUG(SQL_NAME, "DELETING: SQLSequence"<< endl);
#if 0
        /**
         * If you have called ptr_duplicate this instance does
         * not handle anymore the connector so, the check, test
         * this case.
         */
        if (_conn.get())
        _conn->close();
#endif
#if 1
        if (_conn.unique()) //use_count()==1
            _conn->close();
#endif
    }
};

#endif /* SQLSEQUENCE_H_ */
