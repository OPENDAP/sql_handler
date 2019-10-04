/*
 * SQLPlugin.cpp
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
 *  Created on: 13/lug/2010
 *      Author: carlo cancellieri
 */

#include "SQLPlugin.h"

using std::endl;
using std::string;


#if 0
/**
 * static -> no sense:
 * SQPlugin should be unique instance for each package
 */
SQLPluginList *SQLPlugin::_pl=SQLPlugin::findTheList();
#endif

SQLPlugin::~SQLPlugin(void)
{
#if 0
    // THIS IS DONE BY THE SQLRequestHandler::remove_sql_handler
    remove_handlers();
#endif
}

void SQLPlugin::remove_handlers()
{
    if (!_handler_list.empty()) {
        if (_pl) {
            Handler_iter i = _handler_list.begin();
            while (i != _handler_list.end()) {
                remove_handler((*i++).first);
            }
        }
        else
            throw BESInternalFatalError("The base SQLRequestHandler is not loaded into the List!",
            __FILE__, __LINE__);
    }
}

bool SQLPlugin::add_handler(const string &command, sql_request_handler handler_method)
{
    if (this->find_handler(command) == NULL) {
        /**
         *  search into the wrapper for a caller
         *  if no wrapper method is called add
         *  one.
         */
        if (_pl) {
            if (_pl->add_sql_wrapper(command)) {
                _handler_list[command] = handler_method;
            }
            else {
                throw BESInternalFatalError("Cannot wrap correctly this command!",
                __FILE__, __LINE__);
            }
            return true;
        }
        else
            throw BESInternalFatalError("The base SQLRequestHandler is not loaded!",
            __FILE__, __LINE__);
    }
    else
        return false;
}

bool SQLPlugin::remove_handler(const string &command)
{
    SQLPlugin::Handler_iter i = this->_handler_list.find(command);
    if (i != this->_handler_list.end()) {
        /**
         * Update the wrapper count into the SQLRequestHandler
         * If this is the only SQLPlugin which defines this
         * command, the wrapping function will be completely
         * removed.
         */
#if 0
        // _pl is Member of this class now
        if (!_pl)
        _pl=SQLPlugin::find_RequestHandler();
#endif
        if (_pl) {
            //update the SQLRequestHandler wrapper usage counter
            if (!_pl->remove_sql_wrapper(command)) {
                // report the error but can be a not fatal error
                BESDEBUG(SQL_NAME, "This command was not correctly wrapped!"<<endl);
            }
            // erase the function
            this->_handler_list.erase(i);
            return true;
        }
        else
            throw BESInternalFatalError("The base SQLRequestHandler is not loaded!",
            __FILE__, __LINE__);
    }
    else
        return false;
}

string SQLPlugin::get_handler_names()
{
    string ret;
    bool first_name = true;
    SQLPlugin::Handler_citer i = this->_handler_list.begin();
    for (; i != this->_handler_list.end(); i++) {
        if (!first_name) {
            ret += ", ";
        }
        ret += (*i).first;
        first_name = false;
    }
    return ret;
}

void SQLPlugin::dump(ostream &strm) const
{
    strm << BESIndent::LMarg << "SQLPlugin::dump - (" << (void *) this << ")" << endl;
    BESIndent::Indent();
    strm << BESIndent::LMarg << "name: " << _name << endl;
    if (this->_handler_list.size()) {
        strm << BESIndent::LMarg << "registered handler functions:" << endl;
        BESIndent::Indent();
        SQLPlugin::Handler_citer i = this->_handler_list.begin();
        SQLPlugin::Handler_citer ie = this->_handler_list.end();
        for (; i != ie; i++) {
            strm << BESIndent::LMarg << (*i).first << endl;
        }
        BESIndent::UnIndent();
    }
    else {
        strm << BESIndent::LMarg << "registered handler functions: none" << endl;
    }
    BESIndent::UnIndent();
}

sql_request_handler SQLPlugin::find_handler(const string &handler_name)
{
    SQLPlugin::Handler_citer i = this->_handler_list.find(handler_name);
    if (i != this->_handler_list.end()) {
        return (*i).second;
    }
    else
        return NULL;
}

SQLPluginList *
SQLPlugin::findTheList()
{
#if 0
    // NOT SAFE instance can be deleted by SQLRequestHandler dtor
    // try cached position
    if (_pl) {
        BESDEBUG(SQL_NAME,"SQLPlugin: returning cached version: "<<_pl<<endl);
        return _pl;
    }
#endif
    // search for the SQLRequestHandler
    BESRequestHandler *bes_pl = BESRequestHandlerList::TheList()->find_handler(SQL_NAME);

    if (bes_pl) {
        /**
         * @todo: check: THIS IS DANGEROUS!!!
         * The module is unable to 'dynamic_cast<SQLRequestHandler*>'
         * from a BESRequestHandler, this is probably due this:
         * <br>http://gcc.gnu.org/faq.html#dso
         * <br>There are 2 different way to solve:
         * - apply that modification (someone says that RTLD_GLOBAL is dangerous)
         * - use a dangerous static_cast<> to make downcast.
         * @note the first option need to add the flag to:
         * BESPlugin.h:123
         */
        BESDEBUG(SQL_NAME, "SQLPlugin: SQLRequestHandler found has ptr: "<<bes_pl<<endl);
//dynamic_cast<SQLPluginList*>(bes__pl)->dump(std::cerr);
        SQLLinker *l = static_cast<SQLLinker*>(bes_pl);
        if (l) {
            BESDEBUG(SQL_NAME, "SQLPlugin: SQLRequestHandler THE LINK WORKS!!"<<endl);
            return l->theLink();
        }
        else {
            BESDEBUG(SQL_NAME, "SQLPlugin: SQLRequestHandler THE LINK <<DO NOT>> WORKS!!"<<endl);
            return NULL;
        }
        //return dynamic_cast<SQLPluginList*>(bes_pl);
    }
    else
        return NULL;
}
