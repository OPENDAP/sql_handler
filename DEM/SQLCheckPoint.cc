/*
 * SQLCheckPoint.h
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
 *  Created on: 03/giu/2010
 *      Author: carlo cancellieri
 */

#include "SQLCheckPoint.h"

#include <bitset>
// atoi
#include <stdlib.h>

#include <list>

#include <TheBESKeys.h>
#include <BESUtil.h>
#include <BESInternalError.h>

#include "SQLDefinitions.h"

using namespace std;

std::bitset<_SQLH_CHECK_SIZE> SQLCheckPoint::function;
unsigned long SQLCheckPoint::last_type_checked;
bool SQLCheckPoint::last_status;
bool SQLCheckPoint::isSet = false;

//vector<std::string> SQLCheckPoint::_set_check_p;
//bool SQLCheckPoint::isSet(false);

bool SQLCheckPoint::init() throw (BESError)
{
	if (!isSet) {
		last_status = false; // false by default
		last_type_checked = _SQLH_CHECK_SIZE + 1; // never checked
		std::vector<std::string> _set_check_p;
		const string &key = string(_SQLH_CHECK_KEY);
		bool found = false;
		TheBESKeys::TheKeys()->get_values(key, _set_check_p, found);
		if (!found)
			throw BESInternalError("Unable to find \"" + key + "\" key into configuration file", __FILE__, __LINE__);

		for (std::vector<string>::iterator i = _set_check_p.begin(); i != _set_check_p.end(); i++) {
			list<std::string> tocheck;
			BESUtil::explode(_SQLH_CHECK_SEPARATOR, i->c_str(), tocheck);

			for (list<string>::iterator j = tocheck.begin(); j != tocheck.end(); j++) {
				int val = atoi((j->c_str()));
				if (val < _SQLH_CHECK_SIZE && val > 0)
					function.set(val, true);
				else
					throw BESInternalError("Bad value into \"" + string(_SQLH_CHECK_KEY) + "\" key set.", __FILE__,
							__LINE__);
			}
		}
		isSet = true;
	} //if !isSet

	// The docs say this always returns true; I think it could 'return' void
	// but 'return true;' might be a better match for likely uses. jhrg 10/1/14
	return true;
}

bool SQLCheckPoint::check(unsigned long type)
{
	/**
	 * class should be manually initialized!
	 */
	if (type == last_type_checked)
		return last_status;
	else {
		last_type_checked = type;
		return (last_status = function[type]);
	}

}

