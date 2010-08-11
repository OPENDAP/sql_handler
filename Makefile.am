# Automake file for sql-handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

SUBDIRS = bin templates

AUTOMAKE_OPTIONS = foreign check-news subdir-objects

ACLOCAL_AMFLAGS = -I conf

SQL_SRCS = \
		utils/StringMatch.cpp \
		container/SQLContainerFactory.cpp \
		container/SQLContainerStorageVolatile.cpp \
		container/SQLQuery.cpp \
		container/SQLContainer.cpp container/SQLTextContainer.cpp \
		SQLPlugin.cpp \
		DEM/SQLCheckPoint.cpp

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
		utils/SharedPtr.h utils/SmartList.h utils/SmartMap.h \
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

ODBC_SRCS = \
		unixODBC/ODBCPluginLoader.cpp \
		unixODBC/ODBCPlugin.cpp \
		unixODBC/ODBCConnector.cpp \
		unixODBC/ODBCTypeFactoryComponent.cpp \
		unixODBC/ODBCErrorFactoryComponent.cpp

ODBC_HDRS = \
		unixODBC/ODBCPluginLoader.h \
		unixODBC/ODBCPlugin.h \
		unixODBC/ODBCTypes.h \
		unixODBC/ODBCConnector.h \
		unixODBC/ODBCErrorFactoryComponent.h \
		unixODBC/ODBCTypeFactoryComponent.h		

BES_SRCS = \
		SQLModule.cpp SQLRequestHandler.cpp

BES_HDRS = \
		SQLModule.h SQLRequestHandler.h SQLLinker.h

EXTRA_DIST = data doxy.conf COPYRIGHT COPYING sql_handler.spec \
		sql.conf.in odbc.conf.in

EXTRA_PROGRAMS = 

CLEANFILES = *~ sql.conf sql_config.h _configs.sed

lib_LTLIBRARIES = libbes_sql.la
lib_besdir=$(libdir)/bes
lib_bes_LTLIBRARIES = libsql_module.la libodbc_module.la
#############################################################################
libbes_sql_la_SOURCES = $(SQL_SRCS) $(SQL_HDRS)
libbes_sql_la_CPPFLAGS =  $(BES_CPPFLAGS) -D __TESTS__=1 
# -D __TESTS__=1 
libbes_sql_la_LDFLAGS =  -Wl,-Bsymbolic -shared
# -D __TESTS__=1  -D __CLONE__=0
libbes_sql_la_LIBADD = $(BES_DAP_LIBS)
######################################################
libsql_module_la_SOURCES = $(BES_SRCS) $(SQL_SRCS) $(BES_HDRS) $(SQL_HDRS)
libsql_module_la_CPPFLAGS =  $(BES_CPPFLAGS) -D __TESTS__=1
#   -D __TESTS__=1
libsql_module_la_LDFLAGS =  -Wl,-Bsymbolic -shared -module
# -D __TESTS__=1  -D __CLONE__=0   -Wl,-E
libsql_module_la_LIBADD = $(BES_DAP_LIBS)
######################################################
libodbc_module_la_SOURCES = $(ODBC_SRCS) $(SQL_SRCS) $(ODBC_HDRS) $(SQL_HDRS)
libodbc_module_la_CPPFLAGS =  $(BES_CPPFLAGS) $(ODBC_CPPFLAGS) -D __TESTS__=1
libodbc_module_la_LDFLAGS = -Wl,-Bsymbolic -shared -module -lodbc
# -D __TESTS__=1  -D __CLONE__=0   -Wl,-E
libodbc_module_la_LIBADD = $(BES_DAP_LIBS) $(ODBC_LIBS)
######################################################
## EXPORTING HEADERS

#libsql_includedir=$(includedir)/sql/
sqldir=$(includedir)/sql/
nobase_sql_HEADERS=$(SQL_HDRS)

###########################################################################
sample_datadir = $(datadir)/hyrax/data/sql
sample_data_DATA = data/test_full.sql data/test_full1.sql data/test_full2.sql
###########################################################################
install-data-local: sql.conf
	@currdate=`date +"%y%m%d%H%M"`; \
	test -d $(DESTDIR)$(sysconfdir)/bes || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes; \
	test -d $(DESTDIR)$(sysconfdir)/bes/modules || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes/modules; \
	test -f $(DESTDIR)$(sysconfdir)/bes/modules/sql.conf && \
	mv -f $(DESTDIR)$(sysconfdir)/bes/modules/sql.conf \
		$(DESTDIR)$(sysconfdir)/bes/modules/sql.conf.$$currdate; \
	$(INSTALL_DATA) sql.conf $(DESTDIR)$(sysconfdir)/bes/modules/sql.conf

