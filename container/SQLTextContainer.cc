/*
 * SQLTextContainer.cpp
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

#include "SQLTextContainer.h"

using std::endl;
using std::string;

SQLTextContainer::SQLTextContainer(const string &name, const string & real_name, const string &type) :
        SQLContainer(name, real_name, type), _dataset(new DATASET()), d_actual_section(_dataset->begin())
{
    TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLTextContainer"<<endl);
    /**
     * @note this is already done by the SQLContainerFactory
     * this is here in the case you want to use this
     * constructor manually.
     */
    if (!setup()) {
        BESDEBUG(SQL_NAME, "ERROR: SQLTextContainer"<<endl);
        throw BESInternalError("SQLTextContainer: Failed to setup sql dataset", __FILE__, __LINE__);
    }
}

SQLTextContainer::SQLTextContainer(const BESContainer & c) :
        SQLContainer(c), _dataset(new DATASET()), d_actual_section(_dataset->begin())
{
    TESTDEBUG(SQL_NAME_TEST,"CREATING: SQLTextContainer from BESContainer"<<endl);
    /**
     * @note: this is already done by the SQLContainerFactory
     * this is here in the case you want to use this
     * constructor manually.
     */
    BESDEBUG(SQL_NAME, "SQLTextContainer: NOT READY"<<endl);
    if (!setup()) {
        BESDEBUG(SQL_NAME, "ERROR: SQLTextContainer"<<endl);
        throw BESInternalError("SQLTextContainer: Failed to setup sql dataset", __FILE__, __LINE__);
    }
}

SQLTextContainer::SQLTextContainer(const SQLTextContainer & c) :
        SQLContainer(c), _dataset(new DATASET(*c._dataset)), d_actual_section(_dataset->begin())
{
    TESTDEBUG(SQL_NAME_TEST,"COPYING: SQLTextContainer"<<endl);
}

SQLTextContainer::~SQLTextContainer()
{
    if (_dataset) {
        delete _dataset;
    }
    _dataset = 0;
    TESTDEBUG(SQL_NAME_TEST,"DELETING: SQLTextContainer"<<endl);
}
;

void SQLTextContainer::comparatorSubst(SQL_CONSTRAINT_SET& where)
{
    BESDEBUG(SQL_NAME_TEST, "SQLTextContainer::comparatorSubstring start with size: "<<where.size()<<endl );
    // for each constraint
    SQL_CONSTRAINT_SET::iterator wi = where.begin();
    while (wi != where.end()) {
        /**
         *  for each constraints, search in 'other' for
         *  comparator substitution
         */
        string comparator = (*wi).getComparator();

        if (this->getOther(comparator)) {
            // if comparator (as key) is found
            // set its substitute
            SQLConstraint c = (*wi);
            /**
             * comparator is modified by getOther
             * applying spaces for safe append
             */
            c.setSubstitute(" " + comparator + " "); // to standard SQL
            BESDEBUG(SQL_NAME_TEST, "SQLTextContainer::comparatorSubstring: "<<c.toString()<<endl );
            where.erase(wi);
            where.insert(c);
        }
        /**
         *  do we have optional keys which set comparator?
         *  for ex:
         *  ~= = LIKE
         *  != = <>
         */
        else if (comparator.compare("!=") == 0) {
            // from standard dap
            SQLConstraint c = (*wi);
            c.setSubstitute("<>"); // to standard SQL
            BESDEBUG(SQL_NAME_TEST, "SQLTextContainer::comparatorSubstring: "<<c.toString()<<endl );
            where.erase(wi);
            where.insert(c);
        }
        else if (comparator.compare("~=") == 0) {
            // from standard dap
            SQLConstraint c = (*wi);
            c.setSubstitute(" LIKE "); // to standard SQL
            BESDEBUG(SQL_NAME_TEST, "SQLTextContainer::comparatorSubstring: "<<c.toString()<<endl );
            where.erase(wi);
            where.insert(c);
        }
        wi++;
    } BESDEBUG(SQL_NAME_TEST, "SQLTextContainer::comparatorSubstring DONE"<<endl );
}

