/*
 * SQLRequestHandler.cpp
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
 *  Created on: 27/lug/2010
 * 		Author: carlo cancellieri
 */

#include "SQLRequestHandler.h"

using namespace std;

SQLRequestHandler *SQLRequestHandler::_rh = NULL;

sql_handler_map *SQLRequestHandler::_theList = NULL;

sql_wrap_count_map *SQLRequestHandler::_theWrapCount = NULL;

pthread_mutex_t SQLRequestHandler::_mutex;

pthread_once_t SQLRequestHandler::_block = PTHREAD_ONCE_INIT;

SQLRequestHandler::SQLRequestHandler(const string &name) : SQLLinker(name) {
    /**
     * initialize unique instance of the block mutex
     */
    if (pthread_once(&_block, once_init_routine) != 0)
        throw BESInternalError("Could not initialize mutex. Exiting.", __FILE__, __LINE__);

    add_method(VERS_RESPONSE, SQLRequestHandler::version);
    add_method(HELP_RESPONSE, SQLRequestHandler::help);
    BESDEBUG(SQL_NAME, "CREATED: SQLRequestHandler" << endl);
}

SQLRequestHandler::~SQLRequestHandler(void) {
    /**
     * remove all unregistered sql_handlers
     * from the list
     */
    remove_sql_handlers();

    if (_theList)
        delete _theList;
    _theList = 0;
    if (_theWrapCount)
        delete _theWrapCount;
    _theWrapCount = 0;

    DESTROY(&_mutex);

    BESDEBUG(SQL_NAME, "DELETED: SQLRequestHandler" << endl);
}

sql_handler_map &
SQLRequestHandler::theList() {
    LOCK(&_mutex);
    try {
        if (!SQLRequestHandler::_theList) {
            BESDEBUG(SQL_NAME, "SQLRequestHandler: _theList was NULL:" << _theList << " creating new one" << endl);
            SQLRequestHandler::_theList = new sql_handler_map();
        }
    }
    catch (...) {
        UNLOCK(&_mutex);
        throw;
    }
    UNLOCK(&_mutex);
    return (*SQLRequestHandler::_rh->_theList);
}

sql_wrap_count_map &
SQLRequestHandler::theWrapCount() {
    LOCK(&_mutex);
    try {
        if (!SQLRequestHandler::_theWrapCount) {
            BESDEBUG(SQL_NAME,
                     "SQLRequestHandler: _theWrapCount was NULL:" << _theWrapCount << " creating new one" << endl);
            SQLRequestHandler::_theWrapCount = new sql_wrap_count_map();
        }
    }
    catch (...) {
        UNLOCK(&_mutex);
        throw;
    }
    UNLOCK(&_mutex);
    return (*SQLRequestHandler::_rh->_theWrapCount);
}

SQLRequestHandler *
SQLRequestHandler::theSQLRequestHandler(const string &name) {
    LOCK(&_mutex);
    try {
        if (!SQLRequestHandler::_rh) {
            BESDEBUG(SQL_NAME,
                     "SQLRequestHandler: _rh was NULL:" << SQLRequestHandler::_rh << " creating new one" << endl);
            SQLRequestHandler::_rh = new SQLRequestHandler(name);
        }
    }
    catch (...) {
        UNLOCK(&_mutex);
        throw;
    }
    UNLOCK(&_mutex);
    BESDEBUG(SQL_NAME, "SQLRequestHandler: _rh on addr: " << SQLRequestHandler::_rh << endl);
    return (SQLRequestHandler::_rh);
}

/**
 * @brief Run version on the Base handler
 * and on all the SQLPlugins registered
 */
bool SQLRequestHandler::version(BESDataHandlerInterface &dhi) {
    BESVersionInfo *info = dynamic_cast<BESVersionInfo *>(dhi.response_handler->get_response_object());
    info->add_module(SQL_MODULE_PACKAGE_NAME, SQL_MODULE_PACKAGE_VERSION);
    SQLRequestHandler::SQLHandler_citer iterator = theList().begin();
    SQLRequestHandler *plugin = NULL;
    SQLDataHandlerInterface _dhi = SQLDataHandlerInterface(dhi);

    while (iterator != theList().end()) {
        sql_request_handler rh = NULL;

        if ((rh = iterator->second->find_handler(VERS_RESPONSE))) {
            /**
             * @todo: better layout...
             */
            rh(_dhi);
        }
        iterator++;
    };

    return true;
}

