/*
 * OPENDAP_CLASSPlugin.cpp
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

#include "OPENDAP_CLASSPlugin.h"
#DAS#
/**
 * @brief This is the build_das method which fill
 * DAS object using default unixOPENDAP_CLASS static structures.
 *
 * Here is a working example of the Dynamic Type Matching:
 *  @code
 *  // including
 *  #include "DTM/SQLDynamicTypeFactory.h"
 *  ...
 *  // a complete SQLConnector
 *  OPENDAP_CLASSConnector *conn=new OPENDAP_CLASSConnector();
 *
 *  // a multiple FactoryComponent
 *  OPENDAP_CLASSTypeFactoryComponent fc=OPENDAP_CLASSTypeFactoryComponent(*conn);
 *
 *  // using a dynamic TypeFactory
 *  SQLDynamicTypeFactory<SQL_TYPE,OPENDAP_CLASS_TYPE> tf=
 *  	SQLDynamicTypeFactory<SQL_TYPE,OPENDAP_CLASS_TYPE>(*conn);
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
 *  tf.addAction(SQL_C_LONG,new OPENDAP_CLASSFactoryComponent(*conn));
 *  tf.addAction(SQL_C_CHAR,new OPENDAP_CLASSFactoryComponent(*conn));
 *
 *  // running a builder handler
 *  typedef SQLBuildDAS<
 *  	SQL_TYPE,		// connector && SQLTypeFactory
 *		OPENDAP_CLASS_TYPE,		// connector && SQLTypeFactory
 *		ERROR_TYPE,		// SQLErrorFactory(IN) && connector
 *		MSG_TYPE,		// SQLErrorFactory(ARG) && connector
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
OPENDAP_CLASSPlugin::build_das( SQLDataHandlerInterface &dhi ){
#SIMPLE#
	/**
	 * Build the OPENDAP_CLASSCSimpleonnector which implements an
	 * SQLSimpleConnector so this is a SQLTypeConnector
	 */
	OPENDAP_CLASSSimpleConnector conn;
#SIMPLE#
#COMPLETE#
	/**
	 * Build the OPENDAP_CLASSConnector which implements an SQLConnector
	 * so this is a SQLTypeConnector and a SQLErrorConnector
	 */
	OPENDAP_CLASSConnector conn;

	/**
	 * Build the error factory used by this builder
	 * Here we use a StaticErrorFactory.
	 * @see OPENDAP_CLASSErrorFactoryComponent
	 */
	SQLErrorFactory<ERROR_TYPE,MSG_TYPE> ef=
			OPENDAP_CLASSErrorFactoryComponent(conn);

	/**
	 * Set the error factory into the OPENDAP_CLASSConnector for internal
	 * use.
	 * <br>Note that this is not a Connector interface method.
	 */
	conn.setErrorFactory(ef);
#COMPLETE#
	/**
	 * Build the defined FactoryComponent passing the
	 * TypeConnector (which is implemented in OPENDAP_CLASSConnector)
	 * @see OPENDAP_CLASSTypeFactoryComponent
	 * @see OPENDAP_CLASSConnector
	 */
	OPENDAP_CLASSTypeFactoryComponent fc=OPENDAP_CLASSTypeFactoryComponent(conn);

	/**
	 *  Run the data builder static method implemented
	 *  by SQLBuildDAS.
	 *  <br>
	 *  Other working calls:
	 *  DASBuilder::sql_build_das(dhi,ef,tf,conn); // need DTM
	 *  SQLBuildDAS<SQL_TYPE,OPENDAP_CLASS_TYPE>::sql_build_das(dhi,fc,conn);
	 */
#SIMPLE#
	return DASBuilder::sql_build_das(dhi,fc,conn);
#SIMPLE#
#COMPLETE#
	return DASBuilder::sql_build_das(dhi,ef,fc,conn);
