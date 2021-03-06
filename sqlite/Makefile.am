# Automake file for sqlite-handler
#
# 5/17/2005 jhrg
# 6/Aug/2010 ccancellieri
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

AM_CXXFLAGS=
AM_LDFLAGS =
include $(top_srcdir)/coverage.mk

lib_besdir=$(libdir)/bes
lib_bes_LTLIBRARIES = libsqlite_module.la

SQLITE_SRCS = \
		SQLITEPluginLoader.cpp \
		SQLITEPlugin.cpp \
		SQLITESimpleConnector.cpp \
		SQLITETypeFactoryComponent.cpp

SQLITE_HDRS = \
		SQLITEPluginLoader.h \
		SQLITEPlugin.h \
		SQLITEResponseNames.h \
		SQLITETypes.h \
		SQLITESimpleConnector.h \
		SQLITETypeFactoryComponent.h

sample_datadir = $(datadir)/hyrax/data/sqlite/
sample_data_DATA = data/test_full.sql

libsqlite_module_la_SOURCES = $(SQLITE_SRCS) $(SQLITE_HDRS)
libsqlite_module_la_CPPFLAGS = $(BES_CPPFLAGS) -I$(includedir)/sql
#-D __TESTS__=1  -D __CLONE__=0
libsqlite_module_la_LDFLAGS = -avoid-version -module -L$(lib_besdir) -lbes_sql
libsqlite_module_la_LIBADD = $(BES_DAP_LIBS)

EXTRA_PROGRAMS = 

EXTRA_DIST = data COPYRIGHT sqlite_module.spec sqlite.conf.in

CLEANFILES = *~ sqlite.conf

do_subst = sed -e 's,[@]sysconfdir[@],$(sysconfdir),g' \
		   	   -e 's,[@]libdir[@],$(libdir),g' \
		   	   -e 's,[@]datadir[@],$(datadir),g'

##########################################################################

install-data-local: sqlite.conf
	@currdate=`date +"%y%m%d%H%M"`; \
	test -d $(DESTDIR)$(sysconfdir)/bes || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes; \
	test -d $(DESTDIR)$(sysconfdir)/bes/modules || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes/modules; \
	test -f $(DESTDIR)$(sysconfdir)/bes/modules/sqlite.conf && \
	mv -f $(DESTDIR)$(sysconfdir)/bes/modules/sqlite.conf \
		$(DESTDIR)$(sysconfdir)/bes/modules/sqlite.conf.$$currdate; \
	$(INSTALL_DATA) sqlite.conf $(DESTDIR)$(sysconfdir)/bes/modules/sqlite.conf

uninstall-local:
		rm -f $(DESTDIR)$(sysconfdir)/bes/modules/sqlite.conf

sqlite.conf: sqlite.conf.in config.status
		sed -e "s%[@]bes_modules_dir[@]%${lib_besdir}%" $< > sqlite.conf
###########################################################################
#
# Build linux RPMs

srpm: dist
	rpmbuild -ts sqlite_module-@PACKAGE_VERSION@.tar.gz

rpm: dist
	rpmbuild -tb sqlite_module-@PACKAGE_VERSION@.tar.gz

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

