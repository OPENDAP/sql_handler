/*
 * SQLInternalFatalError.h
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
 *  Created on: 03/giu/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLINTERNALFATALERROR_H_
#define SQLINTERNALFATALERROR_H_

#include "SQLDefinitions.h"

#include <BESError.h>

/**
 *
 * 1. Internal Error
 * The error is internal, for example, unable to create an
 * XML document for a response
 *
 * 2. Internal Fatal Error
 * The error is unrecoverable. A memory error, for example.
 * Something that the BES MUST exit from
 *
 * 3. Syntax User Error
 * The user has made an error, such as a constraint expression
 * with syntax errors in it
 *
 * 4. Forbidden Error
 * The user is not allowed to access the resource, directory
 * permissions, dataset permissions, etc...
 *
 * 5. Not Found Error
 * The user has requested a resource that doesn't exist.
 *
 * The BES catches a BESError, sets the appropriate flags and
 * returns the error as an XML document. If it is an Internal
 * Fatal Error, then after it returns the XML error document,
 * it exits.
 *
 * [Patrick West - 06-01-2010]
 *
 */

/**
 * @brief exception thrown if internal error encountered
 * it can be throw by Action and is NOT intercepted
 * by the SQLActionManager
 * <br>NOTE: You'll never should throw this exception outside
 * an SQLAction.
 * @see SQLAction
 * @see SQLErrorManager
 */
class SQLInternalFatalError : public BESError {
protected:
    SQLInternalFatalError() = default;

public:
    SQLInternalFatalError(std::string msg, std::string file, unsigned int line)
        : BESError(std::move(msg), _SQLH_INTERNAL_FATAL_ERROR, std::move(file), line) { }

    ~SQLInternalFatalError() override = default;

    void dump(std::ostream &strm) const override {
        strm << "SQLInternalFatalError::dump - (" << (void *) this << ")" << std::endl;
        BESIndent::Indent();
        BESError::dump(strm);
        BESIndent::UnIndent();
    }
};

#endif /* SQLINTERNALFATALERROR_H_ */