#COMPLETE#
}
#DAS#
#DDS#
bool
OPENDAP_CLASSPlugin::build_dds( SQLDataHandlerInterface &dhi ){
#SIMPLE#
	/**
	 * Build the OPENDAP_CLASSCSimpleonnector which implements an
	 * SQLSimpleConnector so this is a SQLTypeConnector
	 */
	OPENDAP_CLASSSimpleConnector *conn=new OPENDAP_CLASSSimpleConnector();
#SIMPLE#
#COMPLETE#
	/**
	 * Build the OPENDAP_CLASSConnector which implements an SQLConnector
	 * so this is a SQLTypeConnector and a SQLErrorConnector
	 */
	OPENDAP_CLASSConnector *conn=new OPENDAP_CLASSConnector();

	/**
	 * Build the error factory used by this builder
	 * Here we use a StaticErrorFactory.
	 * @see OPENDAP_CLASSErrorFactoryComponent
	 */
	SQLErrorFactory<ERROR_TYPE,MSG_TYPE> ef=
			OPENDAP_CLASSErrorFactoryComponent(*conn);
	/**
	 * Set the error factory into the OPENDAP_CLASSConnector for internal
	 * use.
	 * <br>Note that this is not a Connector interface method.
	 */
	conn->setErrorFactory(ef);
#COMPLETE#

	/**
	 * Build the defined FactoryComponent passing the
	 * TypeConnector (which is implemented in OPENDAP_CLASSConnector)
	 * @see OPENDAP_CLASSTypeFactoryComponent
	 * @see OPENDAP_CLASSConnector
	 */
	OPENDAP_CLASSTypeFactoryComponent fc=OPENDAP_CLASSTypeFactoryComponent(*conn);

	/**
	 *  Run the data builder static method implemented
	 *  by SQLPlugin.
	 */
#SIMPLE#
	return DDSBuilder::sql_build_dds(dhi,fc,conn);
#SIMPLE#
#COMPLETE#
	return DDSBuilder::sql_build_dds(dhi,ef,fc,conn);
#COMPLETE#
}
#DDS#
#DATA#
bool
OPENDAP_CLASSPlugin::build_data( SQLDataHandlerInterface &dhi ){
#SIMPLE#
	/**
	 * Build the OPENDAP_CLASSCSimpleonnector which implements an
	 * SQLSimpleConnector so this is a SQLTypeConnector
	 */
	OPENDAP_CLASSSimpleConnector *conn=new OPENDAP_CLASSSimpleConnector();
#SIMPLE#
#COMPLETE#
	/**
	 * Build the OPENDAP_CLASSConnector which implements an SQLConnector
	 * so this is a SQLTypeConnector and a SQLErrorConnector
	 */
	OPENDAP_CLASSConnector *conn=new OPENDAP_CLASSConnector();

	/**
	 * Build the error factory used by this builder
	 * Here we use a StaticErrorFactory.
	 * @see OPENDAP_CLASSErrorFactoryComponent
	 */
	SQLErrorFactory<ERROR_TYPE,MSG_TYPE> ef=
			OPENDAP_CLASSErrorFactoryComponent(*conn);
	/**
	 * Set the error factory into the OPENDAP_CLASSConnector for internal
	 * use.
	 * <br>Note that this is not a Connector interface method.
	 */
	conn->setErrorFactory(ef);
#COMPLETE#
	/**
	 * Build the defined TypeFactoryComponent passing the
	 * TypeConnector (which is implemented in OPENDAP_CLASSConnector)
	 * @see OPENDAP_CLASSTypeFactoryComponent
	 * @see OPENDAP_CLASSConnector
	 */
	OPENDAP_CLASSTypeFactoryComponent fc=OPENDAP_CLASSTypeFactoryComponent(*conn);

	/**
	 *  Run the data builder static method implemented
	 *  by SQLBuildDATA.
	 */
#SIMPLE#
	return DATABuilder::sql_build_data(dhi,fc,conn);
#SIMPLE#
#COMPLETE#
	return DATABuilder::sql_build_data(dhi,ef,fc,conn);
#COMPLETE#
}
#DATA#
bool
OPENDAP_CLASSPlugin::build_vers( SQLDataHandlerInterface &dhi )
{
	//@todo: make a better output layout (indent?)
	BESVersionInfo *info = dynamic_cast<BESVersionInfo *>(
			dhi.getBesHandler().get_response_object());
	info->add_library( OPENDAP_CLASS_NAME , PACKAGE_VERSION ) ;
	return true;
}
