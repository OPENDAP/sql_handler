/*
 * SQLBaseType.h
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
 *  Created on: 14/mag/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLBASETYPE_H_
#define SQLBASETYPE_H_

// std
#include <string>
// check()
#include <typeinfo>
#if 0
// libdap
#include <Byte.h>
#include <Int16.h>
#include <UInt16.h>
#include <Int32.h>
#include <UInt32.h>
#include <Float32.h>
#include <Float64.h>
#include <Str.h>
#include <Url.h>

#include <Array.h>
#include <Vector.h>

#include <Structure.h>
#include <Sequence.h>
#include <Grid.h>
#endif
#include <BaseType.h>

// BES
#include <BESError.h>
#include <BESInternalFatalError.h>
#include <BESDebug.h>
#include "SQLDefinitions.h"

#include "SQLCastAction.h"
#include "connector/SQLSimpleConnector.h"

#if __CLONE__==1
#include "utils/Clone.h"
#endif
using namespace libdap;
/**
 * @brief This class is used to inherit dap components
 * and to provide casting functionality to types.
 * @inherits Clone if __CLONE__ flag is set to 1
 * @see SQLCastAction.
 * It is extended by:
 * - SQLSimpleType
 * @todo
 * - SQLComplexType (only checks)
 * - SQLCompoundType (is this needed?)
 * to define interfaces needed to the Dynamic Type Matching
 * Layer
 *
 * @see
 * http://docs.opendap.org/index.php/ProgrammerGuideChapter3
 * OpeNDAP documentation
 */
template <	class SQL_TYPE,
			class ODBC_TYPE = void,
			class DAP_TYPE = BaseType, // inherit from
			class OUT = void> // the cast output type
							// default is 'void' which use buf2val
							// change this if you want to use set_val
