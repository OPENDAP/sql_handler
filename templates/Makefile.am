# Automake file for besCreateSQLPlugin
#
# 6/Aug/2010 ccancellieri
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest, ccancellieri $

AUTOMAKE_OPTIONS = foreign check-news

pkgdata_templatesdir = $(datadir)/sql/templates
pkgdata_templates_DATA = \
		Module.cpp Plugin.cpp \
		Connector.cpp \
		TypeFactoryComponent.cpp \
		ErrorFactoryComponent.cpp \
		Module.h ResponseNames.h \
		Plugin.h \
		Types.h \
		Connector.h \
		TypeFactoryComponent.h \
		ErrorFactoryComponent.h \
		module.spec.template COPYRIGHT \
		.conf.in.template Makefile.am.template configure.ac 
		
pkgdata_templatesconfdir = $(datadir)/sql/templates/conf
pkgdata_templatesconf_DATA = conf/acinclude.m4 conf/bes.m4 \
		conf/libdap.m4 conf/pkg.m4

sample_datadir = $(datadir)/sql/templates/data
sample_data_DATA = data/test_full.sql

EXTRA_DIST = Module.cpp Plugin.cpp \
		Connector.cpp \
		TypeFactoryComponent.cpp \
		ErrorFactoryComponent.cpp \
		Module.h ResponseNames.h \
		Plugin.h \
		Types.h \
		Connector.h \
		TypeFactoryComponent.h \
		ErrorFactoryComponent.h \
		COPYRIGHT .conf.in.template \
		Makefile.am.template configure.ac conf/acinclude.m4 \
		conf/bes.m4 conf/libdap.m4 conf/pkg.m4 \
		module.spec.template
		
		
uninstall-local:
	rm -rdf $(datadir)/sql/