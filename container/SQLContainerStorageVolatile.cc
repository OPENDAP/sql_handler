/*
 * SQLContainerStorageVolatile.cpp
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
 *  Created on: 19/lug/2010
 *      Author: carlo cancellieri
 */
#include "SQLContainerStorageVolatile.h"

bool
SQLContainerStorageVolatile::update(SQLContainer *c){
	bool ret;
	// update stored copy
	if (this->del_container(c->get_symbolic_name())){
		ret=true;
	}
	else {
		ret=false;
	}
	// list own the passed container
	this->add_container(c);
	return ret;
}

SQLContainer *
SQLContainerStorageVolatile::look_for(const string &name){
	BESDEBUG(SQL_NAME,"SQLContainerStorage:CASTING"<<endl);
	/**
	 * It's important to note that here the SQLContainer
	 * copy constructor is called which, (during copy),
	 * will check the status (isReady && isUpToDate)
	 * of the passed object setting isReady status
	 * accordingly.
	 * @see SQLContainer
	 */
	SQLContainer *c=dynamic_cast<SQLContainer *>(
		// look_for returns a copy (if found) or NULL (! found)
		BESContainerStorageVolatile::look_for(name));
	BESDEBUG(SQL_NAME,"SQLContainerStorage:CAST DONE"<<endl);
	if (c) {
		// is uptodate?
		if (c->isReady()){
			BESDEBUG(SQL_NAME,"SQLContainerStorage: returning cached "
					"container named: "<<name<<endl);
		}
		else {
			BESDEBUG(SQL_NAME,"SQLContainerStorage: cached object isn't "
					"UpToDate.\nRebuilding container named: "<<name<<endl);

			//read dataset, reset time and set status
			if (!c->setup()){
				BESDEBUG(SQL_NAME,"ERROR: SQLContainerStorage"<<endl);
				throw BESInternalError(
					"SQLContainerStorage: Failed to rebuild sql dataset",
					__FILE__,__LINE__);
			}

			BESDEBUG(SQL_NAME,"SQLContainerStorage: replace cached file "
					"with updated version."<<endl);
			// update own the passed pointer
			update(c->ptr_duplicate());

		}
	} // if (c)

	/**
	 * can return NULL object when plugin is
	 * not found by look_for(name).
	 */
	return c;
}
