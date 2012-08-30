/*
 * SQLType.h
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
 *  Created on: 15/mag/2010
 *      Author: Carlo Cancellieri
 */

#ifndef SQLSIMPLETYPE_H_
#define SQLSIMPLETYPE_H_

#include "action/SQLAction.h"

#include "SQLBaseType.h"

// BES
#include <BaseType.h>

#include <BESDebug.h>
#include <BESError.h>

#include <string>

using namespace libdap;
//namespace SQL {

/**
 * @brief This is a implementation of a SQLBaseType abstract class
 * This is useful when DAP_TYPE could be statically cast to OUT*
 * which is written to the 'buf' variable through the set_val() method
 * <br>WARNING!!!
 * this template MUST be instantiate using a simple BaseType class
 * like (Int16, Int32, Float32 ...) as DAP_TYPE
 * <br>NOTE: if reuse is set to false returned cast pointer deletion will
 * be performed
 */
template <	class SQL_TYPE,
			class ODBC_TYPE = void,
			class DAP_TYPE = BaseType, // inherit from
			class OUT = void > // the cast output type
							// actually it is 'void' to use buf2val
							// change this if you want to use set_val
class SQLSimpleType : public SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,OUT> {
	bool _reuse;
public:

	/**
	 * @brief  Default constructor
	 * @param _connector reference to the SQLTypeConnector to use
	 * @param cast_function a static function returning a void* and
	 * getting a ODBC_TYPE * as input parameter, this method should
	 * return a (void*) value resulting from the cast of the input
	 * paramether. It will be used to pass the correct value to the
	 * val2buf() method (@see libdap::BaseType)
	 * @param reuse a bool indicating if the cast_function returning
	 * value should be deleted (false) or it is reused (true), default
	 * value is true (it will be not deleted until object destruction)
	 * <br>HOWTO:
	 * Set 'reuse' to false only if you return a copy (using 'new'
	 * operator) this may happen into the cast_function or in the
	 * SQLTypeConnector::getNext().
	 * <br>@see SQLTypeConnector::getNext()
	 * <br>@see ODBCFactoryComponent::action()
	 */
	SQLSimpleType(SQLSimpleConnector<SQL_TYPE,ODBC_TYPE> &_connector,
			typename SQLCastAction<ODBC_TYPE,OUT>::CAST cast_function,
			bool reuse=true):
		SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,OUT>(
						_connector,
						cast_function,
						reuse),
		_reuse(reuse){
TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLSimpleType"<<endl);
	};

	virtual ~SQLSimpleType(){};

	/**
	 * WARNING suppose DAP_TYPE is a BaseType inherited object
	 */
	virtual bool read()
	{
	try {
		if (this->read_p()){
TESTDEBUG(SQL_NAME_TEST,"SQLSimpleType: skipping object"<<endl);
			return false;
		}
		else
		{
TESTDEBUG(SQL_NAME_TEST,"SQLSimpleType: reading object"<<endl);
				this->set_value( *this->cast() );
				//this->set_read_p(true); -> done by set_value()
				if (!_reuse) {
					this->getCast().freeValue(); // deleting casted value
				}
TESTDEBUG(SQL_NAME_TEST,"SQLSimpleType: object copied to the buffer"<<endl);
			return false;
		}
		}
	catch (BESError &e){
			BESDEBUG(SQL_NAME,
						"SQLSimpleType: Unable to read variable"<<endl);
			throw BESInternalFatalError(
				"SQLSimpleType: Unable to read variable: "+e.get_message(),
				e.get_file(),e.get_line());
		}
	};

	virtual BaseType *ptr_duplicate()
	{
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,OUT>(*this);
	};

	/**
	 * @brief copy constructor (used by ptr_duplicate())
	 * <br>WARNING suppose DAP_TYPE is a BaseType
	 */
	SQLSimpleType(SQLSimpleType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,OUT> &obj):
			SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,OUT>(obj),
			_reuse(obj._reuse){
	TESTDEBUG(SQL_NAME_TEST,"COPING: SQLSimpleType"<<endl);
		};
};

