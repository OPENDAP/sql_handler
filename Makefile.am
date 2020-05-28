# Automake file for sql-handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

# bin Removed 10/21/19 jhrg 
SUBDIRS = . templates tests

AUTOMAKE_OPTIONS = foreign check-news subdir-objects

ACLOCAL_AMFLAGS = -I conf

AM_CXXFLAGS= -std=c++11 --pedantic

# -D__TESTS__

SQL_SRCS = \
		utils/StringMatch.cc \
		container/SQLContainerFactory.cc \
		container/SQLContainerStorageVolatile.cc \
		container/SQLQuery.cc \
		container/SQLContainer.cc container/SQLTextContainer.cc \
		SQLPlugin.cc \
		DEM/SQLCheckPoint.cc

SQL_HDRS = SQLLinker.h \
		sql_config.h SQLResponseNames.h SQLDefinitions.h \
		SQLPlugin.h SQLPluginList.h \
		container/SQLAttribute.h container/SQLConstraint.h \
		container/SQLContainer.h container/SQLContainerDefinitions.h \
		container/SQLContainerFactory.h \
		container/SQLContainerStorage.h \
		container/SQLContainerStorageVolatile.h \
		container/SQLDataHandlerInterface.h \
		container/SQLQuery.h container/SQLQueryDefinitions.h \
		container/SQLTextContainer.h container/SQLTextDefinitions.h \
		utils/SmartList.h utils/SmartMap.h \
		utils/SmartCheck.h utils/SmartCast.h \
		utils/Delete.h utils/Clone.h \
		utils/StringMatch.h \
		handler/SQLBuildDAS.h \
		handler/SQLBuildDDS.h \
		handler/SQLBuildDATA.h \
		handler/connector/SQLCloseAction.h \
		handler/connector/SQLConnectAction.h \
		handler/connector/SQLQueryAction.h \
		handler/connector/SQLNextTypeAction.h \
		connector/SQLConnector.h connector/SQLSimpleConnector.h \
		connector/SQLTypeConnector.h connector/SQLErrorConnector.h \
		connector/SQLHandleConnector.h \
		DTM/SQLBaseType.h DTM/SQLCastAction.h \
		DTM/SQLComplexType.h DTM/SQLDummySimpleType.h \
		DTM/SQLDynamicTypeFactory.h \
		DTM/SQLObjectTypeFactory.h \
		DTM/SQLSequence.h \
		DTM/SQLSimpleType.h \
		DTM/SQLTypeFactoryComponent.h \
		DTM/SQLTypeManager.h \
		DEM/SQLErrorManager.h DEM/SQLCheckPoint.h \
		DEM/SQLInternalError.h DEM/SQLInternalFatalError.h \
		DEM/SQLDefaultErrorAction.h DEM/SQLErrorFactory.h \
		action/SQLAction.h action/SQLObjectAction.h \
		action/SQLActionList.h action/SQLActionFactory.h \
		action/SQLActionManager.h \
		action/SQLDynamicActionFactory.h action/SQLDynamicActionList.h \
		action/SQLObjectActionFactory.h action/SQLObjectActionList.h \
		action/SQLStaticAction.h action/SQLStaticActionList.h

# removed. jhrg 6/7/19 utils/SharedPtr.h 

ODBC_SRCS = \
		unixODBC/ODBCPluginLoader.cc \
		unixODBC/ODBCPlugin.cc \
		unixODBC/ODBCConnector.cc \
		unixODBC/ODBCTypeFactoryComponent.cc \
		unixODBC/ODBCErrorFactoryComponent.cc



ODBC_HDRS = \
		unixODBC/ODBCPluginLoader.h \
		unixODBC/ODBCPlugin.h \
		unixODBC/ODBCTypes.h \
		unixODBC/ODBCConnector.h \
		unixODBC/ODBCErrorFactoryComponent.h \
		unixODBC/ODBCTypeFactoryComponent.h		

BES_SRCS = \
		SQLModule.cc SQLRequestHandler.cc

BES_HDRS = \
		SQLModule.h SQLRequestHandler.h

EXTRA_DIST = data doxy.conf COPYING sql_handler.spec \
		sql.conf.in odbc.conf.in

EXTRA_PROGRAMS = 

CLEANFILES = *~ sql.conf

DISTCLEANFILES = sql_config.h

# TODO jimg I added -Xlinker -export-dynamic to LDFLAGS for the two modules
# determine if that was/is needed/enough to make the exceptions work across
# modules. What about using RTLD_GLOBAL with dlopen in BESPlugin.h?
# Also, why include SQL_SRCS/HDRS in both the sql_module and odbc_module?
# Maybe because they are loaded with RTLD_LOCAL instead of ..._GLOBAL?
# What about making just one module.

lib_besdir=$(libdir)/bes
lib_bes_LTLIBRARIES = libsql_module.la  libodbc_module.la

libsql_module_la_SOURCES = $(BES_SRCS) $(BES_HDRS) $(SQL_SRCS) $(SQL_HDRS)
libsql_module_la_CPPFLAGS =  $(BES_CPPFLAGS)
libsql_module_la_LDFLAGS = -avoid-version -module
libsql_module_la_LIBADD = $(BES_DAP_LIBS)

libodbc_module_la_SOURCES = $(ODBC_SRCS) $(ODBC_HDRS)
libodbc_module_la_CPPFLAGS =  $(BES_CPPFLAGS) $(ODBC_CPPFLAGS)
libodbc_module_la_LDFLAGS = -avoid-version -module -lodbc
libodbc_module_la_LIBADD = $(BES_DAP_LIBS) $(ODBC_LIBS)

## EXPORTING HEADERS

sqldir=$(includedir)/sql/
nobase_sql_HEADERS=$(SQL_HDRS)

sample_datadir = $(datadir)/hyrax/data/sql
sample_data_DATA = data/sqlite.sql

BUILT_SOURCES = data/sqlite_odbc.ini

moduledir = $(DESTDIR)$(sysconfdir)/bes/modules
module_DATA = sql.conf

data/sqlite_odbc.ini: data/sqlite_odbc.ini.in config.status
	@clean_abs_top_srcdir=`python -c "import os.path; print(os.path.abspath('${abs_top_srcdir}'))"`; \
	sed -e "s%[@]abs_top_srcdir[@]%$$clean_abs_top_srcdir%" \
		-e "s%[@]abs_top_builddir[@]%${abs_top_builddir}%" $< > $@

sql.conf: sql.conf.in config.status
	sed -e "s%[@]bes_modules_dir[@]%${lib_besdir}%" $< > $@

HTML_DOCS=html
DOXYGEN_CONF=doxy.conf

.PHONY: docs
docs: ${DOXYGEN_CONF}
	doxygen ${DOXYGEN_CONF}
