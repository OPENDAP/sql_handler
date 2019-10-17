/*
 * OPENDAP_CLASSModule.cpp
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

#include <iostream>

using std::endl;

#include "OPENDAP_CLASSResponseNames.h"

#include "DEM/SQLCheckPoint.h"

#include "OPENDAP_CLASSPlugin.h"

#include "OPENDAP_CLASSModule.h"

#include <BESDapService.h>
#include <BESContainerStorageList.h>
#include <BESFileContainerStorage.h>
#include <BESCatalogDirectory.h>
#include <BESCatalogList.h>


void
OPENDAP_CLASSModule::initialize(const string &modname) {
    BESDEBUG(modname, "Initializing SQLPlugin "
            << modname << endl);
    /**
	 *  initialize check point reading
	 *  configuration file.
	 */
    SQLCheckPoint::init();

    /**
	 * Add OPENDAP_CLASS SQL plugin to SQLRequestHandler
	 */
    BESDEBUG(modname, "    adding " << modname
                                    << " SQLPlugin to the SQL request handler" << endl);

    if (!SQLRequestHandler::add_sql_handler(modname, new OPENDAP_CLASSPlugin(modname)))
        throw BESInternalFatalError("Unable to load the module named " + modname,
                                    __FILE__, __LINE__);

    BESDEBUG(modname, "    adding " << modname << " debug context" << endl);
    BESDebug::Register(modname);

    BESDEBUG(modname, "SQL Module initialized" << endl);
}

void
OPENDAP_CLASSModule::terminate(const string &modname) {
    BESDEBUG(modname, "Cleaning SQL module " << modname << endl);

    BESDEBUG(modname, "    removing " << modname << " sql plugin handler" << endl);
    SQLRequestHandler::remove_sql_handler(modname);

    BESDEBUG(modname, "Done Cleaning SQLPlugin " << modname << endl);
}

extern "C"
{
BESAbstractModule *maker() {
    return new OPENDAP_CLASSModule;
}
}

void
OPENDAP_CLASSModule::dump(ostream &strm) const {
    strm << BESIndent::LMarg << "SQLModule::dump - ("
         << (void *) this << ")" << endl;
}

