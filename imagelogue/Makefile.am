SUBDIRS = . db cli

noinst_LTLIBRARIES = libimagedb.la

libimagedb_la_SOURCES = \
	db/imagedb_insert.cpp \
	db/imagedb_insert.h \
	db/imagedb.h

libimagedb_la_CXXFLAGS = \
	$(AM_CXXFLAGS) \
	$(SQLITE_CFLAGS)

libimagedb_la_LIBADD = \
	$(SQLITE_LIBS)