string SQLTextContainer::buildQuery()
{
    BESDEBUG(SQL_NAME, "SQLTextContainer: Building query: "<<endl);
    SQLQuery &q = getQuery();

    /**
     * build an attribute set ordered
     * by position and fill it to reorder the
     * attribute before append to the query.
     */
    string query = "SELECT ";
    // buffer
    string buf;
#if 0
    /**
     * >> 1- Why OLFS do not fill in attributes?
     * >> why we have get_attributes into a BESContainer if it is not used?
     * >>(all the constraint string goes in get_constraint()).
     *
     * One of the things that we wrote into one of our modules
     * here at RPI (not OPeNDAP) was the idea of constraining your
     * attribute list as well as your variable list. We also wanted
     * to be able to aggregate attributes. So when I wrote the BES,
     * I added this idea into the container. In addition to the variable
     * constraint, we wanted attribute constraint as well, which is what
     * get_attributes gives me.
     *
     * For now, I don't think you need to worry about it,
     * unless you want to. Nobody else uses that except RPI.
     * Patrick
     */
    string attributes=this->get_attributes();
#endif

    /**
     * still remain attributes into the constraint string
     * let's filter
     *
     * unescape xml escaped characters with
     * BESUtil::www2id
     */
    buf = BESUtil::www2id(this->get_constraint()); //onTheFly
    /**
     * Split:
     * attr1,attr2,attr3&attr3!=10&attr2<100
     */
    size_t split = buf.find("&");

    BESDEBUG(SQL_NAME_TEST, "SQLTextContainer split is: "<<split<<
            "\nBuf: "<<buf<<endl );

    string attributes;
    string constraints;
    if (split == 0) { // only constraints
        constraints = buf;
    }
    else if (split < buf.size()) { // & is found in the middle
        attributes = buf.substr(0, split);
        constraints = buf.substr(split + 1);
    }
    else if (split >= buf.size()) {
        /**
         * & is not found so:
         * only attributes selection OR empty
         */
        attributes = buf;
    }

    /**
     * returns a set of attributes filtered
     * attributes ordered by position as
     * stored in the dataset
     */
    std::auto_ptr<SQL_ATTRIBUTE_SET_BYPOS> attrs(q.attrToSelect(attributes));
    buf.clear();
    if (!attrs->empty()) {
        SQL_ATTRIBUTE_SET_BYPOS::iterator i = attrs->begin();
        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer:: Attrs:"<<(*i).getAttribute()<<endl);
        buf = (*i).getAttribute();
        while (++i != attrs->end()) {
            BESDEBUG(SQL_NAME_TEST, "SQLTextContainer:: Attrs:"<<(*i).getName()<<endl);
            buf += _SQLH_CONT_ATTR_JOIN + (*i).getAttribute();
        }
    }

    if (!buf.empty()) {
        // use this as select clause
        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer:: QUERY APPENDING:"<<buf<<endl);
        query += buf;
        buf.clear();
    }
    else
        throw BESInternalFatalError("SQLTextContainer: Unable to build a correct SELECT clause", __FILE__, __LINE__);

    /**
     *  FROM is fixed
     *  @note: Formally could be a good idea to set
     *  the symbolic name of the dataset to the
     *  resulting table anyway the aliasing
     *  SQL dialect may vary using different RDMS so
     *  we do not proceed to rename.
     */
    query += " FROM " + q.getFrom();

    /**
     * CONSTRAINTS - Selection
     *
     * @brief build a valid SQL WHERE predicate.
     * Look to the dataset for the where predicate to check
     * that 'on the fly' specified contraints are
     * 'more restrictive' then the dataset one.
     *
     * @todo: this is not easy since adding predicates here is not
     * implicit that we are restricting the range.
     * We have to discuss about this. carlo cancellieri 27/Jul/2010
     *
     */

    BESDEBUG(SQL_NAME_TEST, "SQLTextContainer::WHERE starting with:"<<
//	"\nContainer constraints ->"<<q.getWhere()<<"<-"<<
            "\nOnTheFly  constraints ->"<<constraints<<"<-"<<endl );

    // where is optional and could be empty
    if (!q.getWhere().empty()) {
        /**
         * operate comparator substitution
         * on container constraints
         */
        this->comparatorSubst(q.getWhere());

        SQLQuery::whereIterator wi = q.getWhere().begin();
        buf += (*wi++).toString();
        while (wi != q.getWhere().end()) {
            buf += _SQLH_CONT_REG_CONSTR_BASE_JOIN;
            buf += (*wi).toString();
            wi++;
        }
    }

    // ON THE FLY SPECIFIED CONSTRAINTS

    BESDEBUG(SQL_NAME_TEST, "SQLTextContainer::constrToWhere mid: "<<buf<<endl );
    if (!constraints.empty()) {

        // on the fly constraints are not empty
        SQL_CONSTRAINT_SET in_constr = SQLQuery::loadConstraints(constraints);

        /**
         * operate comparator substitution
         * on 'on the fly' specified constraints
         */
        this->comparatorSubst(in_constr);
        // on the fly constraints are correctly loaded into 'in_constr'
        if (!in_constr.empty()) {
            SQLQuery::whereIterator wi = in_constr.begin();
            do {
                if (!buf.empty()) {
                    // '_where' contain previous conditions join (AND)
                    buf += _SQLH_CONT_REG_CONSTR_BASE_JOIN;
                }
                /**
                 * we still have to operate attribute prefix
                 * substitution from:
                 * catalogContainer.attribute (coming from OLFS)
                 * to
                 * table_1.attribute (found in dataset)
                 * so let's search by name
                 *
                 */
                SQLQuery::attrIterator ai = q.getSelect().find((*wi).getAttribute());

                // if found
                if (ai != q.getSelect().end()) {
                    /**
                     * substitute the attribute FullName.
                     * Why not use alias? It is not possible.
                     * How an SQL query gets evaluated. Note that this is a conceptual description;
                     * a good RDBMS will change the order of operation to optimize; as long
                     * as the results remain the same that is not a problem.
                     * Step 1: Evaluate FROM clause, build intermediate table from all rows in
                     * the tables used, joined together on the conditions given. If old style
                     * join syntax is used (with the ON conditions in the WHERE clause), this
                     * step will yield the full carthesian product of all tables used.
                     * Step 2: Evaluate WHERE clause, remove rows that don't match the criteria
                     * from intermediate table.
                     * Step 3: Evaluate GROUP BY clause, group rows together according to the
                     * specified arguments.
                     * Step 4: Evaluate HAVING clause, remove groups that don't match the
                     * criteria from intermediate table.
                     * Step 5: Evaluate SELECT clause, build result set to be returned from the
                     * data in the intermediate table.
                     * Step 6: Evaluate ORDER BY, perform sorting.
                     * Officially, columns that are not included in the SELECT clause are not
                     * available for sorting. Many products (like SQL Server) do allow this, but
                     * it is a non-standard extension of the ISO/ANSI SQL-92 specification (and I
                     * don't think that later SQL specifications included this).
                     * Since the alias of a columns or expression is only effective from step 5
                     * but the WHERE clause is evaluated as step 2, it is clear that an alias
                     * can't be used in the WHERE clause.
                     * @note: thanks to Mr. Hugo Kornelis
                     */
                    buf += (*ai).getFullName();
                }
                // append name
//				buf+=(*wi).getAttribute().getName();
                // append comparator
                if (!(*wi).getSubstitute().empty())
                    buf += (*wi).getSubstitute();
                else
                    buf += (*wi).getComparator();
                // append value
                buf += (*wi).getVal();
            } while (++wi != in_constr.end());
        }
    }

    if (!buf.empty()) {
        // we have: simply use it
        query += " WHERE " + buf;
    }

    query += ";";

    /**
     * @todo ADD some 'ORDER BY' other funny options
     */

    BESDEBUG(SQL_NAME, "SQLTextContainer: QUERY: "<<query<<endl);
    return query;

}

