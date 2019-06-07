/*
 * SQLBuildDDS.h
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

#ifndef SQLBUILDDDS_H_
#define SQLBUILDDDS_H_

#include "connector/SQLConnector.h"

#include "handler/connector/SQLConnectAction.h"
#include "handler/connector/SQLQueryAction.h"
#include "handler/connector/SQLCloseAction.h"
#include "handler/connector/SQLNextTypeAction.h"

// POST CONSTRAINTS
#include <BESDataNames.h>

#include "DTM/SQLTypeFactoryComponent.h"
#include "DTM/SQLTypeManager.h"
#include "DTM/SQLObjectTypeFactory.h"
#include "DTM/SQLSequence.h"
#include "DTM/SQLSimpleType.h"
#include "DTM/SQLDummySimpleType.h"
#include <BaseType.h>
#include <BESDDSResponse.h>

#include "SQLDefinitions.h"

#include <DDS.h>

/**
 * @todo add Manager template arguments and wrapper to pass
 * merge and join functions.
 *
 * @brief Placeholder class which define:
 * - maximum number of template arguments which this class may specify.
 * - default arguments to simplify default SQLBuildDDS::sql_build_dds calls.
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
class SQLBuildDDS {
private:
    SQLBuildDDS()
    {
    }
    ;
    virtual ~SQLBuildDDS()
    {
    }
    ;
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
 * sql_build_dds(
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
class SQLBuildDDS<SQL_TYPE, // connector
        ODBC_TYPE, // connector
        ERROR_TYPE, // SQLErrorFactory
        MSG_TYPE, // SQLErrorFactory
        OUT_TYPE1, // SQLErrorFactory
        SQL_TYPE, // SPECIALIZED
        SQL_TYPE, // SPECIALIZED
        libdap::BaseType> { // SPECIALIZED
private:
    SQLBuildDDS()
    {
    }
    ;
    virtual ~SQLBuildDDS()
    {
    }
    ;
public:

    /**
     * It is a wrapper of the:
     * @code
     * static bool sql_build_dds( BESDataHandlerInterface &dhi ,
     * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>* error_factory,
     * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
     * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>& connector)
     * @endcode
     * Which is provided to make easy to call the dds builder.
     * ODBCFactoryComponent).
     *
     * @param dhi BESDataHandlerInterface reference
     * @param error_factory a reference to
     * SQL{Static|Object|Dynamic}ErrorFactory
     * @param fc a reference to the TypeFactoryComponent
     * to use
     * @param connector a reference to a simple connector
     * SQLSimpleConnector.
     *
     * @see SQLObjectTypeFactory
     * @see ODBCFactoryComponent
     */
    static bool sql_build_dds(SQLDataHandlerInterface &dhi, SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE>& fc,
                              SQLSimpleConnector<SQL_TYPE, ODBC_TYPE>* connector)
    {
        /**
         * Build the TypeFactory which will be our
         * BaseType factory.
         * <br>Note that this is not a BaseTypeFactory
         * but again an ActionFactory derived object.
         */
        SQLObjectTypeFactory<SQL_TYPE, ODBC_TYPE> tf = SQLObjectTypeFactory<SQL_TYPE, ODBC_TYPE>(fc);

        return sql_build_dds(dhi, NULL, tf, connector);
    }

    /**
     * @brief This is a function which uses the SQLTypeManager
     * with the SQLObjectTypeFactory to build BaseType object.
     * It is a wrapper of the:
     * @code
     * static bool sql_build_data( BESDataHandlerInterface &dhi ,
     * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>* error_factory,
     * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
     * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>& connector)
     * @endcode
     * Which is provided to make easy to call the dds builder.
     *
     * @param dhi BESDataHandlerInterface reference
     * @param error_factory a reference to
     * SQL{Static|Object|Dynamic}ErrorFactory
     * @param fc a reference to the TypeFactoryComponent
     * to use
     * @param connector a reference to a complete connector
     * SQLConnector.
     *
     * @see SQLObjectTypeFactory
     * @see ODBCFactoryComponent
     */
    static bool sql_build_dds(SQLDataHandlerInterface &dhi,
                              SQLActionFactory<ERROR_TYPE, MSG_TYPE, OUT_TYPE1>& error_factory,
                              SQLTypeFactoryComponent<SQL_TYPE, ODBC_TYPE>& fc,
                              SQLSimpleConnector<SQL_TYPE, ODBC_TYPE>* connector)
    {
        /**
         * Build the TypeFactory which will be our
         * BaseType factory.
         * <br>Note that this is not a BaseTypeFactory
         * but again an ActionFactory derived object.
         */
        SQLObjectTypeFactory<SQL_TYPE, ODBC_TYPE> tf = SQLObjectTypeFactory<SQL_TYPE, ODBC_TYPE>(fc);

        return sql_build_dds(dhi, &error_factory, tf, connector);
    }

    /**
     * It is a public wrapper of the:
     * @code
     * static bool sql_build_data( BESDataHandlerInterface &dhi ,
     * 		SQLActionFactory<ERROR_TYPE,MSG_TYPE,OUT_TYPE1>& error_factory,
     * 		SQLActionFactory<SQL_TYPE,SQL_TYPE,libdap::BaseType>& type_factory,
     * 		SQLConnector<SQL_TYPE,ODBC_TYPE,ERROR_TYPE,MSG_TYPE>& connector)
     * @endcode
     * Which is provided to make easy to call the dds builder.
     *
     * @param dhi BESDataHandlerInterface reference
     * @param error_factory a reference to
     * SQL{Static|Object|Dynamic}ErrorFactory
     * @param type_factory a reference to the
     * SQL{Static|Object|Dynamic}TypeFactory used
     * @param connector a reference to a simple connector
     * SQLSimpleConnector.
     *
     * @see SQLObjectTypeFactory
     * @see ODBCFactoryComponent
     */
    static bool sql_build_dds(SQLDataHandlerInterface &dhi,
                              SQLActionFactory<ERROR_TYPE, MSG_TYPE, OUT_TYPE1>& error_factory,
                              SQLActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType>& type_factory,
                              SQLSimpleConnector<SQL_TYPE, ODBC_TYPE>* connector)
    {

        return sql_build_dds(dhi, &error_factory, // pass as !NULL pointer
                type_factory, connector);
    }

