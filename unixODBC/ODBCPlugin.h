/*
 * ODBCPlugin.h
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
 *  Created on: 07/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef ODBCPLUGIN_H_
#define ODBCPLUGIN_H_

#include "connector/SQLConnector.h"
// TypeFactory
// ErrorFactory
#include "action/SQLActionFactory.h"

#include "SQLPlugin.h"

#include "container/SQLContainer.h"

//DAS
#include "handler/SQLBuildDAS.h"
//DATA
#include "handler/SQLBuildDATA.h"
//DDS
#include "handler/SQLBuildDDS.h"

#include "unixODBC/ODBCConnector.h"
#include "unixODBC/ODBCTypes.h"

#include "ODBCTypeFactoryComponent.h"
// checking errors
#include "ODBCErrorFactoryComponent.h"
#include "DEM/SQLErrorManager.h"

// unixODBC plugin version
#define ODBC_VERSION "0.0.3"

/**
 * @brief unixODBC the default plugin used by the SQLHandler.
 *
 * This class uses commands definitions found in SQLDefinitions
 * to register static methods.
 * <br>These methods are SQL counterpart of the standard BES
 * commands and are called by the SQLRequestHandler which is
 * the base RequestHandler of the SQLHandler (Hyrax driver).
 * @see ODBCPlugin
 * @see SQLRequestHandler
 * @see BESRequestHandler
 * @see BESDapNames
 *
 * To make possible to develop different API specific SQL
 * drivers should define some (hopefully all) of the following
 * types which will be used by template class of the SQLHandler:
 * 	-	SQL_TYPE						// SQLTypeConnector
 * 	-	ODBC_TYPE						// SQLTypeConnector
 * 	-	ERROR_TYPE						// SQLErrorFactory && SQLErrorConnector
 * 	-	MSG_TYPE = ERROR_TYPE			// SQLErrorFactory && SQLErrorConnector
 * 	-	OUT_TYPE1 = void				// SQLErrorFactory
 * 	-	CODE_TYPE = SQL_TYPE			// SQLTypeFactory
 * 	-	ARGS_TYPE = SQL_TYPE			// SQLTypeFactory
 * 	-	OUT_TYPE2 = libdap::BaseType	// SQLTypeFactory
 * <br>They are used by the indicated class, look at the class
 * documentation for more info.
 * <br>Some of those parameter are settled by default and are not
 * fixed to be able to use different ActionFactory, (your
 * own implementation)
 *
 * <br>Look at the ODBCTypes to se defined types used by
 * the unixODBC SQLPlugin.
 *
 * <br>This handler implements statically many of the
 * functionalities provided by the SQLHandler toolkit,
 * this is done to provide as much performance as
 * possible.
 * <br>
 * <br>Note: using static structures, may
 * result hard or sometime impossible, if so,
 * take a look to dynamic components and note
 * that some example are provided into the code.
 *
 */
class ODBCPlugin : public SQLPlugin	{
public:
	/**
	 * setting DAS builder types
	 * @see ODBCTypes
	 * @see SQLBuildDAS
	 */
	typedef	SQLBuildDAS< SQL_TYPE,			// SQLTypeFactory && connector
						 ODBC_TYPE,			// SQLTypeFactory && connector
						 ERROR_TYPE,		// SQLErrorFactory(IN) && connector
						 MSG_TYPE,			// SQLErrorFactory(ARG) && connector
						 void>				// SQLErrorFactory(OUT)
								DASBuilder;

	/**
	 * setting DATA builder types
	 * @see ODBCTypes
	 * @see SQLBuildDATA
	 */
	typedef	SQLBuildDDS< SQL_TYPE,			// SQLTypeFactory && connector
						 ODBC_TYPE,			// SQLTypeFactory && connector
						 ERROR_TYPE,		// SQLErrorFactory(IN) && connector
						 MSG_TYPE,			// SQLErrorFactory(ARG) && connector
						 void>				// SQLErrorFactory(OUT)
								DDSBuilder;
	/**
	 * setting DATA builder types
	 * @see ODBCTypes
	 * @see SQLBuildDATA
	 */
	typedef	SQLBuildDATA<SQL_TYPE,			// SQLTypeFactory && connector
						 ODBC_TYPE,			// SQLTypeFactory && connector
						 ERROR_TYPE,		// SQLErrorFactory(IN) && connector
						 MSG_TYPE,			// SQLErrorFactory(ARG) && connector
						 void>				// SQLErrorFactory(OUT)
								DATABuilder;
		/**
		 * @brief Default constructor
		 */
	ODBCPlugin(const string &name):	SQLPlugin(name){

		SQLPlugin::add_handler(DAS_RESPONSE, ODBCPlugin::build_das ) ;
		SQLPlugin::add_handler(DDS_RESPONSE, ODBCPlugin::build_dds ) ;
		SQLPlugin::add_handler(DATA_RESPONSE, ODBCPlugin::build_data ) ;
		SQLPlugin::add_handler(HELP_RESPONSE, ODBCPlugin::build_help ) ;
		SQLPlugin::add_handler(VERS_RESPONSE, ODBCPlugin::build_vers ) ;

TESTDEBUG(SQL_NAME,"CREATED: unixODBC Plugin"<<endl);
	};

	virtual ~ODBCPlugin(){
TESTDEBUG(SQL_NAME,"DELETED: unixODBC PLUGIN"<<endl);
	};

	/**
	 * @brief method to build DAS
	 * @param dhi reference to SQLDataHandlerInterface
	 * @see comments in code for detailed explanation
	 */
	static bool	build_das( SQLDataHandlerInterface &c );

	/**
	 * @brief method to build DDS
	 * @param dhi reference to SQLDataHandlerInterface
	 * @see comments in code for detailed explanation
	 */
	static bool	build_dds( SQLDataHandlerInterface &c );

	/**
	 * @brief method to build DATA
	 * @param dhi reference to SQLDataHandlerInterface
	 * @see comments in code for detailed explanation
	 */
	static bool build_data( SQLDataHandlerInterface &c );
	/**
	 * @brief Override default SQLPlugin::version
	 * static version (passing its address to the constructor).
	 * <br>Usually you don't need to do so. Here we have to
	 * use different PACKAGE_NAME and PACKAGE_VERSION variables
	 * since unixODBC is bundled in the same package of the
	 * SQLHandler.
	 * @param SQLDataHandlerInterface &c
	 * @see SQLPlugin::version()
	 * @see ODBCPlugin::version()
	 */
	static bool	build_vers( SQLDataHandlerInterface &c );

	/**
	 * @brief Override default SQLPlugin::help
	 * static version (passing its address to the constructor).
	 * <br>Usually you don't need to do so. Here we have to
	 * use different PACKAGE_NAME and PACKAGE_VERSION variables
	 * since unixODBC is bundled in the same package of the
	 * SQLHandler.
	 * @param SQLDataHandlerInterface &c
	 * @see SQLPlugin::help()
	 * @see ODBCPlugin::help()
	 */
	static bool build_help( SQLDataHandlerInterface &c );

};

#endif /* ODBCPLUGIN_H_ */
