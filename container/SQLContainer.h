/*
 * SQLContainer.h
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
 *  Created on: 23/mag/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLCONTAINER_H_
#define SQLCONTAINER_H_

// base (inherited)
#include <BESFileContainer.h>

// debugging
#include "SQLDefinitions.h"
#include <BESDebug.h>

// errors
#include <BESInternalError.h>
#include <BESInternalFatalError.h>

// members
#include <string>

// timing
#include <sys/stat.h>
#include <time.h>
#include <math.h>
// _SQLH_CONT_TIME_DIF && regex
#include "SQLContainerDefinitions.h"
#include "SQLAttribute.h"
#include "SQLConstraint.h"
#include "SQLQuery.h"

#include <set>
#include <bitset>
#include <vector>

#include <utils/Clone.h>
/**
 * @brief Abstract class which defines methods used by the
 * SQLHandler to explore and get data from a dataset.
 * Implements Clone to be usable with smart::SharedPtr<>.
 *
 * @implement smart::Clone
 * @implement BESFileContainer
 *
 * <br>A SQLContainer implementation MUST:
 *  - Define a structure matching SQLContainer methods
 *  	@see SQLTextDefinition
 *  - Define a persistence storage type
 *  	such as TXT,XML,BINARY,...
 *  - Use relatives API to implement the virtual methods
 * 		follow comments for instructions

 * <br>
 * @note: Since SQLContainer is basically a BESFileContainer
 * it implements methods to check the age of a loaded
 * Container (looking at the file modification time date).
 * @see SQLRequestHandler
 * @see SQLPlugin
 * @see SQLStorageContainerVolatile
 *
 * @note: The desired SQLContainer implementation to load is chosen
 * by the SELECT_CONTAINER function of the SQLContainerFactory.
 * @see SQLContainerFactory
 *
 */
class SQLContainer :public BESFileContainer, smart::Clone<SQLContainer>{
public:

	/**
	 * Using clone you will be able to add SQLContainer
	 * to a SharedPtr<>
	 * @see SQLDataHandlerInterface
	 */
	virtual SQLContainer* clone(){
		return ptr_duplicate();
	}

	/**
	 * Using clone you will be able to add SQLContainer
	 * to a SharedPtr<>
	 * @see SQLDataHandlerInterface
	 */
	virtual SQLContainer* create()throw (std::bad_alloc){
		return ptr_duplicate();
	}

	/**
	 * @brief return a reference to a SQLQuery object
	 * representing the actual dataset query
	 */
	virtual SQLQuery& getQuery()=0;

	/**
	 * @brief effectively build the query string
	 *
	 * @note: This is a good place to define the
	 * attributes order
	 * @see SQLTextContainer
	 */
	virtual string buildQuery()=0;

	/**
	 * Index other (optional) strings.
	 * If getOther provides modification for 'buf'
	 * be sure to return true.
	 * @param a reference to a string representing
	 * the value to modify.
	 * @return boolean true if 'buf' is modified
	 * or false otherwise.
	 * This can be used to manage a
	 * map<string,string> or to build
	 * your own 'buf' modification method.
	 * @see SQLTextContainer::getOther()
	 */
	virtual bool getOther(string & buf)=0;

	/**
	 * @brief return the actual API
	 * from the dataset's API list.
	 */
	virtual string &getApi()=0;

	/**
	 * @brief get the string representing
	 * the user which should be used to access
	 * to the actual selected DB
	 * @return a string reference
	 */
	virtual string &getUser()=0;
	/**
	 * @brief get the string representing
	 * the password which should be used to
	 * access to the actual selected DB
	 * @return a string reference
	 */
	virtual string &getPass()=0;
	/**
	 * @brief get the string representing
	 * the server which should be used to
	 * access to the actual selected DB
	 * @return a string reference
	 */
	virtual string &getServer()=0;
	/**
	 * @brief get the string representing
	 * the password which should be used to
	 * access to the actual selected DB
	 * @return a string reference
	 */
	virtual string &getPort()=0;

