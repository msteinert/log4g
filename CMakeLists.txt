#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# set the minimum version of CMake 
cmake_minimum_required(VERSION 2.6)
# set the module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# include default settings
include(Init)
# set the project name
project(Log4g)
# set the project version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
# set the maintainer
set(MAINTAINER "mike.steinert@gmail.com")
# add sub-directories
add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(tests)
# check for bind_textdomain_codeset()
include(CheckFunctionExists)
check_function_exists(bind_textdomain_codeset
    HAVE_BIND_TEXTDOMAIN_CODESET
)
if(NOT ${HAVE_BIND_TEXTDOMAIN_CODESET})
    set(CMAKE_REQUIRED_LIBRARIES intl)
    check_function_exists(bind_textdomain_codeset
        HAVE_BIND_TEXTDOMAIN_CODESET
    )
endif(NOT ${HAVE_BIND_TEXTDOMAIN_CODESET})
# create config.h
configure_file(cmake/config.h.in
    ${CMAKE_BINARY_DIR}/config.h
    @ONLY
)
# create pkg-config script
configure_file(resources/log4g.pc.in
    ${CMAKE_BINARY_DIR}/resources/log4g.pc
    @ONLY
)
install(FILES ${CMAKE_BINARY_DIR}/resources/log4g.pc
    DESTINATION lib/pkgconfig
)
# install DTD
install(DIRECTORY resources/dtd
    DESTINATION share/log4g
    FILES_MATCHING PATTERN "*.dtd"
)
# install CMake scripts
install(FILES
    resources/FindLog4g.cmake
    cmake/FindGLib-2.0.cmake
    cmake/FindGModule-2.0.cmake
    cmake/FindGObject-2.0.cmake
    cmake/FindGThread-2.0.cmake
    cmake/FindLibXML-2.0.cmake
    cmake/LibFindMacros.cmake
    DESTINATION share/log4g/cmake/Modules
)
