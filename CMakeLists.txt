cmake_minimum_required(VERSION 3.16)
project(TrafficLightSimulator 
    VERSION 1.0.0
    DESCRIPTION "Traffic Light Simulator with Generic State Machine Framework"
    LANGUAGES CXX
)

# Set C++14 (matching current Makefile)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(Threads REQUIRED)

# Compiler-specific options (matching current Makefile)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall 
        -Wextra 
        -pthread 
        -g 
        -O2
    )
    add_link_options(-pthread)
endif()

# Build options
option(BUILD_TESTS "Build unit tests" OFF)
option(ENABLE_ASAN "Enable AddressSanitizer for debugging" OFF)
option(BUILD_DEBUG "Build debug version" OFF)

if(BUILD_DEBUG)
    add_compile_options(-DDEBUG -g3 -O0)
endif()

if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()






add_custom_target(rebuild
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "Clean and rebuild"
)

# Debug target (equivalent to make debug)
add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DBUILD_DEBUG=ON ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "Build debug version"
)

# Print build information
message(STATUS "")
message(STATUS "=== State Machine Framework Build Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Enable ASAN: ${ENABLE_ASAN}")
message(STATUS "Debug build: ${BUILD_DEBUG}")
message(STATUS "====================================================")
message(STATUS "")

# Optional: Add tests if requested
if(BUILD_TESTS)
    enable_testing()
    # Tests will be added later
    message(STATUS "Tests enabled but not implemented yet")
endif()

add_subdirectory(lib)

# Test executable
add_executable(test_lib test_lib.cpp)
target_link_libraries(test_lib PRIVATE state_machine_lib)

add_subdirectory(examples/traffic_light)
add_subdirectory(examples/elevator)
add_subdirectory(examples/traffic_light_threaded)
