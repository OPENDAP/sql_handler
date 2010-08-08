
# Build the Back End Server.
#
# jhrg 9/2/05
# 08/Aug/2010 carlo cancellieri ccancellieri@hotmail.com

AUTOMAKE_OPTIONS = foreign check-news
ACLOCAL_AMFLAGS = -I conf

bin_SCRIPTS = besCreateSQLPlugin filter.awk

EXTRA_DIST = besCreateSQLPlugin.in filter.awk
DISTCLEANFILES = besCreateSQLPlugin filter.awk

besCreateSQLPlugin: besCreateSQLPlugin.in ../config.status
	sed -e "s%[@]pkgdatadir[@]%${pkgdatadir}%" \
		-e "s%[@]sysconfdir[@]%${sysconfdir}%" \
		-e "s%[@]pkglibdir[@]%${pkglibdir}%" \
		-e "s%[@]datadir[@]%${datadir}%" \
		-e "s%[@]prefix[@]%${prefix}%" \
		-e "s%[@]bindir[@]%${bindir}%" $< > besCreateSQLPlugin

install-data-local: filter.awk
	@currdate=`date +"%y%m%d%H%M"`; \
	test -d $(bindir) || \
	cp filter.awk $(bindir)/filter.awk

uninstall-local:
		rm -f $(bindir)/filter.awk
