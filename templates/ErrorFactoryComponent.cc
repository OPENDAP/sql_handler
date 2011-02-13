/*
 * OPENDAP_CLASSErrorFactoryComponent.cpp
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
 *  Created on: 23/lug/2010
 *      Author: carlo cancellieri
 */

#include "OPENDAP_CLASSErrorFactoryComponent.h"

/**
 * @note THESE ARE ONLY EXAMPLES WHICH YOU MAY
 * SAFELY DELETE.
 *
 * @note Be sure to read this class comments
 * to chose your best class base.
 * This example uses static actions which may
 * be difficult to use, if so, try Dynamic or
 * Object base class as explained on top of this
 * component.
 */

// SUCCESS
SQLStaticActionList<MSG_TYPE,void> OPENDAP_CLASSErrorFactoryComponent::_success=
	SQLStaticActionList<MSG_TYPE,void>(SQLDefaultErrorAction<MSG_TYPE>::debugMethod);

// SUCCESS_WITH_INFO
SQLStaticActionList<MSG_TYPE,void> OPENDAP_CLASSErrorFactoryComponent::_info=
	SQLStaticActionList<MSG_TYPE,void>(SQLDefaultErrorAction<MSG_TYPE>::debugMethod);

// NO_DATA
SQLStaticActionList<MSG_TYPE,void> OPENDAP_CLASSErrorFactoryComponent::_no_data=
	SQLStaticActionList<MSG_TYPE,void>(SQLDefaultErrorAction<MSG_TYPE>::debugMethod);

OPENDAP_CLASSErrorFactoryComponent::_list_type OPENDAP_CLASSErrorFactoryComponent::__error[2]=
	{	SQLDefaultErrorAction<MSG_TYPE>::debugMethod,
		SQLDefaultErrorAction<MSG_TYPE>::exceptionMethod };

// ERROR
SQLStaticActionList<MSG_TYPE,void> OPENDAP_CLASSErrorFactoryComponent::_error=
	SQLStaticActionList<MSG_TYPE,void>(OPENDAP_CLASSErrorFactoryComponent::__error,2);

// INVALID
SQLStaticActionList<MSG_TYPE,void> OPENDAP_CLASSErrorFactoryComponent::_invalid=
	SQLStaticActionList<MSG_TYPE,void>(OPENDAP_CLASSErrorFactoryComponent::__error,2);


SQLActionList<MSG_TYPE,void>&
OPENDAP_CLASSErrorFactoryComponent::_getActions(ERROR_TYPE *error)
{
	/**
	 * @note THIS IS AN EXAMPLE WHICH YOU MAY SAFELY DELETE
	 * @todo Here you have to return the ActionList matching the
	 * error code argument.
	 */
	if (!error)
		throw BESInternalFatalError(
				"OPENDAP_CLASSPlugin: error code is NULL",__FILE__,__LINE__);
	switch (*error){
	case 1:
		BESDEBUG(OPENDAP_CLASS_NAME,"SUCCESS"<<endl);
		return OPENDAP_CLASSErrorFactoryComponent::_success;
		break;
	case 2:
		BESDEBUG(OPENDAP_CLASS_NAME,"INFO"<<endl);
		return OPENDAP_CLASSErrorFactoryComponent::_info;
		break;
	case 3:
		BESDEBUG(OPENDAP_CLASS_NAME,"NO_DATA"<<endl);
		return OPENDAP_CLASSErrorFactoryComponent::_no_data;
		break;
	case 4:
		BESDEBUG(OPENDAP_CLASS_NAME,"ERROR"<<endl);
		return OPENDAP_CLASSErrorFactoryComponent::_error;
		break;
	case 5:
		BESDEBUG(OPENDAP_CLASS_NAME,"INVALID"<<endl);
		return OPENDAP_CLASSErrorFactoryComponent::_invalid;
		break;
	default:
		BESDEBUG(OPENDAP_CLASS_NAME,"NOT MAPPED ERROR"<<endl);
		throw SQLInternalFatalError("No ErrorList is found",__FILE__,__LINE__);
	}
};

bool
OPENDAP_CLASSErrorFactoryComponent::_stop(ERROR_TYPE * error) {
	/**
	 * @note THESE ARE ONLY EXAMPLES WHICH YOU MAY
	 * SAFELY DELETE.
	 *
	 * @note Be sure to read this class comments
	 * to chose your best class base.
	 * This example uses static actions which may
	 * be difficult to use, if so, try Dynamic or
	 * Object base class as explained on top of this
	 * component.
	 *
	 * @todo Here you have decide which error code represent
	 * a normal (no action to take) condition.
	 * You HAVE to return TRUE (which mean stop) when
	 * this error code is incoming (as parameter)
	 */
	if (!error)
		throw SQLInternalFatalError(
				"Error code is NULL",__FILE__,__LINE__);
	if ((*error)==1)
		return true;
	if ((*error)==2)
		return true;
	else
		return false;
};
