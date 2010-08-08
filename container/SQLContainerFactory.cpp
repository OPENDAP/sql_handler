/*
 * SQLContainerFactory.cpp
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
 *  Created on: 16/lug/2010
 *      Author: carlo cancellieri
 */

#include "SQLContainerFactory.h"

SQLContainerStorage *SQLContainerFactory::cs=NULL;
SELECT_CONTAINER SQLContainerFactory::_select_func=&selectContainer;

/**
 * Build a container starting from the actual selected container
 * of the passed dhi (dhi.container), or get it from the storage
 * (if available).
 * It also take care of the AGE of the stored file calling, (if
 * needed), methods to update the container, substitute the
 * stored copy and return the updated container.
 * This is the method to call if you want to use the
 * ContainerStorage. If you don't, try to use buildContainer.
 * @param BESDataHandlerInterface &
 * @return an copy of the SQLContainer* or NULL if dhi.container
 * is NULL
 * @see SQLContainer
 */
SQLContainer *
SQLContainerFactory::getContainer(BESDataHandlerInterface &dhi)
{
if (dhi.container) {
	if (cs) {
		BESDEBUG(SQL_NAME,"SQLContainerFactory: SQL storage container found. "<<endl);

		SQLContainer *c;
#if 1
		/**
		 * Try to look if dhi.container is already an SQLContainer.
		 */
		if (c=dynamic_cast<SQLContainer*>(dhi.container)) {
			if (c->isReady()){
				BESDEBUG(SQL_NAME,
					"SQLContainerFactory: Container is already a READY SQLContainer"<<endl);
				//this is a copy
				return c;//c->ptr_duplicate();
			}
			else {
				BESDEBUG(SQL_NAME,
					"SQLContainerFactory: Container is already SQLContainer but not READY"<<endl);
				/**
				 * Since it's not ready let's update it
				 * read dataset, reset time and set status
				 */
				if (!c->setup()){
					BESDEBUG(SQL_NAME,"ERROR: SQLRequestHandler"<<endl);
					throw BESInternalError(
						"SQLContainerFactory: Failed to rebuild sql dataset",
						__FILE__,__LINE__);
				}
				/**
				 * update the stored copy
				 */
				cs->update(c->ptr_duplicate());
				//return the same copy for local (to the caller) use
				return c;
			}
		}
		else
		{
#endif
			BESDEBUG(SQL_NAME,"SQLContainerFactory: Searching for cached container."<<endl);
			// NOTE: using getName
			c=cs->look_for(getName(*dhi.container));
			if (c){
				BESDEBUG(SQL_NAME,"SQLContainerFactory: Container is cached"<<endl);
				/**
				 * Applying new constraints and attributes
				 * coming from the actual request
				 * NOTE: this will delete previous stored constraints
				 */
				c->set_attributes(dhi.container->get_attributes());
				c->set_constraint(dhi.container->get_constraint());

				//return the same copy for local (to the caller) use
				return c;
			}
			else {
				BESDEBUG(SQL_NAME,"SQLContainerFactory: Container not cached: Try to build it"<<endl);
				try {
					BESDEBUG(SQL_NAME,"SQLContainerFactory: Setup SQLContainer: Creating dataset."<<endl);
					/**
					 * Applying new constraints and attributes
					 * coming from the actual request
					 * NOTE: this will delete previous stored constraints
					 */
					SQLContainer *c=buildContainer(*dhi.container);
TESTDEBUG(SQL_NAME,"SQLContainerFactory: Setup SQLContainer: Completed."<<endl);
					/**
					 * Add to the SQLContainerStorageVolatile list
					 * - using add_container since object isReady
					 * - duplicate: list own the passed container
					 */
					cs->add_container(c->ptr_duplicate());

					//return the same copy for local (to the caller) use
					return c;
				}
				catch(exception &e){
					BESDEBUG(SQL_NAME,"SQLContainerFactory: Setup SQLContainer: ERROR->"<<e.what());
					throw BESInternalError(e.what(),__FILE__,__LINE__);
				}
			} //end dhi.container is not an SQLContainer
		} // endif (bc)

	} //endif (cs)
	else
	{
		BESDEBUG(SQL_NAME,"SQLContainerFactory: Not using persistence"<<endl);
		//this is a copy
		SQLContainer *c=buildContainer(*dhi.container);

		//return the same copy for local (to the caller) use
		return c;
	}
}
else // dhi.container is NULL
	return NULL;
}

/**
 * @brief factory method which build an updated container
 * This method use the overrider (if) method to build
 * and update an SQLContainer.
 *
 */
SQLContainer*
SQLContainerFactory::buildContainer(BESContainer & c) throw (BESInternalError)
{
	// NOTE: using getName
	SQLContainer* cont=(*_select_func)(getName(c),
										c.get_real_name(),
										c.get_container_type());

	// copy members
	cont->set_attributes(c.get_attributes());
	cont->set_constraint(c.get_constraint());

	/**
	 *  run setup() which effectively read() and set all
	 *  the private members of the selected container
	 *  returning the final status.
	 */
	if (!cont->isReady())
		if (!cont->setup())
			throw BESInternalError(
				"SQLContainerFactory: Failed to setup sql dataset",
				__FILE__,__LINE__);
	return cont;
}

/**
 * @brief factory method
 *
 * This method use the overrider (if) method to build
 * and update an SQLContainer.
 *
 * @WARNING no way to set attributes and constraints use
 * this only if you want to build a container from scratch
 *
 */
SQLContainer*
SQLContainerFactory::buildContainer(const string &name,
									const string &real_name,
									const string &type) throw (BESInternalError)
{
	// NOTE: using getName
	SQLContainer* cont=(*_select_func)(getName(name,real_name,type),
										real_name,
										type);
	/**
	 *  run setup() which effectively read() and set all
	 *  the private members of the selected container
	 *  returning the final status.
	 */
	if (!cont->isReady())
		if (!cont->setup())
			throw BESInternalError(
				"SQLContainerFactory: Failed to setup sql dataset",
				__FILE__,__LINE__);
	return cont;
}

void
SQLContainerFactory::setPersistence(SQLContainerStorage *container_storage)
{
	if (container_storage){
		BESDEBUG(SQL_NAME,
			"SQLContainerFactory: Assigning persistence"<<endl);
		cs=container_storage;
	}
	else
		BESDEBUG(SQL_NAME,
			"SQLContainerFactory: Set SQLContainerFactory "
			"to use no persistence"<<endl);
}

void
SQLContainerFactory::setSelector(SELECT_CONTAINER select_function){
	if (select_function){
		BESDEBUG(SQL_NAME,
			"SQLContainerFactory: Assigning new selector function"<<endl);
		_select_func=select_function;
	}
	else
		throw BESInternalError("Unable to initialize the SQLContainerFactory"
				"using a NULL SELECT_CONTAINER function ptr",__FILE__,__LINE__);
}
