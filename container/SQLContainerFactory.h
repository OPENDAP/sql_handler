/*
 * SQLContainerFactory.h
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

#ifndef SQLCONTAINERFACTORY_H_
#define SQLCONTAINERFACTORY_H_

#include <BESInternalError.h>
#include <BESContainer.h>
#include <string>
#include <BESDataHandlerInterface.h>

#include "SQLContainer.h"
#include "SQLContainerStorage.h"
// default container
#include "SQLTextContainer.h"


typedef SQLContainer *(*SELECT_CONTAINER)(std::string name, std::string real_name, std::string type);

/**
 * @brief Factory used to build SQLContainers, this factory
 * provides an abstraction layer between SQLHandler and
 * SQL{TYPE}Container to build/use.
 */
class SQLContainerFactory {
public:

    /**
     * @brief constructor
     * @param container_storage an instance of SQLContainerStorageVolatile
     * @todo define a SQLContainerStorage
     * @param select_function a function which select the SQLContainer to
     * return using the dataset MIME or a personal algorithm. (Default
     * algorithm returns always a SQLTextContainer)
     * <br>NOTE: container_storage pointer should be handled (dtor) externally
     * since it probably is coming from a BESContainerStorageList::TheList
     * which doesn't build a copy.
     */
    SQLContainerFactory(SQLContainerStorage *container_storage = NULL,
                        SELECT_CONTAINER select_function = &selectContainer) {
        if (select_function)
            _select_func = select_function;
        else
            throw BESInternalFatalError("Unable to initialize the SQLContainerFactory"
                                        "using a NULL SELECT_CONTAINER function ptr", __FILE__, __LINE__);
        if (container_storage)
            cs = container_storage;
        else {
            BESDEBUG(SQL_NAME,
                     "Building SQLContainerFactory without persistence" << std::endl);
            cs = NULL;
        }
    }

    virtual ~SQLContainerFactory() {
    };

    /**
     * @brief Wrapper for getName(string,string,string)
     * @param c reference to a container
     * @return a string containing a unique name for this container
     */
    static std::string getName(BESContainer &c) {
        return getName(c.get_symbolic_name(),
                       c.get_real_name(),
                       c.get_container_type());
    }

    /**
     * @brief Different container should use a different name, this
     * function return the real name of an object.
     * This will be used to register and search object into/from the
     * ContainerStorage
     * @param name symbolic name of the container
     * @param real_name real name of the container
     * @param type type of the container
     * @return a string containing a unique name for this container
     */
    static std::string getName(const std::string &name,
                          const std::string &real_name,
                          const std::string &type) {
        return real_name;
    }

    /**
     * @param container_storage an instance of SQLContainerStorage
     * <br>NOTE: container_storage pointer should be handled (dtor) externally
     * since it probably is coming from a BESContainerStorageList::TheList
     * which doesn't build a copy.
     */
    static void setPersistence(SQLContainerStorage *container_storage);

    /**
     * @param select_function a function which select the SQLContainer to
     * return using the dataset MIME or a personal algorithm. (Default
     * algorithm returns always a SQLTextContainer)
     */
    static void setSelector(SELECT_CONTAINER select_function = &selectContainer);

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
     * @return an copy of the SQLContainer
     * @see SQLContainer
     */
    static SQLContainer *getContainer(BESDataHandlerInterface &dhi);

    /**
     * @return a container ready to be used
     * @throws a BESInternalError if container setup fails.
     */
    static SQLContainer *buildContainer(BESContainer &c) throw(BESInternalError);

    /**
     * @brief factory method
     * This method use the overrider (if) method to build
     * and update an SQLContainer.
     * @WARNING no way to set attributes and constraints use
     * this only if you want to build a container from scratch
     */
    static SQLContainer *buildContainer(const std::string &name, const std::string &real_name,
                                        const std::string &type) throw(BESInternalError);

protected:
    /**
     * @brief implements logic to choose the
     * SQLContainer to use.
     */
    static SQLContainer *selectContainer(std::string name, std::string real_name, std::string type) {
        // NOTE: symbolic_name == real_name
        return new SQLTextContainer(name, real_name, type);
    }

private:
    static SELECT_CONTAINER _select_func;
    static SQLContainerStorage *cs;
};

#endif /* SQLCONTAINERFACTORY_H_ */
