
/*
 * StringMatch.h
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

#ifndef STRINGMATCH_H_
#define STRINGMATCH_H_
//using namespace std; jhrg 10/1/14
#include <vector>
#include <string>

#include <bitset>
#include <list>
// regex
#include <sys/types.h>
#include <regex.h>

// debug
#include "SQLDefinitions.h"
#include <BESInternalError.h>
#include <BESInternalFatalError.h>

// As of gcc 4.0, the bundled STL uses _Find_first() and not
// _Find_next(0). jhrg 1/1/14
#if 1
#define FIND_FIRST _Find_first()
#else
#define FIND_FIRST _Find_next(0)
#endif

/**
 * @brief This is a structure to keep result of the match
 * function.
 * The struct matched is sized on the maximum number of groups
 * to match into a row but it only stores (into the match string
 * array), the matched string.
 * To know what groups are stored into the match array you can
 * use the bitset<sz> map which has set to true only bit
 * corresponding to the stored group.
 */
template<size_t sz>
class matched {
	/**
	 *  commodity number indicating
	 *  map.count()
	 */
	size_t _sizeOn;
	// matched strings
	std::string *match;
	// which are the matched group
	std::bitset<sz> map;

public:
	size_t sizeOn()
	{
		return _sizeOn;
	}

	std::bitset<sz> getMap()
	{
		return map;
	}

	std::string getMatch(size_t i)
	{
		if (i < _sizeOn && match)
			return match[i];
		else
			throw BESInternalFatalError("Index out of bounds", __FILE__, __LINE__);
	}

	void setMatch(size_t i, string s)
	{
		if (i < _sizeOn && match)
			match[i] = s;
		else
			throw BESInternalFatalError("Index out of bounds", __FILE__, __LINE__);
	}

	/**
	 * @brief constructor
	 * @param the bitset of size sz with bits initialized to true
	 * corresponding to the found/matched groups.
	 * @note: sz <= m.count() && sz==m.size()
	 * @note: group 0 represent the entire regex matching substring
	 */
	matched<sz>(std::bitset<sz> &m) :
			_sizeOn(m.count()), match(_sizeOn > 0 ? new std::string[_sizeOn]() : NULL), map(m)
	{
		TESTDEBUG( SQL_NAME,"CREATING: matched\nsize: "<<sz<<
				"\nsizeOn: "<<_sizeOn<<
				"\nmap: "<<map.to_string()<<endl );
	}

	/**
	 * @brief copy constructor
	 * @param the matched instance to copy
	 * @note: group 0 represent the entire regex matching substring
	 */
	matched<sz>(const matched<sz> &m) :
			_sizeOn(m._sizeOn), match(new string[m._sizeOn]), map(m.map)
	{
		// copy strings
		for (size_t s = 0; s < _sizeOn; s++)
			match[s] = m.match[s];

		TESTDEBUG( SQL_NAME,"CREATING: matched\nsize: "<<sz<<
				"\nsizeOn: "<<_sizeOn<<
				"\nmap: "<<map.to_string()<<endl );
	}
#if 0
	// not needed
	void setMatch(bitset<sz> &bs, string a[]) {
		map=bs;
		if (match)
		delete [] match;
		match=a;
	}
#endif
	virtual ~matched()
	{
		TESTDEBUG( SQL_NAME,"DELETING: matched"<<endl);
		if (match) {
			delete[] match;
		}
		match = 0;
		TESTDEBUG( SQL_NAME,"DELETED: matched"<<endl);
	}
};

/**
 * Starting at index 'i', look for the first element in the bitset
 * that is set and return its index. Include the index 'i'.
 * @param bs
 * @param i
 * @return The first/next index that's set
 */
template <size_t sz>
size_t get_next(std::bitset<sz> bs, size_t i)
{
	while (!bs.test(i) && (i < bs.size())) ++i;

	return i;
}

/**
 * @brief utility class containing string matching related functions
 */
class StringMatch {
	StringMatch();

public:

	virtual ~StringMatch();

	/**
	 * @deprecated this function is deprecated and is going to be
	 * removed.
	 * Use the safer and faster:
	 * <br>list<string[]> match(regex,groups,row)
	 *
	 * @brief Given a 'row' this function extract all the matching
	 * groups (listed in groups) of the 'regex'.
	 * This is done while regex match substrings in the row string.
	 * The result is a vector containing vectors of strings matched
	 * by groups.
	 * 'row' is modified eliminating all the matched regex.
	 * <br>Note: that the size of the inner vector is the same as
	 * the 'groups' and the size of the outer vector is the number
	 * of matching string (in the row).
	 * @param regex a const char * containing the regex to use if
	 * NULL a BESInternalFatalError is thrown
	 * @param size_t number of groups in this regex. Note that this
	 * indicate how many groups are in regex, NOT into groups!
	 * @param groups vector of size_t indicating which groups of
	 * the passed regex are stored.
	 * @param row the input string (will be modified)
	 * @return vector<vector<string> > containing extracted groups
	 * @throws BESInternalFatalError (if regex is null),
	 * BESInternalError if fails to compile regex_t
	 */
	static std::vector<std::vector<string> > match(const char * regex, size_t n_groups, std::vector<size_t> & groups,
			std::string & row);

