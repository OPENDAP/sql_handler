/*
 * SQLContainerStorageVolatile.h
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
 *  Created on: 04/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLCONTAINERSTORAGEVOLATILE_H_
#define SQLCONTAINERSTORAGEVOLATILE_H_

#include <BESContainerStorageVolatile.h>
#include <BESDebug.h>
#include "SQLDefinitions.h"
#include <BESInternalError.h>
#include <BESInternalFatalError.h>
#include <typeinfo>

#include "SQLContainer.h"
#include "SQLContainerStorage.h"

/**
 * @brief SQLContainerStorageVolatile is a temporary
 * list of loaded SQLContainer.
 * When a SQLContainer is loaded into this storage,
 * represented dataset should be completely read.
 */
class SQLContainerStorageVolatile:
	public BESContainerStorageVolatile, public SQLContainerStorage{

public:
		SQLContainerStorageVolatile(const string & name):
			BESContainerStorageVolatile(name),
			SQLContainerStorage()
		{
TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLContainerStorageVolatile"<<endl);
		};

		/**
		 * Simply wrap the inherited add_container
		 */
		void add_container(SQLContainer *c)
		{
			BESContainerStorageVolatile::add_container(c);
		}

		/**
		 * Simply wrap the inherited del_container
		 */
		bool del_container(const string &name){
			return BESContainerStorageVolatile::del_container(name);
		}

		/**
		 * Simply wrap the inherited del_containers
		 */
		bool del_containers(){
			return BESContainerStorageVolatile::del_containers();
		}

		/**
		 * @brief look_for method to implement
		 * checks on modification time and rebuild
		 * mechanisms of the SQLContainers before
		 * returning.
		 * <br>Can return NULL pointer
		 * @param name the symbolic name of the container
		 * @return an updated SQLContainer pointer if
		 * passed name match a container in the list
		 * or NULL if no container with that name is found
		 */
		SQLContainer * look_for(const string &name);

		/**
		 * @brief Update the content of the ContainerStorage with
		 * the passed SQLContainer object.
		 * @note: You have to pass a copy.
		 */
		bool update(SQLContainer *c);

		virtual ~SQLContainerStorageVolatile(){
TESTDEBUG(SQL_NAME_TEST,"DELETING: SQLContainerStorageVolatile"<<endl);
			del_containers();
		};


#if 0
		// commented out: not used && to update for use SQLContainerFactory
		void add_container(const string & name,
							const string & real_name,
							const string & type)
		{
//			if (_cf)
				BESContainerStorageVolatile::add_container(
					_cf.getContainer(name,real_name,type));
#if 0
			else
				throw BESInternalFatalError(
					"SQLContainerStorageVolatile: Unable to load "
					"SQLContainer(s), passed factory ptr is NULL",
					__FILE__,__LINE__);
#endif
			dump(std::cerr);
		}
#endif

};

#endif /* SQLCONTAINERSTORAGEVOLATILE_H_ */
