
bin_PROGRAMS = psredit psrtxt psrconv vip vap

psredit_SOURCES		= psredit.C
psrtxt_SOURCES		= psrtxt.C
psrconv_SOURCES		= psrconv.C
vip_SOURCES		= vip.C
vap_SOURCES		= vap.C

check_PROGRAMS = test_threads

test_threads_SOURCES	= test_threads.C

#############################################################################

AM_CPPFLAGS = -I$(top_builddir)/local_include

LDADD = $(top_builddir)/Base/libpsrbase.la \
	$(top_builddir)/Util/libpsrutil.la

