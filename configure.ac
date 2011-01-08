dnl Autoconf script for Dolphin Connector
dnl William Lima <wlima@primate.com.br>

AC_PREREQ([2.59])

AC_INIT([Dolphin Connector], [1.0], [wlima@primate.com.br])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR(README)
AM_INIT_AUTOMAKE([1.9 foreign])

CXXFLAGS="$CXXFLAGS -Wall -fno-rtti -fno-exceptions"

AC_PROG_CXX
AC_PROG_LIBTOOL
AC_LANG(C++)

dnl Check for mysql
AC_ARG_WITH(mysql,
[  --with-mysql=<path> specifies how to run the mysql_config script [default=mysql_config]],
		[mysql_config_path="$withval"], [mysql_config_path="mysql_config"])

AC_MSG_CHECKING([for mysql headers and libraries])
MYSQL_INCLUDE=`$mysql_config_path --include`
MYSQL_LIBS=`$mysql_config_path --libs_r`

if test -z "$MYSQL_INCLUDE"; then
	AC_MSG_ERROR([Could not find mysql_config script. Make sure the mysql client libraries are installed (e.g. "mysql-devel").])
else
	AC_MSG_RESULT([yes])
fi

dnl Check for Boost
AC_CHECK_HEADERS([boost/scoped_array.hpp boost/noncopyable.hpp], [], AC_MSG_ERROR([Boost headers not found]))

dnl Substitions
AC_SUBST(MYSQL_INCLUDE)
AC_SUBST(MYSQL_LIBS)

dnl Output files
AC_CONFIG_FILES([Makefile include/Makefile sample/Makefile])

AC_OUTPUT
