/*
 * SQLQuery.h
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
 *      Author: carlo cancellieri
 */

#ifndef SQLQUERY_H_
#define SQLQUERY_H_
// match()
#include "utils/StringMatch.h"

//debug
#include "SQLDefinitions.h"

#include "SQLConstraint.h"
#include "SQLAttribute.h"
#include "SQLQueryDefinitions.h"

#include <string>
#include <set>
// FIXME Removed jhrg 10/1/14 using namespace std;

// order type can be set in constructor
typedef std::set<SQLAttribute, SQLAttrComp> SQL_ATTRIBUTE_SET;
// set ordered by Attribute rest+name
typedef std::set<SQLAttribute, SQLAttrAttrComp> SQL_ATTRIBUTE_SET_BYATTR;
// set ordered by Attribute name
typedef std::set<SQLAttribute, SQLAttrNameComp> SQL_ATTRIBUTE_SET_BYNAME;
// set ordered by position
typedef std::set<SQLAttribute, SQLAttrPosComp> SQL_ATTRIBUTE_SET_BYPOS;

typedef std::set<SQLConstraint> SQL_CONSTRAINT_SET;

/**
 * @brief Class which introduce a layer to take
 * SQL query manipulation separated by the SQLContainer.
 * It implement storage for clauses and predicates and
 * a set of filter functions which make take decisions
 * on what insert and what not in the SQL query.
 * @note: this class should be used to represent a dataset
 * query, its methods compare the local variable
 * with the passed one to filter and return a valid
 * query clause or predicate.
 * @note set only support single instance of the same
 * variable so the attribute name must be unique (if not,
 * use alias).
 * The string getQuery() is implemented in SQLContainer.
 * @see SQLContainer
 */
class SQLQuery {
private:
    SQL_ATTRIBUTE_SET_BYNAME select;
    std::string from;
    SQL_CONSTRAINT_SET where;

public:
    /**
     * @brief constructor
     */
    SQLQuery() : select(), from(), where() {
    }

    /**
     * @brief copy constructor
     * @param q const reference to the SQLQuery to copy
     */
    SQLQuery(const SQLQuery &q) :
            select(q.select), from(q.from), where(q.where) {
    }

    /**
     * @brief dtor
     */
    virtual ~SQLQuery() {
    }

    /**
     * @brief defines attribute iterator (ordered by attribute name)
     */
    typedef SQL_ATTRIBUTE_SET_BYNAME::iterator attrIterator;

    /**
     * @brief insert an attribute into the query
     * @param s an SQLAttribute ordered by attribute name
     */
    void addSelect(SQLAttribute s) {
        select.insert(s);
    }

    /*
     * @brief return a set of attribute representing the
     * select clause of this query.
     * @note this set is ordered by attribute (rest+name) so you
     * may want to transform it in an ordered by position set.
     */
    virtual SQL_ATTRIBUTE_SET_BYNAME &getSelect() {
        return select;
    }

    /**
     * @brief set the select clause as parameter.
     * @param s an SQLAttribute ordered by attribute
     */
    void setSelect(SQL_ATTRIBUTE_SET_BYNAME &s) {
        select = s;
    }

    /**
     * @note: only append is done
     * you have to add join string
     * manually
     */
    void addFrom(std::string s) {
        from.append(s);
    }

    virtual std::string &getFrom() {
        return from;
    }

    void setFrom(std::string f) {
        from = f;
    }

    /**
     * @brief define the constraint iterator
     */
    typedef SQL_CONSTRAINT_SET::iterator whereIterator;

    SQL_CONSTRAINT_SET &getWhere() {
        return where;
    }

    void setWhere(SQL_CONSTRAINT_SET c) {
        where = c;
    }


    /**
     * @brief Load all constraint matching the regex and return
     * the loaded set of SQLContraints.
     * @param attr a reference to the string containing constraints
     * @return a SQL_CONSTRAINTS_SET
     * @note the substitute should still be settled
     */
    static SQL_CONSTRAINT_SET loadConstraints(std::string &where);

    /**
     * Load all attributes matching the regex and return
     * the loaded set of attributes as string
     * @param attr a reference to the string containing attributes
     * @return a new SQL_ATTRIBUTE_SET_BYNAME
     *
     * @see attrToSelect
     */
    static SQL_ATTRIBUTE_SET_BYNAME loadAttributes(std::string &attr);

    /**
     * Projection
     *
     * returns a set of attributes filtered
     * attributes ordered by position as
     * stored in the dataset
     */
    virtual SQL_ATTRIBUTE_SET_BYPOS *attrToSelect(std::string &onTheFly);

#if 0
    /**
     * MOVED INTO SQLTextContainer::READ()
     * Selection
     * @brief Standard function which filter SQLContainer attributes
     * to build a valid SQL WHERE predicate.
     * The function should look at the attributes and to the dataset
     * specified where predicate to check that 'on the fly' specified
     * contraints are 'more restrictive' then the dataset one.
     *
     * @todo: this is not easy since adding predicates here is not
     * implicit that we are restricting the range.
     * We have to discuss about this. carlo cancellieri 27/Jul/2010
     *
     * <br>@note: This member uses the match function.
     * <br>It also uses the bool getOther(string &) method
     * to check if some exception to the standard SQL comparator
     * are provided for the driver (API) in use.
     *
     * @return a string representing a valid SQL WHERE predicate.
     */
    virtual std::string constrToWhere(std::string &);
#endif

#if 0
    /**
     * @brief Utility function to return a set<T> as string
     * @param s the set to use
     * @param the join string to put between strings
     * @return the resulting string
     */
    static std::string setToStr(std::set<std::string> &s, const std::string join);
#endif
};

#endif /* SQLQUERY_H_ */