void SQLRequestHandler::dump(ostream &strm) const {
    strm << BESIndent::LMarg << "SQLRequestHandler::dump - (" << (void *) this << ")" << endl;
    BESIndent::Indent();
    BESRequestHandler::dump(strm);
    BESIndent::UnIndent();
    BESIndent::Indent();
    if (theList().size()) {
        strm << BESIndent::LMarg << "registered SQLPlugins:" << endl;
        BESIndent::Indent();
        SQLHandler_citer i = theList().begin();
        SQLHandler_citer ie = theList().end();
        for (; i != ie; i++) {
            strm << BESIndent::LMarg << (*i).first << endl;
            BESIndent::Indent();
            (*i).second->dump(strm);
            BESIndent::UnIndent();
        }
        BESIndent::UnIndent();
    }
    else {
        strm << BESIndent::LMarg << "registered SQLPlugins: none" << endl;
    }
    BESIndent::UnIndent();
}

bool SQLRequestHandler::help(BESDataHandlerInterface &dhi) {
    bool ret = true;
    BESInfo *info = dynamic_cast<BESInfo *>(dhi.response_handler->get_response_object());

    // This is an example. If you had a help file you could load it like
    // this and if your handler handled the following responses.
    map<string, string> attrs;
    attrs["name"] = SQL_MODULE_PACKAGE_NAME;
    attrs["version"] = SQL_MODULE_PACKAGE_VERSION;
    list<string> services;
    BESServiceRegistry::TheRegistry()->services_handled(SQL_NAME, services);
    if (services.size() > 0) {
        string handles = BESUtil::implode(services, ',');
        attrs["handles"] = handles;
    }
    info->begin_tag("module", &attrs);
    info->end_tag("module");

    return ret;
}

SQLPlugin *
SQLRequestHandler::find_sql_handler(const string &name) {
    BESDEBUG(SQL_NAME, "SQLRequestHandler: searching into theList " << endl);
    SQLHandler_iterator i = theList().find(name);
    if (i != theList().end())
        return (*i).second.get();
    else
        return NULL;
}

bool SQLRequestHandler::remove_sql_handler(const string &name) {
    BESDEBUG(SQL_NAME, "SQLRequestHandler: removing " << name << " sql_handler" << endl);
    SQLHandler_iterator i = theList().find(name);
    if (i != theList().end()) {
        // remove all the handlers and setting wrappers_count
        (*i).second->remove_handlers();
        // erase the sql_handles
        theList().erase(i);
        return true;
    }
    else
        return false;
}

void SQLRequestHandler::remove_sql_handlers() {
    BESDEBUG(SQL_NAME, "SQLRequestHandler: Removing all (" << theList().size() << ") sql_handlers" << endl);

    SQLHandler_iterator i = theList().begin();
    while (i != theList().end()) {
        BESDEBUG(SQL_NAME, "SQLRequestHandler: removing " << (*i).second->get_name() << " sql_handler" << endl);
        // remove all the handlers and setting wrappers_count
        (*i).second->remove_handlers();
        // erase the sql_handles
        theList().erase(i);
        i++;
    }
}

bool SQLRequestHandler::add_sql_handler(const string &name, SQLPlugin *handler) {
    if (find_sql_handler(name) == NULL) {
        BESDEBUG(SQL_NAME, "SQLRequestHandler: theList" << endl);
        theList().insert(name, handler);
        BESDEBUG(SQL_NAME, "SQLRequestHandler: Total sql_handlers registered: " << theList().size() << endl);
        return true;
    }
    else
        return false;
}

