/*
 * SQLBuildDATA.h
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
 *  Created on: 13/lug/2010
 *      Author: carlo cancellieri
 */

#ifndef SQLBUILDDATA_H_
#define SQLBUILDDATA_H_

#include <DDS.h>
#include <BESDataNames.h>
#include <BESDebug.h>


#include "connector/SQLConnector.h"

#include "handler/connector/SQLConnectAction.h"
#include "handler/connector/SQLQueryAction.h"
#include "handler/connector/SQLCloseAction.h"
#include "handler/connector/SQLNextTypeAction.h"

// POST CONSTRAINTS
#include "DTM/SQLSequence.h"
#include "DTM/SQLTypeManager.h"
#include "DTM/SQLDummySimpleType.h"
#include "DTM/SQLTypeFactoryComponent.h"
#include "DTM/SQLObjectTypeFactory.h"
#include <libdap/BaseType.h>
#include <BESDataDDSResponse.h>

#include "SQLDefinitions.h"


/**
 * @todo add Manager template arguments and wrapper to pass
 * merge and join functions.
 *
 * @brief Placeholder class which define:
 * - maximum number of template arguments which this class may specify.
 * - default arguments to simplify default SQLBuildDATA::sql_build_data calls.
 * @see specializations for more info.
 */
template<class SQL_TYPE, // connector
        class ODBC_TYPE, // connector
        class ERROR_TYPE = void, // SQLErrorFactory
        class MSG_TYPE = ERROR_TYPE, // SQLErrorFactory
        class OUT_TYPE1 = void, // SQLErrorFactory
        class CODE_TYPE = SQL_TYPE, // SQLTypeFactory
        class ARGS_TYPE = SQL_TYPE, // SQLTypeFactory
        class OUT_TYPE2 = libdap::BaseType> // SQLTypeFactory
class SQLBuildDATA {
private:
    SQLBuildDATA() {
    };

    virtual ~SQLBuildDATA() {
    };
public:
#if 0
    static bool
    sql_build_das( BESDataHandlerInterface &dhi ,
            SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>& error_factory,
            SQLActionFactory<CODE_TYPE,ARGS_TYPE,OUT_TYPE2>& type_factory,
            SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>& connector) {
        throw SQLInternalError(
                "Unable to use this undefined BuldDAS method, use a specialized one",
                __FILE__,__LINE__);
    }
#endif
};

/**
 * @todo add Manager template arguments and wrapper to pass
 * merge and join functions. For Errors and Types!
 *
 * @brief SPECIALIZATION class of SQLBuildDAS which implement basic
 * functions to make this class usable.
 * It provide a group of public wrappers which simplify
 * calls to the main (private) function.
 * @code
 * private:
 * static bool
 * sql_build_data(
 * 		BESDataHandlerInterface &dhi ,
 * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>* error_factory,
 * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
 * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>* connector)
 * @endcode
 * This is a specialization since there no way to build DAS
 * without a TypeFactory definition.
 * With this specialization you can use the following
 * (provided) implementations of the ActionFactory:
 * - SQLDynamicTypeFactory 	(dynamic type mapping)
 * - SQLObjectTypeFactory	(stateful static type mapping)
 * And deprecated:
 * - SQLStaticTypeFactory	(stateless static type mapping)
 * All usable with the ActionManager implementation
 * called SQLTypeManager (used by the connector action
 * SQLNextTypeAction located in /handler/connector/...)
 * @see SQLDynamicTypeFactory
 * @see SQLObjectTypeFactory
 * @see SQLTypeManager
 * @see SQLNextTypeAction
 * <br>
 * <br>Note also that no Error related specialization is done
 * anyway some assumptions are taken by the connector related
 * actions (/handler/connector/...) such as the use of the
 * ErrorManager.
 * @see SQLQueryAction
 * @see SQLConnectAction
 * @see SQLCloseAction
 *
 *
 */
