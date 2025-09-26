# Traffic Light Example
add_executable(traffic_light_example
    src/models/traffic_states.cpp
    src/models/traffic_events.cpp
    src/controllers/traffic_light_controller.cpp
    src/handlers/traffic_light_action_handler.cpp
    src/services/ascii_display_service.cpp
    src/services/console_display_service.cpp
    src/factories/traffic_light_factory.cpp
    src/utils/traffic_enum_utils.cpp
    example_traffic.cpp
)

target_include_directories(traffic_light_example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/models
    ${CMAKE_CURRENT_SOURCE_DIR}/include/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/handlers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/services
    ${CMAKE_CURRENT_SOURCE_DIR}/include/factories
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
)

target_link_libraries(traffic_light_example PRIVATE 
    state_machine_lib
    Threads::Threads
)
