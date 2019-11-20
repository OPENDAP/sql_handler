/*
 * SQLContainerStorage.h
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
 *  Created on: 02/ago/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLCONTAINERSTORAGE_H_
#define SQLCONTAINERSTORAGE_H_

#include <BESContainerStorage.h>
#include "SQLContainer.h"

/**
 * @brief Interface to implement SQLContainer persistence
 * to pass to the SQLContainerFactory.
 */
class SQLContainerStorage {

public:
    /**
     * @brief Constructor
     */
    SQLContainerStorage() {};

    /**
     * @brief Dtor
     */
    virtual ~SQLContainerStorage() {}

    /**
     * @brief add a container to the storage
     * @param a pointer to the SQLContainer to store
     */
    virtual void add_container(SQLContainer *c) = 0;

    /**
     * @brief removes all container
     *
     * This method removes all containers from the persistent store. It does
     * not delete the real data behind the container.
     * This method should be called in dtor
     *
     * @return true if successfully removed and false otherwise
     */
    virtual bool del_containers() = 0;

    /**
     * @brief removes a container with the given symbolic name
     *
     * This method removes a container to the persistence store with the
     * given symbolic name. It deletes the container.
     *
     * @param s_name symbolic name for the container
     * @return true if successfully removed and false otherwise
     */
    virtual bool del_container(const std::string &s_name) = 0;

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
    virtual SQLContainer *look_for(const std::string &name) = 0;

    /**
     * @brief Update the content of the ContainerStorage with
     * the passed SQLContainer object.
     * @note: You have to pass a copy.
     */
    virtual bool update(SQLContainer *c) = 0;
};

#endif /* SQLCONTAINERSTORAGE_H_ */
