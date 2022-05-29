AC_PREREQ([2.63])
AC_INIT([imagedb], [0.1])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config])
AM_INIT_AUTOMAKE([subdir-objects tar-ustar filename-length-max=256 foreign])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE([enable])

LT_INIT([dlopen])

# Checks for programs.
AC_DEFINE([_GNU_SOURCE], 1,
          [Define _GNU_SOURCE so that we get all necessary prototypes])
AC_PROG_CC_C99
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_PROG_CC_C_O

# Checks for libraries
PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([SQLITE], [sqlite3], [], [])
PKG_CHECK_MODULES([PROTOBUF], [protobuf], [], [])
PKG_CHECK_MODULES([GRPCPP], [grpc++], [], [])

AC_CHECK_PROG([PROTOC], [protoc], [protoc])
AS_IF([test "x${PROTOC}" == "x"],
    [AC_MSG_ERROR([ProtoBuf compiler "protoc" not found.])])


AC_CONFIG_FILES([Makefile
  src/Makefile
  src/common/Makefile
  src/common/libtap/Makefile
  imagelogue/Makefile
  imagelogue/db/Makefile
  imagelogue/db/test/Makefile
  imagelogue/cli/Makefile
  t/Makefile
  etc/Makefile])
AC_OUTPUT

echo "
  $PACKAGE_NAME version $PACKAGE_VERSION
  Prefix...........: $prefix
  Debug Build......: $debug
  C Compiler.......: $CC
  C++ Compiler.....: $CXX
  CFLAGS...........: $CFLAGS
  CPPFLAGS.......... $CPPFLAGS
  CXXFLAGS.......... $CXXFLAGS
  LDFLAGS..........: $LDFLAGS
  LIBS.............: $LIBS
  Linker...........: $LD
  PROTOBUF_CPPFLAGS: $PROTOBUF_CPPFLAGS
  PROTOBUF_CFLAGS..: $PROTOBUF_CFLAGS
  PROTOBUF_LIBS....: $PROTOBUF_LIBS
  PROTOBUF_LDFLAGS.: $PROTOBUF_LDFLAGS
  GRPCPP_CPPFLAGS..: $GRPCPP_CPPFLAGS
  GRPCPP_CFLAGS....: $GRPCPP_CFLAGS
  GRPCPP_LIBS......: $GRPCPP_LIBS
  GRPCPP_LDFLAGS...: $GRPCPP_LDFLAGS
"
