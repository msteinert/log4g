#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# include packaging targets
include(CPack)
# include uninstall target
include(Uninstall)
# include user preferences
if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
    if(EXISTS $ENV{HOME}/.preferences.cmake)
        message(STATUS "Including user preferences")
        include($ENV{HOME}/.preferences.cmake)
        # setup CMake include preferences
        if(MODULE_PATH)
            set(CMAKE_MODULE_PATH ${MODULE_PATH} ${CMAKE_MODULE_PATH})
        endif(MODULE_PATH)
        # setup pkg-config preferences
        if(PKG_CONFIG_PATH)
            set(ENV{PKG_CONFIG_PATH} ${PKG_CONFIG_PATH}:$ENV{PKG_CONFIG_PATH})
        endif(PKG_CONFIG_PATH)
    endif(EXISTS $ENV{HOME}/.preferences.cmake)
endif(NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
# setup autoconf compatibility
if(CMAKE_INSTALL_PREFIX)
    set(prefix ${CMAKE_INSTALL_PREFIX})
else(CMAKE_INSTALL_PREFIX)
    set(prefix /usr/local)
endif(CMAKE_INSTALL_PREFIX)
set(top_srcdir ${CMAKE_SOURCE_DIR})
