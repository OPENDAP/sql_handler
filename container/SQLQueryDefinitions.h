/*
 * SQLQueryDefinitions.h
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
 *  Created on: 27/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLQUERYDEFINITIONS_H_
#define SQLQUERYDEFINITIONS_H_

/**
 * WARNING
 * @note You may want to change regex expression,
 * this is possible anyway if you vary the group
 * number into the regex string, be shure to change
 * GROUPS and XXX_GROUP number accordingly.
 * If you add a XX_GROUP probably you have to change
 * something into the code to add the matching group
 * to the list.
 * @see StringMatch::match
 * @see SQLQuery
 *
 * This is true for ALL the regex which are used
 * with StringMatch::match function
 */

/**
 *  CONSTRAINTS
 *  projection -> attributes
 */

/**
 * SQLAttributes:
 * All the string matching group 2
 * of the following regex
 */
#define _SQLH_CONT_REG_ATTR \
    "(^|,)[ ]*([A-z0-9\\_\\. ]+|[\\*])[ ]*(,|$)"
// number of groups '()' in regex +1
#define _SQLH_CONT_REG_ATTR_GROUPS 4
/**
 *  set the interesting group
 *  table alias
 */
#define _SQLH_CONT_REG_ATTR_GROUP 2
// how to join string in SQL
#define _SQLH_CONT_ATTR_JOIN ","

/**
 * DAP
 * http://docs.opendap.org/index.php/UserGuideOPeNDAPMessages#Constraint_Expressions
 *
 * Class 	 					Operators
 * Simple Types
 * Byte, Int32, UInt32, Float64 < > = != <= >=
 * String 						= != ~=
 * URL 							= != ~=
 * @todo: Compound Types
 * Array 						[start:stop] [start:stride:stop]
 * List 						length(list), nth(list,n), member(list,elem)
 * Structure 					.
 * Sequence 					.
 * Grid 						[start:stop] [start:stride:stop] .
 *
 * SQL
 * Operator  	Description
 * = 			Equal
 * <> 			Not equal
 * @todo: Note: In some versions of SQL the <> operator may be written as !=
 * > 			Greater than
 * < 			Less than
 * >= 			Greater than or equal
 * <= 			Less than or equal
 * LIKE 		Search for a pattern
 * @todo: to discuss
 * BETWEEN 		Between an inclusive range
 * IN 			If you know the exact value you
 * 				want to return for at least one of the columns
 */

// selection -> constraints
/*
 * @todo: actually no compound types are supported
 * so the regex simply not match their selection
 * expression which will be ignored.
 */
/**
 *  todo: fix base regex
 *  anyway LIKE works, probably a more restrictive regex
 *  will be safer
 *  @note: The following regex is used to match constraints
 *  specified into the dataset AND on the fly specified constraints
 *  so it should match a DAP constraints AND a SQL WHERE predicate
 */
#define _SQLH_CONT_REG_CONSTR_BASE \
    "[ ]*&?(([A-z,0-9]+\\.)?(\\w+[0-9]*\\.?\\_?\\w*[0-9]*))" \
        "[ ]*(<|>|=|!=|<=|>=|~=|<>)" \
            "[ ]*([\\',\\\",\\%,\\*]*\\w*[\\.?,\\_?,\\w*,[0-9]*]*[\\',\\\",\\%,\\*]*)"
//"[ ]*AND|and|OR?|or?"

// number of groups '()' in regex +1
#define _SQLH_CONT_REG_CONSTR_BASE_GROUPS 6
// set the interesting group (starts from 0)
#define _SQLH_CONT_REG_CONSTR_BASE_ATTR 1
//#define _SQLH_CONT_REG_CONSTR_BASE_KEY_ATTR 2
#define _SQLH_CONT_REG_CONSTR_BASE_COMPARATOR 4
#define _SQLH_CONT_REG_CONSTR_BASE_VAL 5
#define _SQLH_CONT_REG_CONSTR_BASE_JOIN " AND "


#endif /* SQLQUERYDEFINITIONS_H_ */