/**
 * @brief SPECIALIZATION class which use void* as cast output type,
 * this class define a different read() which uses buf2val instead
 * of the set_val() since the returning val is a void*.
 * <br>WARNING!!!
 * this template MUST be instantiate using a simple BaseType class
 * like (Int16, Int32, Float32 ...) as DAP_TYPE
 * <br>NOTE: if reuse is set to false returning cast pointer
 * deletion will be performed using free(*void) so use this
 * only for malloc allocated objects.
 */
template <	class SQL_TYPE,
			class ODBC_TYPE,
			class DAP_TYPE> // inherit from
		//	void> // FORCED the cast output type
							// actually it is 'void' to use buf2val
							// change this if you want to use set_val
class SQLSimpleType<SQL_TYPE,ODBC_TYPE,DAP_TYPE> :
	public SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,void> {
private:
		bool _reuse;
public:
	/**
	 * @brief  Default constructor
	 * @param _connector reference to the SQLTypeConnector to use
	 * @param cast_function a static function returning a void* and
	 * getting a ODBC_TYPE * as input parameter, this method should
	 * return a (void*) value resulting from the cast of the input
	 * paramether. It will be used to pass the correct value to the
	 * val2buf() method (@see libdap::BaseType)
	 * @param reuse a bool indicating if the cast_function returning
	 * value should be deleted (false) or it is reused (true), default
	 * value is true (it will be not deleted until object destruction)
	 * <br>HOWTO:
	 * Set 'reuse' to false only if you return a copy (using 'malloc'
	 * operator) this may happen into the cast_function or in the
	 * SQLTypeConnector::getNext().
	 * <br>@see SQLTypeConnector::getNext()
	 * <br>@see ODBCFactoryComponent::action()
	 */
	SQLSimpleType(SQLSimpleConnector<SQL_TYPE,ODBC_TYPE> &_connector,
			typename SQLCastAction<ODBC_TYPE,void>::CAST cast_function,
			bool reuse=true):
		SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,void>(_connector, cast_function),
		_reuse(reuse){};

	virtual ~SQLSimpleType(){};

	/**
	 * WARNING suppose DAP_TYPE is a BaseType inherited object
	 */
	virtual bool read()
	{
	try {
		if (this->read_p()){
TESTDEBUG(SQL_NAME_TEST,"SQLSimpleType: skipping object"<<endl);
			return false;
		}
		else
		{
TESTDEBUG(SQL_NAME_TEST,"SQLSimpleType: reading object"<<endl);
				this->val2buf(this->cast());// cast value to the new type
				if (!_reuse)
					this->getCast().freeValue(); // deleting casted value
TESTDEBUG(SQL_NAME_TEST,"SQLSimpleType: object copied to the buffer"<<endl);
			// not usable!
			// since we cannot know the argument type here
			// this->set_value( this->doRead() );
			this->set_read_p(true);
			return false;
		}
		} // try
	catch (BESError &e){
			BESDEBUG(SQL_NAME,"SQLSimpleType: Unable to read variable: "
				+e.get_message());
			throw BESInternalFatalError(
				"SQLSimpleType: Unable to read variable: "+e.get_message(),
				e.get_file(),e.get_line());
		}
	};

	virtual BaseType *ptr_duplicate()
	{
		return new SQLSimpleType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,void>(*this);
	};

	/**
	 * @brief used in ptr_duplicate()
	 * WARNING suppose DAP_TYPE is a BaseType
	 */
	SQLSimpleType(SQLSimpleType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,void> &obj):
		SQLBaseType<SQL_TYPE,ODBC_TYPE,DAP_TYPE,void>(obj),
		_reuse(obj._reuse)
	{
TESTDEBUG(SQL_NAME_TEST,"COPING: SQLSimpleType"<<endl);
	};
};


#endif /* SQLSIMPLETYPE_H_ */
