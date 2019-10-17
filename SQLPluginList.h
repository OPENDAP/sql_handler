/*
 * SQLPluginList.h
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
 *  Created on: 10/Ago/2010
 * 		Author: carlo cancellieri
 */

#ifndef SQLPLUGINLIST_H_
#define SQLPLUGINLIST_H_

#include <string>
#include <BESInternalFatalError.h>

#include "SQLDefinitions.h"
#include "SQLPlugin.h"

class SQLPlugin;

class SQLPluginList {
public:
    SQLPluginList() {};

    virtual ~SQLPluginList() {};

    /**
     * update the wrap_count and return
     * @param name the name of the function handler
     * @oaram add set to true if the counter should be
     * increased or to false if should be decreased.
     * @return true only if the function handler
     * (named 'name') should be deleted.
     */
    virtual bool
    update_wrap_count(const string &name, bool add)
    throw(BESInternalFatalError) = 0;

    /**
     * @brief Used to find an SQLPlugin into the list
     * @param name the name of the SQLPlugin to find
     * @return a pointer to the found plugin or NULL
     * if no SQLPlugin is found.
     */
    virtual SQLPlugin *
    find_sql_handler(const string &name) = 0;

    /**
     * @brief Remove an SQLPlugin from the list.
     * @param name the name of the SQLPlugin to remove
     * @return true if remove is correctly completed
     */
    virtual bool
    remove_sql_handler(const string &name) = 0;

    /**
     * @brief Remove all the SQLPlugin from the list.
     */
    virtual void
    remove_sql_handlers() = 0;

    /**
     * @brief Add an SQLPlugin to the list.
     * @param name the name of the SQLPlugin to add
     * @param handler a new pointer instance to a SQLPlugin
     * implementation
     * @return true if add is correctly completed
     */
    virtual bool
    add_sql_handler(const string &name, SQLPlugin *handler) = 0;

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
    virtual bool
    add_sql_wrapper(const string &command) = 0;

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
    virtual bool
    remove_sql_wrapper(const string &command) = 0;

};

#endif /* SQLPLUGINLIST_H_ */