/**
 * @todo: we can implement a mechanism to ask to
 * handlers which minimum information are required
 * for a certain 'api', handler may respond with
 * a bitset initialized with minimum required information.
 *
 * From section 5 of the dataset's draft in use:
 *  # Each section should contain at least:
 *  0 # [section]
 *  1 # api=APINAME
 *  2 # server=SERVER_NAME
 *  3 # [select]
 *  4 # #sql string for select clause
 *  5 # [from]
 *  6 # #valid and complete SQL 'from' clause
 *  so we have to check 7 values as needed.
 *
 */
std::bitset<_SQLH_DATASET_PARTS_NUM> SQLTextContainer::requirements()
{
    std::bitset<_SQLH_DATASET_PARTS_NUM> complete;
    complete.set(section_tag);
    complete.set(api);
    complete.set(server);
    complete.set(select_tag);
    complete.set(from_tag);
    return complete;
}

bool SQLTextContainer::read() throw (BESError)
{

    BESDEBUG(SQL_NAME, "SQLTextContainer: start reading dataset"<<endl);
    // to read the accessed file
    std::fstream file;
#if 0
    // pointer to the new section to add
    SQLH_DATASET_SECTION *new_section=NULL;
#endif
    // FIXME Move the compiles inside the try/catch
    regex_t _reg_v;
    if (regcomp(&_reg_v, _SQLH_DATASET_REG_VAR, REG_EXTENDED) != 0) {
        throw BESInternalError("SQLTextContainer: Unable to compile the REGEX", __FILE__, __LINE__);
    }
    regex_t _reg_c;
    if (regcomp(&_reg_c, _SQLH_DATASET_REG_COMMENTS, REG_EXTENDED) != 0) {
        regfree(&_reg_v);
        throw BESInternalError("SQLTextContainer: Unable to compile the REGEX", __FILE__, __LINE__);
    }
    regex_t _reg_t;
    if (regcomp(&_reg_t, _SQLH_DATASET_REG_TAGS, REG_EXTENDED) != 0) {
        regfree(&_reg_v);
        regfree(&_reg_c);
        throw BESInternalError("SQLTextContainer: Unable to compile the REGEX", __FILE__, __LINE__);
    }
    regex_t _reg_k;
    if (regcomp(&_reg_k, _SQLH_DATASET_REG_KEY_VAL, REG_EXTENDED) != 0) {
        regfree(&_reg_v);
        regfree(&_reg_c);
        regfree(&_reg_t);
        throw BESInternalError("SQLTextContainer: Unable to compile the REGEX", __FILE__, __LINE__);
    }
    regex_t _reg_dv;
    if (regcomp(&_reg_dv, _SQLH_DATASET_REG_DEFVAR, REG_EXTENDED) != 0) {
        regfree(&_reg_v);
        regfree(&_reg_c);
        regfree(&_reg_t);
        regfree(&_reg_k);
        throw BESInternalError("SQLTextContainer: Unable to compile the REGEX", __FILE__, __LINE__);
    }

    try {
        regex_t _reg_v, _reg_c, _reg_t, _reg_k, _reg_d;
        if (regcomp(&_reg_v, _SQLH_DATASET_REG_VAR, REG_EXTENDED) != 0
                || regcomp(&_reg_c, _SQLH_DATASET_REG_COMMENTS, REG_EXTENDED) != 0
                || regcomp(&_reg_t, _SQLH_DATASET_REG_TAGS, REG_EXTENDED) != 0
                || regcomp(&_reg_k, _SQLH_DATASET_REG_KEY_VAL, REG_EXTENDED) != 0
                || regcomp(&_reg_dv, _SQLH_DATASET_REG_DEFVAR, REG_EXTENDED) != 0) {
            throw BESInternalError("SQLTextContainer: Unable to compile the REGEX", __FILE__, __LINE__);
        }

        // Open the dataset file.
        file.open(get_real_name().c_str(), std::fstream::in);

        if (file.fail() || !(file.is_open()))
            throw BESInternalError("Cannot open accessed file!", __FILE__, __LINE__);

        // This variable represents minimum dataset requirements.
        // TODO when 'api=name' what is required might change
        std::bitset<_SQLH_DATASET_PARTS_NUM> required = requirements();
        // initialize completion diary registering where we were
        std::bitset<_SQLH_DATASET_PARTS_NUM> complete;
        // index to the (actual) reading section part (where we are)
        _SQLH_DATASET_PARTS reading_part; //no need to set
        // initial condition
        bool _start = true;

        SQLH_DATASET_SECTION new_section;

        // map used to perform variable substitutions into the dataset
        std::map<string, string> vars;

        // READ THE DATASET
        while (!file.eof()) {
            std::string row; // line buffer
            getline(file, row);
            BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: READING ROW->"<<row<<"<-"<<endl );

            if (!row.empty()) {
                // index of matched regex groups
                regmatch_t indexes[_SQLH_DATASET_MAX_REG_GROUPS];
                // search for COMMENTS
                if (regexec(&_reg_c, row.c_str(), (size_t) _SQLH_DATASET_REG_COMMENTS_GROUPS, indexes, 0) == 0) {
                    BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: Comment->"<<row<<"<-"<<endl );
                }

                // variable definition
                else if (regexec(&_reg_dv, row.c_str(), (size_t) _SQLH_DATASET_REG_DEFVAR_GROUPS, indexes, 0) == 0) {
                    BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: defineVariable->"<<row<<"<-"<<endl );
                    defineVariable(vars, row, indexes[_SQLH_DATASET_REG_DEFVAR_KEY_GROUP],
                                   indexes[_SQLH_DATASET_REG_DEFVAR_VAL_GROUP]);
                }

                // TAGS (which delimit parts)
                else if (regexec(&_reg_t, row.c_str(), (size_t) _SQLH_DATASET_REG_TAGS_GROUPS, indexes, 0) == 0) {
                    BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: partTracking->"<<row<<"<-"<<endl );
                    partTracking(_start, new_section, required, complete, reading_part, row,
                                 indexes[_SQLH_DATASET_REG_TAGS_GROUP]);
                }

                // scan for all other lines
                else {
                    BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: variableSubstitution->"<<row<<"<-"<<endl );
                    // variable substitution; substitute $NAME$ everywhere on this row.
                    while (regexec(&_reg_v, row.c_str(), (size_t) _SQLH_DATASET_REG_VAR_GROUPS, indexes, 0) == 0) {
                        variableSubstitution(vars, row, indexes[_SQLH_DATASET_REG_VAR_GROUP]);
                    }

                    // KEY=VALUE lines
                    if (regexec(&_reg_k, row.c_str(), (size_t) _SQLH_DATASET_REG_KEY_VAL_GROUPS, indexes, 0) == 0) {
                        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: addKeyValue->"<<row<<"<-"<<endl );
                        addKeyValue(new_section, required, complete, reading_part, row,
                                    indexes[_SQLH_DATASET_REG_KEY_GROUP], indexes[_SQLH_DATASET_REG_VAL_GROUP]);
                    }

                    // all other lines (free form line)
                    else {
                        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: addFreeForm->"<<row<<"<-"<<endl );
                        addFreeForm(
                        //section to add to
                                new_section,
                                //actual section completion diary.
                                complete, reading_part, row);
                    }
                }
            } // if row!empty
        }

        if (file.is_open())
            file.close();

        regfree(&_reg_v);
        regfree(&_reg_c);
        regfree(&_reg_t);
        regfree(&_reg_k);
        regfree(&_reg_dv);

        if (_start) {
            BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: empty dataset"<<endl );
            // empty dataset?
            return false;
        }
        else {
            BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: last addNewSection"<<endl );
            addNewSection(new_section, required, complete);
            return true;
        } //final if
    } //try
    catch (BESError &e) {
        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: caught exception: " << e.get_message() << endl);

        regfree(&_reg_v);
        regfree(&_reg_c);
        regfree(&_reg_t);
        regfree(&_reg_k);
        regfree(&_reg_dv);

        throw;
    } //catch
    catch (...) {
        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: caught unknown exception" << endl);

        regfree(&_reg_v);
        regfree(&_reg_c);
        regfree(&_reg_t);
        regfree(&_reg_k);
        regfree(&_reg_dv);

        throw;
    } //catch
} //end

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
void SQLTextContainer::partTracking(bool &start, SQLH_DATASET_SECTION &new_section,
                                    std::bitset<_SQLH_DATASET_PARTS_NUM> &required,
                                    std::bitset<_SQLH_DATASET_PARTS_NUM> &complete,
                                    _SQLH_DATASET_PARTS &reading_part, string & row,
                                    regmatch_t & tag)
{
    BESDEBUG(SQL_NAME_TEST, "Found a tag: " << row << "\nT start: "
            << tag.rm_so << "T end: " << tag.rm_eo << endl);

    // SECTION TAG
    if (row.compare(tag.rm_so, tag.rm_eo - tag.rm_so, _SQLH_DATASET_SECTION_TAG) == 0) {
        /**
         * If this is the starting dataset section
         * do nothing with 'new_section' but
         * track the reading_part and flip start
         */
        if (start) //starting condition
            start = false;
        else {
            BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: addNewSection->"<<row<<"<-"<<endl );
            /**
             * Add the read section to the dataset list
             */
            addNewSection(new_section, required, complete);

            // reset completion status
            complete.reset();
            // clear section
            new_section = SQLH_DATASET_SECTION();
        }

        reading_part = section_tag; // where we are

        BESDEBUG( SQL_NAME, "SQLTextContainer: Start reading a [section]:"<<endl);
    }
    // SELECT TAG
    else if (row.compare(tag.rm_so, tag.rm_eo - tag.rm_so, _SQLH_DATASET_SELECT_TAG) == 0) {
        reading_part = select_tag;
        BESDEBUG( SQL_NAME, "Entering a [select] part: "<<endl);
    }
    // FROM TAG
    else if (row.compare(tag.rm_so, tag.rm_eo - tag.rm_so, _SQLH_DATASET_FROM_TAG) == 0) {
        reading_part = from_tag;
        BESDEBUG( SQL_NAME, "Entering a [from] part: "<<endl);
    }
    // WHERE TAG
    else if (row.compare(tag.rm_so, tag.rm_eo - tag.rm_so, _SQLH_DATASET_WHERE_TAG) == 0) {
        reading_part = where_tag;
        BESDEBUG( SQL_NAME, "Entering a [where] part: "<<endl);
    }
    // OTHER TAG
    else if (row.compare(tag.rm_so, tag.rm_eo - tag.rm_so, _SQLH_DATASET_OTHER_TAG) == 0) {
        reading_part = other_tag;
        BESDEBUG( SQL_NAME, "Entering a [other] part: "<<endl);
    }
}

