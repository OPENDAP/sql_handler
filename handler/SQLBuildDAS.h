/*
 * SQLBuildDAS.h
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
 *  Created on: 11/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLBUILDDAS_H_
#define SQLBUILDDAS_H_


#include "connector/SQLConnector.h"

#include "handler/connector/SQLConnectAction.h"
#include "handler/connector/SQLQueryAction.h"
#include "handler/connector/SQLCloseAction.h"
#include "handler/connector/SQLNextTypeAction.h"

#include "DTM/SQLTypeFactoryComponent.h"
#include "DTM/SQLTypeManager.h"
#include "DTM/SQLObjectTypeFactory.h"
#include <BaseType.h>
#include <BESDASResponse.h>

#include "SQLDefinitions.h"

/**
 * @brief Placeholder class which define:
 * - maximum number of template arguments which this class may specify.
 * - default arguments to simplify default SQLBuildDAS::sql_build_das calls.
 * @see specializations for more info.
 */
template <	class SQL_TYPE,		// connector
				class ODBC_TYPE,	// connector
				class ERROR_TYPE=void,	// SQLErrorFactory
				class MSG_TYPE=ERROR_TYPE,		// SQLErrorFactory
				class OUT_TYPE1=void,	// SQLErrorFactory
				class CODE_TYPE=SQL_TYPE,		// SQLTypeFactory
				class ARGS_TYPE=SQL_TYPE,	// SQLTypeFactory
				class OUT_TYPE2=libdap::BaseType>	// SQLTypeFactory
class SQLBuildDAS {
private:
	SQLBuildDAS(){};
	virtual ~SQLBuildDAS(){};
public:
#if 0
	static bool
		sql_build_das( SQLDataHandlerInterface &dhi ,
				SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>& error_factory,
				SQLActionFactory<CODE_TYPE,ARGS_TYPE,OUT_TYPE2>& type_factory,
				SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>& connector){
		throw SQLInternalError(
				"Unable to use this undefined BuldDAS method, use a specialized one",
				__FILE__,__LINE__);
	}
#endif
};


/**
 * @brief SPECIALIZATION class of SQLBuildDAS which implement basic
 * functions to make this class usable.
 * It provide a group of public wrappers which simplify
 * calls to the main (private) function.
 * @code
 * private:
 * static bool
 * sql_build_das(
 * 		SQLDataHandlerInterface &dhi ,
 * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>* error_factory,
 * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
 * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>& connector)
 * @endcode
 * This is a specialization since there no way to build DAS
 * without a TypeFactory definition.
 * With this specialization you can use the following
 * (provided) implementations of the ActionFactory:
 * - SQLDynamicTypeFactory 	(dynamic type mapping)
 * - SQLObjectTypeFactory	(stateful static type mapping)
 * And deprecated:
 * - SQLStaticTypeFactory	(stateless static type mapping)
 * All usable with the ActionManager implementation
 * called SQLTypeManager (used by the connector action
 * SQLNextTypeAction located in /handler/connector/...)
 * @see SQLDynamicTypeFactory
 * @see SQLObjectTypeFactory
 * @see SQLTypeManager
 * @see SQLNextTypeAction
 * <br>
 * <br>Note also that no Error related specialization is done
 * anyway some assumptions are taken by the connector related
 * actions (/handler/connector/...) such as the use of the
 * ErrorManager.
 * @see SQLQueryAction
 * @see SQLConnectAction
 * @see SQLCloseAction
 *
 *
 */
template <	class SQL_TYPE,		// connector
			class ODBC_TYPE,	// connector
			class ERROR_TYPE,	// SQLErrorFactory
			class MSG_TYPE,		// SQLErrorFactory
			class OUT_TYPE1		// SQLErrorFactory
			>	// SQLTypeFactory
