#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# find necessary packages
find_package(GObject-2.0 REQUIRED)
find_package(LibXML-2.0 REQUIRED)
# include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${GObject-2.0_INCLUDE_DIRS}
    ${LibXML-2.0_INCLUDE_DIRS}
)
# compiler switches
add_definitions(-std=gnu99 -Wall -Werror)
# generate marshalling routines
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/support/marshal.h
    COMMAND ${GLib-2.0_GENMARSHAL} --header ${CMAKE_CURRENT_SOURCE_DIR}/support/marshal.list > ${CMAKE_CURRENT_SOURCE_DIR}/support/marshal.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/support/marshal.list
)
# generate marshalling routines
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/support/marshal.c
    COMMAND ${GLib-2.0_GENMARSHAL} --body ${CMAKE_CURRENT_SOURCE_DIR}/support/marshal.list > ${CMAKE_CURRENT_SOURCE_DIR}/support/marshal.c
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/support/marshal.list
)
# setup appenders
set(APPENDERS
    log4g/appender/appender-skeleton.h
    log4g/appender/async-appender.h
    log4g/appender/console-appender.h
    log4g/appender/file-appender.h
    log4g/appender/null-appender.h
    log4g/appender/rolling-file-appender.h
    log4g/appender/syslog-appender.h
    log4g/appender/writer-appender.h
)
# setup base headers
set(BASE
    log4g.c
    log4g/disable.h
    log4g/dom-configurator.h
    log4g/error.h
    log4g/hierarchy.h
    log4g/layout.h
    log4g/level.h
    log4g/log4g.h
    log4g/log-manager.h
    log4g/logger.h
    log4g/logging-event.h
    log4g/mdc.h
    log4g/ndc.h
    log4g/provision-node.h
    log4g/root-logger.h
)
# setup filters
set(FILTERS
    log4g/filter/deny-all-filter.h
    log4g/filter/level-match-filter.h
    log4g/filter/level-range-filter.h
    log4g/filter/string-match-filter.h
    log4g/filter.h
)
# setup helpers
set(HELPERS
    log4g/helpers/appender-attachable-impl.h
    log4g/helpers/counting-quiet-writer.h
    log4g/helpers/date-layout.h
    log4g/helpers/default-logger-factory.h
    log4g/helpers/default-repository-selector.h
    log4g/helpers/log.h
    log4g/helpers/only-once-error-handler.h
    log4g/helpers/pattern-converter.h
    log4g/helpers/pattern-parser.h
    log4g/helpers/quiet-writer.h
    log4g/helpers/thread.h
)
# setup interfaces
set(INTERFACES
    log4g/interface/appender.h
    log4g/interface/appender-attachable.h
    log4g/interface/configurator.h
    log4g/interface/error-handler.h
    log4g/interface/logger-factory.h
    log4g/interface/logger-repository.h
    log4g/interface/option-handler.h
    log4g/interface/repository-selector.h
)
# setup layouts
set(LAYOUTS
    log4g/layout/html-layout.h
    log4g/layout/pattern-layout.h
    log4g/layout/simple-layout.h
    log4g/layout/ttcc-layout.h
    log4g/layout/xml-layout.h
)
# install header files
install(DIRECTORY log4g
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
# project source files
add_library(log4g SHARED
    appender.c
    appender-attachable.c
    appender-attachable-impl.c
    appender-skeleton.c
    async-appender.c
    basic-configurator.c
    configurator.c
    console-appender.c
    counting-quiet-writer.c
    date-layout.c
    default-logger-factory.c
    default-repository-selector.c
    deny-all-filter.c
    dom-configurator.c
    error.c
    error-handler.c
    file-appender.c
    filter.c
    hierarchy.c
    html-layout.c
    layout.c
    level.c
    level-match-filter.c
    level-range-filter.c
    log-manager.c
    log.c
    logger.c
    logger-factory.c
    logger-repository.c
    logging-event.c
    mdc.c
    ndc.c
    null-appender.c
    only-once-error-handler.c
    option-handler.c
    pattern-converter.c
    pattern-layout.c
    pattern-parser.c
    provision-node.c
    quiet-writer.c
    repository-selector.c
    rolling-file-appender.c
    root-logger.c
    simple-layout.c
    string-match-filter.c
    support/marshal.c
    support/marshal.h
    syslog-appender.c
    thread.c
    ttcc-layout.c
    writer-appender.c
    xml-layout.c
    ${APPENDERS}
    ${BASE}
    ${FILTERS}
    ${HELPERS}
    ${INTERFACESS}
    ${LAYOUTS}
)
# linked libraries
target_link_libraries(log4g
    ${GObject-2.0_LIBRARIES}
    ${LibXML-2.0_LIBRARIES}
)
# install library
install(TARGETS log4g LIBRARY
    DESTINATION lib
)
