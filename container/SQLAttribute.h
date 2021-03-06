/*
 * SQLAttribute.h
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

#ifndef SQLATTRIBUTE_H_
#define SQLATTRIBUTE_H_

#include <list>
#include <bitset>

#include "utils/StringMatch.h"

/**
 * @brief Defines Attributes for a SQLQuery.
 *
 * The attribute can be in various forms:
 * - Table.attribute
 * - attr as attribute
 * - attr As attribute
 * - attr attribute
 * ...
 * This class is for store as much information
 * as possible on an attribute, such as:
 * - position in the table
 * - attribute name (or alias)
 * - the fullName of the definition
 *
 * for SQLHandler internal use only the alias
 * will be used, instead, to build the query,
 * all the information should be used.
 * @see SQLContainer::buildQuery()
 *
 */
#define _SQLH_REG_ATTR \
    "^[ ]*(([A-z0-9\\_]+\\.)*([A-z0-9\\_]+))[ ]*$|"\
    "^[ ]*(([A-z0-9\\_]+\\.)*([A-z0-9\\_]+))([ ]+| [Aa][Ss] )([A-z0-9\\_]+)[ ]*$|"\
    "^[ ]*([A-z0-9\\_]+|[\\*])[ ]*$"

#define _SQLH_ATTR_REG_GROUPS 10 //!< number of groups '()' in regex +1

/**
 *  set the group (starts from 0)
 *  table alias
 */
#define _SQLH_ATTR_REG_PREFIX_GROUP_op1 1
#define _SQLH_ATTR_REG_PREFIX_GROUP_op2 4

/**
 * #define _SQLH_ATTR_REG_PREFIX_GROUP_op3 -1
 */
// attribute part
#define _SQLH_ATTR_REG_ATTR_GROUP_op1 3
#define _SQLH_ATTR_REG_ATTR_GROUP_op2 8
#define _SQLH_ATTR_REG_ATTR_GROUP_op3 9

class SQLAttribute {
private:
    /**
     * @brief string representation of this attribute
     * this will represent a column of the SQL table
     * or its alias
     * @see AttrNameComp
     */
    std::string name;

    /**
     * @brief string representing the fullName of the
     * information on this attribute:
     * This can contain:
     * - "table."
     * - "attr as "
     * - "attr As "
     * - "attr "
     * - ""
     */
    std::string fullName;

    /**
     * we add this member to speed up ordering by
     * Attribute
     */
    std::string attribute;

    /**
     * @brief unsigned integer representing the position
     * in which this attribute is found
     * This can be used to decide the order of the
     * column in the final representation.
     * @see AttrPosComp
     */
    size_t position;
protected:
    /**
     * @brief change the value of the name
     */
    void setName(std::string n) {
        name = n;
    }

    /**
     * @brief change the value of the fullName
     */
    void setFullName(std::string n) {
        fullName = n;
    }

public:

    /**
     * @brief return a string representation of this class
     */
    std::string toString() const {
        return attribute;
    }

    /**
     * @brief return the value of the name
     */
    const std::string &getName() const {
        return name;
    }

    /**
     * @brief return the value of the fullName
     */
    const std::string &getFullName() const {
        return fullName;
    }

    /**
     * @brief return the value of this attribute.
     *
     */
    const std::string &getAttribute() const {
        return attribute;
    }

    /**
     * @brief change the position of this name
     */
    void setPosition(size_t pos) {
        position = pos;
    }

    /**
     * @brief return the position of this name
     */
    const size_t &getPosition() const {
        return position;
    }

