# Traffic Light Example
add_executable(traffic_light_observer_example
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/models/traffic_states.cpp
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/models/traffic_events.cpp
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/utils/traffic_enum_utils.cpp
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/services/console_display_service.cpp
    src/observers/console_logger_observer.cpp
    src/observers/pedestrian_observer.cpp
    src/observers/timer_observer.cpp
    src/observers/display_observer.cpp
    src/observers/file_logger_observer.cpp

    example_observer.cpp
)

target_include_directories(traffic_light_observer_example PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/controllers/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/observers/
    
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include     
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include/models
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include/utils
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include/services
)

target_link_libraries(traffic_light_observer_example PRIVATE 
    state_machine_lib
    Threads::Threads
)