class SQLBaseType: public DAP_TYPE
#if __CLONE__==1
,public smart::Clone<SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,OUT> >
{
public:

	/**
	 * @brief This is the implementation of the
	 * Clone interface.
	 * @return a pointer to a clone of this object
	 */
	virtual SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,OUT>* create()
		throw (std::bad_alloc)
	{
		return this->clone();
	};

	/**
	 * @brief This is the implementation of the
	 * Clone interface.
	 * @return a pointer to a clone of this object
	 */
	virtual SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,OUT> *clone(){
		return new SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,OUT>(*this);
	}
#else
{
	public:
#endif

	/**
	 * @brief this method applies the member
	 * action to cast the getNext coming value
	 */
	OUT * cast (){
		if (_connector.isReady()){
			ODBC_TYPE *t=_connector.getNext();
			return _cast.action(t);}
		else
			throw BESInternalFatalError(
				"Unable to cast the next object using a NOT READY connector",
				__FILE__,__LINE__);
	}

	/**
	 *
	 * @brief copy constructor
	 *
	 * @param a reference to the SQLBaseType derived object to
	 * copy
	 *
	 */
	SQLBaseType(SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,OUT> & obj):
		DAP_TYPE(obj),
		_connector(obj.getConnector()),	// initialize the connector reference
		_col(obj.getCol()),				// initialize position
		_cast(obj.getCast())			// initialize the CastAction
	{
		// check for type
		check();
#if 0
		// check position
		if (!(position<conn.getCols()))
			throw BESInternalFatalError(
				"Unable to set column outside sequence limits",
				__FILE__,__LINE__);
#endif
		/*
		 * This shows differences between actual column
		 * and requested object _col.
		 * When the object is cloned cursor may be not on
		 * that column!
		 */
TESTDEBUG(SQL_NAME,"COPING: SQLBaseType with:\n"
		"\n using connector: "<<
		"\n on column: "<<obj.getConnector().getCol()<<
		"\n using passed position:"
		"\n name: "<<obj.getConnector().getColName(obj.getCol())<<
		"\n on column: "<<obj.getCol()<<endl);

	};

	/**
	 * @brief Constructor for complex/compound type.
	 * @param a reference to the SQLSimpleConnector
	 * @param pointer to the BaseType object to pass to the complex
	 * object constructor
	 * @param the CAST function pointer
	 * @param a boolean indicating if the casted pointer value is
	 * reused or can be deleted.
	 * @note this should be used to build complex or compound types
	 */
	SQLBaseType(SQLSimpleConnector<SQL_TYPE,ODBC_TYPE> &conn,
				BaseType * obj,
				typename SQLCastAction<ODBC_TYPE,OUT>::CAST cast_function,
				bool reuse=true):
		DAP_TYPE(conn.getColName(conn.getCol()), obj),
		_connector(conn),		// initialize the connector reference
		_col(conn.getCol()), 	// initialize position
		_cast(cast_function,reuse)	// initialize the CastAction
	{
		// check for type
		check();

TESTDEBUG(SQL_NAME,"SQLBaseType: CLONING ComplexObject with:\n"
	"\n name: "<<conn.getColName(_col)<<"\n on column: "<<_col<<endl);

	};

	/**
	 * @brief Constructor for simple type.
	 * @param a reference to the SQLSimpleConnector
	 * @param the CAST function pointer
	 * @param a boolean indicating if the casted pointer value is
	 * reused or can be deleted.
	 * @note this should be used to build simple types
	 */
	SQLBaseType(SQLSimpleConnector<SQL_TYPE,ODBC_TYPE> &conn,
			typename SQLCastAction<ODBC_TYPE,OUT>::CAST cast_function,
			bool reuse=true):
		DAP_TYPE(conn.getColName(conn.getCol())),
		_connector(conn),		// initialize the connector reference
		_col(conn.getCol()),	// initialize position
		_cast(cast_function,reuse)	// initialize the CastAction
	{
		// check for type
		check();

TESTDEBUG(SQL_NAME,"SQLBaseType: Creating SimpleObject with:\n name: "
	<<conn.getColName(conn.getCol())<<"\n on column: "<<conn.getCol()<<endl);
	};

	virtual ~SQLBaseType(){
TESTDEBUG(SQL_NAME,"DELETING: SQLBaseType"<<endl);
	}

private:
	// connector
	SQLSimpleConnector<SQL_TYPE,ODBC_TYPE> &_connector;

	// variable indicating the column in which this variable is placed
	size_t _col;

	/*
	 * @brief cast action to be able to cast from ODBC_TYPE to desired OUT type.
	 * Usually OUT is void (which mean 'void*') but if you want to use
	 * set_val() (@see SimpleType()) you may want to change it.
	 * We use dynamic allocation to be able to delete this action once
	 * used, since it handle a copy of the casted object until the buf2val()
	 * read (and copy its value).
	 * @see SimpleType::read()
	 *<br>
	 * NOTE: the casted value is handled only when the read() method is
	 * called and is deleted after the *void is passed to the val2buf()
	 * or automatically on this object deletion.
	 *<br>
	 * This is done to keep track of the *void object deletion.
	 * So you should be sure to create a new instance of the object into
	 * the SQL{Simple|Complex}Type cast method, its deletion will be handled
	 * by this SQLAction.
	 */
	SQLCastAction<ODBC_TYPE,OUT> _cast;

	// not usable
	SQLBaseType(){
		throw BESInternalFatalError(
			"Trying to instantiate using a not permitted constructor!",
				__FILE__,__LINE__);
	};

protected:

	/**
	 * @brief returns a reference to the CastAction
	 */
	SQLCastAction<ODBC_TYPE,OUT> & getCast(){
		return _cast;
	}


	SQLSimpleConnector<SQL_TYPE,ODBC_TYPE> & getConnector(){
		return _connector;
	}

	size_t getCol(){
		return _col;
	}

	/**
	 *  @brief We should check for the DAP_TYPE type that should
	 *  be a BaseType implementation.
	 *  In many commons cases this check can be done at compile
	 *  time:
	 *  class xxx has no member named xxx
	 *  since some BaseType method are included into extending
	 *  classes.
	 *  Anyway we also check this condition at run-time HERE.
	 *  carlo cancellieri May/2010
	 *
	 * @todo this should be re-implemented as static check
	 * @see smart::SmartCheck::isStr()
	 *
	 * carlo cancellieri 15 jun 2010
	 *
	 */
	virtual void check()throw(BESError){
		try {
			// NOTE:
			// IF PARENT of DAP_TYPE is 'BaseType'
			if (!typeid(BaseType).before(typeid(DAP_TYPE)))
				throw BESInternalFatalError(
					"Trying to instantiate a NOT BaseType derived object!",
						__FILE__,__LINE__);
		}
		catch (bad_typeid){
			BESDEBUG(SQL_NAME,"BadTypeID");
			throw BESInternalFatalError(
				"Trying to instantiate a BadTypeID pointer!",
					__FILE__,__LINE__);
		}
		catch (...){
			BESDEBUG(SQL_NAME,"Generic exception");
			throw BESInternalFatalError("Generic exception!",
				__FILE__,__LINE__);
		}
	}



};

#endif /* SQLBASETYPE_H_ */