/**
 * Check section requirements and (if satisfied) add a copy of
 * new_section to the dataset list and clear it for the next
 * section scan.
 * @param new_section a NOT NULL pointer to the DATASET_SECTION
 * to add
 * @param required diary of what is required
 * @param complete diary of what is found into the reading dataset
 * @see SQLTextDefinitions
 */
void SQLTextContainer::addNewSection(SQLH_DATASET_SECTION &new_section,
                                     std::bitset<_SQLH_DATASET_PARTS_NUM> &required,
                                     std::bitset<_SQLH_DATASET_PARTS_NUM> &complete)
{
    if (complete.test(where_tag)) {
        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer::addNewSection() Processing WHERE tag"<<endl );
       // load constraints into the query
        SQL_CONSTRAINT_SET cs = SQLQuery::loadConstraints(_where);
        new_section.query.setWhere(cs);
        //clear _where buffer
        _where.clear();
        // if where is still empty
        if (new_section.query.getWhere().empty())
            complete.set(where_tag, false);
    }

    if (complete.test(select_tag)) {
        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer::addNewSection() Processing SELECT tag."<<endl );
        /**
         * load attribute into the query
         * set is ordered by attribute name to permit search by name
         * @note set only supports a single instance of the same
         * set so the name must be unique (if not, use alias).
         */
        SQL_ATTRIBUTE_SET_BYNAME as = SQLQuery::loadAttributes(_attribute);
        new_section.query.setSelect(as);
        //clear _attribute buffer
        _attribute.clear();
        // if where is still empty
        if (new_section.query.getSelect().empty())
            complete.set(select_tag, false);
    }

    if (required == (required & complete)) {
        // completion match requirements
        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer::addNewSection() push_back complete section->"<<new_section.api<<"<-"<<endl );
        // push a copy of the last parsed section
        _dataset->push_back(new_section);
    }
    else {
        BESDEBUG(SQL_NAME_TEST,
                "SQLTextContainer: Unable to complete dataset scan. "
                "\nNOTE: could be BigEndian or LittleEndian:"<<
                "\nCOMPLETITION MAP: "<<complete.to_string()<<
                "\nREQUIRED MAP:     "<<required.to_string()<<
                "\nINDEX: \nsection_tag=0\napi=1\nserver=2\nport=3\ndbname=4"
                "\nuser=5\npass=6\nselect_tag=7\nfrom_tag=8\nwhere_tag=9"
                "\nother_tag=10"<<endl);
        throw BESError("SQL Handler: Unable to complete dataset scan. Section has missing elements.",
                BES_SYNTAX_USER_ERROR, __FILE__, __LINE__);
    }
}