template<class SQL_TYPE, // connector
        class ODBC_TYPE, // connector
        class ERROR_TYPE, // SQLErrorFactory
        class MSG_TYPE, // SQLErrorFactory
        class OUT_TYPE1> // SQLErrorFactory
class SQLBuildDATA<SQL_TYPE, // connector
        ODBC_TYPE, // connector
        ERROR_TYPE, // SQLErrorFactory
        MSG_TYPE, // SQLErrorFactory
        OUT_TYPE1, // SQLErrorFactory
        SQL_TYPE, // SPECIALIZED
        SQL_TYPE, // SPECIALIZED
        libdap::BaseType> { // SPECIALIZED
private:
    SQLBuildDATA() {
    };

    virtual ~SQLBuildDATA() {
    };
public:

    /**
     * It is a wrapper of the:
     * @code
     * static bool sql_build_data( BESDataHandlerInterface &dhi ,
     * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>* error_factory,
     * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
     * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>* connector)
     * @endcode
     * Which is provided to make easy to call the data builder.
     * ODBCFactoryComponent).
     *
     * @param dhi BESDataHandlerInterface reference
     * @param error_factory a reference to
     * SQL{Static|Object|Dynamic}ErrorFactory
     * @param fc a reference to the FactoryComponent
     * to use
     * @param connector a pointer to a simple connector
     * SQLSimpleConnector. Its deletion is done by the
     * built data so you don't have to delete it manually.
     *
     * @see SQLObjectTypeFactory
     * @see ODBCFactoryComponent
     */
    static bool sql_build_data(SQLDataHandlerInterface &dhi, SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE> &fc,
                               SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> *connector) {
        /**
         * Build the TypeFactory which will be our
         * BaseType factory.
         * <br>Note that this is not a BaseTypeFactory
         * but again an ActionFactory derived object.
         */
        SQLObjectTypeFactory<SQL_TYPE, ODBC_TYPE> tf = SQLObjectTypeFactory<SQL_TYPE, ODBC_TYPE>(fc);

        return sql_build_data(dhi, NULL, tf, connector);
    }

    /**
     * @brief This is a function which uses the SQLTypeManager
     * with the SQLObjectTypeFactory to build BaseType object.
     * It is a wrapper of the:
     * @code
     * static bool sql_build_data( BESDataHandlerInterface &dhi ,
     * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>* error_factory,
     * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
     * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>* connector)
     * @endcode
     * Which is provided to make easy to call the data builder.
     *
     * @param dhi BESDataHandlerInterface reference
     * @param error_factory a reference to
     * SQL{Static|Object|Dynamic}ErrorFactory
     * @param fc a reference to the TypeFactoryComponent
     * to use
     * @param connector a pointer to a complete connector
     * SQLConnector. Its deletion is done by the
     * built data so you don't have to delete it manually.
     *
     * @see SQLObjectTypeFactory
     * @see ODBCFactoryComponent
     */
    static bool sql_build_data(SQLDataHandlerInterface &dhi,
                               SQLActionFactory<ERROR_TYPE, MSG_TYPE, OUT_TYPE1> &error_factory,
                               SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE> &fc,
                               SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> *connector) {
        /**
         * Build the TypeFactory which will be our
         * BaseType factory.
         * <br>Note that this is not a BaseTypeFactory
         * but again an ActionFactory derived object.
         */
        SQLObjectTypeFactory<SQL_TYPE, ODBC_TYPE> tf = SQLObjectTypeFactory<SQL_TYPE, ODBC_TYPE>(fc);

        return sql_build_data(dhi, &error_factory, tf, connector);
    }

    /**
     * It is a public wrapper of the:
     * @code
     * static bool sql_build_data( BESDataHandlerInterface &dhi ,
     * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>& error_factory,
     * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
     * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>* connector)
     * @endcode
     * Which is provided to make easy to call the data builder.
     *
     * @param dhi BESDataHandlerInterface reference
     * @param error_factory a reference to
     * SQL{Static|Object|Dynamic}ErrorFactory
     * @param type_factory a reference to the
     * SQL{Static|Object|Dynamic}TypeFactory used
     * @param connector a pointer to a simple connector
     * SQLSimpleConnector. Its deletion is done by the
     * built data so you don't have to delete it manually.
     *
     * @see SQLObjectTypeFactory
     * @see ODBCFactoryComponent
     */
    static bool sql_build_data(SQLDataHandlerInterface &dhi,
                               SQLActionFactory<ERROR_TYPE, MSG_TYPE, OUT_TYPE1> &error_factory,
                               SQLActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType> &type_factory,
                               SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> *connector) {

        return sql_build_data(dhi, &error_factory, // pass as !NULL pointer
                              type_factory, connector);
    }

private:
    /**
     * used to return unrecognized object
     * @param unused void*
     * @return a new string containing
     * the default name for DAS
     */
    static string *dummy_cast(void *) {
        return new string(_SQLH_DEFAULT_DAS_NAME);
    }

    /**
     *
     * @brief Main static function which implement DAS construction
     * it make use of the /handler/connector/... actions to
     * perform connection operation and can be used with or
     * without ErrorFactory (setting its pointer to NULL).
     * See wrapper comments for more info
     *
     */
    static bool sql_build_data(SQLDataHandlerInterface &dhi,
                               SQLActionFactory<ERROR_TYPE, MSG_TYPE, OUT_TYPE1> *error_factory,
                               SQLActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType> &type_factory,
                               SQLSimpleConnector<SQL_TYPE, ODBC_TYPE> *connector) {
        BESDEBUG(SQL_NAME, "SQLBuildDATA: Connecting " << endl);
        SQLConnectAction<ERROR_TYPE, MSG_TYPE, OUT_TYPE1>::connect(
                // SQLHandleConnector
                *connector,
                // get container from storage
                dhi.getSQLContainer(),
                // check action to do on errors
                error_factory); // try to connect
        BESDEBUG(SQL_NAME, "SQLBuildDATA: Connected" << endl);

        BESDEBUG(SQL_NAME, "SQLBuildDATA: Executing query" << endl);
        SQLQueryAction<ERROR_TYPE, MSG_TYPE, OUT_TYPE1>::query(*connector, error_factory);
        BESDEBUG(SQL_NAME, "SQLBuildDATA: Query successfully done" << endl);

        BESResponseObject *response = dhi.getResponseObject();

        BESDataDDSResponse *bdds = dynamic_cast<BESDataDDSResponse *>(response);
        if (!bdds)
            throw BESInternalFatalError("SQLBuildDATA: Unable to get DataDDSResponse object", __FILE__, __LINE__);

        libdap::DDS *dds = bdds->get_dds();

        // The name of the Sequence was being set to
        // dhi.getBesContainer()->get_symbolic_name() but I changed that
        // to the DSN because it seemed like a better name. The symbolic
        // name was, in fact, the name of the last column of the select
        // statement, and that seemed like a poor choice. Using the table
        // name(s) would be awkward and would also 'leak' information about
        // the database. 9/10/12 jhrg
        SQLSequence<SQL_TYPE, ODBC_TYPE> *seq
                = new SQLSequence<SQL_TYPE, ODBC_TYPE>(connector->getParams().getServer(),
                                                       connector->getParams().get_real_name(), connector);
#if 0
        SQLSequence<SQL_TYPE, ODBC_TYPE> *seq = new SQLSequence<SQL_TYPE, ODBC_TYPE>(
                dhi.getBesContainer()->get_symbolic_name(), connector->getParams().get_real_name(), connector);
#endif
#if 0
        // WE GET DAS TABLE FROM DDS
        DAS *das = new DAS;
        BESDASResponse bdas( das );
        bdas.set_container(connector->getParams().get_symbolic_name());
        AttrTable *attr=das->get_table(connector->getParams().get_symbolic_name());

        if (!attr)
        attr=das->add_table(
                connector->getParams().get_symbolic_name(),
                new AttrTable());
#endif
        libdap::AttrTable &attr = dds->get_attr_table();

        /**
         *  Reset cursor position
         */
        connector->reset();

        /**
         * For each column get the mapped DAP_TYPE
         * @note No value is read here, only DAP object build.
         */
        for (size_t i = 0; i < connector->getCols(); i++) {
            libdap::BaseType *bt = NULL;

            BESDEBUG(SQL_NAME, "SQLBuildDATA: getting next object" << endl);

            bt = SQLNextTypeAction<SQL_TYPE, ODBC_TYPE, ERROR_TYPE, MSG_TYPE, OUT_TYPE1>::nextType(*connector,
                                                                                                   type_factory,
                                                                                                   error_factory);

            if (bt) { // if 'bt' is created
                BESDEBUG(SQL_NAME, "SQLBuildDATA: The " << bt->type_name() <<
                                                        (bt->is_vector_type() ? " array" : " instance") << " named '"
                                                        << bt->name() <<
                                                        "'  is ready, adding..." << endl);

                // FIXME connector->getColDesc(i) is not the correct type value for
                // most of the variables.
                attr.append_attr(bt->name(), bt->type_name(), connector->getColDesc(i));
                seq->add_var_nocopy(bt, libdap::nil);
#if 0
                delete bt;
                bt = 0;
#endif
            }
            else // error occurred on 'bt' creation
            {
                BESDEBUG(SQL_NAME, "SQLBuildDATA: : Unrecognized NULL object, Adding default place-holder." << endl);
                /**
                 * create new Str type and initialize it using
                 * the dummy_reader which simply returns a
                 * new string(_SQLH_DEFAULT_DAS_VAL) (when called).
                 * <br>reuse flag is false since the new operator
                 * is used in the cast function dummy_reader.
                 *
                 * @todo: If connector fail to get column number
                 * setNext don't know how long to go...
                 */
                bt = new SQLDummySimpleType(_SQLH_DEFAULT_DAS_NAME, dummy_cast, false);
                connector->setNext();
                if (bt) {
                    seq->add_var_nocopy(bt, libdap::nil);
                    // TODO Check these values
                    attr.append_attr(_SQLH_DEFAULT_DAS_NAME, _SQLH_DEFAULT_DAS_TYPE, _SQLH_DEFAULT_DAS_VAL);
                }
                else
                    throw BESInternalError("Unable to allocate SQLObject", __FILE__, __LINE__);
            }
        } // endfor col

        // reset connector index position before start transfer
        connector->reset();

        if (seq) {
            BESDEBUG(SQL_NAME, "SQLBuildDATA: Adding SQLSequence variable '" << seq->name() << "' to DDS" << endl);
            dds->add_var_nocopy(seq);

            // The BES is setting this to 'virtual'. This is a better
            // name because it is consistent with how other responses
            // are named. 9/10/12 jhrg
            string name = connector->getParams().get_real_name();
            size_t p = name.find_last_of('/');
            if (p != string::npos && p + 1 <= name.length())
                name = name.substr(p + 1);
            dds->set_dataset_name(name);
            BESDEBUG(SQL_NAME, "SQLBuildDATA: Set dataset name to: '" << name << "'" << endl);
        }
        else
            throw BESInternalError("Unable to build an SQLSequence", __FILE__, __LINE__);

        dhi.getBesHandler().data[POST_CONSTRAINT] = dhi.getBesContainer()->get_constraint();

        BESDEBUG(SQL_NAME, "SQLBuildDATA: Set constraint" << endl);
        bdds->set_constraint(dhi.getBesHandler());

        BESDEBUG(SQL_NAME, "SQLBuildDATA: Clear container" << endl);
        bdds->clear_container();

        BESDEBUG(SQL_NAME, "SQLBuildDATA: DDS response object has been built" << endl);
        return true;
    }
};

#endif /* SQLBUILDDATA_H_ */
