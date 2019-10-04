/*
 * StringMatch.cpp
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

#include "StringMatch.h"

using std::endl;
using std::string;

using namespace std;

StringMatch::StringMatch() {}

StringMatch::~StringMatch() {}

std::vector<std::vector<string> >
StringMatch::match(const char * regex, size_t n_groups, std::vector<size_t>& groups, string & row)
{
	if (!regex){
		throw BESInternalFatalError(
			"StringMatch::match Unable to use NULL regex pointer",
				__FILE__,__LINE__);
	}
	// regex
	regex_t _reg_expr;
	// compile regex
	if (regcomp(&_reg_expr,regex,REG_EXTENDED) != 0){
		throw BESInternalError(
			"StringMatch: Unable to compile the REGEX: "+(string)regex,
			__FILE__,__LINE__);
	}
	// returned vector
	std::vector<std::vector<string> > res;
	/**
	 *  index where to find matched regex groups
	 */
	regmatch_t indexes[n_groups]; // index matches
	// while regex match into the string
	while (regexec(&_reg_expr,row.c_str(),n_groups,indexes,0)==0)
	{
		size_t gr=0;
		std::vector<size_t>::iterator it=groups.begin();
		std::vector<string> group_vect=
				std::vector<string>(groups.size());
		// while groups in the list
		while (it!=groups.end()) {
TESTDEBUG( SQL_NAME,
	"\nSubstring-> start:"<<indexes[*it].rm_so<<
	"\nSubstring-> size:"<<indexes[*it].rm_eo-indexes[*it].rm_so<<
	"\nString-> size:"<<row.size()<<endl );
			// check if limits are acceptable (group is found)
			if (indexes[*it].rm_so!=-1) // if some group is found
				group_vect[gr]=
						row.substr(
							indexes[*it].rm_so,
							indexes[*it].rm_eo-indexes[*it].rm_so);
#if __TESTS__==1
			else // no group found
TESTDEBUG( SQL_NAME,
	"StringMatch: String matches but no group number "<<*it<<" is found"<<endl);
#endif

TESTDEBUG( SQL_NAME,"StringMatch::match extracting: "<<
		"\ngroup: "<<*it<<"\nfound: "<<group_vect[gr]<<endl );
			// next group to match
			it++;
			// store in position
			gr++;
		}
		// delete matching string (entire string)
		row.erase(indexes[0].rm_so,indexes[0].rm_eo-indexes[0].rm_so);
		// store found groups
		res.push_back(group_vect);

TESTDEBUG( SQL_NAME,"StringMatch::match remain: "<<row<<endl );

	}
	regfree(&_reg_expr);
TESTDEBUG( SQL_NAME,"StringMatch::match done"<<endl );
	return res;
}