class SQLBuildDAS<	SQL_TYPE,			// connector
					ODBC_TYPE,			// connector
					ERROR_TYPE,			// SQLErrorFactory
					MSG_TYPE,			// SQLErrorFactory
					OUT_TYPE1,			// SQLErrorFactory
					SQL_TYPE,			// SPECIALIZED
					SQL_TYPE,			// SPECIALIZED
					libdap::BaseType> {	// SPECIALIZED
private:
	SQLBuildDAS(){};
	virtual ~SQLBuildDAS(){};
public:

	/**
	 * @brief It is a wrapper of the:
	 * @code
	 * static bool sql_build_das( BESDataHandlerInterface &dhi ,
	 * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>* error_factory,
	 * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
	 * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>& connector)
	 * @endcode
	 * Which is provided to make easy to call the das builder.
	 *
	 * @param dhi SQLDataHandlerInterface reference
	 * @param error_factory a reference to
	 * SQL{Static|Object|Dynamic}ErrorFactory
	 * @param fc a reference to the TypeFactoryComponent
	 * to use
	 * @param connector a reference to a simple connector
	 * SQLSimpleConnector.
	 *
	 * @see SQLObjectTypeFactory
	 * @see ODBCFactoryComponent
	 */
	static bool
	sql_build_das( SQLDataHandlerInterface &dhi ,
		SQLTypeFactoryComponent<SQL_TYPE,ODBC_TYPE>& fc,
		SQLSimpleConnector<SQL_TYPE,ODBC_TYPE>& connector)
	{
		/**
		 * Build the TypeFactory which will be our
		 * BaseType factory.
		 * <br>Note that this is not a BaseTypeFactory
		 * but again an ActionFactory derived object.
		 */
		SQLObjectTypeFactory<SQL_TYPE,ODBC_TYPE> tf=
			SQLObjectTypeFactory<SQL_TYPE,ODBC_TYPE>(fc);

		return sql_build_das(dhi,NULL,tf,connector);
	}

	/**
	 * @brief This is a function which uses the SQLTypeManager
	 * with the SQLObjectTypeFactory to build BaseType object.
	 * It is a wrapper of the:
	 * @code
	 * static bool sql_build_das( SQLDataHandlerInterface &dhi ,
	 * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>* error_factory,
	 * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
	 * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>& connector)
	 * @endcode
	 * Which is provided to make easy to call the das builder.
	 * ODBCFactoryComponent).
	 *
	 * @param dhi SQLDataHandlerInterface reference
	 * @param error_factory a reference to
	 * SQL{Static|Object|Dynamic}ErrorFactory
	 * @param fc a reference to the TypeFactoryComponent
	 * to use
	 * @param connector a reference to a complete connector
	 * SQLConnector.
	 *
	 * @see SQLObjectTypeFactory
	 * @see ODBCFactoryComponent
	 */
	static bool
	sql_build_das( SQLDataHandlerInterface &dhi ,
		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>& error_factory,
		SQLTypeFactoryComponent<SQL_TYPE,ODBC_TYPE>& fc,
		SQLSimpleConnector<SQL_TYPE,ODBC_TYPE>& connector)
	{
		/**
		 * Build the TypeFactory which will be our
		 * BaseType factory.
		 * <br>Note that this is not a BaseTypeFactory
		 * but again an ActionFactory derived object.
		 */
		SQLObjectTypeFactory<SQL_TYPE,ODBC_TYPE> tf=
			SQLObjectTypeFactory<SQL_TYPE,ODBC_TYPE>(fc);

		return sql_build_das(
						dhi,
						&error_factory,
						tf,
						connector);
	}

	/**
	 * @brief It is a public wrapper of the:
	 * @code
	 * static bool sql_build_das( SQLDataHandlerInterface &dhi ,
	 * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>& error_factory,
	 * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
	 * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>& connector)
	 * @endcode
	 * Which is provided to make easy to call the das builder.
	 *
	 * @param dhi SQLDataHandlerInterface reference
	 * @param error_factory a reference to
	 * SQL{Static|Object|Dynamic}ErrorFactory
	 * @param type_factory a reference to the
	 * SQL{Static|Object|Dynamic}TypeFactory used
	 * @param connector a reference to a simple connector
	 * SQLSimpleConnector.
	 *
	 * @see SQLObjectTypeFactory
	 * @see ODBCFactoryComponent
	 */
	static bool
	sql_build_das( SQLDataHandlerInterface &dhi ,
			SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>& error_factory,
			SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
			SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>& connector)
	{

		return sql_build_das(
						dhi,
						&error_factory, // pass as !NULL pointer
						type_factory,
						connector);
	}


private:
	/**
	 * @brief Main static function which implement DAS construction
	 * it make use of the /handler/connector/... actions to
	 * perform connection operation and can be used with or
	 * without ErrorFactory (setting its pointer to NULL).
	 * See wrapper comments for more info
	 *
	 */
static bool
sql_build_das( SQLDataHandlerInterface &dhi ,
		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>* error_factory,
		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
		SQLSimpleConnector<SQL_TYPE,ODBC_TYPE>& connector)
	{
	BESDEBUG(SQL_NAME,"SQLBuildDAS: Start build_das"<<endl);
	BESResponseObject *response =dhi.getResponseObject();
		//dhi.response_handler->get_response_object();

	BESDASResponse *bdas = dynamic_cast < BESDASResponse * >(response);
	if (!bdas)
		throw BESInternalFatalError(
				"Unable to get DASResponse object",__FILE__,__LINE__);
	bdas->set_container( dhi.getBesContainer()->get_symbolic_name() ) ;

	DAS *das = bdas->get_das();

	BESDEBUG(SQL_NAME,"SQLBuildDAS: Connecting "<<endl);
	SQLConnectAction<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>::connect(
					// SQLHandleConnector
					connector,
					// get container from storage
					dhi.getSQLContainer(),
					// check action to do on errors
					error_factory);// try to connect
	BESDEBUG(SQL_NAME,"SQLBuildDAS: Connected"<<endl);

	BESDEBUG(SQL_NAME,"SQLBuildDAS: Executing query"<<endl);
	SQLQueryAction<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>::query(connector,error_factory);
	BESDEBUG(SQL_NAME,"SQLBuildDAS: Query successfully done"<<endl);

	AttrTable *seq=das->get_table(dhi.getBesContainer()->get_symbolic_name());
	if (!seq)
		seq=das->add_table(dhi.getBesContainer()->get_symbolic_name(),new AttrTable());

	/**
	 *  Reset index position
	 */
	connector.resetCol();
	connector.resetRow();
	/**
	 * For each column get the mapped DAP_TYPE
	 * @note No value is read here, only DAP object build.
	 */
	for (size_t i=0; i<connector.getCols(); i++){
		BESDEBUG(SQL_NAME,"SQLBuildDAS: Doing actions on column: "<<i<<
				" of: "<<connector.getCols()<<endl);
		BaseType *bt=NULL;

		BESDEBUG(SQL_NAME,"SQLBuildDAS: getting next object"<<endl);
		bt=SQLNextTypeAction<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE,OUT_TYPE1>::
				nextType(connector,type_factory,error_factory);


		if (bt){// if 'bt' is created
			BESDEBUG(SQL_NAME,"SQLBuildDAS: done"<<endl);
			seq->append_attr(
					bt->name(),
					bt->type_name(),
					connector.getColDesc(i));
					//@todo : methods needed in connector?
			delete bt;
			bt=0;
		}
		else // error occurred on 'bt' creation
		{
			BESDEBUG(SQL_NAME,"SQLBuildDAS: Unrecognized NULL object. "
					"Adding default place-holder."<<endl);
			seq->append_attr(
						_SQLH_DEFAULT_DAS_NAME,
						_SQLH_DEFAULT_DAS_TYPE,
						_SQLH_DEFAULT_DAS_VAL);
		}
	}
	// closing connection
	BESDEBUG(SQL_NAME,"SQLBuildDAS: Closing connection"<<endl);
	SQLCloseAction<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>::close(connector,error_factory);
	BESDEBUG(SQL_NAME,"SQLBuildDAS: Successfully closed"<<endl);

	return true ;
	}

};

#endif /* SQLBUILDDAS_H_ */
