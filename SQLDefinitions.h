/*
 * SQLDefinitions.h
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
 *  Created on: 05/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLDEFINITIONS_H_
#define SQLDEFINITIONS_H_


// DEBUG
#include "SQLResponseNames.h"
#include <BESDebug.h>
#ifdef __TESTS__
#define TESTDEBUG(X,Y) BESDEBUG(X,Y)
#else
#define TESTDEBUG(X,Y)
#endif




/**
 * catalog
 * SQLModule::initialize
 * SQLModule::terminate
 */
#define _SQLH_CATALOG "catalog"
#define _SQLH_STORAGE "sqlstorage"

#if 0
/**
 * NO MORE NEEDED
 *
 * SQLPlugin
 */
#define _SQLH_DAS_RESPONSE "get.sql.das"
#define _SQLH_DDS_RESPONSE "get.sql.dds"
#define _SQLH_DATA_RESPONSE "get.sql.data"
#define _SQLH_HELP_RESPONSE "show.sql.help"
#define _SQLH_VERS_RESPONSE "show.sql.vers"
#endif
/**
 * SQLCheckPoint
 */
#define _SQLH_CHECK_SEPARATOR ','
#define _SQLH_CHECK_KEY "SQL.CheckPoint"

/**
 * SQLInternalError
 * SQLInternalFatalError
 *
 * #define BES_INTERNAL_ERROR 1
 * #define BES_INTERNAL_FATAL_ERROR 2
 * #define BES_SYNTAX_USER_ERROR 3
 * #define BES_FORBIDDEN_ERROR 4
 * #define BES_NOT_FOUND_ERROR 5
 */
#define _SQLH_INTERNAL_ERROR 6
#define _SQLH_INTERNAL_FATAL_ERROR 7

#define _SQLH_STACK_REPORT_SIZE 5

/**
 * Actions
 */

// DEM/SQLDebugAction
#define _SQLH_DEBUG_PREFIX "Action->DEBUG: "
#define _SQLH_FATAL_PREFIX "Action->FATAL: "
#define _SQLH_EXCEPTION_PREFIX "Action->EXCEPTION: "

// SQLBuildDAS
//@todo: move to BuildDASDefinitions
#define _SQLH_DEFAULT_DAS_NAME "DefaultName"
#define _SQLH_DEFAULT_DAS_TYPE "String"
#define _SQLH_DEFAULT_DAS_VAL "Place-holder for unread variables"

#if 0
// connector
#define _SQLH_ROW_CACHE_SIZE "BES.sql.connector.cache_rows"

/**
 * SQLCacheManager
 */
#define _SQLH_CACHE_THRESHOLD "BES.sql.cache_threshold"
#define _SQLH_CACHE_DIR "BES.sql.CacheDir"
#define _SQLH_CACHE_PREFIX "BES.sql.CachePrefix"
#define _SQLH_CACHE_SIZE "BES.sql.CacheSize"
#endif
#endif /* SQLDEFINITIONS_H_ */
