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
#if 1
#include "utils/SmartMap.h"
//using smart::SmartValueMap;
#endif
#include "SQLLinker.h"
#include "SQLPluginList.h"
class SQLPlugin;

#include <pthread.h>

// instance_mutex is used to ensure that only one instance is created.
// That is, it protects the body of the HTTPCache::instance() method. This
// mutex is initialized from within the static function once_init_routine()
// and the call to that takes place using pthread_once_init() where the mutex
// once_block is used to protect that call. All of this ensures that no matter
// how many threads call the instance() method, only one instance is ever
// made.
static pthread_mutex_t _mutex;
static pthread_once_t _block = PTHREAD_ONCE_INIT;

#define LOCK(m) pthread_mutex_lock((m))
#define TRYLOCK(m) pthread_mutex_trylock((m))
#define UNLOCK(m) pthread_mutex_unlock((m))
#define INIT(m) pthread_mutex_init((m), 0)
#define DESTROY(m) pthread_mutex_destroy((m))
static void
once_init_routine(){
	if (INIT(&_mutex)!= 0)
		throw BESInternalError(
			"Could not initialize mutex. Exiting.",__FILE__, __LINE__);
}

/**
 * @brief SQLHandler base RequestHandler which is the main SQL
 * request handler.
 * <br>This RequestHandler act as a wrapper for all the SQL type requests.
 * <br>It uses its functions to search and run loaded SQLPlugin
 * which effectively implements Components and methods to build
 * DDS, DAS, DATA ... objects.
 * @see SQLPlugin
 */
class SQLRequestHandler :public SQLLinker, public SQLPluginList {
private:
	/**
	 *  self referred to implement the Singleton pattern)
	 */
	static SQLRequestHandler *_rh;

	typedef smart::SmartValueMap<const string,SQLPlugin> sql_handler_map;
	typedef sql_handler_map::iterator SQLHandler_iterator;
	typedef sql_handler_map::const_iterator	SQLHandler_citer;

	/**
	 *  implements the Singleton pattern
	 */
	static sql_handler_map *_theList;

	typedef std::map<string,size_t> sql_wrap_count_map;
	/**
	 *  track how many (value) SQLPlugins are using a
     *  wrap function (the key is the command name).
	 *  @note: implements the Singleton pattern)
	 */
    static sql_wrap_count_map *_theWrapCount;

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
	bool
	lastChanceRunner(SQLDataHandlerInterface &dhi, const string &command);

	/**
	 * @brief search into the BESRequestHandler list the SQLPlugin
	 * corresponding to the actual api in the dataset server list.
	 * @param a SQLContainer reference which represents the dataset.
	 * @return a SQLPlugin instance ready called
	 * as c.getApi() returned string or NULL pointer if not found.
	 *
	 */
//	static
	SQLPlugin *
	find_sql_plugin(SQLContainer &c);


	/**
	 * @brief default constructor
	 * This constructor uses add_handler to add default command handler
	 * functionalities to the list. (This is default behaviour for a
	 * BESRequestHandler plugin).
	 * @param name the name of this handler
	 */
	SQLRequestHandler(const string &name) :
		SQLLinker(name),
		SQLPluginList()
	{
		/**
		 * initialize unique instance of the block mutex
		 */
		if (pthread_once(&_block,once_init_routine)!=0)
			throw BESInternalError(
				"Could not initialize mutex. Exiting.",__FILE__, __LINE__);

		add_handler(VERS_RESPONSE,SQLRequestHandler::version);
		add_handler(HELP_RESPONSE,SQLRequestHandler::help);
TESTDEBUG(SQL_NAME,"CREATED: SQLRequestHandler"<<endl);
	}

	SQLRequestHandler(const SQLRequestHandler &); // not defined
	SQLRequestHandler & operator=(const SQLRequestHandler &); // not defined

public:
	/**
	 * @brief Lazy initialization of _theList singleton
	 * @return the singleton sql_handler_map
	 * @note: thread safe
	 */
	static sql_handler_map &
	theList(){
		LOCK(&_mutex);
		try {
			if (!SQLRequestHandler::_theList){
TESTDEBUG(SQL_NAME,"SQLRequestHandler: _theList was NULL:"<<
	_theList<<" creating new one"<<endl);
				SQLRequestHandler::_theList=new sql_handler_map();
			}
		}catch(...)
		{
			UNLOCK(&_mutex);
			throw;
		}
		UNLOCK(&_mutex);
		return (*SQLRequestHandler::_rh->_theList);
	}

	/**
	 * @brief Lazy initialization of _theWrapCount singleton
	 * @return the singleton sql_wrap_count_map
	 * @note: thread safe
	 */
	static sql_wrap_count_map &
	theWrapCount(){
		LOCK(&_mutex);
		try{
			if (!SQLRequestHandler::_theWrapCount){
TESTDEBUG(SQL_NAME,"SQLRequestHandler: _theWrapCount was NULL:"<<
	_theWrapCount<<" creating new one"<<endl);
				SQLRequestHandler::_theWrapCount=new sql_wrap_count_map();
			}
		}catch(...)
		{
			UNLOCK(&_mutex);
			throw;
		}
		UNLOCK(&_mutex);
		return (*SQLRequestHandler::_rh->_theWrapCount);
	}

	static SQLRequestHandler *
	theSQLRequestHandler(const string &name){
		LOCK(&_mutex);
		try{
			if (!SQLRequestHandler::_rh){
TESTDEBUG(SQL_NAME,"SQLRequestHandler: _rh was NULL:"<<
	SQLRequestHandler::_rh<<" creating new one"<<endl);
				SQLRequestHandler::_rh=new SQLRequestHandler(name);
			}
		}catch(...)
		{
			UNLOCK(&_mutex);
			throw;
		}
		UNLOCK(&_mutex);
TESTDEBUG(SQL_NAME,"SQLRequestHandler: _rh on addr: "<<
	SQLRequestHandler::_rh<<endl);
		return (SQLRequestHandler::_rh);
	}

    ~SQLRequestHandler(void) {
    	/**
    	 * remove all unregistered sql_handlers
    	 * from the list
    	 */
    	remove_sql_handlers();

    	if (_theList)
			delete _theList;
		_theList=0;
		if (_theWrapCount)
			delete _theWrapCount;
		_theWrapCount=0;

    	DESTROY(&_mutex);

TESTDEBUG(SQL_NAME,"DELETED: SQLRequestHandler"<<endl);
    };
/**
 * @todo
 * @return
 */
    virtual SQLPluginList *theLink(){
    	return theSQLRequestHandler(get_name());
    }

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
    	return theSQLRequestHandler(SQL_NAME)->
    			lastChanceRunner(sql_dhi,dhi.action);
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
    SQLPlugin*
    find_sql_handler(const string& name);

    /**
     * @brief Remove an SQLPlugin from the list.
     * @param name the name of the SQLPlugin to remove
     * @return true if remove is correctly completed
     */
    bool
    remove_sql_handler(const string &name);

    /**
	 * @brief Remove all the SQLPlugin from the list.
	 */
	void
    remove_sql_handlers();

    /**
     * @brief Add an SQLPlugin to the list.
     * @param name the name of the SQLPlugin to add
     * @param handler a new pointer instance to a SQLPlugin
     * implementation
     * @return true if add is correctly completed
     */
    bool
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



};

#endif // SQLRequestHandler.h

