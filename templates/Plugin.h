/*
 * OPENDAP_CLASSPlugin.h
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

#ifndef OPENDAP_CLASSPLUGIN_H_
#define OPENDAP_CLASSPLUGIN_H_

#include "config.h"

/**
 *  TypeFactory
#COMPLETE#
 *  ErrorFactory
#COMPLETE#
 */
#include "action/SQLActionFactory.h"

#include "SQLPlugin.h"

#include "container/SQLContainer.h"

#COMPLETE#

#include "connector/SQLConnector.h"
#include "OPENDAP_CLASSConnector.h"
#include "OPENDAP_CLASSErrorFactoryComponent.h"

#COMPLETE#
#SIMPLE#

#include "connector/SQLSimpleConnector.h"
#include "OPENDAP_CLASSSimpleConnector.h"

#SIMPLE#

#include "OPENDAP_CLASSTypes.h"
#include "OPENDAP_CLASSTypeFactoryComponent.h"

#include "OPENDAP_CLASSResponseNames.h"

#include "SQLResponseNames.h"


// POST CONSTRAINTS
#include <BESDataNames.h>

#DAS#
//DAS
#include "handler/SQLBuildDAS.h"

#DAS#
#DDS#
//DDS
#include "handler/SQLBuildDDS.h"

#DDS#
#DATA#
//DATA
#include "handler/SQLBuildDATA.h"

#DATA#


#include <BESDebug.h>

/**
 * @brief SQLPlugin for the SQLHandler.
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
class OPENDAP_CLASSPlugin : public SQLPlugin {
public:
#DAS#
    /**
     * setting DAS builder types
     * @see ODBCTypes
     * @see SQLBuildDAS
     */
    typedef SQLBuildDAS<SQL_TYPE,            // SQLTypeFactory && connector
#SIMPLE#
            ODBC_TYPE>            // SQLTypeFactory && connector
#SIMPLE#
#COMPLETE#
            ODBC_TYPE,            // SQLTypeFactory && connector
            ERROR_TYPE,        // SQLErrorFactory(IN) && connector
            MSG_TYPE,            // SQLErrorFactory(ARG) && connector
    void>                // SQLErrorFactory(OUT)
#COMPLETE#
    DASBuilder;
#DAS#
#DDS#
    /**
     * setting DATA builder types
     * @see ODBCTypes
     * @see SQLBuildDATA
     */
    typedef SQLBuildDDS<SQL_TYPE,            // SQLTypeFactory && connector
#SIMPLE#
            ODBC_TYPE>            // SQLTypeFactory && connector
#SIMPLE#
#COMPLETE#
            ODBC_TYPE,            // SQLTypeFactory && connector
            ERROR_TYPE,        // SQLErrorFactory(IN) && connector
            MSG_TYPE,            // SQLErrorFactory(ARG) && connector
    void>                // SQLErrorFactory(OUT)
#COMPLETE#
    DDSBuilder;
#DDS#
#DATA#
    /**
     * setting DATA builder types
     * @see ODBCTypes
     * @see SQLBuildDATA
     */
    typedef SQLBuildDATA<SQL_TYPE,            // SQLTypeFactory && connector
#SIMPLE#
            ODBC_TYPE>            // SQLTypeFactory && connector
#SIMPLE#
#COMPLETE#
            ODBC_TYPE,            // SQLTypeFactory && connector
            ERROR_TYPE,        // SQLErrorFactory(IN) && connector
            MSG_TYPE,            // SQLErrorFactory(ARG) && connector
    void>                // SQLErrorFactory(OUT)
#COMPLETE#
    DATABuilder;
#DATA#

    /**
     * @brief Default constructor
     */
    OPENDAP_CLASSPlugin(const string &name) :
            SQLPlugin(name) {
#DAS#
        SQLPlugin::add_handler(DAS_RESPONSE, OPENDAP_CLASSPlugin::build_das);
#DAS#
#DDS#
        SQLPlugin::add_handler(DDS_RESPONSE, OPENDAP_CLASSPlugin::build_dds);
#DDS#
#DATA#
        SQLPlugin::add_handler(DATA_RESPONSE, OPENDAP_CLASSPlugin::build_data);
#DATA#
        SQLPlugin::add_handler(VERS_RESPONSE, OPENDAP_CLASSPlugin::build_vers);

    }

    virtual ~OPENDAP_CLASSPlugin() {}

#DAS#

    /**
     * @brief method to build DAS
     * @param dhi reference to SQLDataHandlerInterface
     * @see comments in code for detailed explanation
     */
    static bool build_das(SQLDataHandlerInterface &c);

#DAS#
#DDS#

    /**
     * @brief method to build DDS
     * @param dhi reference to SQLDataHandlerInterface
     * @see comments in code for detailed explanation
     */
    static bool build_dds(SQLDataHandlerInterface &c);

#DDS#
#DATA#

    /**
     * @brief method to build DATA
     * @param dhi reference to SQLDataHandlerInterface
     * @see comments in code for detailed explanation
     */
    static bool build_data(SQLDataHandlerInterface &c);

#DATA#

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
    static bool build_vers(SQLDataHandlerInterface &c);

};

#endif /* OPENDAP_CLASSPLUGIN_H_ */