uninstall-local:
		rm -f $(DESTDIR)$(sysconfdir)/bes/modules/sql.conf

sql.conf: sql.conf.in config.status
		sed -e "s%[@]bes_modules_dir[@]%${lib_besdir}%" $< > sql.conf
###########################################################################
install-data-local: odbc.conf
	@currdate=`date +"%y%m%d%H%M"`; \
	test -d $(DESTDIR)$(sysconfdir)/bes || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes; \
	test -d $(DESTDIR)$(sysconfdir)/bes/modules || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes/modules; \
	test -f $(DESTDIR)$(sysconfdir)/bes/modules/odbc.conf && \
	mv -f $(DESTDIR)$(sysconfdir)/bes/modules/odbc.conf \
		$(DESTDIR)$(sysconfdir)/bes/modules/odbc.conf.$$currdate; \
	$(INSTALL_DATA) odbc.conf $(DESTDIR)$(sysconfdir)/bes/modules/odbc.conf

uninstall-local:
		rm -f $(DESTDIR)$(sysconfdir)/bes/modules/odbc.conf
		
odbc.conf: odbc.conf.in config.status
		sed -e "s%[@]bes_modules_dir[@]%${lib_besdir}%" $< > odbc.conf

#########################################################################

# docs

.PHONY: docs
docs:
	srcdir=$(srcdir) doxygen $(srcdir)/doxy.conf
	(cd docs && tar -czf html.tar.gz html)

###########################################################################

# Fortify targets.

.PHONY: fortify
fortify:
	sourceanalyzer -b @PACKAGE@ $(MAKE)
	sourceanalyzer -b @PACKAGE@ -scan -f @PACKAGE@-@PACKAGE_VERSION@.fpr

# Use this to clean the fortify project.
.PHONY: fortifyclean
fortifyclean:
	sourceanalyzer -b @PACKAGE@ -clean

###########################################################################
#
# Build linux RPMs

srpm: dist
	rpmbuild -ts sql_module-@PACKAGE_VERSION@.tar.gz

rpm: dist
	rpmbuild -tb sql_module-@PACKAGE_VERSION@.tar.gz


###########################################################################

#SQL_check_HDRS = \
#		Tests/SQLTests.h
#		
#SQL_check_SRCS = \
#		Tests/SQLTests.cpp


## Dynamic Type Matching Check
#CHECKS = dtmc
#dtmcdir = tests/
#bin_PROGRAMS=$(CHECKS)
#dtmc_SOURCES = $(SQL_check_SRCS)
#dtmc_CPPFLAGS = $(BES_CPPFLAGS) $(CPPUNIT_CFLAGS) -D __CLONE__=0
#dtmc_LDFLAGS =$(CPPUNIT_LIBS)
#dtmc_LDADD = $(BES_DAP_LIBS) libsql_module.la
#dtmc_HEADERS = $(SQL_check_HDRS)
#check:
#	$(MAKE)


###########################################################################

# Build OS/X Packages.

PACKAGEMAKER=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker*

UNIV_ARCH=-arch ppc -arch i386
MAC_SYSROOT=Developer/SDKs/MacOSX10.4u.sdk

PKG_CONF_FLAGS=

clean-pkg:
	-rm -rf mac_osx @PACKAGE@-@PACKAGE_VERSION@.pkg @PACKAGE@-@PACKAGE_VERSION@

pkg-build:
	./configure --prefix=/usr --disable-dependency-tracking $(PKG_CONF_FLAGS)
	make clean all
	DESTDIR=${PWD}/mac_osx make install

pkg-main:
	./OSX_Resources/update_mac_package_contents.pl README "configure.ac" "mac_osx/usr"
	${PACKAGEMAKER} -build -p @PACKAGE@-@PACKAGE_VERSION@.pkg \
		-f mac_osx/usr \
		-ds -r OSX_Resources \
		-i OSX_Resources/Info.plist \
		-d OSX_Resources/Description.plist
	-rm -rf mac_osx

pkg-dmg:
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@.dmg
	mkdir @PACKAGE@-@PACKAGE_VERSION@
	mv @PACKAGE@-@PACKAGE_VERSION@.pkg @PACKAGE@-@PACKAGE_VERSION@
	cp README NEWS @PACKAGE@-@PACKAGE_VERSION@
	dropdmg -i --sanitize-for-servers --format zlib @PACKAGE@-@PACKAGE_VERSION@
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@

pkg: clean-pkg pkg-build pkg-main pkg-dmg