/*
 * SQLTextContainer.h
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

#ifndef SQLTEXTCONTAINER_H_
#define SQLTEXTCONTAINER_H_

#include "SQLContainer.h"

#include <BESUtil.h>

// debugging
#include "SQLDefinitions.h"
#include <BESDebug.h>

// errors
#include <BESInternalError.h>
#include <BESInternalFatalError.h>

// members
#include <list>
#include <string>

#if 0
#include "utils/SharedPtr.h"
#endif


// read
#include <bitset>
//#include <sys/types.h> included by SQLContainer
//#include <regex.h> included by SQLContainer
#include <fstream>
#include "SQLTextDefinitions.h"

// getQuery
#include "SQLQuery.h"

//auto_ptr
#include <memory>

/**
 * @brief The container used to load and store sql dataset
 * This is an implementation of the SQLContainer abstract
 * class which implement its methods using as persistence
 * a text file.
 * As specified this Container implement persistence using
 * shared object to share the status with its copies.
 * @see SQLContainer
 *
 */
class SQLTextContainer : public SQLContainer {
public:
    SQLTextContainer(const string &name, const string &real_name, const string &type);

    /**
     * @brief constructor
     * @param c reference to a BESContainer
     */
    explicit
    SQLTextContainer(const BESContainer &c);

    /**
     * @brief copy constructor
     * @param c a const reference to the object instance
     * to copy
     */
    explicit
    SQLTextContainer(const SQLTextContainer &c);

    /**
     * @brief dtor
     */
    virtual ~SQLTextContainer();

    /**
     * @brief build the query string
     * @return the string containing a valid SQL query
     */
    virtual string buildQuery();

    /**
     * @brief Build a COUNT query
     * Use this query to get teh COUNT(*) of the rows for the query
     * returned by buildQuery().
     * @return Use this query to get the row count
     */
    virtual string buildCountQuery();

    /**
     * @brief return a reference to the actual section
     * SQLQuery object
     * @return a reference to the SQLQuery object
     */
    virtual SQLQuery &getQuery() {
        return (d_actual_section)->query;
    }

    /**
     * @see SQLContainer
     * @param buf string reference (is modified)
     * @return true if buf is found in the actual
     * section
     */
    virtual bool getOther(string &buf) {
        std::map<string, string>::iterator it = (d_actual_section)->other.find(buf);
        if (it != (d_actual_section)->other.end()) {
            buf = (*it).second;
            return true;
        }
        else
            return false;
    }

    /**
     * @brief return the actual API
     * from the dataset's API list.
     * @see SQLContainer
     */
    virtual string &getApi() {
        return (d_actual_section)->api;
    }

    /**
     * @brief get the string representing
     * the user which should be used to access
     * to the actual selected DB
     * @return a string reference
     * @see SQLContainer
     */
    virtual string &getUser() {
        return (d_actual_section)->user;
    }

    /**
     * @brief get the string representing
     * the password which should be used to
     * access to the actual selected DB
     * @return a string reference
     * @see SQLContainer
     */
    virtual string &getPass() {
        return (d_actual_section)->pass;
    }

    /**
     * @brief get the string representing
     * the server which should be used to
     * access to the actual selected DB
     * @note for the odbc SQLPlugin this
     * represent the DSN name.
     * @see SQLContainer
     */
    virtual string &getServer() {
        return (d_actual_section)->server;
    }

    /**
     * @brief get the string representing
     * the database name which should be
     * used to access to the actual
     * selected DB
     * @return a string reference
     * @see SQLContainer
     */
    virtual string &getDBName() {
        return (d_actual_section)->dbname;
    }

    /**
     * @brief get the string representing
     * the password which should be used to
     * access to the actual selected DB
     * @return a string reference
     * @see SQLContainer
     */
    virtual string &getPort() {
        return (d_actual_section)->port;
    }

    /**
     * @brief reset the cursor to the first
     * section.
     * <br>In a list from 0 to (n-1)
     * - reset() put cursor to 0
     * @see SQLContainer
     */
    virtual void reset() {
        (d_actual_section) = _dataset->begin();
    }

    /**
     * Clear the internal structures used
     * to store data.
     * This is used when an update is needed
     * to clear storage before read() is
     * called again.
     * @see setup()
     * @see SQLContainer
     */
    virtual void clear() {
        if (_dataset)
            delete _dataset;
        _dataset = new DATASET();
        d_actual_section = _dataset->begin();
    }

