AM_CPPFLAGS = -I$(top_srcdir)

TESTS = imagedb_test01

check_PROGRAMS = $(TESTS)
imagedb_test01_SOURCES = imagedb_test01.cpp
imagedb_test01_CXXFLAGS = \
    $(AM_CXXFLAGS) \
    -I$(top_srcdir)/imagelogue/db \
    -I$(top_srcdir)/src/common/libtap
imagedb_test01_LDADD = \
    $(top_builddir)/src/common/libtap/libtap.la \
    $(top_builddir)/imagelogue/libimagedb.la