	/**
	 * @brief get the string representing
	 * the database name which should be
	 * used to access to the actual
	 * selected DB
	 * @return a string reference
	 */
	virtual string &getDBName()=0;

	/**
	 * CURSOR/LIST OPERATIONs
	 */

	/**
	 * reset the cursor to the first section
	 * <br>In a list from 0 to (n-1)
	 * - reset() put cursor to 0
	 */
	virtual void reset()=0;

	/**
	 * Clear the internal structures used
	 * to store data.
	 * This is used when an update is needed
	 * to clear storage before read() is
	 * called again.
	 * @see setup()
	 */
	virtual void clear()=0;

	/**
	 * Tell if the end of the list is reached.
	 * In a list from 0 to (n-1):
	 * <br>If the actual position of the cursor
	 * points to n return true.
	 * <br>Note: in a list from 0 to 0
	 * - end() return true
	 * - empty() return true
	 * @see setNext()
	 */
	virtual bool end()=0;

	/**
	 * Tell if the section list is empty.
	 */
	virtual bool empty()=0;

	/**
	 * returns true if there is next section
	 * in the list and set actual section to
	 * the next one.
	 * <br>In a list from 0 to (n-1)
	 * - reset() put cursor to 0
	 * - FIRST setNext() call set cursor to 1
	 * - LAST setNext() call set cursor to n-1
	 * returning true
	 * - ALL the successive calls to the setNext()
	 * must return false until reset() is call
	 */
	virtual bool setNext()=0;

	/**
	 * CONTAINER OPERATIONs
	 */

	/**
	 * @brief read dataset (read()) and time (init())
	 * modification setting status accordingly.
	 *
	 * Initialize all the members reading the source
	 * file and set to the initial status the 'cursor'.
	 * @return true if setup is successfully done.
	 */
	bool setup(){
		if (!empty())
			clear(); // clear before update
		if ((_isReady=(read() && init()))){ // read
			/**
			 * reset to the new start
			 * created by the read()
			 */
			reset();
		}
		return _isReady;
	}

	bool & isReady(){
		return _isReady;
	}

	explicit SQLContainer(BESFileContainer *c);


	explicit SQLContainer(BESContainer &c);

	/**
	 * @brief Resulting object will be a copy of the passed one,
	 * which may be a not Ready object; You need to check and
	 * eventually run init().
	 */
	explicit SQLContainer(SQLContainer *c);

	/**
	 * @brief Resulting object will be a copy of the passed one,
	 * which may be a not Ready object; You need to check and
	 * eventually run init().
	 */
	explicit SQLContainer(SQLContainer &c);

	/**
	 * @brief constructor
	 * <br>Note that no attribute nor constraints will be settled
	 */
	SQLContainer(const string &name,const string &real_name,const string &type);

	virtual ~SQLContainer();

    /** @brief pure abstract method to duplicate this instances of BESContainer
     */
    virtual SQLContainer *	ptr_duplicate( )=0;

private:
	/**
	 * Initialize all the object members reading
	 * from the source file.
	 * @return true if setup is successfully done.
	 * @throws BESInternalError if parse encounter
	 * unrecoverable syntax errors
	 */
	virtual bool read()throw (BESError)=0;

	/**
	 * @brief Initialize time modification of the
	 * referred file.
	 * <br>NOTE: You have to run setup() from the
	 * implementing class to make this
	 * container ready to use.
	 */
	bool init();

	/**
	 * Check time modification of the file.
	 * @return true if SQLContainer has same
	 * timestamp of the file modification time.
	 */
	bool isUpToDate();

	/**
	 * store AGE info
	 */
	time_t _time;

	/**
	 * isReady flag
	 */
	bool _isReady;

	/**
	 * @brief clone private members, no
	 * ready or upToDate check is performed
	 */
	void clone(SQLContainer &c);

};

#endif /* SQLCONTAINER_H_ */