bool SQLRequestHandler::add_sql_wrapper(const string &command) {
    BESDEBUG(SQL_NAME, "SQLRequestHandler: wrapper adding" << endl);

    if (find_sql_handler(command) == NULL) {
        // update the wrap count
        update_wrap_count(command, true);
        // add the command
        add_method(command, SQLRequestHandler::wrapper);
        BESDEBUG(SQL_NAME, "SQLRequestHandler: wrapper " << command << " added" << endl);
        return true;
    }
    else
        return false;
}

bool SQLRequestHandler::remove_sql_wrapper(const string &command) {
    if (find_method(command) != NULL) {
        BESDEBUG(SQL_NAME, "SQLRequestHandler: wrapper " << command << " removing" << endl);
        // update the wrap count
        if (update_wrap_count(command, false)) {
            BESRequestHandler::remove_method(command);
            BESDEBUG(SQL_NAME, "SQLRequestHandler: wrapper " << command << " removed" << endl);
        }
        return true;
    }
    else {
        BESDEBUG(SQL_NAME, "SQLRequestHandler: wrapper " << command << " already removed" << endl);
        return false;
    }
}

bool SQLRequestHandler::update_wrap_count(const string &name, bool add) throw(BESInternalFatalError) {
    std::map<string, size_t>::iterator wi = theWrapCount().find(name);
    if (wi != theWrapCount().end()) {
        if (add) {
            ++((*wi).second);
            BESDEBUG(SQL_NAME, "SQLRequestHandler: wrapper " << name << " use_count: " << (*wi).second << endl);
        }
        else { //add==false -> remove
            if ((--((*wi).second)) == 0) {
                BESDEBUG(SQL_NAME,
                         "SQLRequestHandler: wrapper " << name << " use_count: 1 and is going to be erased" << endl);
                theWrapCount().erase(wi);
                return true;
            }
        }
    }
    else {
        if (add) {
            BESDEBUG(SQL_NAME, "SQLRequestHandler: wrapper " << name << " use_count: 1" << endl);
            theWrapCount().insert(std::make_pair(name, 1));
        }
        else
            throw BESInternalFatalError("This handler seams to be not correctly wrapped!",
                                        __FILE__, __LINE__);
    }
    return false;
}

#if 0
/**
 *@todo: check: Is it possible/necessary to return a stack trace to the client?
 */
// see below
#include <sstream>
#endif