/**
 * Store variable definition found in the passed row: variable
 * should be found and key and val should index the variable key
 * start and the variable value start and stop index.
 * @param vars Container to store variables definitions (a value-result parameter)
 * @param row The line from the dataset file that contains the variable definition.
 * @param key a regmatch_t indexing the variable KEY limits
 * @param val a regmatch_t indexing the variable VALUE limits
 * @see SQLTextDefinitions
 */
void SQLTextContainer::defineVariable(std::map<string, string> & vars, string & row,
                                      regmatch_t &key, regmatch_t &val)
{
    BESDEBUG(SQL_NAME_TEST, "Storing variable definition: "<<row<<endl);

    BESDEBUG(SQL_NAME_TEST, "Indexes (K for Key, V for Value):\nK start: "
            <<key.rm_so<<" K end: "<<key.rm_eo<<
            "\nV start: "<<val.rm_so<<" V end: "<<val.rm_eo<<endl);

    if (key.rm_so == -1 || val.rm_so == -1)
        throw BESInternalError("SQLTextContainer: row doesn't contain a valid variable definition: "
                + row, __FILE__, __LINE__);

    string var = row.substr(key.rm_so, key.rm_eo - key.rm_so);
    string value = row.substr(val.rm_so, val.rm_eo - val.rm_so);

    // check for malformed declarations (i.e., $VAR$=$VAR$)
    if (var.compare(value) == 0)
        throw BESError("SQL Handler: Unable to define variable with its declaration: " + row,
                        BES_SYNTAX_USER_ERROR, __FILE__, __LINE__);

    vars.insert(std::make_pair(var, value));

    BESDEBUG(SQL_NAME, "Stored variable: " << var << " value: " << value << endl);
}