    /**
     * Tell if the end of the list is reached.
     * In a list from 0 to (n-1):
     * <br>If the actual position of the cursor
     * points to n return true.
     * <br>Note: in a list from 0 to 0
     * - end() return true
     * - empty() return true
     * @see setNext()
     * @see SQLContainer
     */
    virtual bool end() {
        if ((d_actual_section) == _dataset->end())
            return true;
        else
            return false;
    }

    /**
     * @brief Tell if the section list is empty.
     * @see SQLContainer
     */
    virtual bool empty() {
        if (_dataset->empty())
            return true;
        else
            return false;
    }

    /**
     * @brief returns true if there is next section
     * in the list and set actual section to
     * the next one.
     * <br>In a list from 0 to (n-1)
     * - reset() put cursor to 0
     * - FIRST setNext() call set cursor to 1
     * - LAST setNext() call set cursor to n-1
     * returning true
     * - ALL the successive calls to the setNext()
     * must return false until reset() is call
     * @return boolean false if end is reached
     * @see SQLContainer
     */
    virtual bool setNext() {
        if (end()) {
            return false;
        }
        else {
            (d_actual_section)++;
            if (end()) {
                return false;
            }
            else {
                return true;
            }
        }
    }

    /**
     * @brief method to duplicate this instances of BESContainer
     */
    SQLTextContainer *ptr_duplicate() {
        return new SQLTextContainer(*this);
    }

    /**
     * @brief Fill a bitset with minimum requirements
     * for a dataset of this type.
     * Requirements specified by this function should
     * be a very minimum set of information used by:
     *
     * 1- The SQLRequestHandler to implement (and run)
     * the SQLRequestHandler::lastChanceRunner function
     * such as:
     * - api name
     * 2- SQL{Impl}Container to formally define a
     * minimal SQL dataset structure.
     *
     * @todo: place into SQLContainer as standard requirements
     * function.
     *
     * @todo: a better place for another implementation
     * of this function should be the plugin implementing
     * the SQLPlugin and specially the SQLSimpleConnector
     * implementing the connect() and query()
     * functions which effectively uses the required information.
     * That function can be called once the api name
     * (should be specified as required by this function)
     * is read from the dataset.
     * @see SQLTextContainer::addKeyValue()
     *
     *
     * @todo: can we fix _SQLH_DATASET_PARTS_NUM ???
     * if so place it into SQLContainerDefinitions
     *
     * @see SQLTextDefinitions
     */
    std::bitset<_SQLH_DATASET_PARTS_NUM> requirements();

    /**
     * list of configuration of the same API type
     */
    typedef std::list<SQLH_DATASET_SECTION> DATASET;
private:

    /**
     * @brief operate substitution of comparators in a
     * set of SQLConstraints using the getOther function.
     * It also provide basic standard substitution which
     * transform DAP comparator to standard SQL's one.
     * @param reference to a set of SQLConstraints
     */
    void comparatorSubst(std::set<SQLConstraint> &where);

    /**
     * Initialize all the object members reading
     * from the source file this->get_real_name().
     * @return true if read is successfully done.
     */
    virtual bool read() throw(BESError);

    /**
     * Apply dataset's rules if a [TAG] row is found and set accordingly
     * the reading_part to track the 'part' which the tag is starting.
     * If:
     * - 'reading_part' parameter is a [section] tag
     * - 'start' parameter is set to false (initial condition)
     * The passed 'new_section' parameter is supposed to be build so
     * it is passed to addNewSection to be checked and stored.
     * @param start boolean setting the initial condition. Set this to
     * true if the passed new_section should not be checked/added to
     * the dataset (this happen when the dataset scan is starting).
     * @param new_section a NOT NULL pointer to the DATASET_SECTION
     * @param required diary of what is required
     * @param complete diary of what is found into the reading dataset
     * @param reading_part will be initialized with the 'part' which
     * this row match (using matched tag).
     * @param tag a regmatch_t indexing TAG limits
     * @see SQLTextDefinitions
     */
    void partTracking(
            bool &start,
            //section to add
            SQLH_DATASET_SECTION &new_section,
            //minimum dataset requirements.
            std::bitset<_SQLH_DATASET_PARTS_NUM> &required,
            //actual section completion diary.
            std::bitset<_SQLH_DATASET_PARTS_NUM> &complete,
            _SQLH_DATASET_PARTS &reading_part, string &row,
            regmatch_t &tag);

