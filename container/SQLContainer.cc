/*
 * SQLContainer.cpp
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
 *  Created on: 23/mag/2010
 *      Author: carlo cancellieri
 */

#include "SQLContainer.h"

/**
 * @brief Initialize time modification of the
 * referred file.
 * <br>NOTE: You have to run setup() from the
 * implementing class to make this
 * container ready to use.
 */
bool
SQLContainer::init(){
	// setting time modification
	struct stat buf;
	if (!stat(this->get_real_name().c_str(),&buf)){
		_time=buf.st_mtime;
	}
	else
	{
		BESDEBUG(SQL_NAME,"SQLContainer: Unable to get time/date modification "
				"setting to local time"<<endl);
		time(&_time);
	}
	return true;

}

/**
 * Check time modification of the file.
 * @return true if SQLContainer has same
 * timestamp of the file modification time.
 */
bool
SQLContainer::isUpToDate()const {
	struct stat buf;
	if (!stat(this->get_real_name().c_str(),&buf)){
#if __TESTS__==1
		char timeStr[100];
		strftime(timeStr, 100, "%d-%m-%Y %H:%M:%S", localtime( &buf.st_mtime));
		BESDEBUG(SQL_NAME,"SQLContainer: Last modified date and time: "<<timeStr<<endl);
		strftime(timeStr, 100, "%d-%m-%Y %H:%M:%S", localtime( &_time));
		BESDEBUG(SQL_NAME,"SQLContainer: this container date and time: "<<timeStr<<endl);
		BESDEBUG(SQL_NAME,"SQLContainer: Time difference in seconds: "<<
				fabs(difftime(_time,buf.st_mtime))<<endl);
#endif
		if (fabs(difftime(_time,buf.st_mtime)) < _SQLH_CONT_TIME_DIF){
			BESDEBUG(SQL_NAME,"SQLContainer: Cached file is up to date"<<endl);
			return true;
		}
		else
		{
			BESDEBUG(SQL_NAME,
				"SQLContainer: Cached file is old and should be updated"<<endl);
			return false;
		}
	}
	else
	{
		BESDEBUG(SQL_NAME,
			"SQLContainer: Unable to get time/date modification"<<endl);
		return false;
	}
}

/**
 * @WARNING Note that no attribute nor constraints will be settled.
 * <br>NOTE: You have to run setup() from the
 * implementing class to make this
 * container ready to use.
 */
SQLContainer::SQLContainer(const string &name,
							const string &real_name,
							const string &type):
	BESFileContainer(name,real_name,type)
{
	TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLContainer"<<endl);
	// initialize members
	_isReady=false; // you have to run setup()
};

/**
 * @brief Constructor
 * @param c a BESFileContainer of sql type
 * <br>NOTE: You have to run setup() from the
 * implementing class to make this
 * container ready to use.
 */
SQLContainer::SQLContainer(const BESFileContainer *c):
		BESFileContainer(*c)
{
	TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLContainer"<<endl);
	if (c) {
		// initialize members
		set_attributes(c->get_attributes());
		set_constraint(c->get_constraint());
		_isReady=false; // you have to run setup()
	}
	else
		throw BESInternalError("SQLContainer: Unable to create an SQLContainer "
				"using a NULL BESContainer pointer",__FILE__,__LINE__);
};

/**
 * @brief Constructor
 * @param c a BESContainer of sql type
 * <br>NOTE: You have to run setup() from the
 * implementing class to make this
 * container ready to use.
 */
SQLContainer::SQLContainer(const BESContainer &c):
	BESFileContainer(c.get_symbolic_name(),
					c.get_real_name(),
					c.get_container_type())
{
	TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLContainer"<<endl);
	// initialize members
	set_constraint(c.get_constraint());
	set_attributes(c.get_attributes());
	_isReady=false; // you have to run setup()
};

/**
 * @brief Resulting object will be a copy of the passed one,
 * which may be a not Ready object; You need to check and
 * eventually run init().
 */
SQLContainer::SQLContainer(const SQLContainer *c):
	BESFileContainer(*c)
{
	if (c) {
		if (c->isReady() && c->isUpToDate()){
			TESTDEBUG(SQL_NAME_TEST,"COPYING: SQLContainer"<<endl);
			clone(*c);
			// status
			_isReady=true;
		}
		else {
			BESDEBUG(SQL_NAME,"SQLContainer: IS NOT READY"<<endl);
			clone(*c);
			// status
			_isReady=false;
		}
	}
	else
		throw BESInternalError("SQLContainer: Unable to create an SQLContainer "
				"using a NULL SQLContainer pointer",__FILE__,__LINE__);
};

/**
 * @brief Resulting object will be a copy of the passed one,
 * which may be a not Ready object; You need to check and
 * eventually run init().
 */
SQLContainer::SQLContainer(const SQLContainer &c):
	BESFileContainer(c.get_symbolic_name(),
					c.get_real_name(),
					c.get_container_type())
{
	if (c.isReady() && c.isUpToDate()){
		TESTDEBUG(SQL_NAME_TEST,"COPYING: SQLContainer"<<endl);
		clone(c);
		// status
		_isReady=true;
	}
	else {
		BESDEBUG(SQL_NAME,"SQLContainer: IS NOT READY"<<endl);
		clone(c);
		// status
		_isReady=false;
	}

};

SQLContainer::~SQLContainer(){
	TESTDEBUG(SQL_NAME_TEST,"DELETING: SQLContainer"<<endl);
};

/**
 * @brief clone private members, no
 * ready or upToDate check is performed
 */
void
SQLContainer::clone(const SQLContainer &c){
	//cloning
	set_attributes(c.get_attributes());
	set_constraint(c.get_constraint());
	// timestamp
	_time=c._time;
}