private:
    /**
     * used to return unrecognized object
     */
    static string * dummy_cast(void *)
    {
        return new string(_SQLH_DEFAULT_DAS_VAL);
    }

    /**
     *
     * Main static function which implement DDS construction
     * it make use of the /handler/connector/... actions to
     * perform connection operation and can be used with or
     * without ErrorFactory (setting its pointer to NULL).
     * See wrapper comments for more info
     *
     */
    static bool sql_build_dds(SQLDataHandlerInterface &dhi,
                              SQLActionFactory<ERROR_TYPE, MSG_TYPE, OUT_TYPE1>* error_factory,
                              SQLActionFactory<SQL_TYPE, SQL_TYPE, libdap::BaseType>& type_factory,
                              SQLSimpleConnector<SQL_TYPE, ODBC_TYPE>* connector)
    {
        BESDEBUG(SQL_NAME, "SQLBuildDDS: Connecting "<<endl);
        SQLConnectAction<ERROR_TYPE, MSG_TYPE, OUT_TYPE1>::connect(
        // SQLHandleConnector
                *connector,
                // get container from storage
                dhi.getSQLContainer(),
                // check action to do on errors
                error_factory); // try to connect
        BESDEBUG(SQL_NAME, "SQLBuildDDS: Connected"<<endl);

        BESDEBUG(SQL_NAME, "SQLBuildDDS: Executing query"<<endl);
        SQLQueryAction<ERROR_TYPE, MSG_TYPE, OUT_TYPE1>::query(*connector, error_factory);
        BESDEBUG(SQL_NAME, "SQLBuildDDS: Query successfully done"<<endl);

        BESResponseObject *response = dhi.getResponseObject();

        BESDDSResponse *bdds = dynamic_cast<BESDDSResponse *>(response);
        if (!bdds)
            throw BESInternalFatalError("SQLBuildDDS: Unable to get DDSResponse object", __FILE__, __LINE__);

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

        libdap::AttrTable &attr = dds->get_attr_table();

        /**
         *  Reset cursor position
         */
        connector->reset();

        /**
         * For each column get the mapped DAP_TYPE
         * NOTE: No value is read here, only DAP object build.
         */
        for (size_t i = 0; i < connector->getCols(); i++) {
            libdap::BaseType *bt = NULL;

            try {
                BESDEBUG(SQL_NAME, "SQLBuildDDS: getting next object"<<endl);

                bt = SQLNextTypeAction<SQL_TYPE, ODBC_TYPE, ERROR_TYPE, MSG_TYPE, OUT_TYPE1>::nextType(*connector,
                        type_factory, error_factory);

                if (bt) { // if 'bt' is created
                    BESDEBUG(SQL_NAME, "SQLBuildDDS: done"<<endl);
                    // FIXME connector->getColDesc(i) is not teh correct type of value
                    // for most of the variables.
                    attr.append_attr(bt->name(), bt->type_name(), connector->getColDesc(i));
                    seq->add_var_nocopy(bt);
#if 0
                    delete bt;
                    bt = 0;
#endif
                }
                else // error occurred on 'bt' creation
                {
                    BESDEBUG(SQL_NAME, "SQLBuildDDS: : Unrecognized NULL object, Adding default place-holder."<<endl);
                    /**
                     * create new Str type and initialize it using
                     * the dummy_reader which simply returns a
                     * new string(_SQLH_DEFAULT_DAS_VAL) (when called).
                     * <br>reuse flag is false since the new operator
                     * is used in the cast function dummy_reader.
                     *
                     * @todo: check this may fail on connection fail
                     * since simpleType constructor use the connector
                     * which HERE may fail.
                     */
                    bt = new SQLDummySimpleType(_SQLH_DEFAULT_DAS_NAME, dummy_cast, false);
                    connector->setNext();
                    if (bt) {
                        seq->add_var_nocopy(bt, libdap::nil);
#if 0
                        delete bt;
                        bt = 0;
#endif
                        // TODO Check on these values. cross check in BuildDAS, too.
                        attr.append_attr(_SQLH_DEFAULT_DAS_NAME, _SQLH_DEFAULT_DAS_TYPE, _SQLH_DEFAULT_DAS_VAL);
                    }
                    else
                        throw BESInternalError("SQLBuildDDS: Unable to allocate SQLObjects", __FILE__, __LINE__);
                }
            } // end try
            catch (BESError &e) {
                BESDEBUG(SQL_NAME, "SQLBuildDDS: Caught error: " << e.get_message() << endl);
                // FIXME BESLog
                //cerr << "SQLBuildDDS: Caught error: " << e.get_message() << endl;
                throw;
            } catch (...) {
                BESDEBUG(SQL_NAME, "SQLBuildDDS: Caught something" << endl);
                // FIXME BESLog
                //cerr << "caught something" << endl;
                throw;
            }
        } // endfor col

        // reset connector cursor position before start transfer
        connector->reset(); // done by SQLSequence constructor

        if (seq) {
            BESDEBUG(SQL_NAME, "SQLBuildDDS: Adding variable to dds"<<endl);
            dds->add_var_nocopy(seq);
#if 0
            /**
             * To be added ptr_duplicate is called so this sequence
             * is no more usable since connector handle is passed to
             * its duplicate.
             * @see SQLSequence
             */
            delete seq; //safe
#endif
#if 0
            dds->set_dataset_name(dhi.getBesContainer()->get_real_name());
#endif
            // The BES is setting this to 'virtual'. This is a better
            // name because it is consistent with how other responses
            // are named. 9/10/12 jhrg
            string name = connector->getParams().get_real_name();
            size_t p = name.find_last_of('/');
            if (p != string::npos && p+1 <= name.length())
                name = name.substr(p + 1);
            dds->set_dataset_name(name);
#if 0
            // Not needed; set in the BES and not used anyway. 9/10/12 jhrg.
            dds->filename(dhi.getBesContainer()->get_real_name());
#endif

#if __TESTS__==1
            TESTDEBUG(SQL_NAME_TEST,"--------------DATA DUMP---------------"<<endl);
            attr.dump(std::cerr);
            TESTDEBUG(SQL_NAME_TEST,"------------SEQ DUMP DONE------------"<<endl);
#endif
        }
        else
            throw BESInternalError("Unable to build an SQLSequence", __FILE__, __LINE__);

        dhi.getBesHandler().data[POST_CONSTRAINT] = dhi.getBesContainer()->get_constraint();

        BESDEBUG(SQL_NAME, "SQLBuildDDS: Set constraint"<<endl);
        bdds->set_constraint(dhi.getBesHandler());

        BESDEBUG(SQL_NAME, "SQLBuildDDS: Clear container"<<endl);
        bdds->clear_container();

        BESDEBUG(SQL_NAME, "SQLBuildDDS: DDS is built"<<endl);
        return true;
    }
};
#endif /* SQLBUILDDDS_H_ */
