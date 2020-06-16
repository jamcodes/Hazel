cmake_minimum_required(VERSION 3.15)

project(HazelEngine)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(options)
include(config)
include(release)

SetGlobalFlags(DEFAULT_BUILD_TYPE Debug)
DeclareCompilerConfigTarget(HzBuildFlags)
add_library(Hazel::BuildFlags ALIAS HzBuildFlags)

option(HZ_ENABLE_INSTRUMENTATION "Enable Hazel profiling and instrumentation" OFF)

set(validContractLevels OFF ASSUME IGNORED ENFORCE AUDIT)
if (NOT HZ_CONTRACT_LEVEL IN_LIST validContractLevels)
    message(STATUS "HZ_CONTRACT_LEVEL unspecified - set to ENFORCE; specify one of ${validContractLevels}")
    set(HZ_CONTRACT_LEVEL "ENFORCE" CACHE STRING "Specify Hazel contract enforcement level")
endif()
if (NOT HZ_CONTRACT_LEVEL IN_LIST validContractLevels)
    message(FATAL_ERROR "Invalid HZ_CONTRACT_LEVEL specified valid values = ${validContractLevels}")
endif()

target_compile_definitions(HzBuildFlags
    INTERFACE
        $<$<BOOL:${HZ_ENABLE_INSTRUMENTATION}>:HZ_ENABLE_INSTRUMENTATION=1>
        # Hardcode enabled contracts for now, in the future - configure based on the build type
        "HZ_CONTRACT_LEVEL=${HZ_CONTRACT_LEVEL}"
        HZ_CONTRACT_USE_DEBUGTRAP_HANDLER=1
)


set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<BOOL:${BUILD_SHARED_LIBS}>:DLL>")

add_subdirectory(Hazel)
add_subdirectory(Hazelnut)
add_subdirectory(Sandbox)

set(VS_STARTUP_PROJECT Sandbox)