	/**
	 * @brief Given a 'row' this function extract all the matching
	 * groups (flagged in groups) of the 'regex'.
	 * This is done while regex match substrings in the row string.
	 * The result is a list containing matched instances.
	 * @see matched
	 * 'row' is modified eliminating all the matched regex.
	 * @note: the size of the list correspond to the number of
	 * substrings matching the passed row.
	 * @note: 'sz' represent the maximum number of groups to
	 * parse into the regex +1 (which represents the whole substring)
	 * @param regex a const char * containing the regex to use if
	 * NULL a BESInternalFatalError is thrown
	 * @param groups a bitset in which the interesting groups are
	 * set to true. All those group (if match) will be extracted
	 * from the matching substring and returned in the
	 * corresponding 'matched' instance.
	 * @param row the input string (will be modified)
	 * @return vector<vector<string> > containing extracted groups
	 * @throws BESInternalFatalError (if regex is null),
	 * BESInternalError if fails to compile regex_t
	 */
	template<size_t sz>
	static std::list< matched<sz> > match(const char * regex, std::bitset<sz> &groups, std::string & row)
	{
		if (!regex) {
			throw BESInternalFatalError("StringMatch::match Unable to use NULL regex pointer",
			__FILE__, __LINE__);
		}
		// regex
		regex_t _reg_expr;
		// compile regex
		if (regcomp(&_reg_expr, regex, REG_EXTENDED) != 0) {
			throw BESInternalError("StringMatch: Unable to compile the REGEX: " + (string) regex,
			__FILE__, __LINE__);
		}
		// returned
		std::list<matched<sz> > ret;

		/**
		 *  index where to find matched regex groups
		 */
		regmatch_t indexes[sz]; // index matches

		try {
			// while regex match into the string
			while (regexec(&_reg_expr, row.c_str(), groups.size(), indexes, 0) == 0) {
				// this substring matching groups to add to ret
				std::bitset < sz > gr;
				// each matching substring
				gr.reset();
				/**
				 * starting from 0
				 * Finds the index of the next "on" bit after _Find_next param.
				 * @return  The index of the next bit set, or size() if not found.
				 */
				//for (size_t i = groups._Find_next(0); i < sz; i = groups._Find_next(i)) {
				for (size_t i = get_next(groups, 0); i < sz; i = get_next(groups, i)) {
					// check if limits are acceptable (group is found)
					if (indexes[i].rm_so != -1) { // if group is found
						TESTDEBUG( SQL_NAME,"Group number "<<i<<" is found: "
								"\nSubstring-> start:"<<indexes[i].rm_so<<
								"\nSubstring-> size:"<<indexes[i].rm_eo-indexes[i].rm_so<<
								"\nString-> size:"<<row.size()<<endl );
						gr.set(i, true);
					}
#if __TESTS__==1
					else // no group found
					TESTDEBUG( SQL_NAME,
							"StringMatch: String matches but no group number "<<i<<" is found"<<endl);
#endif
				}
				// if some group is found in the substring
				if (gr.any()) {
					TESTDEBUG( SQL_NAME,"StringMatch::Some group found, extracting: "<<endl );
					/**
					 * now we know how many groups are found
					 * let's create and populate array
					 */
					size_t i_matched = 0;
					// build the matched object
					matched<sz> m(gr);
					//for (size_t i = m.getMap().FIND_FIRST; i < sz; i = m.getMap()._Find_next(i)) {
					for (size_t i = get_next(m.getMap(), 0); i < sz; i = get_next(m.getMap(), i)) {
						m.setMatch(i_matched, row.substr(indexes[i].rm_so, indexes[i].rm_eo - indexes[i].rm_so));
						TESTDEBUG( SQL_NAME,"StringMatch::match extracting: "<<
								"\ngroup: "<<i<<"\nfound: "<<m.getMatch(i_matched)<<endl );
						i_matched++;
					}
					// store found groups
					ret.push_back(m);
				}

				// delete matching string (entire string)
				row.erase(indexes[0].rm_so, indexes[0].rm_eo - indexes[0].rm_so);

				TESTDEBUG( SQL_NAME,"StringMatch::match remain: "<<row<<endl );

			}
			regfree(&_reg_expr);
		}
		catch (...) {
			regfree(&_reg_expr);
			throw;
		} TESTDEBUG( SQL_NAME,"StringMatch::match done"<<endl );
		return ret;
	}
};

#endif /* STRINGMATCH_H_ */
