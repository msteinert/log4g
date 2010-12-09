#serial 1

# LOG4G_TRY_CC_FLAGS(flags, [action-if-true], [action-if-false])
# --------------------------------------------------------------
# Test if one or more compiler flags are supported.
AC_DEFUN([LOG4G_TRY_CC_FLAGS],
[AC_REQUIRE([AC_PROG_CC])
for flag in "$1"; do
    log4g_cflags=$CFLAGS
    CFLAGS="$flag $CFLAGS"
    AC_MSG_CHECKING([if compiler accepts '$flag'])
    AC_TRY_COMPILE([], [],
        [AC_MSG_RESULT([yes])
        CFLAGS=$log4g_cflags
        $2],
        [AC_MSG_RESULT([no])
        CFLAGS=$log4g_cflags
        $3])
done
])dnl

# LOG4G_TRY_LD_FLAGS(flags, [action-if-true], [action-if-false])
# --------------------------------------------------------------
# Test if one or more linker flags are supported.
AC_DEFUN([LOG4G_TRY_LD_FLAGS],
[for flag in "$1"; do
    log4g_ldflags=$LDFLAGS
    LDFLAGS="$flag $LDFLAGS"
    AC_MSG_CHECKING([if linker accepts '$flag'])
    AC_TRY_LINK([], [],
        [AC_MSG_RESULT([yes])
        LDFLAGS=$log4g_ldflags
        $2],
        [AC_MSG_RESULT([no])
        LDFLAGS=$log4g_ldflags
        $3])
done
])dnl

# LOG4G_CFLAGS(flags)
# -------------------
# Enable compiler flags.
AC_DEFUN([LOG4G_CFLAGS],
[for flag in "$1"; do
    case " $CFLAGS " in
    *[[\ \	]]$flag[[\ \	]]*) ;;
    *) CFLAGS="$CFLAGS $flag" ;;
    esac
done
])dnl

# LOG4G_LDFLAGS(flags)
# --------------------
# Enable linker flags.
AC_DEFUN([LOG4G_LDFLAGS],
[for flag in "$1"; do
    case " $LDFLAGS " in
    *[[\ \	]]$flag[[\ \	]]*) ;;
    *) LDFLAGS="$LDFLAGS $flag" ;;
    esac
done
])dnl
