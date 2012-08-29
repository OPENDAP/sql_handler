AUTOMAKE_OPTIONS = foreign

TESTSUITE = $(srcdir)/testsuite

EXTRA_DIST = testsuite.at package.m4.in $(TESTSUITE) $(srcdir)/package.m4 \
        atlocal.in template.bescmd.in bes.conf.in baselines

noinst_DATA = bes.conf
DISTCLEANFILES = bes.conf

check-local: atconfig atlocal $(TESTSUITE)
	$(SHELL) '$(TESTSUITE)' $(TESTSUITEFLAGS)

clean-local:
	test ! -f '$(TESTSUITE)' || \
	$(SHELL) '$(TESTSUITE)' --clean

distclean-local:
	-rm atconfig
	-rm bes.conf


# Use the missing script for better error checking on autom4te run.
AUTOM4TE = $(SHELL) $(top_srcdir)/conf/missing --run autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

$(TESTSUITE): $(srcdir)/testsuite.at $(srcdir)/package.m4 bes.conf
	$(AUTOTEST) -I '$(srcdir)' -o ${srcdir}/$@.tmp $@.at
	mv ${srcdir}/$@.tmp ${srcdir}/$@

bes.conf: bes.conf.in ../config.status
	sed -e "s%[@]pkgdatadir[@]%${pkgdatadir}%" \
		-e "s%[@]sysconfdir[@]%${sysconfdir}%" \
		-e "s%[@]beslibdir[@]%${BES_MODULE_DIR}%" \
		-e "s%[@]datadir[@]%${datadir}%" \
		-e "s%[@]abs_top_srcdir[@]%${abs_top_srcdir}%" \
		-e "s%[@]abs_top_builddir[@]%${abs_top_builddir}%" \
		-e "s%[@]bindir[@]%${bindir}%" $< > bes.conf


