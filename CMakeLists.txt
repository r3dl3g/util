cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("util" CXX)

# see: http://stackoverflow.com/questions/23684789/cmake-build-multiple-executables-in-one-project-with-static-library

include (GenerateExportHeader)
include (CMakePackageConfigHelpers)
include (GNUInstallDirs)

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

option(UTIL_BUILD_STATIC_MODULE_LIB "On to build a static library for this module, Off for shared library. default On" ON)
option(UTIL_CONFIG_INSTALL "On to make an installable standalone build, Off to build as part of a project. Default Off" OFF)
option(UTIL_CXX_STANDARD "C++ standard to overwrite default cmake standard" ${CMAKE_CXX_STANDARD})

function(DebugPrint MSG)
  if(VERBOSE)
    message(STATUS "${MSG}")
  endif()
endfunction(DebugPrint)

if(NOT TARGET util)

  set (UTIL_VERSION_MAJOR 1)
  set (UTIL_VERSION_MINOR 0)
  set (UTIL_VERSION_PATCH 3)
  set (UTIL_VERSION "${UTIL_VERSION_MAJOR}.${UTIL_VERSION_MINOR}.${UTIL_VERSION_PATCH}")

  set (UTIL_LIBRARIES util::util)

  if (UTIL_CXX_STANDARD STREQUAL "OFF")
    set (UTIL_CXX_STANDARD 17)
  endif ()

  set (UTIL_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${PROJECT_BINARY_DIR}
    )

  if (GUIPP_USE_BOOST)
    find_package(Boost 1.65 REQUIRED COMPONENTS filesystem)
    set (UTIL_CXX_FLAGS "-DUSE_BOOST")
    set (UTIL_SYS_LIBRARIES ${UTIL_SYS_LIBRARIES} Boost::filesystem)
  endif()

  if(APPLE)
    set (UTIL_SYS_LIBRARIES ${UTIL_SYS_LIBRARIES} pthread)
    set (UTIL_INCLUDE_DIRS ${UTIL_INCLUDE_DIRS} /usr/local/include)
    if (CMAKE_CXX_COMPILER_VERSION GREATER 10)
      set (UTIL_CXX_FLAGS "-DUSE_FILE_TIME_POINT")
    endif ()
  elseif (ANDROID)
    set(UTIL_CXX_FLAGS ${UTIL_CXX_FLAGS} -DANDROID)
    set (UTIL_INCLUDE_DIRS ${UTIL_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../filesystem/include)

  elseif ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
          ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") AND NOT (CMAKE_CXX_PLATFORM_ID STREQUAL "Windows")))
    set (UTIL_SYS_LIBRARIES ${UTIL_SYS_LIBRARIES} stdc++fs pthread)
  endif()

  if (WIN32)
    set(UTIL_CXX_FLAGS ${UTIL_CXX_FLAGS} -DWIN32_LEAN_AND_MEAN)
    if ((CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") AND (MSVC_VERSION GREATER 1910))
      set(UTIL_CXX_FLAGS ${UTIL_CXX_FLAGS} -DUSE_FILE_TIME_POINT)
    endif ()
  endif ()

  if (UTIL_BUILD_STATIC_MODULE_LIB)
    set (UTIL_CXX_FLAGS ${UTIL_CXX_FLAGS} "-DUTIL_BUILT_AS_STATIC_LIB")
    set (UTIL_LINK STATIC)
  else ()
    set (UTIL_LINK SHARED)
  endif()

  if(CMAKE_CXX_PLATFORM_ID STREQUAL "MinGW")
    set (UTIL_CXX_FLAGS ${UTIL_CXX_FLAGS} "-DUSE_MINGW")
    set (UTIL_SYS_LIBRARIES ${UTIL_SYS_LIBRARIES} stdc++fs pthread)
  endif()

  get_directory_property(hasParent PARENT_DIRECTORY)
  if (hasParent)
    set (UTIL_SYS_LIBRARIES ${UTIL_SYS_LIBRARIES} PARENT_SCOPE)
    set (UTIL_INCLUDE_DIRS ${UTIL_INCLUDE_DIRS} PARENT_SCOPE)
    set (UTIL_LIBRARIES ${UTIL_LIBRARIES} PARENT_SCOPE)
    set (UTIL_CXX_FLAGS ${UTIL_CXX_FLAGS} PARENT_SCOPE)
    set (UTIL_CXX_STANDARD ${UTIL_CXX_STANDARD} PARENT_SCOPE)
  else()
    set (UTIL_CONFIG_INSTALL On)
  endif()

  DebugPrint("UTIL_CXX_FLAGS: ${UTIL_CXX_FLAGS}")
  DebugPrint("UTIL_LIBRARIES: ${UTIL_LIBRARIES}")
  DebugPrint("UTIL_SYS_LIBRARIES: ${UTIL_SYS_LIBRARIES}")
  DebugPrint("UTIL_CXX_STANDARD: ${UTIL_CXX_STANDARD}")

  include_directories(${UTIL_INCLUDE_DIRS})

  # header file to pass version settings to the source code
  configure_file (
    "${PROJECT_SOURCE_DIR}/version.h.in"
    "${PROJECT_BINARY_DIR}/util_version.h"
  )

  set(SOURCE_FILES command_line.cpp
    csv_reader.cpp
    string_util.cpp
    time_util.cpp
  )
  set(INCLUDE_FILES bind_method.h
    blocking_queue.h
    command_line.h
    csv_reader.h
    ostreamfmt.h
    ostream_resetter.h
    robbery.h
    sort_order.h
    string_util.h
    sys_fs.h
    time_util.h
    tuple_util.h
    variadic_util.h
    vector_util.h
  )

  if (NOT ANDROID)
    set(CMAKE_DEBUG_POSTFIX d)
  endif ()

  add_definitions(-Dutil_EXPORTS ${UTIL_CXX_FLAGS})

  add_library(util ${UTIL_LINK} ${SOURCE_FILES} ${INCLUDE_FILES})
  add_library(util::util ALIAS util)

  set_target_properties(util PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    FOLDER libraries
    VERSION ${UTIL_VERSION}
    SOVERSION ${UTIL_VERSION_MAJOR}
    CXX_STANDARD ${UTIL_CXX_STANDARD}
  )

  if(UTIL_BUILD_STATIC_MODULE_LIB)
    DebugPrint("UTIL: Create static Library")
  else()
    DebugPrint("UTIL: Create shared Library")

    target_link_libraries(util PRIVATE ${UTIL_SYS_LIBRARIES})
  endif()

  GENERATE_EXPORT_HEADER(util
    BASE_NAME UTIL
    EXPORT_MACRO_NAME UTIL_EXPORT
    EXPORT_FILE_NAME util-export.h
    STATIC_DEFINE UTIL_BUILT_AS_STATIC_LIB
  )

  if (UTIL_CONFIG_INSTALL)
    set (INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/util)
    set (LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/util)

    target_include_directories(util
                               PUBLIC
                               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                               "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>"
    )

    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/utilConfigVersion.cmake
                                     VERSION ${UTIL_VERSION}
                                     COMPATIBILITY SameMajorVersion)

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
                                  "${CMAKE_CURRENT_BINARY_DIR}/utilConfig.cmake"
                                  INSTALL_DESTINATION ${LIBRARY_INSTALL_DIR}
                                  PATH_VARS CMAKE_INSTALL_INCLUDEDIR)

    install(TARGETS util
            EXPORT utilTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    install(FILES ${INCLUDE_FILES}
            ${PROJECT_BINARY_DIR}/util-export.h
            ${PROJECT_BINARY_DIR}/util_version.h
            DESTINATION ${INCLUDE_INSTALL_DIR}
    )

    install(EXPORT utilTargets
            FILE utilTargets.cmake
            NAMESPACE util::
            DESTINATION ${LIBRARY_INSTALL_DIR}
    )

    export(TARGETS util FILE utilTargets.cmake)

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/utilConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/utilConfigVersion.cmake"
            DESTINATION ${LIBRARY_INSTALL_DIR})

  endif()

endif()