/**
 * Apply variable substitution on the passed row. variable
 * should be defined and stored in 'vars' and key should index
 * the variable start and stop index.
 * @param vars a map container containing variables definitions
 * @param row the row to change.
 * @param key a regmatch_t indexing the variable
 * @see SQLTextDefinitions
 */
void SQLTextContainer::variableSubstitution(std::map<string, string> & vars, string & row, regmatch_t &key)
{
    BESDEBUG(SQL_NAME_TEST, "Row before substitution: "<<row<<
//		"\nstart: "<<indexes[0].rm_so<<" end: "<<indexes[0].rm_eo<<
            "\nV start: "<<key.rm_so<<" V end: "<<key.rm_eo<<endl);
    string var;
    if (key.rm_so != -1) {
        var = row.substr(key.rm_so, key.rm_eo - key.rm_so);
    }
    else
        throw BESInternalError("SQLTextContainer: row doesn't match->" + row, __FILE__, __LINE__);
    std::map<string, string>::iterator i = vars.find(var);
    if (i != vars.end()) { // variable is defined
            //substitute
        string row_buf = row;
        row = row_buf.substr(0, key.rm_so);
        row.append((*i).second);
        row.append(row_buf.substr(key.rm_eo, key.rm_eo - key.rm_so));
    }
    else {
        throw BESInternalError("SQLTextContainer: Variable not defined: " + var, __FILE__, __LINE__);
    } BESDEBUG(SQL_NAME_TEST, "Row  after substitution: "<<row<<endl );
}

