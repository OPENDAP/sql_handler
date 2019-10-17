/*
 * SQLConstraint.h
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

#ifndef SQLCONSTRAINT_H_
#define SQLCONSTRAINT_H_

// FIXME Removed jhrg 10/1/14 using namespace std;
#include "SQLAttribute.h"

class SQLConstraint {
    /**
     * @brief string representation of this attribute
     * this will represent a column of the SQL table
     */
    SQLAttribute attribute;
    /**
     * @brief string representation of a comparator
     * such as:
     * =
     * <=
     * >=
     * @see SQLQueryDefinition for a list
     */
    std::string comparator;
    /**
     * @brief substitute is a string representation
     * of a valid SQL substitute for the stored
     * DAP comparator
     */
    std::string substitute; // substitute for comparator
    /**
     * @brief string representation of the value
     * of this constraint
     */
    std::string value;
public:
    /**
     * @brief less comparison operator which will
     * be used to compare two SQLConstraints
     */
    bool operator<(const SQLConstraint &c) const {
        //a<b
        if (attribute.getAttribute().compare(c.attribute.getAttribute()) > 0 &&
            comparator.compare(c.comparator) > 0 &&
            //				substitute.compare(c.substitute)>0 &&
            value.compare(c.value) > 0)
            return false;
        else
            return true;
    }

    /**
     * @brief print a string representation of this
     * SQLConstraint
     */
    std::string toString() const {
        std::string ret = attribute.getAttribute();
        ret += (!substitute.empty()) ? substitute : comparator;
        ret += value;
        return ret;
    }

    /**
     * @brief change the value of this constraint
     */
    void setAttribute(const SQLAttribute &attr) {
        attribute = attr;
    }

    /**
     * @brief return the value of this constraint
     */
    const SQLAttribute &getAttribute() const {
        return attribute;
    }

    /**
     * @brief change the value of this constraint
     */
    void setVal(std::string val) {
        value = val;
    }

    /**
     * @brief return the value of this constraint
     */
    const std::string &getVal() const {
        return value;
    }

    /**
     * @brief change the comparator of this constraint
     */
    void setComparator(std::string comp) {
        comparator = comp;
    }

    /**
     * @brief return the comparator of this constraint
     */
    const std::string &getComparator() const {
        return comparator;
    }

    /**
     * @brief set the substitute comparator for this constraint
     */
    void setSubstitute(std::string subst) {
        substitute = subst;
    }

    /**
     * @brief return the substitute comparator for this constraint
     */
    const std::string &getSubstitute() const {
        return substitute;
    }

    /**
     * @brief Constructor to build the SQLConstraint
     * @param the attribute
     * @param the comparator
     * @param the value
     * @param an (optional) substitution string for comparator
     */
    SQLConstraint(const SQLAttribute &attr, const std::string &comp,
                  const std::string &val, const std::string &subst = "") :
            attribute(attr),
            comparator(comp),
            substitute(subst),
            value(val) {
    };

    /**
     * @brief Constructor to build the SQLConstraint
     * @param the attribute 'rest' part
     * @param the attribute 'name' part
     * @param the comparator
     * @param the value
     * @param an (optional) substitution string for comparator
     */
    SQLConstraint(const std::string &attr, const std::string &comp,
                  const std::string &val, const std::string &subst = "") :
            attribute(attr, 0),
            comparator(comp),
            substitute(subst),
            value(val) {
    };

    /**
     * @brief copy constructor
     */
    SQLConstraint(const SQLConstraint &c) :
            attribute(c.attribute),
            comparator(c.comparator),
            substitute(c.substitute),
            value(c.value) {
    };

    /**
     * @brief dtor
     */
    virtual ~SQLConstraint() {
    };
};

#endif /* SQLCONSTRAINT_H_ */
