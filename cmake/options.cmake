include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)

macro(SetOptionNames prefix)
    set(opt_werror           ${prefix}_WERROR)
    set(opt_tests            ${prefix}_TESTS)
    set(opt_coverage         ${prefix}_COVERAGE)
    set(opt_sanitize_address ${prefix}_SANITIZE_ADDRESS)
    set(opt_sanitize_leak    ${prefix}_SANITIZE_LEAK)
    set(opt_sanitize_thread  ${prefix}_SANITIZE_THREAD)
    set(opt_sanitize_ub      ${prefix}_SANITIZE_UDEFINED_BEHAVIOR)
    message(STATUS "${ColorYellow}${opt_werror}          ${ColorReset}")
    message(STATUS "${ColorYellow}${opt_tests}           ${ColorReset}")
    message(STATUS "${ColorYellow}${opt_coverage}        ${ColorReset}")
    message(STATUS "${ColorYellow}${opt_sanitize_address}${ColorReset}")
    message(STATUS "${ColorYellow}${opt_sanitize_leak}   ${ColorReset}")
    message(STATUS "${ColorYellow}${opt_sanitize_thread} ${ColorReset}")
    message(STATUS "${ColorYellow}${opt_sanitize_ub}     ${ColorReset}")
endmacro()

#[[
SetOptions - adds options for controlling the build process.
    Options names are prefixed with the given PREFIX argument.
    Given a PREFIX = "EXAMPLE", the following options are added:

    EXAMPLE_WERROR
    EXAMPLE_TESTS
    EXAMPLE_COVERAGE
    EXAMPLE_SANITIZE_ADDRESS
    EXAMPLE_SANITIZE_LEAK
    EXAMPLE_SANITIZE_THREAD
    EXAMPLE_SANITIZE_UNDEFINED_BEHAVIOR

    All of the options are set to OFF by default.
]]
function(SetOptions)
    set(options)
    set(args PREFIX)
    set(list_args)
    cmake_parse_arguments(arg "${options}" "${args}" "${list_args}" ${ARGN})

    foreach(unparsed_arg IN LISTS arg_UNPARSED_ARGUMENTS)
        message(WARNING "${ColorYellow}Unparsed argument: ${unparsed_arg}${ColorReset}")
    endforeach()

    if(NOT arg_PREFIX)
        message(FATAL_ERROR "${ColorRed}SetOptions: PREFIX argument required${ColorReset}")
    endif()

    SetOptionNames(${PREFIX})
    option(${opt_werror} "Enable warning as errors for ${CMAKE_PROJECT_NAME}" OFF)
    option(${opt_tests} "Build ${CMAKE_PROJECT_NAME} tests" OFF)
    option(${opt_coverage} "Generate ${CMAKE_PROJECT_NAME} code coverage" OFF)
    option(${opt_sanitize_address} "Enable address-sanitizer for ${CMAKE_PROJECT_NAME}" OFF)
    option(${opt_sanitize_leak} "Enable leak-sanitizer for ${CMAKE_PROJECT_NAME}" OFF)
    option(${opt_sanitize_thread} "Enable thread-sanitizer for ${CMAKE_PROJECT_NAME}" OFF)
    option(${opt_sanitize_ub} "Enable undefined-behavior-sanitizer for ${CMAKE_PROJECT_NAME}" OFF)

    if (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
        if (${${opt_coverage}})
            message(FATAL_ERROR
                "${ColorBoldRed}${${opt_coverage}}=ON valid only for CMAKE_BUILD_TYPE=Debug${ColorReset}"
            )
        endif()
        if (${opt_sanitize_address} OR ${opt_sanitize_leak} OR ${opt_sanitize_thread} OR ${opt_sanitize_ub})
            message(FATAL_ERROR
                "${ColorBoldRed}Sanitizers can be used only with CMAKE_BUILD_TYPE=Debug${ColorReset}"
            )
        endif()
    endif()
endfunction()
