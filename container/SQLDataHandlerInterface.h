/*
 * SQLDataHandlerInterface.h
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
 *  Created on: 30/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLDATAHANDLERINTERFACE_H_
#define SQLDATAHANDLERINTERFACE_H_

#include <BESDataHandlerInterface.h>
#include <BESInternalFatalError.h>

#include "SQLContainerFactory.h"

#include <BESResponseHandler.h>

class BESResponseHandler;

/**
 * @brief The SQLDataHandlerInterface is used as interface to
 * pass container informations and usual object from
 * BESDataHandlerInterface such as the BESResponseObject.
 *
 * @note: This class not inherit from BESDataHandlerInterface
 * but own a reference to it and add a SQLContainer pointer
 * loaded using the SQLContainerFactory::getContainer(dhi).
 * @see SQLContainerFactory::getContainer()
 * @see SQLContainer
 */
class SQLDataHandlerInterface {
private:
    BESDataHandlerInterface *_dhi;
    /**
     * This is a copy coming from the SQLContainerFactory
     * and is handled locally.
     * You'll never have to delete it.
     */
    SQLContainer *_sql_container;
#if 0
    // working but dangerous
    // see below
    std::shared_ptr<SQLContainer,smart::Clone<SQLContainer> > _sql_container;
#endif

    SQLDataHandlerInterface() :
            _dhi(NULL), _sql_container(NULL) {
    };
public:

    /**
     * @brief Constructor
     */
    SQLDataHandlerInterface(BESDataHandlerInterface &dhi) :
            _dhi(&dhi), _sql_container(SQLContainerFactory::getContainer(dhi)) {
    }

#if 0
    /**
     * WORKING BUT DANGEROUS
     * (with command "show version" BES set a null dhi.container
     * which make this constructor throw the error)
     *
     * @brief Constructor
     *
     * @note: the SharedPtr<> can't be loaded if
     * SQLContainerFactory::getContainer() returns a NULL
     * pointer. In that case a fatal error is thrown.
     */
    SQLDataHandlerInterface(BESDataHandlerInterface &dhi):
    _dhi(&dhi),
    _sql_container(
            SQLContainerFactory::getContainer(dhi)?
            :
            throw BESInternalFatalError(
                    "Unable to build SQLDataHandlerInterface with a NULL SQLContainer",
                    __FILE__,__LINE__)) {
    };
#endif

    virtual ~SQLDataHandlerInterface() {
        if (_sql_container)
            delete _sql_container;
        _sql_container = 0;
    }

    BESDataHandlerInterface &getBesHandler() {
        return *_dhi;
    }

    BESContainer *getBesContainer() {
        return _dhi->container;
    }

    BESResponseObject *getResponseObject() {
        return _dhi->response_handler->get_response_object();
    }

    /**
     * @brief returns the SQLContainer
     */
    SQLContainer *getSQLContainer() {
        return _sql_container;
    }

};

#endif /* SQLDATAHANDLERINTERFACE_H_ */
