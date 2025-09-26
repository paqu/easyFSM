# Traffic Light Threaded Example
add_executable(traffic_light_threaded
    # Link traffic light implementation files
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/controllers/traffic_light_controller.cpp
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/handlers/traffic_light_action_handler.cpp
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/factories/traffic_light_factory.cpp
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/services/ascii_display_service.cpp
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/services/console_display_service.cpp
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/models/traffic_states.cpp
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/models/traffic_events.cpp
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/src/utils/traffic_enum_utils.cpp
    src/utils/traffic_executor.cpp
    example_traffic_threaded.cpp
)

    
target_include_directories(traffic_light_threaded PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include     
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include/models
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include/controllers
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include/handlers
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include/services
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include/factories
    ${CMAKE_SOURCE_DIR}/examples/traffic_light/include/utils
)

target_link_libraries(traffic_light_threaded PRIVATE 
    state_machine_lib
    Threads::Threads
)

