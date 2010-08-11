/*
 * SQLActionClose.h
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
 *  Created on: 07/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLACTIONCLOSE_H_
#define SQLACTIONCLOSE_H_

#include "connector/SQLHandleConnector.h"
#include "action/SQLAction.h"

/**
 * @brief Implements the close action for the
 * connector.
 * <br>'ON_EVENT' actions are performed using the
 * SQLErrorManager and the passed ErrorFactory.
 * Set this variable to NULL to skip this phase.
 * <br>You can also use the static method close(...)
 * to make manual call.
 * <br>NOTE: We haven't fixed the ARGS_TYPE and OUT
 * types since they are unused. Set those
 * parameter accordingly with the ActionList
 * you want to use as container.
 */
template <	class ERROR_TYPE,
			class ARGS_TYPE,
			class OUT=void >
class SQLCloseAction:public SQLAction<ARGS_TYPE,OUT> { // error
private:
	SQLCloseAction(){};
	SQLHandleConnector &hc;
	/**
	 * ErrorFactory is stored as pointer because it can be NULL
	 * (which mean to run without checking errors)
	 * anyway it comes from a reference so no deletion is provided.
	 * If needed deletion must be handled externally
	 */
	SQLActionFactory<ERROR_TYPE,ARGS_TYPE,OUT> *ef;
	bool _force;
public:
#if __CLONE__==1
	/**
	 * @brief This is the implementation of the
	 * Clone interface.
	 * @return a pointer to a clone of this object
	 */
	virtual SQLAction<ARGS_TYPE,OUT>* create()throw (std::bad_alloc){
		return this->clone();
	};

	/**
	 * @brief This is the implementation of the
	 * Clone interface.
	 * @return a pointer to a clone of this object
	 */
	virtual SQLAction<ARGS_TYPE,OUT> *clone(){
		return new SQLCloseAction<ARGS_TYPE,OUT>(this->hc,this->ef,this->_force);
	}
#endif
	virtual ~SQLCloseAction(){};

	/**
	 * @brief Default constructor
	 * @param connector the SQLHandleConnector to use
	 * @param error_factory the ActionFactory used to take actions on
	 * errors (or any code returned of ERROR_TYPE). Can be NULL which
	 * mean that no actions are performed on any error code.
	 * @param force a bool to force check point activation (default is
	 * 'false' which mean that is active only if you have set it into
	 * configuration).
	 * @see SQLActionFactory
	 * @see SQLContainer
	 * @see SQLHandlerConnector
	 */
	SQLCloseAction(
			SQLHandleConnector &c,
			SQLActionFactory<ERROR_TYPE,ARGS_TYPE,OUT> *error_factory,
			bool force=false):
		hc(c),
		ef(error_factory),
		_force(false),
		SQLAction<ARGS_TYPE,OUT>(){};

	/**
	 * @brief Manual connecting
	 *
	 * This simply wrap action method passing a NULL
	 * pointer as required.
	 */
	OUT * close(){
		return close(hc,ef,_force);
	}

	/**
	 * This is the action which will be called by the
	 * ActionFactory.
	 */
	OUT * action(ARGS_TYPE *)throw (SQLInternalFatalError){
		return close(hc,ef,_force);
	}

	/**
	 * @brief Static method to try to close connection
	 * @param connector a reference to the SQLHandleConnector to use
	 * @param error_factory an ActionFactory used to take actions on
	 * errors (if _SQLH_ON_CLOSE check point is active or 'force'
	 * is true). Can be NULL which mean that no actions are performed
	 * on any error code.
	 * @param force a bool to force check point activation (default is
	 * 'false' which mean that is active only if you have set it into
	 * configuration).
	 * @see SQLActionFactory
	 * @see SQLErrorFactory
	 * @see SQLContainer
	 * @see SQLHandleConnector
	 */
	static OUT*
	close(SQLHandleConnector & connector,
			SQLActionFactory<ERROR_TYPE,ARGS_TYPE,OUT> *error_factory,
			bool force=false)
	{
		// closing connection
		BESDEBUG(SQL_NAME,"SQLCloseAction: Closing connection"<<endl);
		try
		{
			//if (connector.isReady()) no need
			bool ret=connector.close();
			if (error_factory){
				if (force && error_factory)
					SQLErrorManager<OUT>::trigger(_SQLH_ON_ALWAYS,
							*error_factory);
				else
					SQLErrorManager<OUT>::trigger(_SQLH_ON_CLOSE,
							*error_factory);
			}
			else if (!ret)
				throw SQLInternalFatalError(
					"SQLCloseAction: close execution fails (returning false)",
						__FILE__,__LINE__);
			BESDEBUG(SQL_NAME,"SQLCloseAction: Successfully closed"<<endl);
		}
		catch(SQLInternalError &ie){
			/**
			 *  This exception here (applied to close) should be fatal.
			 *  It will be throw as Fatal Error.
			 */
			throw SQLInternalError(ie.get_message(),
										ie.get_file(),
										ie.get_line());
		}
		catch(BESError &e){
			throw SQLInternalError(e.get_message(),
					e.get_file(),e.get_line());
		}
		catch(exception &e){
			throw SQLInternalError(e.what(),
					__FILE__,__LINE__);
		}
		catch(...){
			throw SQLInternalError(
				"SQLCloseAction: Unknow error while connecting",
				__FILE__,__LINE__);
		}
		return NULL; // to avoid warning
	}

};


#endif /* SQLACTIONCLOSE_H_ */