cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("util" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)

set(UTIL_BUILD_STATIC_MODULE_LIB ON CACHE BOOL "On to build a static library for this module, Off for shared library. default On")

if(NOT TARGET util-obj)

    set (util_include
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_BINARY_DIR}
    )

    if (APPLE)
      set (util_include ${util_include} /usr/local/include)
    endif (APPLE)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set (util_sys_libraries stdc++fs pthread)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (util_sys_libraries stdc++fs pthread)
    endif()

    if (UTIL_BUILD_STATIC_MODULE_LIB)
        set (util_libraries util-static)
        set (util_defines "-DUTIL_BUILT_AS_STATIC_LIB")
    else()
        set (util_libraries util)
    endif()

    set (util_sys_libraries ${util_sys_libraries} PARENT_SCOPE)
    set (util_include ${util_include} PARENT_SCOPE)
    set (util_libraries ${util_libraries} PARENT_SCOPE)
    set (util_defines ${util_defines} PARENT_SCOPE)

    message(STATUS "util_defines: ${util_defines}")
    message(STATUS "util_libraries: ${util_libraries}")
    message(STATUS "util_sys_libraries: ${util_sys_libraries}")

    set (UTIL_VERSION_MAJOR 1)
    set (UTIL_VERSION_MINOR 0)
    set (UTIL_VERSION_PATCH 0)
    set (UTIL_VERSION "${UTIL_VERSION_MAJOR}.${UTIL_VERSION_MINOR}.${UTIL_VERSION_PATCH}")

    set (UTIL_INCLUDE_INSTALL_DIR include/guipp/util)
    set (UTIL_LIBRARY_INSTALL_DIR lib/guipp)

    include_directories(${util_include})

    # header file to pass version settings to the source code
    configure_file (
        "${PROJECT_SOURCE_DIR}/version.h.in"
        "${PROJECT_BINARY_DIR}/util_version.h"
    )

    file(GLOB SOURCE_FILES "*.h" "*.cpp" "*.inl")
    file(GLOB INCLUDE_FILES "*.h" "*.inl")

    if (WIN32)
      add_definitions(-DWIN32_LEAN_AND_MEAN)
    endif (WIN32)

    add_definitions(-DEXPERIMENTAL -Dutil_obj_EXPORTS ${util_defines})

    add_library(util-obj OBJECT ${SOURCE_FILES})

    set_target_properties(util-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if(UTIL_BUILD_STATIC_MODULE_LIB)
        add_library(util-static STATIC $<TARGET_OBJECTS:util-obj>)
        install(TARGETS util-static DESTINATION ${UTIL_LIBRARY_INSTALL_DIR})
    else()
        add_library(util SHARED $<TARGET_OBJECTS:util-obj>)

        target_link_libraries(util PRIVATE ${util_sys_libraries})

        set_target_properties(util PROPERTIES
                                     POSITION_INDEPENDENT_CODE ON
                                     VERSION ${UTIL_VERSION}
                                     SOVERSION ${UTIL_VERSION_MAJOR})

        install(TARGETS util DESTINATION ${UTIL_LIBRARY_INSTALL_DIR})
    endif()

    GENERATE_EXPORT_HEADER(util-obj
        BASE_NAME UTIL
        EXPORT_MACRO_NAME UTIL_EXPORT
        EXPORT_FILE_NAME util-export.h
        STATIC_DEFINE UTIL_BUILT_AS_STATIC_LIB
    )
    install(FILES ${INCLUDE_FILES}
                  ${PROJECT_BINARY_DIR}/util-export.h
                  ${PROJECT_BINARY_DIR}/util_version.h
            DESTINATION ${UTIL_INCLUDE_INSTALL_DIR})

endif()
