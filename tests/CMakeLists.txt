#                                          -*- Cmake -*-
# Process this file with cmake to produce build scripts.

# find necessary packages
find_package(GLib-2.0 REQUIRED)
find_package(GThread-2.0)
# include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}
    ${GLib-2.0_INCLUDE_DIRS}
    ${GThread-2.0_INCLUDE_DIRS}
)
# compiler switches
add_definitions(-std=gnu99 -Wall -Werror)
# define tests
set(TESTS
    appender-attachable-impl-test
    async-appender-test
    basic-configurator-test
    console-appender-test
    deny-all-filter-test
    dom-configurator-test
    file-appender-test
    hierarchy-test
    level-test
    log4g-test
    logger-test
    mdc-test
    ndc-test
    only-once-error-handler-test
    pattern-layout-test
    provision-node-test
    quiet-writer-test
    rolling-file-appender-test
    syslog-appender-test
    writer-appender-test
)
# add executables
foreach(test ${TESTS})
    add_executable(${test}
        EXCLUDE_FROM_ALL
        ${test}.c
    )
    target_link_libraries(${test}
        ${GLib-2.0_LIBRARIES}
        ${GThread-2.0_LIBRARIES}
        log4g
    )
endforeach(test)
if(GLib-2.0_GTESTER)
    # add `make check' target
    add_custom_target(check
        COMMAND ${GLib-2.0_GTESTER} ${TESTS}
        DEPENDS ${TESTS}
    )
    if(GLib-2.0_GTESTER_REPORT)
        # add `make report' target
        add_custom_command(OUTPUT ${PROJECT_NAME}.xml
            COMMAND ${GLib-2.0_GTESTER} -k -o ${PROJECT_NAME}.xml ${TESTS}
            DEPENDS ${TESTS}
        )
        add_custom_command(OUTPUT ${PROJECT_NAME}.html
            COMMAND ${GLib-2.0_GTESTER_REPORT} ${PROJECT_NAME}.xml > ${PROJECT_NAME}.html
            DEPENDS ${PROJECT_NAME}.xml
        )
        add_custom_target(report
            DEPENDS ${PROJECT_NAME}.html
        )
    endif(GLib-2.0_GTESTER_REPORT)
endif(GLib-2.0_GTESTER)
# add `make memcheck' target
find_program(VALGRIND valgrind)
if(VALGRIND)
    add_custom_target(memcheck
        COMMAND G_SLICE=always-malloc G_DEBUG=gc-friendly
            ${VALGRIND} --leak-check=full ./log4g-test
        DEPENDS ${TESTS}
    )
endif(VALGRIND)