bool SQLRequestHandler::lastChanceRunner(SQLDataHandlerInterface &dhi, const string &command) {
#if 0
    // stackTrace
    std::list<string> stack;
    // buffer
    string buf;
#endif
    // getting dataset
    SQLContainer *c = dhi.getSQLContainer();
    // result
    bool res = false;
    // hold plugin to load
    SQLPlugin *plugin = NULL;

    /**
     * If the found handler fail to build
     * requested object, the next handler in
     * this list will be located and used.
     */
    while (!res && !c->end()) {
        BESDEBUG(SQL_NAME, "SQLRequestHandler: Searching plugin" << endl);
        /**
         *  search for 'api' plugin into the list
         */
        plugin = find_sql_plugin(*c);
        try {
            // if a api match a loaded plugin
            if (plugin) {
                BESDEBUG(SQL_NAME,
                         "SQLRequestHandler: Handler " << plugin->get_name() << " found. Executing request." << endl);
                // search the method
                sql_request_handler function = ((plugin)->find_handler(command));
                // if found
                if (function) {
                    res = function(dhi); //execute
                    if (!res) {
#if 0
                        buf="SQLRequestHandler: Errors occurred, while using";
                        buf+=plugin->get_name();
                        buf+=" API handler ";
                        stack.push_back(buf);
                        BESDEBUG(SQL_NAME,buf << endl);
#endif
                        BESDEBUG(SQL_NAME,
                                 "SQLRequestHandler: Errors occurred, while using" << plugin->get_name()
                                                                                   << " API handler " << endl);
                    }
                }
                else {
#if 0
                    buf="SQLRequestHandler: The selected handler does not provide this function";
                    stack.push_back(buf);
                    BESDEBUG(SQL_NAME,buf << endl);
#endif
                    BESDEBUG(SQL_NAME,
                             "SQLRequestHandler: The selected handler does not provide this function" << endl);
                }

            }
            else {
#if 0
                buf="SQLRequestHandler: No matching plugin found";
                stack.push_back(buf);
                BESDEBUG(SQL_NAME,buf << endl);
#endif
                BESDEBUG(SQL_NAME, "SQLRequestHandler: No matching plugin found" << endl);
                break; //c->end() may return false
            }
        }
        catch (BESError &e) {
#if 0
            buf="SQLRequestHandler: Error: ";
            buf+=e.get_error_typeype();
            buf+=" Message: "+e.get_message();
            buf+=" File: "+e.get_file();
            buf+=" Line: "+e.get_line();
            stack.push_back(buf);
            BESDEBUG(SQL_NAME,buf << endl);
#endif
            BESDEBUG(SQL_NAME,
                     "SQLRequestHandler: Error: " << e.get_bes_error_type() << " Message: " << e.get_message()
                                                  << " File: " << e.get_file() << " Line: " << e.get_line() << endl);
        }
        catch (...) {
#if 0
            buf="SQLRequestHandler: Unrecognized error thrown using plugin named: ";
            buf+=plugin->get_name()+". Next API in the list will be used.";
            stack.push_back(buf);
            BESDEBUG(SQL_NAME,buf << endl);
#endif
            BESDEBUG(SQL_NAME,
                     "SQLRequestHandler: Unrecognized error thrown using plugin named: " << plugin->get_name()
                                                                                         << ". Next API in the list will be used."
                                                                                         << endl);
        }
        c->setNext();
    }
    // if no working plugin is found
    if (!res) {
#if 0
        /**
         * Probably a buffer overflow causes:
         *
         *  OUCH!

         Something Bad Happened On This Server.

         The error message associated with this error is:

         BES out of memory
         *
         */
        if (!stack.empty()) {
            std::list<string>::iterator i=stack.begin();
            stringstream ss;
            size_t error=0;
            do {
                ss<<"Error N: "+error;
                ss<<" "+(*i)+"\n";
                BESDEBUG(SQL_NAME,"SQLRequestHandler: ERR:"<<error<<" buf: "<<ss << endl);
            }while (i++!=stack.end() && error++<_SQLH_STACK_REPORT_SIZE);

            BESDEBUG(SQL_NAME,"SQLRequestHandler: Error occurred" << endl);
            throw BESInternalError(
                    "SQLRequestHandler: Error occurred, error stack:\n"<<buf,
                    __FILE__,__LINE__);
        }
        else
#endif
        throw BESInternalError("SQLRequestHandler: Error occurred, bad constraints specified "
                               "or no active database server for this dataset. Please, "
                               "try a less restrictive constraint or contact the administrator.",
                               __FILE__, __LINE__);
    }
    else
        BESDEBUG(SQL_NAME, "SQLRequestHandler: SUCCESS. Exit status is GOOD" << endl);

    return res;
}

/**
 *
 */
SQLPlugin *
SQLRequestHandler::find_sql_plugin(SQLContainer &c) {
    /**
     * while
     * plugin NOT found
     * or
     * end of plugin API list
     * search for sub-SQLPlugin into the
     * BESRequestHandlerList
     */

    SQLPlugin *plugin = NULL;

    while (!c.end()) {
        BESDEBUG(SQL_NAME, "SQLRequestHandler: getApi" << endl);
        string api = c.getApi();
        if (api.empty())
            throw BESInternalError("SQLRequestHandler: 'api' string can't be empty",
                                   __FILE__, __LINE__);
        BESDEBUG(SQL_NAME, "SQLRequestHandler: Searching for " << api << endl);
        if (api.compare(SQL_NAME) == 0) {
            /**
             * this check is to avoid recursive calling to
             * this handler. Anyway you'll never have to
             * add 'sql' (the name of the base sql module)
             * to the APIs list (into the dataset).
             */
            BESDEBUG(SQL_NAME, "SQLRequestHandler: Never use " << api << " in the API list!" << endl);
            // continue; -> added 'else' before 'if (plugin...'
        }
        else if ((plugin = SQLRequestHandler::find_sql_handler(api))) {
            return plugin;
        }
        c.setNext();
    }
    return plugin;
}

