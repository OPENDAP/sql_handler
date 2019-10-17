/*
 * ODBCPluginLoader.cpp
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

#include "ODBCPluginLoader.h"

extern "C"
{
/**
 * This method is needed to make the class
 * correctly loaded by the BES
 * @return a pointer to an instance of this class
 */
BESAbstractModule *maker() {
    return new ODBCPluginLoader;
}
}

void
ODBCPluginLoader::initialize(const string &modname) {
    BESDEBUG(modname, "Initializing SQLPlugin "
            << modname << endl);
    /**
	 *  re-initialize check point reading
	 *  configuration file to get modification
	 */
    SQLCheckPoint::init();

    /**
	 * Add ODBCPlugin to RequestHandler list
	 */
    SQLPluginList *pl = NULL;
    if ((pl = SQLPlugin::findTheList())) {
        BESDEBUG(modname, "    adding " << modname <<
                                        " SQLPlugin handler to the list" << endl);
        if (pl->add_sql_handler(modname, new ODBCPlugin(modname))) {

            BESDEBUG(modname, "Done Initializing SQLPlugin " << modname << endl);

            BESDEBUG(modname, "    adding " << modname << " debug context" << endl);
            BESDebug::Register(modname);
        }
        else {
            throw BESInternalError(
                    "Unable to load the ODBCPlugin into the SQLRequestHandler list",
                    __FILE__, __LINE__);
        }
    }
    else {
        BESDEBUG(modname,
                 "SQLRequestHandler not found, SQLPlugin " << modname
                                                           << " initialization aborted" << endl);
    }
}

void
ODBCPluginLoader::terminate(const string &modname) {
    BESDEBUG(modname, "Cleaning SQLPlugin " << modname << endl);
    /**
	 * Remove ODBCPlugin from RequestHandler list
	 */
    SQLPluginList *pl = NULL;
    if ((pl = SQLPlugin::findTheList())) {
        BESDEBUG(modname, "    removing " << modname <<
                                          " SQLPlugin handler from the list" << endl);
        if (pl->remove_sql_handler(modname)) {
            BESDEBUG(modname, "Done Cleaning SQLPlugin module " << modname << endl);
        }
        else {
            throw BESInternalError(
                    "Unable to un-load the ODBCPlugin from the SQLRequestHandler list",
                    __FILE__, __LINE__);
        }
    }
    else
        BESDEBUG(modname,
                 "SQLRequestHandler not found, SQLPlugin " << modname
                                                           << " termination aborted" << endl);
}


void
ODBCPluginLoader::dump(ostream &strm) const {
    strm << BESIndent::LMarg << "ODBCPluginLoader::dump - ("
         << (void *) this << ")" << endl;
}
