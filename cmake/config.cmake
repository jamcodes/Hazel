include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/options.cmake)

macro(SetGlobalFlags)
    set(setglobalflags_options)
    set(setglobalflags_args DEFAULT_BUILD_TYPE)
    set(setglobalflags_list_args)
    cmake_parse_arguments(setglobalflags
        "${setglobalflags_options}"
        "${setglobalflags_args}"
        "${setglobalflags_list_args}"
        ${ARGN}
    )
    foreach(unparsed_arg IN LISTS setglobalflags_UNPARSED_ARGUMENTS)
        message(WARNING "${ColorYellow}Unparsed argument: ${unparsed_arg}${ColorReset}")
    endforeach()
    if(NOT setglobalflags_DEFAULT_BUILD_TYPE)
        message(FATAL_ERROR "${ColorRed}SetGlobalFlags: DEFAULT_BUILD_TYPE argument required${ColorReset}")
    endif()

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)
    set(CMAKE_CXX_STANDARD_REQUIRED     ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Visibility controll for non-Windows systems
    set(CMAKE_CXX_VISIBILITY_PRESET     hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
    # Sane RUNPATH/RPATH for Linux
    set(CMAKE_INSTALL_RPATH $ORIGIN)

    # Default build type if not specified explicitly at configure time
    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        message(STATUS "${ColorYellow}Setting build type to '${setglobalflags_DEFAULT_BUILD_TYPE}' as none was specified.${ColorReset}")
        set(CMAKE_BUILD_TYPE ${setglobalflags_DEFAULT_BUILD_TYPE} CACHE STRING "Choose the type of build." FORCE)
    endif()

    # TODO: Reliably detect target architecture for other paltforms and compilers
    # if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_VS_MSBUILD_COMMAND)
    #     set(_setglobalflags_arch "-${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}")
    # endif()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY                ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin/Debug-${CMAKE_SYSTEM_NAME})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin/Release-${CMAKE_SYSTEM_NAME})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin/Dist-${CMAKE_SYSTEM_NAME})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY                ${CMAKE_SOURCE_DIR}/bin-int/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin-int/Debug-${CMAKE_SYSTEM_NAME})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin-int/Release-${CMAKE_SYSTEM_NAME})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin-int/Dist-${CMAKE_SYSTEM_NAME})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY                ${CMAKE_SOURCE_DIR}/bin-int/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin-int/Debug-${CMAKE_SYSTEM_NAME})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin-int/Release-${CMAKE_SYSTEM_NAME})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin-int/Dist-${CMAKE_SYSTEM_NAME})
endmacro()

function(DeclareCompilerConfigTarget target)
    add_library(${target} INTERFACE)
    target_compile_features(${target}
        INTERFACE
            cxx_std_17
    )
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${target} INTERFACE
            /W4
            /permissive-
            $<$<BOOL:${${opt_werror}}>:/WX>
        )
    else()
        target_compile_options(${target} INTERFACE
            -Wall
            -Wdouble-promotion
            -Weffc++
            -Wextra
            -Wformat=2
            -Wnon-virtual-dtor
            -Wnull-dereference
            -Wold-style-cast
            -Woverloaded-virtual
            -Wpedantic
            -Wshadow
            -Wsign-conversion
            -Wunused
            -Wcast-align
            -pedantic-errors

            -Wimplicit-fallthrough
            -Wsuggest-attribute=pure
            -Wsuggest-attribute=const
            -Wsuggest-attribute=cold
            -Wsuggest-final-types
            -Wsuggest-final-methods
            -Wduplicated-branches
            -Wduplicated-cond

            $<$<BOOL:${${opt_werror}}>:-Werror>
            $<$<CONFIG:Debug>:-Og>
            $<$<CONFIG:Debug>:-ggdb3>
            -fuse-ld=gold
        )
        target_link_options(${target} INTERFACE
            -fuse-ld=gold
            $<$<BOOL:${${opt_coverage}}>:--coverage>
            $<$<BOOL:${${opt_coverage}}>:-ftest-coverage>
            $<$<BOOL:${${opt_coverage}}>:-fprofile-arcs>
            $<$<BOOL:${${opt_sanitize_address}}>:-fno-omit-frame-pointer>
            $<$<BOOL:${${opt_sanitize_address}}>:-fsanitize=address>
            $<$<BOOL:${${opt_sanitize_leak}}>:-fsanitize=leak>
            $<$<BOOL:${${opt_sanitize_thread}}>:-fsanitize=thread>
            $<$<BOOL:${${opt_sanitize_ub}}>:-fsanitize=undefined>
        )
    endif()
endfunction()
