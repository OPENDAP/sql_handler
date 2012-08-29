/**
 * SQLTextDefinitions.h
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
 *  Created on: 17/lug/2010
 *      Author: carlo cancellieri
 *
 *  Here are defined constants used to parse
 *  DATASET v0.4. This is done by SQLTextContainer
 */


#ifndef SQLTEXTDEFINITIONS_H_
#define SQLTEXTDEFINITIONS_H_

#include "SQLConstraint.h"
#include "SQLQuery.h"

/**
 * @brief The dataset definition
 */
typedef struct dataset_section {
	string api;		//!< name of the SQLPlugin
	string server;	//!< server name or DSN
	string port;	//!< server port
	string dbname;	//!< database name
	string user;	//!< user name
	string pass;	//!< password
	SQLQuery query; //!< SQLQuery object
	map<string,string> other; //!< optional rows

	/*
	 * @brief copy constructor
	 * @param reference to a const instance of this class
	 */
	dataset_section(const dataset_section &ds):
		api(ds.api),server(ds.server),port(ds.port),
		dbname(ds.dbname),user(ds.user),pass(ds.pass),
		query(ds.query),other(ds.other)
	{
TESTDEBUG(SQL_NAME,"CREATE: DATASET_SECTION"<<endl);
	}

	/**
	 * @brief default constructor
	 */
	dataset_section(){
TESTDEBUG(SQL_NAME,"CREATE: SQLH_DATASET_SECTION"<<endl);
	}
	/**
	 * @brief dtor
	 */
	virtual ~dataset_section(){
TESTDEBUG(SQL_NAME,"DELETE: SQLH_DATASET_SECTION ->api: "<<api<<endl);
	}

}SQLH_DATASET_SECTION;

/**
 * @brief To easily flag dataset parts completion
 */
typedef enum _dataset_parts {
	section_tag=0,//!< section_tag [section]
	api=1,        //!< api		key=val
	server=2,     //!< server	key=val
	port=3,       //!< port		key=val
	dbname=4,     //!< dbname	key=val
	user=5,       //!< user		key=val
	pass=6,       //!< pass		key=val
	select_tag=7, //!< select_tag	[select]
	from_tag=8,   //!< from_tag		[from]
	where_tag=9,  //!< where_tag	[where]
	other_tag=10  //!< other_tag	[other]
}_SQLH_DATASET_PARTS;

/**
 * @brief define count number of parts of a section
 */
#define _SQLH_DATASET_PARTS_NUM 11

/**
 * WARNING
 * @note You may want to change regex expression,
 * this is possible anyway if you vary the group
 * number into the regex string, be shure to change
 * GROUPS and XXX_GROUP number accordingly.
 * If you add a XX_GROUP probably you have to change
 * something (few things) into the code to add the
 * matching group to the list.
 * @see StringMatch::match
 * @see SQLQuery
 *
 * * This is true for ALL the regex which are used
 * with StringMatch::match function
 *
 * @note: for performance question read does not
 * use match for all the rows so be careful when
 * you change regex since this functionality is
 * not tested. (avoid add/delete groups)
 *
 * @todo: test change groups in text dataset regex.
 */
// variable
#define _SQLH_DATASET_REG_VAR "(\\$[A-z]*\\$)"
// number of groups '()' in regex +1
#define _SQLH_DATASET_REG_VAR_GROUPS 2
// set the interesting group (starts from 0)
#define _SQLH_DATASET_REG_VAR_GROUP 1
// variable definition
#define _SQLH_DATASET_REG_DEFVAR \
	"[ ]*define[ ]+(\\$[A-z,0-9,_]*\\$)[ ]*=[ ]*([A-z,0-9,\\',\\\", ,\\,]+)"
// number of groups '()' in regex +1
#define _SQLH_DATASET_REG_DEFVAR_GROUPS 3
// set the interesting group (starts from 0)
#define _SQLH_DATASET_REG_DEFVAR_KEY_GROUP 1
#define _SQLH_DATASET_REG_DEFVAR_VAL_GROUP 2
// comments
#define _SQLH_DATASET_REG_COMMENTS "^[ ]*#"
// number of groups '()' in regex +1
#define _SQLH_DATASET_REG_COMMENTS_GROUPS 0
// set the interesting group (starts from 0)
#define _SQLH_DATASET_REG_COMMENTS_GROUP 0;
// tags
//#define _SQLH_DATASET_REG_TAGS "^[ ]*(\\[\\w+\\])[ ]*$"
#define _SQLH_DATASET_REG_TAGS "^[ ]*(\\[[A-z,0-9,_]*\\])[ ]*$"
// number of groups '()' in regex +1
#define _SQLH_DATASET_REG_TAGS_GROUPS 2
// set the interesting group (starts from 0)
#define _SQLH_DATASET_REG_TAGS_GROUP 1
// key=value
//@todo: check this
#define _SQLH_DATASET_REG_KEY_VAL \
	"^[ ]*([A-z,0-9,_]*|!=|<=|>=|~=|\\*)[ ]*=[ ]*" \
	"(==|>=|<=|~=|!=|[A-z,0-9,\\',\\\", ,\\,,\\*,\\%,<,>,!,~]+)"
//NOTE: Start/End spaces in variable are stored
/*
* = 			Equal
 * <> 			Not equal
 * @todo: Note: In some versions of SQL the <> operator may be written as !=
 * > 			Greater than
 * < 			Less than
 * >= 			Greater than or equal
 * <= 			Less than or equal
 * LIKE 		Search for a pattern
 */
//	"&(\\w+)(<|>|=|!=|<=|>=|~=)([\\',\\\",\\%,\\*]*\\w+[\\',\\\",\\%,\\*]*)"

// number of groups '()' in regex +1
#define _SQLH_DATASET_REG_KEY_VAL_GROUPS 3
// set the interesting group for KEY
#define _SQLH_DATASET_REG_KEY_GROUP 1
// set the interesting group for VAL
#define _SQLH_DATASET_REG_VAL_GROUP 2

// max number of groups '()' in regex +1
#define _SQLH_DATASET_MAX_REG_GROUPS 3


/**
 * SQLContainer
 * keys to set into dataset
 * # reserved keys are:
 * # - api
 * # - dbname
 * # - server
 * # - port
 * # - user
 * # - pass
 *
 */
#define _SQLH_DATASET_API "api"
#define _SQLH_DATASET_SERVER "server"
#define _SQLH_DATASET_DBNAME "dbname"
#define _SQLH_DATASET_PORT "port"
#define _SQLH_DATASET_USER "user"
#define _SQLH_DATASET_PASS "pass"
/**
 * keys to set into dataset
 * # Tags are:
 * # - [section]
 * # - [select]
 * # - [from]
 * # - [where]
 * # - [other]
 */
#define _SQLH_DATASET_SECTION_TAG "[section]"
#define _SQLH_DATASET_SELECT_TAG "[select]"
#define _SQLH_DATASET_FROM_TAG "[from]"
#define _SQLH_DATASET_WHERE_TAG "[where]"
#define _SQLH_DATASET_OTHER_TAG "[other]"

#endif /* SQLTEXTDEFINITIONS_H_ */
