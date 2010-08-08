/*
 * SQLPlugin.cpp
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
 *  Created on: 13/lug/2010
 *      Author: carlo cancellieri
 */

#include "SQLPlugin.h"

SQLPlugin::~SQLPlugin( void )
{
#if 0
	// THIS IS DONE BY THE SQLRequestHandler::remove_sql_handler
	if (!_handler_list.empty()){
#if 0
		// rh is Member of this class now
		if (rh=SQLPlugin::find_RequestHandler()){
#endif
		if (rh){
			Handler_iter i=_handler_list.begin();
			while (i!=_handler_list.end()){
				remove_handler((*i++).first);
			}
		}
		else
			throw BESInternalFatalError(
				"The base SQLRequestHandler is not loaded into the List!",
				__FILE__,__LINE__);
	}
#endif
}

void SQLPlugin::remove_handlers(){
	if (!_handler_list.empty()){
		if (rh){
			Handler_iter i=_handler_list.begin();
			while (i!=_handler_list.end()){
				remove_handler((*i++).first);
			}
		}
		else
			throw BESInternalFatalError(
				"The base SQLRequestHandler is not loaded into the List!",
				__FILE__,__LINE__);
	}
}

bool
SQLPlugin::add_handler( const string &command,
			        sql_request_handler handler_method)
{
    if( this->find_handler( command ) == NULL )
    {
#if 0
    	// rh is Member of this class now
    	if (!rh)
    		rh=SQLPlugin::find_RequestHandler();
#endif
    	/**
    	 *  search into the wrapper for a caller
    	 *  if no wrapper method is called add
    	 *  one.
    	 */
		if (rh) {
			if (rh->add_sql_wrapper(command)) {
				_handler_list[command] = handler_method ;
			}
			else {
				throw BESInternalFatalError(
					"Cannot wrap correctly this command!",
					__FILE__,__LINE__);
			}
			return true ;
		}
		else
			throw BESInternalFatalError(
				"The base SQLRequestHandler is not loaded!",
				__FILE__,__LINE__);
    }
    else
		return false ;
}

bool
SQLPlugin::remove_handler( const string &command)
{
    SQLPlugin::Handler_iter i=this->_handler_list.find( command ) ;
    if( i != this->_handler_list.end() )
    {
    	/**
		 * Update the wrapper count into the SQLRequestHandler
		 * If this is the only SQLPlugin which defines this
		 * command, the wrapping function will be completely
		 * removed.
		 */
#if 0
    	// rh is Member of this class now
    	if (!rh)
    		rh=SQLPlugin::find_RequestHandler();
#endif
    	if (rh) {
        	//update the SQLRequestHandler wrapper usage counter
			if (!rh->remove_sql_wrapper(command)){
				// report the error but can be a not fatal error
				BESDEBUG(SQL_NAME,"This command was not correctly wrapped!");
			}
			// erase the function
			this->_handler_list.erase( i ) ;
			return true ;
    	}
    	else
    		throw BESInternalFatalError(
				"The base SQLRequestHandler is not loaded!",
				__FILE__,__LINE__);
    }
    else
    	return false ;
}

string
SQLPlugin::get_handler_names()
{
    string ret ;
    bool first_name = true ;
    SQLPlugin::Handler_citer i = this->_handler_list.begin() ;
    for( ; i != this->_handler_list.end(); i++ )
    {
		if( !first_name ){
			ret += ", " ;
		}
		ret += (*i).first ;
		first_name = false ;
    }
    return ret ;
}

void
SQLPlugin::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "BESRequestHandler::dump - ("
			     << (void *)this << ")" << endl ;
    BESIndent::Indent() ;
    strm << BESIndent::LMarg << "name: " << _name << endl ;
    if( this->_handler_list.size() )
    {
		strm << BESIndent::LMarg << "registered handler functions:" << endl ;
		BESIndent::Indent() ;
		SQLPlugin::Handler_citer i = this->_handler_list.begin() ;
		SQLPlugin::Handler_citer ie = this->_handler_list.end() ;
		for( ; i != ie; i++ )
		{
			strm << BESIndent::LMarg << (*i).first << endl ;
		}
		BESIndent::UnIndent() ;
    }
    else
    {
    	strm << BESIndent::LMarg << "registered handler functions: none" << endl ;
    }
    BESIndent::UnIndent() ;
}


sql_request_handler
SQLPlugin::find_handler( const string &handler_name )
{
    SQLPlugin::Handler_citer i=this->_handler_list.find( handler_name ) ;
    if( i != this->_handler_list.end() )
    {
    	return (*i).second;
    }
    else
    	return NULL;
}

SQLRequestHandler *
SQLPlugin::find_RequestHandler()
{
	// search for the SQLRequestHandler
	BESRequestHandler *bes_rh=
			BESRequestHandlerList::TheList()->find_handler(SQL_NAME);

    if( bes_rh )
    {
    	return dynamic_cast<SQLRequestHandler*>(bes_rh);
    }
    else
    	return NULL;
}
