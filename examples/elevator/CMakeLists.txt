# Elevator Example
add_executable(elevator_example
    src/models/elevator_states.cpp
    src/models/elevator_events.cpp
    src/controllers/elevator_controller.cpp
    src/handlers/elevator_action_handler.cpp
    src/factories/elevator_factory.cpp
    src/utils/elevator_enum_utils.cpp
    src/services/elevator_console_display_service.cpp
    example_elevator.cpp
)

target_include_directories(elevator_example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/models
    ${CMAKE_CURRENT_SOURCE_DIR}/include/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/handlers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/services
    ${CMAKE_CURRENT_SOURCE_DIR}/include/factories
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
)

# Linkuj bibliotekę (to automatycznie dodaje include paths z biblioteki)
target_link_libraries(elevator_example PRIVATE 
    state_machine_lib    # To już zawiera lib/include w swoich INTERFACE_INCLUDE_DIRECTORIES
    Threads::Threads
)
