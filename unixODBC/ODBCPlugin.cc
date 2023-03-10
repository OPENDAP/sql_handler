/*
 * ODBCPlugin.cpp
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

#include "ODBCPlugin.h"

/**
 * @brief This is the build_das method which fill
 * DAS object using default unixODBC static structures.
 *
 * Here is a working example of the Dynamic Type Matching:
 *  @code
 *  // including
 *  #include "DTM/SQLDynamicTypeFactory.h"
 *  ...
 *  // a complete SQLConnector
 *  ODBCConnector *conn=new ODBCConnector();
 *
 *  // a multiple FactoryComponent
 *  ODBCTypeFactoryComponent fc=ODBCTypeFactoryComponent(*conn);
 *
 *  // using a dynamic TypeFactory
 *  SQLDynamicTypeFactory<SQL_TYPE,ODBC_TYPE> tf=
 *  	SQLDynamicTypeFactory<SQL_TYPE,ODBC_TYPE>(*conn);
 *
 *  // adding mapping
 *  // with __CLONE__ set to 1
 *  tf.addAction(SQL_C_LONG,&fc);
 *  ...
 *  tf.addAction(SQL_C_CHAR,&fc);
 *  // objects wil be cloned to be
 *  // pushed safely into the
 *  // DynamicActionList
 *  // which is a smart::SmartList
 *
 *  // with __CLONE__ set to 0
 *  tf.addAction(SQL_C_LONG,new ODBCFactoryComponent(*conn));
 *  tf.addAction(SQL_C_CHAR,new ODBCFactoryComponent(*conn));
 *
 *  // running a builder handler
 *  typedef SQLBuildDAS<
 *  	SQL_TYPE,		// connector && SQLTypeFactory
 *		ODBC_TYPE,		// connector && SQLTypeFactory
 *		error_type,		// SQLErrorFactory(IN) && connector
 *		message_t,		// SQLErrorFactory(ARG) && connector
 *		void>			// SQLErrorFactory(OUT)
 *				buildDAS;
 *	DASbuilder::sql_build_das(dhi,ef,tf,*conn);
 *  @endcode
 *
 *  NOTE: obviously, it's better to code a different
 *  FactoryComponent for each SQL_CODE so consider this
 *  a working demonstrative example.
 */
bool
ODBCPlugin::build_das(SQLDataHandlerInterface &dhi) {
    /**
     * Build the ODBCConnector which implements an SQLConnector
     * so this is a SQLTypeConnector and a SQLErrorConnector
     */
    ODBCConnector conn;

    /**
     * Build the error factory used by this builder
     * Here we use a StaticErrorFactory.
     * @see ODBCErrorFactoryComponent
     */
    ODBCErrorFactoryComponent efc(conn);
    SQLErrorFactory<error_type, message_t> ef = efc;

    /**
     * Set the error factory into the ODBCConnector for internal
     * use.
     * <br>Note that this is not a Connector interface method.
     */
    conn.setErrorFactory(ef);

    /**
     * Build the defined FactoryComponent passing the
     * TypeConnector (which is implemented in ODBCConnector)
     * @see ODBCTypeFactoryComponent
     * @see ODBCConnector
     */
    ODBCTypeFactoryComponent fc = ODBCTypeFactoryComponent(conn);

    /**
     *  Run the data builder static method implemented
     *  by SQLBuildDAS.
     *  <br>
     *  Other working calls:
     *  DASBuilder::sql_build_das(dhi,ef,tf,conn); // need DTM
     *  SQLBuildDAS<SQL_TYPE,ODBC_TYPE>::sql_build_das(dhi,fc,conn);
     */
    return DASBuilder::sql_build_das(dhi, ef, fc, conn);
}

bool
ODBCPlugin::build_dds(SQLDataHandlerInterface &dhi) {
    /**
     * Build the ODBCConnector which implements an SQLConnector
     * so this is a SQLTypeConnector and a SQLErrorConnector
     */
    ODBCConnector *conn = new ODBCConnector();

    /**
     * Build the error factory used by this builder
     * Here we use a StaticErrorFactory.
     * @see ODBCErrorFactoryComponent
     */
    ODBCErrorFactoryComponent efc(*conn);
    SQLErrorFactory<error_type, message_t> ef = efc;
    /**
     * Set the error factory into the ODBCConnector for internal
     * use.
     * @note: that this is not a Connector interface method.
     */
    conn->setErrorFactory(ef);

    /**
     * Build the defined FactoryComponent passing the
     * TypeConnector (which is implemented in ODBCConnector)
     * @see ODBCTypeFactoryComponent
     * @see ODBCConnector
     */
    ODBCTypeFactoryComponent fc = ODBCTypeFactoryComponent(*conn);

    /**
     *  Run the data builder static method implemented
     *  by SQLPlugin.
     */
    return DDSBuilder::sql_build_dds(dhi, ef, fc, conn);
}

bool
ODBCPlugin::build_data(SQLDataHandlerInterface &dhi) {
    /**
     * Build the ODBCConnector which implements an SQLConnector
     * so this is a SQLTypeConnector and a SQLErrorConnector
     */
    ODBCConnector *conn = new ODBCConnector();

    /**
     * Build the error factory used by this builder
     * Here we use a StaticErrorFactory.
     * @see ODBCErrorFactoryComponent
     */
    ODBCErrorFactoryComponent efc(*conn);
    SQLErrorFactory<error_type, message_t> ef = efc;
    /**
     * Set the error factory into the ODBCConnector for internal
     * use.
     * @note that this is not a Connector interface method.
     */
    conn->setErrorFactory(ef);

    /**
     * Build the defined TypeFactoryComponent passing the
     * TypeConnector (which is implemented in ODBCConnector)
     * @see ODBCTypeFactoryComponent
     * @see ODBCConnector
     */
    ODBCTypeFactoryComponent fc = ODBCTypeFactoryComponent(*conn);

    /**
     *  Run the data builder static method implemented
     *  by SQLBuildDATA.
     */
    return DATABuilder::sql_build_data(dhi, ef, fc, conn);
}

bool
ODBCPlugin::build_vers(SQLDataHandlerInterface &dhi) {
    /**
     * @todo: make a better output layout (indent?)
     */
    BESVersionInfo *info = dynamic_cast<BESVersionInfo *>(
            dhi.getBesHandler().get_response_object());
    info->add_library(ODBC_NAME, ODBC_VERSION);
    return true;
}

bool
ODBCPlugin::build_help(SQLDataHandlerInterface &dhi) {
    BESInfo *info = dynamic_cast<BESInfo *>(
            dhi.getBesHandler().get_response_object());
/**
 * @todo: This is an example. If you had a help file you
 * could load it like this and if your handler handled
 * the following responses.
 */
    map<string, string> attrs;
    attrs["name"] = ODBC_NAME;
    attrs["version"] = ODBC_VERSION;
    list<string> services;
    BESServiceRegistry::TheRegistry()->services_handled(SQL_NAME, services);
    if (services.size() > 0) {
        string handles = BESUtil::implode(services, ',');
        attrs["handles"] = handles;
    }
    info->begin_tag("module", &attrs);
    //info->add_data_from_file( "SQL.Help", "SQL Help" ) ;
    info->end_tag("module");

    return true;
}
