/*
 *  SQLRequestHandler.h
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
 *  Author: carlo cancellieri,	24 Jun 2010
 */

#ifndef I_SQLRequestHandler_H
#define I_SQLRequestHandler_H

#include "sql_config.h"
#include "SQLResponseNames.h"

#include "container/SQLContainer.h"
#include "container/SQLContainerFactory.h"
#include "container/SQLContainerStorageVolatile.h"

#include "container/SQLDataHandlerInterface.h"

#include <BESContainerStorageList.h>
#include <BESInternalFatalError.h>
// inherit
#include <BESRequestHandler.h>

#include <BESResponseHandler.h>
#include <BESResponseNames.h>
#include <BESRequestHandlerList.h>
#include <BESVersionInfo.h>
#include <BESTextInfo.h>
#include <BESDapNames.h>
#include <BESDataDDSResponse.h>
#include <BESDDSResponse.h>
#include <BESDASResponse.h>
#include <BESConstraintFuncs.h>
#include <BESServiceRegistry.h>
#include <BESUtil.h>

#include "utils/SmartMap.h"
using smart::SmartValueMap;

#include "SQLPlugin.h"
class SQLPlugin;

/**
 * @brief SQLHandler base RequestHandler which is the main SQL
 * request handler.
 * <br>This RequestHandler act as a wrapper for all the SQL type requests.
 * <br>It uses its functions to search and run loaded SQLPlugin
 * which effectively implements Components and methods to build
 * DDS, DAS, DATA ... objects.
 * @see SQLPlugin
 */
class SQLRequestHandler : public BESRequestHandler {
public:
	typedef smart::SmartValueMap<const string,SQLPlugin>::iterator
			SQLHandler_iterator;
	typedef smart::SmartValueMap<const string,SQLPlugin>::const_iterator
			SQLHandler_citer;
	/**
	 * @brief default constructor
	 * This constructor uses add_handler to add default command handler
	 * functionalities to the list. (This is default behaviour for a
	 * BESRequestHandler plugin).
	 * @param name the name of this handler
	 */
	SQLRequestHandler( const string &name) :
		BESRequestHandler( name )
	{
		add_handler(VERS_RESPONSE,SQLRequestHandler::version);
		add_handler(HELP_RESPONSE,SQLRequestHandler::help);

TESTDEBUG(SQL_NAME,"CREATED: SQLRequestHandler"<<endl);
	}

    virtual		~SQLRequestHandler( void ) {
#if 0
    	// cs dtor is handled by the BESContainerStorageList
//        if (cs)
//       	delete cs;
#endif

TESTDEBUG(SQL_NAME,"DELETED: SQLRequestHandler"<<endl);
    };

    /**
     * @brief a generic wrapper used by SQLPlugin
     *  to add method wrapper.
     *
     *  @see SQLPlugin
     */
    static bool wrapper(BESDataHandlerInterface &dhi ) {
    	// build the SQLDataHandlerInterface
    	SQLDataHandlerInterface sql_dhi(dhi);
		// call the last chance algorithm
    	return lastChanceRunner(sql_dhi,dhi.action);
    };

	/**
	 * @brief Run version on the Base handler
	 * and on all the SQLPlugins registered
	 */
    static bool version(BESDataHandlerInterface &dhi );

    /**
     * @brief Dump a text representation of this handler
     * and all registered SQLPlugin(s).
     */
    virtual void
    dump( ostream &strm ) const;

    /**
     * @brief Run help version on the SQLRequestHandler
     * and on all the SQLPlugins registered
     */
    static bool help(BESDataHandlerInterface &dhi );

    /**
     * @brief Used to find an SQLPlugin into the list
     * @param name the name of the SQLPlugin to find
     * @return a pointer to the found plugin or NULL
     * if no SQLPlugin is found.
     */
    static SQLPlugin*
    find_sql_handler(const string& name);

    /**
     * @brief Remove an SQLPlugin from the list.
     * @param name the name of the SQLPlugin to remove
     * @return true if remove is correctly completed
     */
    static bool
    remove_sql_handler(const string &name);

    /**
     * @brief Add an SQLPlugin to the list.
     * @param name the name of the SQLPlugin to add
     * @param handler a new pointer instance to a SQLPlugin implementation
     * @return true if add is correctly completed
     */
    static bool
    add_sql_handler(const string& name,	SQLPlugin* handler);

    /**
     * @brief Add wrapper for this command, this will enable
     * the wrapper to call not predefined commands.
     * This method is called by SQLPlugin::add_handler to
     * make sure that the added SQLPlugin handler function
     * is correctly wrapped by a function in the
     * SQLRequestHandler.
     * @return 'true' if the 'command' is already wrapped
     * 'false' otherwise.
     */
    bool
    add_sql_wrapper(const string& command);

    /**
     * @brief test deletion of the wrapper for this command,
     * this will check how many handlers are wrapped by this
     * command, if _wrap_count is '1' the wrapping function
     * is removed.
     * This method is called by SQLPlugin::remove_handler to
     * make sure that the wrapper for the SQLPlugin function
     * is correctly removed.
     * @return 'true' if the 'command' is correctly removed
     * 'false' otherwise.
     */
    bool
    remove_sql_wrapper(const string& command);

private:
    static smart::SmartValueMap<const string,SQLPlugin> _handlerList;

    /**
     *  track how many (value) SQLPlugins are using a
     *  wrap function (the key is the command name).
     */
    std::map<string,size_t> _wrap_count;

    /**
     * update the wrap_count and return
     * @param name the name of the function handler
     * @oaram add set to true if the counter should be
     * increased or to false if should be decreased.
     * @return true only if the function handler
     * (named 'name') should be deleted.
     */
    bool
    update_wrap_count(const string & name, bool add)
    throw (BESInternalFatalError);

	/**
	 * @brief function used to implement recursive calls through
	 * available servers listed into the dataset.
	 * This function look at the actual container in the 'dhi' and
	 * load the corresponding SQLContainer (using setupContainer).
	 * For each server listed into the container lastChanceRunner
	 * tries to load corresponding API SQLPlugin
	 * (using find_sql_plugin), if plugin is found it tries to run
	 * the passed command argument.
	 * This is done until:
	 *  - (OR) server list ends
	 *  - (OR) called handler returns true.
	 *  @param dhi the BESDataHandlerInterface
	 *  @param the requested command (may differ from 'dhi.command')
	 *  @return bool true if response object is correctly built,
	 *  false otherwise.
	 */
	static bool
	lastChanceRunner(SQLDataHandlerInterface &dhi, const string &command);

	/**
	 * @brief search into the BESRequestHandler list the SQLPlugin
	 * corresponding to the actual api in the dataset server list.
	 * @param a SQLContainer reference which represents the dataset.
	 * @return a SQLPlugin instance ready called
	 * as c.getApi() returned string or NULL pointer if not found.
	 *
	 */
	static SQLPlugin *
	find_sql_plugin(SQLContainer &c);

};

#endif // SQLRequestHandler.h

