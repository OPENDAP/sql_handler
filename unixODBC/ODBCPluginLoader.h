/*
 * ODBCPluginLoader.h
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
 * Created on: 09/Ago/2010
 * Author: carlo cancellieri
 *
 */

#ifndef ODBCPLUGINLOADER_H_
#define ODBCPLUGINLOADER_H_

#include "BESAbstractModule.h"
#include "ODBCPlugin.h"

/**
 * @brief SQLPluginLoader is the SQL counterpart of a BESAbstractModule
 * and is used to load, initialize and terminate an SQLPlugin.
 */
class ODBCPluginLoader : public BESAbstractModule {
public:
    /**
     * @brief ctor
     */
    ODBCPluginLoader() : BESAbstractModule() {};

    /**
     * @brief dtor
     */
    virtual ~ODBCPluginLoader() {};

    /**
     * @brief initialize SQL components.
     * Register ODBCPlugin into the SQLRequestHandler
     */
    virtual void initialize(const string &modname);

    /**
     * @brief de-register SQLHandler components from the
     * De-register ODBCPlugin from the SQLRequestHandler
     */
    virtual void terminate(const string &modname);

    virtual void dump(ostream &strm) const;
};

#endif /* ODBCPLUGINLOADER_H_ */
