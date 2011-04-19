ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libdolphinconn.la

SUBDIRS = . include

libdolphinconn_la_SOURCES = connection.cc resultset.cc
libdolphinconn_la_LIBADD = @MYSQL_LIBS@
libdolphinconn_la_LDFLAGS = -version-info 3:0:2

INCLUDES = -I$(srcdir)/include @MYSQL_INCLUDE@
