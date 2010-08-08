/*
 * SQLModule.h
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
 * Created on: 24/Jun/2010
 * Author: carlo cancellieri
 *
 */

#ifndef I_SQLModule_H
#define I_SQLModule_H 1

#include "BESAbstractModule.h"
#include "SQLRequestHandler.h"

/**
 * @brief The module of the SQLHandler it
 * load the main components of the handler.
 */
class SQLModule : public BESAbstractModule
{
public:
	/**
	 * SQLRequestHandler is a wrapper for ALL Requests of
	 * type sql.
	 * It is build at the SQLModule load and deleted when
	 * it is unloaded.
	 */
	static SQLRequestHandler rh;

    /**
     *  load and update instances of SQLContainer(s)
     *  from/to the SQLContainerStorage and the filesystem
     */
    static SQLContainerFactory cf;

/**
 * @note: this implementation of SQLRequestHandler uses
 * SQLContainerStorageVolatile if you want to implement
 * your own persistence you can use the
 * SQLContainer::setPersistence(SQLContainerStorage*)
 * to change it on the fly.
 * You could also set it to NULL to make SQLContainerFactory
 * to rebuild the SQLContainer each time it is requested.
 */
    // store instances of loaded SQLContainer(s)
    static SQLContainerStorage *cs;

	/**
	 * @brief ctor
	 */
	SQLModule() {}
    /**
     * @brief dtor
     */
	virtual	~SQLModule() {}
	/**
	 * @brief initialize BES components.
	 * Register SQLHandler components from the BES environment
	 */
    virtual void		initialize( const string &modname ) ;
    /**
     * @brief de-register SQLHandler components from the
     * BES environment
     */
    virtual void		terminate( const string &modname ) ;

    virtual void		dump( ostream &strm ) const ;
} ;

#endif // I_SQLModule_H