/**
 * Apply dataset rules if a free form row is found.
 * Each part of the dataset may support or not free form
 * rows (see dataset rules), using the reading_part flag
 * indicating where row is found (in which section) this function
 * is able to decide actions to do.
 * No indication on starting and stop index are provided since this
 * is typically a string which doesn't match any of the defined regex
 * this is why they are called free form rows.
 *
 * @note jhrg I'm not sure how this is actually used. 9/11/12
 *
 * @param new_section pointer to the DATASET_SECTION to populate
 * @param complete diary of what is found into the reading dataset
 * @param reading_part the 'part' where these row is found
 * @see SQLTextDefinitions
 */
void SQLTextContainer::addFreeForm(SQLH_DATASET_SECTION &new_section,
                                   std::bitset<_SQLH_DATASET_PARTS_NUM> &complete,
                                   _SQLH_DATASET_PARTS &reading_part, string & row)
{
    BESDEBUG(SQL_NAME_TEST, "FreeForm row: "<<row<<endl);
    /**
     * Reading the dataset:
     * # 7
     * # optionally each section may contain:
     * ...
     * # [OTHER]
     * # all the entries in this tag should
     * # be in this form:
     * # key = value
     * ...
     */

    if (reading_part == other_tag) {
        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: bad string format in OTHER part, skipping "
        "line: "<<row<<" should be in KEY=VALUE format."<<endl);
    }
    /**
     * Reading the dataset:
     * # 3
     * ...
     * # Note that all the lines into the
     * # [section] part are in the form:
     * # RESERVED_KEY = VALUE
     * ...
     */
    // if reading part [section]
    else if (reading_part == section_tag) {
        throw BESError("SQLTextContainer: free form lines in section part are not "
                "permitted. Error on row: " + row, BES_SYNTAX_USER_ERROR, __FILE__, __LINE__);
    }
    // if reading part [select]
    else if (reading_part == select_tag) {
        /**
         * The check is done at the end
         * of the [section] parsing the
         * temporary _attribute string.
         */
        _attribute += " " + row; // append

        /**
         * At last one line is read, mark
         * this part as 'complete'
         */
        complete.set(select_tag, true);
    }
    // if reading part [from]
    else if (reading_part == from_tag) {
        new_section.query.addFrom(" " + row);
        /**
         * At last one line is read, mark
         * this part as 'complete'
         */
        complete.set(from_tag, true);
    }
    // if reading part [where]
    else if (reading_part == where_tag) {
        /**
         * The check is done at the end
         * of the [section] parsing the
         * temporary _attribute string.
         */
        _where += " " + row; // append

        /**
         * At last one line is read, mark
         * this part as 'complete'
         */
        complete.set(where_tag, true);
    }
    else {
        // this is not a trivial error
        // SKIP this line
        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: bad string format, skipping: "<<row<<endl);
    }
}

/**
 * Apply dataset rules if a KEY=VAL row is found
 * @param new_section pointer to the DATASET_SECTION to populate
 * @param required diary of what is required
 * @todo if api=name is found required can be modified applying
 * specific handler needs.
 * @param complete diary of what is found into the reading dataset
 * @param reading_part the 'part' where these key=value is found
 * @param key a regmatch_t indexing the KEY limits
 * @param val a regmatch_t indexing the VALUE limits
 * @see SQLTextDefinitions
 */
