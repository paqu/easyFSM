# State Machine Library
cmake_minimum_required(VERSION 3.16)

add_library(state_machine_lib INTERFACE)

target_include_directories(state_machine_lib
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(state_machine_lib INTERFACE cxx_std_14)

file(GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
if(LIB_SOURCES)
    # Convert to static library if we have sources
    add_library(state_machine_lib_impl STATIC ${LIB_SOURCES})
    target_include_directories(state_machine_lib_impl PUBLIC include)
    target_compile_features(state_machine_lib_impl PUBLIC cxx_std_14)
    
    # Link the implementation to interface
    target_link_libraries(state_machine_lib INTERFACE state_machine_lib_impl)
endif()
