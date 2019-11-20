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
#include <BESInternalError.h>

#include <BESVersionInfo.h>
#include <BESTextInfo.h>
#include <BESDapNames.h>

#include "utils/SmartMap.h"

#include "SQLLinker.h"

#include <BESDataDDSResponse.h>
#include <BESDDSResponse.h>
#include <BESDASResponse.h>
#include <BESResponseHandler.h>
#include <BESResponseNames.h>
#include <BESRequestHandlerList.h>
#include <BESConstraintFuncs.h>
#include <BESServiceRegistry.h>
#include <BESUtil.h>

#include <pthread.h>

// get the lock
#define LOCK(m) pthread_mutex_lock((m))
// test the lock
#define TRYLOCK(m) pthread_mutex_trylock((m))
// unlock
#define UNLOCK(m) pthread_mutex_unlock((m))
// initialize the mytex
#define INIT(m) pthread_mutex_init((m), 0)
// destroy the mutex
#define DESTROY(m) pthread_mutex_destroy((m))

/**
 * @brief type definition of the list of SQLPlugin
 */
typedef smart::SmartValueMap<const std::string, SQLPlugin> sql_handler_map;

/**
 * @brief type definition of the list of wrapped functions
 */
typedef std::map<std::string, size_t> sql_wrap_count_map;

/**
 * @brief SQLHandler base RequestHandler which is the main SQL
 * request handler.
 * <br>This RequestHandler act as a wrapper for all the SQL type requests.
 * <br>It uses its functions to search and run loaded SQLPlugin
 * which effectively implements Components and methods to build
 * DDS, DAS, DATA ... objects.
 * @implements the SQLLinker abstract class
 * @note implement a multiple (lazy safe) singleton of its members and
 * itself.
 * @see SQLLinker
 * @see SQLPluginList
 * @see BESRequestHandler
 * @see SQLPlugin
 */
class SQLRequestHandler : public SQLLinker {
private:
    /**
     * instance_mutex is used to ensure that only one instance is created.
     * It protects the body of the theXXX methods. This mutex is initialized
     * from within the static function once_init_routine() and the call to
     * that takes place using pthread_once_init() where the mutex once_block
     * is used to protect that call. All of this ensures that no matter how
     * many threads call the instance() method, only one instance is ever made.
     */
    static pthread_mutex_t _mutex;
    static pthread_once_t _block;

    /**
     * @brief mutex initialization routine
     */
    static void once_init_routine() {
        if (INIT(&_mutex) != 0)
            throw BESInternalError("Could not initialize mutex. Exiting.", __FILE__, __LINE__);
    }

    /**
     * @brief self referred to implement the Singleton pattern
     */
    static SQLRequestHandler *_rh;

    typedef sql_handler_map::iterator SQLHandler_iterator;
    typedef sql_handler_map::const_iterator SQLHandler_citer;

    /**
     * @brief list of loaded SQLPlugin
     *  implements the Singleton pattern
     */
    static sql_handler_map *_theList;

    /**
     * @brief track how many (value) SQLPlugins are using a
     * wrap function (the key is the command name).
     * @note: implements the Singleton pattern)
     */
    static sql_wrap_count_map *_theWrapCount;

    /**
     * @brief Update the wrap_count and return
     * @param name the name of the function handler
     * @oaram add set to true if the counter should be
     * increased or to false if should be decreased.
     * @return true only if the function handler
     * (named 'name') should be deleted.
     */
    bool update_wrap_count(const std::string &name, bool add) throw(BESInternalFatalError);