    /**
     * Check section requirements and (if satisfied) add the loaded
     * section add a copy of new_section to the dataset list.
     * @param new_section pointer to the DATASET_SECTION to populate
     * @param required diary of what is required
     * @param complete diary of what is found into the reading dataset
     * @see SQLTextDefinitions
     */
    void addNewSection(
            //section to add
            SQLH_DATASET_SECTION &new_section,
            //minimum dataset requirements.
            std::bitset<_SQLH_DATASET_PARTS_NUM> &required,
            //actual section completion diary.
            std::bitset<_SQLH_DATASET_PARTS_NUM> &complete);

    /**
     * Store variable definition found in the passed row: variable
     * should be found and key and val should index the variable key
     * start and the variable value start and stop index.
     * @param vars a map container to store variables definitions
     * @param row the row to change.
     * @param key a regmatch_t indexing the variable KEY limits
     * @param val a regmatch_t indexing the variable VALUE limits
     * @see SQLTextDefinitions
     */
    void defineVariable(
            //variable definitions container
            std::map<string, string> &vars,
            // row where definition is found
            string &row,
            //index of variable KEY
            regmatch_t &key,
            //index of variable VALUE
            regmatch_t &val);

    /**
     * Apply variable substitution on the passed row: variable
     * should be defined and stored in 'vars' and key should index
     * the variable start and stop index.
     * @param vars a map container containing variables definitions
     * @param row the row to change.
     * @param key a regmatch_t indexing the variable
     * @see SQLTextDefinitions
     */
    void variableSubstitution(
            //variable definitions container
            std::map<string, string> &vars,
            // row to apply
            string &row,
            //index of variable to substitute
            regmatch_t &key);

    /**
     * Apply dataset rules if a KEY=VAL row is found
     * @param new_section pointer to the DATASET_SECTION to populate
     * @param required diary of what is required
     * @todo if api=name is found required can be modified applying
     * specific handler needs.
     * @param complete diary of what is found into the reading dataset
     * @param reading_part the 'part' where key=value is found
     * @param key a regmatch_t indexing the KEY limits
     * @param val a regmatch_t indexing the VALUE limits
     * @see SQLTextDefinitions
     * @todo: we can implement a mechanism to ask to
     * handlers which minimum information are required
     * for a certain 'api', handler may respond with
     * a bitset initialized with minimum required information.
     */
    void addKeyValue(
            //section to add to
            SQLH_DATASET_SECTION &new_section,
            //minimum dataset requirements.
            std::bitset<_SQLH_DATASET_PARTS_NUM> &required,
            //actual section completion diary.
            std::bitset<_SQLH_DATASET_PARTS_NUM> &complete,
            _SQLH_DATASET_PARTS &reading_part, string &row,
            regmatch_t &key, regmatch_t &val);

    /**
     * Apply dataset rules if a free form row is found.
     * Each part of the dataset may support or not free form
     * rows (see dataset rules), using the reading_part flag
     * indicating where row is found (in which section) this function
     * is able to decide actions to do.
     * No indication on starting and stop index are provided since this
     * is typically a string which doesn't match any of the defined regex
     * this is why they are called free form rows.
     * @param new_section pointer to the DATASET_SECTION to populate
     * @param complete diary of what is found into the reading dataset
     * @param reading_part the 'part' where these row is found
     * @see SQLTextDefinitions
     */
    void
    addFreeForm(
            //section to add to
            SQLH_DATASET_SECTION &new_section,
            //actual section completion diary.
            std::bitset<_SQLH_DATASET_PARTS_NUM> &complete, _SQLH_DATASET_PARTS &reading_part, string &row);

    /**
     * Temporary string to append ff string
     * they will be checked at the end of the
     * [section]
     */
    string _where;
    string _attribute;

    /**
     * Containers are stored in a shared list and
     * are copied on requests so the list and its
     * iterator are shared pointer which means
     * that the last owner (probably the list)
     * have to eliminate them.
     * 'd_actual_section' should also be shared since
     * represent the actual Container status which
     * is shared between SQLRequestHandler and
     * SQLPlugin(s) using the
     * SQLContainerStorage.
     */
    DATASET *_dataset;
    //std::shared_ptr<DATASET> _dataset;
    DATASET::iterator d_actual_section;
    //std::shared_ptr<DATASET::iterator> d_actual_section;
};

#endif /* SQLTEXTCONTAINER_H_ */
