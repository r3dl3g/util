# C++ utility library

A c++ utility library.

## Motivation

Missing functions in std library and often used patterns.

## Usage

### In cmake

In your CMakeLists.txt include this project directory and add the library util to your target:

```cmake

add_subdirectory(util)

add_definitions(${UTILG_CXX_FLAGS})

add_executable(MyApp MyApp.cpp)
target_link_libraries(MyApp ${UTILG_LIBRARIES} ${UTILG_SYS_LIBRARIES})
set_target_properties(MyApp PROPERTIES CXX_STANDARD ${UTILG_CXX_STANDARD})

```