    /**
     * @todo when an incoming request fails due to bad query, the
     * same constraints are applied to the next section of the dataset
     * which may has (probably) different column and tables names
     * so constraints will not match and all the query to the following
     * sections will fail.
     * <br>This not happen when constraints are empty
     * <br>SOLUTIONs:
     * <br>Administrator: can set homogeneous views with same columns
     * and attributes for each database used by this dataset.
     * <br>Coding: we can add some specification to dataset which
     * tells if sections are homogeneous or not, in this case (not
     * homogeneous) we use only the fist 'working' (can connect) section.
     *
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
    bool lastChanceRunner(SQLDataHandlerInterface &dhi, const std::string &command);

    /**
     * @brief search into the BESRequestHandler list the SQLPlugin
     * corresponding to the actual api in the dataset server list.
     * @param a SQLContainer reference which represents the dataset.
     * @return a SQLPlugin instance ready called
     * as c.getApi() returned string or NULL pointer if not found.
     *
     */
    SQLPlugin *find_sql_plugin(SQLContainer &c);

    /**
     * @brief default constructor
     * This constructor uses add_handler to add default command handler
     * functionalities to the list. (This is default behaviour for a
     * BESRequestHandler plugin).
     * @param name the name of this handler
     */
    SQLRequestHandler(const std::string &name);

    SQLRequestHandler(const SQLRequestHandler &); // not defined
    SQLRequestHandler &operator=(const SQLRequestHandler &); // not defined

public:
    /**
     * @brief destructor
     * @note do not destruct this class, only its members,
     * you have to delete the SQLRequestHandler instance outside.
     */
    ~SQLRequestHandler(void);

// SINGLETON
    /**
     * @brief Lazy initialization of _theList singleton
     * @return the singleton sql_handler_map
     * @note: thread safe
     */
    static sql_handler_map &theList();

// SINGLETON
    /**
     * @brief Lazy initialization of _theWrapCount singleton
     * @return the singleton sql_wrap_count_map
     * @note: thread safe
     */
    static sql_wrap_count_map &theWrapCount();

// SINGLETON
    /**
     * @brief Lazy initialization of the class
     * @return the singleton of this class
     * @note: thread safe
     */
    static SQLRequestHandler *theSQLRequestHandler(const std::string &name);


// The SQLLink interface implementation
    /**
     * @brief Implementation of 'SQLLink' interface
     * @return pointer to the this instance (singleton)
     */
    virtual SQLPluginList *theLink() {
        return theSQLRequestHandler(get_name());
    }


// SQLRequestHandler specific method
    /**
     * @brief a generic wrapper used by SQLPlugin
     *  to add method wrapper.
     *
     *  @see SQLPlugin
     */
    static bool wrapper(BESDataHandlerInterface &dhi) {
        // build the SQLDataHandlerInterface
        SQLDataHandlerInterface sql_dhi(dhi);
        // call the last chance algorithm
        return theSQLRequestHandler(SQL_NAME)->
                lastChanceRunner(sql_dhi, dhi.action);
    };

    /**
     * @brief Run version on the Base handler
     * and on all the SQLPlugins registered
     */
    static bool version(BESDataHandlerInterface &dhi);

    /**
     * @brief Run help version on the SQLRequestHandler
     * and on all the SQLPlugins registered
     */
    static bool help(BESDataHandlerInterface &dhi);


// BESRequestHandler override
    /**
     * @brief Dump a text representation of this handler
     * and all registered SQLPlugin(s).
     */
    virtual void
    dump(std::ostream &strm) const;


//SQLPluginList methods implementation
    /**
     * @brief Used to find an SQLPlugin into the list
     * @param name the name of the SQLPlugin to find
     * @return a pointer to the found plugin or NULL
     * if no SQLPlugin is found.
     */
    SQLPlugin *find_sql_handler(const std::string &name);

    /**
     * @brief Remove an SQLPlugin from the list.
     * @param name the name of the SQLPlugin to remove
     * @return true if remove is correctly completed
     */
    bool
    remove_sql_handler(const std::string &name);

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
    add_sql_handler(const std::string &name, SQLPlugin *handler);

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
    add_sql_wrapper(const std::string &command);

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
    remove_sql_wrapper(const std::string &command);
};

#endif // SQLRequestHandler.h

