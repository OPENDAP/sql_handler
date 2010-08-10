/*
 *  SQLPlugin.h
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
 *  Author: carlo cancellieri, 23 Jun 2010
 */


#ifndef SQLPLUGINREQUESTHANDLER_H
#define SQLPLUGINREQUESTHANDLER_H

#include <BESDapNames.h>
#include <BESResponseNames.h>
#include <BESRequestHandler.h>
#include <BESServiceRegistry.h>
#include <BESRequestHandlerList.h>
#include <BESVersionInfo.h>

#include "SQLLinker.h"
#include "SQLPluginList.h"
class SQLPluginList;

#include "SQLResponseNames.h"
//#include "container/SQLContainer.h"
#include "container/SQLDataHandlerInterface.h"
// add_handler
#include <BESInternalFatalError.h>

/**
 * define an sql function handler
 */
typedef bool (*sql_request_handler)(SQLDataHandlerInterface &);

/**
 * @brief SQLPlugin base SQLHandler class which
 * should be extended to build a plugin.
 * SQLPlugin base SQLHandler class which should be extended
 * to build a plugin, it implement methods needed to interact
 * with the base SQLRequestHandler which will wrap any SQL
 * request to the BES engine.
 */
class SQLPlugin {
private:
	// handle handler's function map
    map< string, sql_request_handler > _handler_list ;

    // SQLPlugin name
    string _name ;

    // Pointer to the SQLRequestHandler
    //static SQLRequestHandler *rh; // do not delete
    static SQLPluginList *rh; // do not delete

public:
    typedef map< string, sql_request_handler >::const_iterator Handler_citer ;
    typedef map< string, sql_request_handler >::iterator Handler_iter ;

    virtual const string &	get_name( ) const { return _name ; }

    /**
	 * @brief Find the SQLRequestHandler instance that should be
	 * loaded into the BESRequestHandlerList.
	 * <br>It is used by add_handler and remove_handler to find
	 * the SQLRequestHandler instance to check for the wrapper
	 * function existence or update relative usage counter.
	 * @see SQLRequestHandler
	 *
	 * @return the located instance of the SQLRequestHandler or
	 * NULL if no instance is found.
	 */
	static SQLPluginList * findTheList();

    /**
     * @brief find the method that can handle the specified response object type
     *
     * <br>Find the method that can handle the specified response object type. The
     * response object type is the same as the handler name.
     *
     * @param handler_name name of the method that can fill in the response object type
     * @return the method that can fill in the specified response object type
     * @see BESResponseObject
     * @see BESResponseNames
     */
    virtual sql_request_handler	find_handler( const string &handler_name ) ;

    /**
     * @brief return a comma separated list of response object types handled by
     * this request handler
     *
     * @return the comma separated list of response object types
     * @see BESResponseObject
     * @see BESResponseNames
     */
    virtual string		get_handler_names() ;

    /**
     * @brief dumps information about this object
     *
     * <br>Displays the pointer value of this instance, the name of the request
     * handler, and the names of all registered handler functions
     *
     * @param strm C++ i/o stream to dump the information to
     */
    virtual void dump( ostream &strm ) const ;

	/**
	 * @brief Default constructor.
	 */
	SQLPlugin( const string & name):
		_handler_list(),
		_name( name )//,
//		rh(SQLPlugin::find_RequestHandler())
    {
#if 0
		if (!rh)
			throw BESInternalFatalError(
				"Unable to locate SQLRequestHandler",
				__FILE__,__LINE__);
#endif
	};

	/**
	 * @brief copy constructor.
	 */
	SQLPlugin( const SQLPlugin & p):
		_handler_list(p._handler_list),
		_name( p._name )//,
//		rh(p.rh)
    {
#if 0
		if (!rh)
			throw BESInternalFatalError(
				"Unable to locate SQLRequestHandler",
				__FILE__,__LINE__);
#endif
	};

	/**
	 * @brief add a handler method to the SQLPlugin that knows how to fill
	 * in a specific response object
	 *
	 * <br>Add a handler method for a specific response object to the SQLPlugin.
	 * <br>The handler method takes a reference to a BESDataHandlerInterface and
	 * returns bool, true if the response object is filled in successfully by the
	 * method, false otherwise.
	 *
	 * @param handler_name name of the response object this method can fill in
	 * @param handler_method a function pointer to the method that can fill in the
	 * specified response object
	 * @return true if the handler is added, false if it already exists
	 * @see BESResponseObject
	 * @see BESResponseNames
	 */
	virtual bool add_handler(const string &handler_name,
								sql_request_handler handler_method);

	/**
	 * @brief remove the specified handler method from this SQLPlugin
	 * @param handler_name name of the method to be removed, same as the name of
	 * the response object
	 * @return true if successfully removed, false if not found
	 * @see SQLResponseNames
	 */
    virtual bool remove_handler( const string &handler_name);

    /**
	 * @brief remove all the handler method from this SQLPlugin
	 * @param handler_name name of the method to be removed, same as the name of
	 * the response object
	 * @return true if successfully removed, false if not found
	 * @see SQLResponseNames
	 */
    virtual void remove_handlers();


    /**
	 * @brief If some handler are still in the map,
	 * call remove.
	 * This is done to keep
	 * SQLRequestHandler::_wrap_count updated.
	 */
	virtual		~SQLPlugin( void );

};

#endif // SQLPLUGINREQUESTHANDLER_H