    /**
     * @brief parse the passed string to fill in the
     * name and fullName attributes.
     * @param the attribute string.
     */
    virtual bool loadAttribute(const std::string &attr) {
        BESDEBUG(SQL_NAME, "SQLAttribute::loadAttribute starting->attr: " << attr << std::endl);

        // non const string
        std::string buf = attr;
        // filling groups to extract (see match())
        std::bitset<_SQLH_ATTR_REG_GROUPS> groups;
        /**
         * attribute -> table.attribute
         */
        // matching the table fullName part
        groups.set(_SQLH_ATTR_REG_PREFIX_GROUP_op1, true);
        groups.set(_SQLH_ATTR_REG_PREFIX_GROUP_op2, true);

        // matching the name part
        groups.set(_SQLH_ATTR_REG_ATTR_GROUP_op1, true);
        groups.set(_SQLH_ATTR_REG_ATTR_GROUP_op2, true);
        groups.set(_SQLH_ATTR_REG_ATTR_GROUP_op3, true);

        std::list<matched<_SQLH_ATTR_REG_GROUPS> > _match = StringMatch::match(_SQLH_REG_ATTR, groups, buf);

        /**
         * The obtained list should contain only one Attribute
         * otherwise the passed string is not an attribute or it
         * is badly recognized.
         */
        if (_match.size() == 1) {

            std::list<matched<_SQLH_ATTR_REG_GROUPS> >::iterator i =
                    _match.begin();
            std::bitset<_SQLH_ATTR_REG_GROUPS> bs = (*i).getMap();
            size_t i_matched = 0;
            if (bs.test(_SQLH_ATTR_REG_PREFIX_GROUP_op1))
                this->setFullName((*i).getMatch(i_matched++));
            else if (bs.test(_SQLH_ATTR_REG_PREFIX_GROUP_op2))
                this->setFullName((*i).getMatch(i_matched++));
            else
                this->setFullName("");
            BESDEBUG(SQL_NAME, "SQLAttribute::loadAttribute PREFIX:" << getFullName() << std::endl);
            if (bs.test(_SQLH_ATTR_REG_ATTR_GROUP_op1))
                this->setName((*i).getMatch(i_matched++));
            else if (bs.test(_SQLH_ATTR_REG_ATTR_GROUP_op2))
                this->setName((*i).getMatch(i_matched++));
            else if (bs.test(_SQLH_ATTR_REG_ATTR_GROUP_op3))
                this->setName((*i).getMatch(i_matched++));
            else
                throw BESInternalError("Unable to match required attribute name.", __FILE__, __LINE__);

            BESDEBUG(SQL_NAME, "SQLAttribute::loadAttribute NAME:" << getName() << std::endl);
        }
        else
            throw BESInternalError("Unable to match the SQLAttributes parts", __FILE__, __LINE__);

        BESDEBUG(SQL_NAME, "SQLAttribute::loadAttribute done" << std::endl);
        return true;
    }

#if 0
    // KEEP SQLAttribute implementation into this class

    /**
     * @brief constructor
     * @param the string representing the fullName to set
     * @param the string representing name to set
     * @param its reveled position
     * @see SQLAttribute
     */
    SQLAttribute(const std::string &r, const std::string &n, const size_t &pos):
        name(n),
        fullName(r),
        attribute(r+n),
        position(pos){
        BESDEBUG( SQL_NAME,"CREATING: SQLAttribute" << std::endl );
    };
#endif

    /**
     * @brief constructor
     * @param the string representing the fullName to set
     * @param the string representing name to set
     * @param its reveled position
     * @see SQLAttribute
     */
    SQLAttribute(const std::string &attr, const size_t &pos) :
            attribute(attr),
            position(pos) {
        if (!this->loadAttribute(attr))
            throw BESInternalError("Passed string is not recognized as SQLAttribute", __FILE__, __LINE__);
        BESDEBUG(SQL_NAME, "CREATING: SQLAttribute" << std::endl);
    };

    /**
     * @brief copy constructor
     */
    SQLAttribute(const SQLAttribute &a) :
            name(a.name),
            fullName(a.fullName),
            attribute(a.attribute),
            position(a.position) {
        BESDEBUG(SQL_NAME, "COPING: SQLAttribute" << std::endl);
    };

    /**
     * @brief dtor
     */
    virtual ~SQLAttribute() {
        BESDEBUG(SQL_NAME, "DELETING: SQLAttribute" << std::endl);
    };
};

/**
 * @brief Interface for less comparator
 */
class SQLLessComp {
public:
    SQLLessComp() {}

    virtual ~SQLLessComp() {};

    /**
     * @return true if a1 is less than a2
     */
    virtual bool less(const SQLAttribute &a1, const SQLAttribute &a2) const = 0;
};