void SQLTextContainer::addKeyValue(SQLH_DATASET_SECTION &new_section,
                                   std::bitset<_SQLH_DATASET_PARTS_NUM> &required,
                                   std::bitset<_SQLH_DATASET_PARTS_NUM> &complete,
                                   _SQLH_DATASET_PARTS &reading_part, string & row,
                                   regmatch_t &_key, regmatch_t &_val)
{
    BESDEBUG(SQL_NAME_TEST, "Found line in the form KEY=VALUE: "<<row<<endl);

    BESDEBUG(SQL_NAME_TEST, "Indexes (K for Key, V for Value):\nK start: "<<
            _key.rm_so<<" K end: "<<_key.rm_eo<<
            "\nV start: "<<_val.rm_so<<" V end: "<<_val.rm_eo<<endl);

    string key;
    string val;
    if (_key.rm_so != -1 && _val.rm_so != -1) {
        key = row.substr(_key.rm_so, _key.rm_eo - _key.rm_so);
        val = row.substr(_val.rm_so, _val.rm_eo - _val.rm_so);
    }
    else
        throw BESInternalError("SQLTextContainer: row doesn't match->" + row, __FILE__, __LINE__);

    // if reading part [section]
    if (reading_part == section_tag) {
        BESDEBUG(SQL_NAME_TEST, "API"<<endl);
        if (key.compare(_SQLH_DATASET_API) == 0) {
            BESDEBUG(SQL_NAME_TEST, "NEW API key=val found->"<<row<<endl);
            new_section.api = val;
            complete.set(api, true);

            /**
             * @todo: we can implement a mechanism to ask to
             * handlers which minimum information are required
             * for a certain 'api', handler may respond with
             * a bitset initialized with minimum required information.
             */
            /**
             * To set complete the [section] part
             * we may have settled:
             *  - api
             *  - server
             */
            if (complete.test(server))
                complete.set(section_tag, true);
        }
        else if (key.compare(_SQLH_DATASET_SERVER) == 0) {
            new_section.server = val;
            complete.set(server, true);
            /**
             * To set complete the [section] part
             * we may have settled:
             *  - api
             *  - server
             */
            if (complete.test(api))
                complete.set(section_tag, true);
        }
        else if (key.compare(_SQLH_DATASET_PORT) == 0) {
            new_section.port = val;
            complete.set(port, true);
        }
        else if (key.compare(_SQLH_DATASET_DBNAME) == 0) {
            new_section.dbname = val;
            complete.set(dbname, true);
        }
        else if (key.compare(_SQLH_DATASET_USER) == 0) {
            new_section.user = val;
            complete.set(user, true);
        }
        else if (key.compare(_SQLH_DATASET_PASS) == 0) {
            new_section.pass = val;
            complete.set(pass, true);
        }
        else {
            // is this a trivial error ?
            throw BESError("SQL Handler: Only reserved keywords are allowed in the 'section' part of the dataset file. \n"
                    + string("The line where the error occurred: ") + row, BES_SYNTAX_USER_ERROR, __FILE__, __LINE__);
        }
    }
    /**
     * Reading the dataset rules:
     * # 7
     * # optionally each section may contain:
     * ...
     * # [OTHER]
     * # all the entries in this tag should
     * # be in this form:
     * # key = value
     * # They are inserted into a map which
     * # the driver may handle.
     * # Here you can specify connection
     * # parameters or sql query string.
     */
    // if reading part [other]
    else if (reading_part == other_tag) {
        new_section.other.insert(std::make_pair(key, val));
        complete.set(other_tag, true);
    }
    // if reading part [select]
    else if (reading_part == select_tag) {
        /**
         * Is strange
         * 	to have a KEY=VALUE in FROM
         * 	clause anyway some SQL dialect
         * 	may use '=' to declare alias
         * The check is done at the end
         * of the [section] parsing the
         * temporary _attribute string.
         */
        _attribute += " " + row; // append
        /**
         * At last one line is read, mark
         * this part as 'complete'
         */
        complete.set(select_tag, true);
    }
    // if reading part [from]
    else if (reading_part == from_tag) {
        /**
         * @todo check if could be strange
         * 	to have a KEY=VALUE in SELECT
         * 	clause anyway some SQL dialect
         * 	may use '=' to declare alias
         */
        new_section.query.addFrom(" " + row);
        /**
         * At last one line is read, mark
         * this part as 'complete'
         */
        complete.set(from_tag, true);
    }
    // if reading part [where]
    else if (reading_part == where_tag) {
        //new_section->where.insert(row);
        /**
         * Is strange
         * 	to have a KEY=VALUE in WHERE
         * 	clause anyway some SQL dialect
         * 	may use '=' to declare alias
         * The check is done at the end
         * of the [section] parsing the
         * temporary _where string
         */
        _where += " " + row; // append
        /**
         * At last one line is read, mark
         * this part as 'complete'
         */
        complete.set(where_tag, true);
    }
    else {
        BESDEBUG(SQL_NAME_TEST, "SQLTextContainer: String outside of parts, skipping: "<<row<<endl);
    }
}
