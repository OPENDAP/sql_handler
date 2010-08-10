/*
 * SQLLinker.h
 *
 *  Created on: 10/ago/2010
 *      Author: bianco
 */

#ifndef SQLLINKER_H_
#define SQLLINKER_H_
#include <BESRequestHandler.h>
#include "SQLPluginList.h"
class SQLPluginList;

class SQLLinker:public BESRequestHandler {
public:
	SQLLinker(const string &name):
		BESRequestHandler(name){};

	virtual ~SQLLinker(){};

	virtual SQLPluginList *theLink()=0;
};

#endif /* SQLLINKER_H_ */
