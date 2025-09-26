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

# Include directories (matching current Makefile structure)
set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/interfaces
    ${CMAKE_SOURCE_DIR}/include/models
    ${CMAKE_SOURCE_DIR}/include/implementations/services
    ${CMAKE_SOURCE_DIR}/include/implementations/state_machines
    ${CMAKE_SOURCE_DIR}/include/implementations/transitions
    ${CMAKE_SOURCE_DIR}/include/implementations/handlers
    ${CMAKE_SOURCE_DIR}/include/controllers
    ${CMAKE_SOURCE_DIR}/include/utils
    ${CMAKE_SOURCE_DIR}/include/factories
)

# Source files (matching current Makefile subdirectories)
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/models/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/implementations/services/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/implementations/handlers/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/controllers/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/utils/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/factories/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
)

# Create the executable
add_executable(traffic_light_simulator ${SOURCES})

# Set target properties
target_include_directories(traffic_light_simulator PRIVATE ${INCLUDE_DIRS})
target_link_libraries(traffic_light_simulator PRIVATE Threads::Threads)

# Set output directory (matching current Makefile)
set_target_properties(traffic_light_simulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Custom targets for convenience (matching current Makefile targets)
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/bin/traffic_light_simulator
    DEPENDS traffic_light_simulator
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running Traffic Light Simulator"
)

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
message(STATUS "=== Traffic Light Simulator Build Configuration ===")
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
target_link_libraries(traffic_light_simulator PRIVATE state_machine_lib)

# Test executable
add_executable(test_lib test_lib.cpp)
target_link_libraries(test_lib PRIVATE state_machine_lib)

add_subdirectory(examples/traffic_light)
add_subdirectory(examples/elevator)
