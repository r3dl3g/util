cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project("util-test" CXX)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/testing testing)

include_directories(${PROJECT_BINARY_DIR} ${TESTING_INCLUDE_DIRS})

enable_testing()

set(tests
    csv_test
    fs_test
)

add_definitions(${UTIL_CXX_FLAGS})

foreach(test ${tests})
    add_executable(${test} WIN32 ${test}.cpp)
    target_link_libraries(${test} testing ${UTIL_LIBRARIES} ${UTIL_SYS_LIBRARIES})
    add_test(${test} ${CMAKE_CURRENT_BINARY_DIR}/${test})
    set_target_properties(${test} PROPERTIES
                          FOLDER tests
                          CXX_STANDARD ${UTIL_CXX_STANDARD})
endforeach(test)
