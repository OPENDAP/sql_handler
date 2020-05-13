/*
 *  SQLLinker.h
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
 *  Author: carlo cancellieri,	11/Ago/2010
 *
 */
#ifndef SQLLINKER_H_
#define SQLLINKER_H_
#include <BESRequestHandler.h>
#include "SQLPluginList.h"

// class SQLPluginList;

/**
 * @brief Abstract class which define the link
 * layer between shared library module libsql_module
 * and other components of the sql_handler.
 * @see ODBCPluginLoader
 * @see SQLPlugin
 * @see SQLRequestHandler
 */
class SQLLinker:public BESRequestHandler, public SQLPluginList {
public:
	SQLLinker(const std::string &name):
		BESRequestHandler(name){};

	virtual ~SQLLinker(){};

	/**
	 * @brief Method to implement to return an instance
	 * of the SQLPluginList implementing class.
	 * @return the instance of the class implementing the
	 * SQLPluginList interface
	 * @see SQLRequestHandler
	 */
	virtual SQLPluginList *theLink()=0;
};

#endif /* SQLLINKER_H_ */
