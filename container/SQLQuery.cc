/*
 * SQLQuery.cpp
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

#include "SQLQuery.h"

/**
 * Load all attributes matching the regex and return
 * the loaded set of attributes ordered by attribute name.
 *
 * @param attr a reference to the string containing attributes
 *
 * @see attrToSelect
 */
SQL_ATTRIBUTE_SET_BYNAME SQLQuery::loadAttributes(string & attr)
{
	BESDEBUG( SQL_NAME,"SQLQuery::loadAttributes() starting-> attr: "<< attr <<endl );
	SQL_ATTRIBUTE_SET_BYNAME ret;
	// filling groups to extract (see match())
	std::vector<size_t> groups;
	/**
	 * attribute -> table.attribute
	 */
	// matching the table alias part
	groups.push_back(_SQLH_CONT_REG_ATTR_GROUP);

	std::vector < std::vector<string> > _match = StringMatch::match(_SQLH_CONT_REG_ATTR,
	_SQLH_CONT_REG_ATTR_GROUPS, groups, attr);

	std::vector<std::vector<string>>::const_iterator mit =_match.begin();
	for(size_t i=0 ;mit != _match.end(); mit++, i++){
	    string first_matched = (*mit)[0];
        BESDEBUG( SQL_NAME,"SQLQuery::loadAttributes() matched[" << i << "]: "<< first_matched << endl );

        SQLAttribute sql_attr(first_matched, i);
        ret.insert(sql_attr);

        // ret.insert(SQLAttribute(first_matched, i));
	}
#if 0
	for (size_t m = 0; m < _match.size(); m++) {
	    BESDEBUG( SQL_NAME,"SQLQuery::loadAttributes() match "<< m << endl );
		// (table.),(attribute), position
		ret.insert(SQLAttribute(_match[m][0], m));
	}
#endif

	BESDEBUG( SQL_NAME,"SQLQuery::loadAttributes() found n_attributes: "<<ret.size()<<endl );
	return ret;
}

/**
 * Load all constraint matching the regex and return
 * the loaded set of SQLContraints
 * @param attr a reference to the string containing constraints
 * @note: the substitute should still be settled
 *
 */
SQL_CONSTRAINT_SET SQLQuery::loadConstraints(string & where)
{
	SQL_CONSTRAINT_SET ret;
	BESDEBUG( SQL_NAME,"SQLQuery::loadConstraints starting: "<<where<<endl );
	std::vector<size_t> groups;
	groups.push_back(_SQLH_CONT_REG_CONSTR_BASE_ATTR);
	groups.push_back(_SQLH_CONT_REG_CONSTR_BASE_COMPARATOR);
	groups.push_back(_SQLH_CONT_REG_CONSTR_BASE_VAL);

	std::vector < std::vector<string> > _match = StringMatch::match(_SQLH_CONT_REG_CONSTR_BASE,
	_SQLH_CONT_REG_CONSTR_BASE_GROUPS, groups, where);

	for (size_t m = 0; m < _match.size(); m++) {
		ret.insert(SQLConstraint(_match[m][0],	// attribute
				_match[m][1],	// comparator
				_match[m][2])); // value
	} BESDEBUG( SQL_NAME,"SQLQuery::loadConstraints found n_constraints: "<<ret.size()<<endl );
	return ret;
}

/**
 * Projection
 *
 * returns a set of attributes filtered
 * attributes ordered by position as
 * stored in the dataset
 */
SQL_ATTRIBUTE_SET_BYPOS*
SQLQuery::attrToSelect(string &onTheFly)
{

	SQL_ATTRIBUTE_SET_BYPOS *attr = new SQL_ATTRIBUTE_SET_BYPOS();

	/**
	 * getting on the fly specified attributes
	 * @note: this should be a sub-selection of the
	 * dataset's one.
	 */
	//string attr=get_attributes();
	BESDEBUG( SQL_NAME,"SQLQuery::attrToSelect starting: "<<onTheFly<<endl );

	// getting a reference to the actual attribute query object
	// build a set ordered by attribute name
	SQL_ATTRIBUTE_SET_BYNAME _dataset_attr = getSelect();

	// build INPUT set of attributes ordered by 'attribute'
	SQL_ATTRIBUTE_SET_BYNAME _onThFly_attr = SQLQuery::loadAttributes(onTheFly);

	/**
	 * check if ALL attributes are selected using '*'
	 *
	 * @todo: this is a known problem:
	 * if the '*' operator is specified we have to
	 * make a query to the DB to know which attributes
	 * are supported
	 * So actually ALL the requested attributes will be added
	 */
	bool all = false;
	attrIterator i = _dataset_attr.find(SQLAttribute("*", 0));
	if (i != _dataset_attr.end()) {
	    BESDEBUG( SQL_NAME,
				"SQLQuery::attrToSelect located: * attribute: "<<
				"\nAttribute: "<<(*i).getAttribute()<<
				"\nPosition: "<<(*i).getPosition()<<endl );
		// This code _was_ all == true - a test, not an assignment. I'm editing this
		// but right now the handler is not building with newer versions of gcc. jhrg
		// 10/1/14
		all = true; // skip check
		// all the attributes are permitted
		// WARNING! 'attr' may still contain not valid attributes

	}

	if (_onThFly_attr.size() > 0) {
		attrIterator ai = _onThFly_attr.begin();
		while (ai != _onThFly_attr.end()) {
			if (!all) {
				/**
				 * For security and formal question
				 * we have to check if the specified
				 * attraint are sub-selection of
				 * the dataset.
				 */
				i = _dataset_attr.find(*ai);
				if (i != _dataset_attr.end()) {
					/**
					 *  Store this attribute since it is
					 *  found in the dataset attribute
					 *  list.
					 *  We use the dataset copy to keep
					 *  the 'rest' part of this SQLAttribute
					 */
					attr->insert(*i);
				}
				else
					BESDEBUG(SQL_NAME, "SQLQuery::attrToSelect skipping attribute: " <<(*ai).getAttribute()<<endl);

			}
			else {
				/**
				 * we filter the 'rest' part since it contain sequence name
				 * which can be a not valid SQL entry.
				 */
				attr->insert(SQLAttribute((*ai).getName(), (*ai).getPosition()));
			}
			ai++;
		}
	}
	if (attr->empty()) {
		BESDEBUG( SQL_NAME,"SQLQuery::attrToSelect select is still empty "<<endl);
		/**
		 * on the fly attribute list is empty
		 * or
		 * no specified on the fly attribute doesn't match the dataset list
		 * we use the dataset list
		 */
		SQLQuery::attrIterator i = _dataset_attr.begin();
		while (i != _dataset_attr.end()) {
			BESDEBUG( SQL_NAME,
					"SQLQuery::attrToSelect adding: "<<(*i).getAttribute()<<endl );
			attr->insert((*i));
			i++;
		}
	}

	BESDEBUG( SQL_NAME,"SQLTextContainer::attrToSelect"<<endl );

	return attr;
}