/**
 * @brief comparator by names for SQLAttributes
 */
class SQLAttrNameComp : public SQLLessComp {
public:
    /**
     * @brief constructor
     */
    SQLAttrNameComp() : SQLLessComp() {
        BESDEBUG(SQL_NAME, "CREATING: SQLAttrNameComp::comparator" << std::endl);
    }

    /**
     * @brief dtor
     */
    virtual ~SQLAttrNameComp() {
        BESDEBUG(SQL_NAME, "DELETING: SQLAttrNameComp::comparator" << std::endl);
    };

    /**
     * comparison of elements
     */
    bool operator()(const SQLAttribute &a1, const SQLAttribute &a2) const {
        return less(a1, a2);
    }

    // comparison of elements
    bool less(const SQLAttribute &a1, const SQLAttribute &a2) const {
        BESDEBUG(SQL_NAME, "SQLAttrComp::comparing using name" << std::endl);
        return ((a1.getName()).compare(a2.getName()) < 0);
    }
};


/**
 * @brief comparator by attribute for SQLAttributes
 */
class SQLAttrAttrComp : public SQLLessComp {
public:
    /**
     * @brief constructor
     */
    SQLAttrAttrComp() : SQLLessComp() {
        BESDEBUG(SQL_NAME, "CREATING: SQLAttrAttrComp::comparator" << std::endl);
    }

    /**
     * @brief dtor
     */
    virtual ~SQLAttrAttrComp() {
        BESDEBUG(SQL_NAME, "DELETING: SQLAttrAttrComp::comparator" << std::endl);
    };

    /**
     * comparison of elements
     */
    bool operator()(const SQLAttribute &a1, const SQLAttribute &a2) const {
        return less(a1, a2);
    }

    // comparison of elements
    bool less(const SQLAttribute &a1, const SQLAttribute &a2) const {
        BESDEBUG(SQL_NAME, "SQLAttrAttrComp::comparing using attribute" << std::endl);
        return ((a1.getAttribute()).compare(a2.getAttribute()) < 0);
    }
};


/**
 * @brief comparator by position for SQLAttributes
 * <br> Default is set to order by position
 */
class SQLAttrPosComp : public SQLLessComp {
public:
    /**
     * @brief constructor
     */
    SQLAttrPosComp() : SQLLessComp() {
        BESDEBUG(SQL_NAME, "CREATING: SQLAttrPosComp::comparator" << std::endl);
    }

    /**
     * @brief dtor
     */
    virtual ~SQLAttrPosComp() {
        BESDEBUG(SQL_NAME, "DELETING: SQLAttrPosComp::comparator" << std::endl);
    };

    /**
     * @brief comparison of elements
     */
    bool operator()(const SQLAttribute &a1, const SQLAttribute &a2) const {
        return less(a1, a2);
    }

    /**
     * @brief comparison of elements
     */
    bool less(const SQLAttribute &a1, const SQLAttribute &a2) const {
        BESDEBUG(SQL_NAME, "SQLAttrComp::comparing using Position" << std::endl);
        return (a1.getPosition() < a2.getPosition());
    }
};

/**
 * @brief modifiable comparator for SQLAttribute
 * <br> Default is set to order by position
 * SQLAttrPosComp,SQLAttrNameComp,SQLAttrFullNameAttrComp
 */
class SQLAttrComp {
public:
    /**
     * @brief constructor for sorting criterion
     * default criterion uses value position
     * @param a pointer to the desired type of
     * comparator
     * @note pointer is owned by this class and
     * will be delete on dtor call.
     */
    SQLAttrComp(SQLLessComp *c) : comp(c) {
        BESDEBUG(SQL_NAME, "CREATING: SQLAttrComp::comparator" << std::endl);
    }

    // comparison of elements
    bool operator()(const SQLAttribute &a1, const SQLAttribute &a2) const {
        return comp->less(a1, a2);
    }

    /**
     * @brief dtor
     */
    virtual ~SQLAttrComp() {
        if (comp) {
            delete comp;
            comp = 0;
        }
    }

private:
    //SQLAttrSortMode mod;
    SQLLessComp *comp;
};

#endif /* SQLATTRIBUTE_H_ */
