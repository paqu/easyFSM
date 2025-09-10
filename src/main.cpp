#include <stdint.h>

#include <iostream>

#include <pthread.h>
#include <unistd.h>

#include "console_display_service.h"
#include "function_timer_service.h"
#include "runtime_state_machine.h"
#include "simple_state_transition.h"
#include "traffic_light_transition.h"

#include "traffic_light_action_handler.h"
#include "traffic_light_controller.h"

char shared_char;
bool char_available = false;
bool timeout_expired = false;
bool timeout_set = false;
uint32_t shared_timeout_sec = 0;

pthread_mutex_t worker_mutex;
pthread_mutex_t timeout_mutex;
pthread_cond_t worker_convar;
pthread_cond_t start_timeout_convar;

void demo();
void simulation();
void start_timeout(uint32_t duration_sec);
void *read_char(void *arg);
void *worker(void *arg);
void *timeout_generator(void *arg);

std::shared_ptr<RuntimeStateMachine> state_machine;
std::unique_ptr<TrafficLightController> controller;

int main() {
    std::cout << "Choose mode:\n";
    std::cout << "1. Demo\n";
    std::cout << "2. Simulation\n";
    std::cout << "Enter choice (1 or 2): ";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        demo();
    } else {
        simulation();
    }

    return 0;
}
void demo() {
    std::cout << "Demo started!\n";
    // End program
}

void simulation() {
    // Original code
    state_machine =
        std::make_shared<RuntimeStateMachine>(TrafficState::CAR_RED);

    auto traffic_handler = std::make_unique<TrafficLightActionHandler>(
        std::make_unique<ConsoleDisplayService>(),
        std::make_unique<FunctionTimerService>(start_timeout));

    auto handler_raw = traffic_handler.get();

    controller = std::make_unique<TrafficLightController>(
        state_machine, std::move(traffic_handler));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_GREEN, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_YELLOW));

    state_machine->add_transition(std::make_unique<TrafficLightTransition>(
        TrafficState::CAR_YELLOW, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_RED, TrafficState::WALK_PREP,
        [handler_raw]() -> bool {
            return handler_raw->has_pedestrian_request();
        }));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_RED, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_RED_YELLOW));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK_PREP, SystemEvent::TIME_EXPIRED,
        TrafficState::WALK));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK, SystemEvent::TIME_EXPIRED,
        TrafficState::WALK_FINISH));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK_FINISH, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_RED_YELLOW));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_RED_YELLOW, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_GREEN));

    pthread_t reader_thread, worker_thread, timeout_thread;
    pthread_mutex_init(&worker_mutex, nullptr);
    pthread_mutex_init(&timeout_mutex, nullptr);
    pthread_cond_init(&worker_convar, nullptr);
    pthread_cond_init(&start_timeout_convar, nullptr);

    pthread_create(&reader_thread, nullptr, read_char, nullptr);
    pthread_create(&worker_thread, nullptr, worker, nullptr);
    pthread_create(&timeout_thread, nullptr, timeout_generator, nullptr);

    pthread_join(reader_thread, nullptr);
    pthread_join(worker_thread, nullptr);
    pthread_join(timeout_thread, nullptr);

    pthread_mutex_destroy(&worker_mutex);
    pthread_mutex_destroy(&timeout_mutex);
    pthread_cond_destroy(&worker_convar);
    pthread_cond_destroy(&start_timeout_convar);
}

void process_traffic_light(bool timeout_expired, bool button_pressed);

/**
 * @brief Reading console
 * Read the char from console and store the char in the @ref shared_char
 * variable and then signal the conditional variable @ref worker_convar.
 * In case the pressed char is a 'q' the thread will end.
 *
 * @param arg not used
 * @return void* not used
 */
void *read_char(void *arg) {
    while (true) {
        char input_char;
        std::cin >> input_char;
        pthread_mutex_lock(&worker_mutex);
        char_available = true;
        shared_char = input_char;
        pthread_cond_signal(&worker_convar);
        pthread_mutex_unlock(&worker_mutex);

        if ('q' == input_char) {
            break;
        }
    }
    return nullptr;
}

/**
 * @brief Function used to start generation of the timeout
 * This function will store the required timeout value in @ref
 * shared_timeout_sec varaible and signal the @ref start_timeout_convar
 * conditional variable to start the @ref timeout_generator thread. This thread
 * will then signal the @ref worker_convar conditional variable after the
 * timeout expired.
 *
 * @param timeout_sec the timeout value in seconds
 */
void start_timeout(uint32_t timeout_sec) {
    pthread_mutex_lock(&timeout_mutex);
    shared_timeout_sec = timeout_sec;
    timeout_set = true;
    pthread_cond_signal(&start_timeout_convar);
    pthread_mutex_unlock(&timeout_mutex);
}

/**
 * @brief This thread is used to generate timeouts.
 * When the @ref start_timeout() function has been called. This thread will
 * signal the @ref worker_convar conditional variable after given number of
 * seconds.
 * @param arg not used
 * @return void* not used
 */
void *timeout_generator(void *arg) {
    while (true) {
        uint32_t local_timeout_sec = 0;
        pthread_mutex_lock(&timeout_mutex);
        while (!timeout_set) {
            pthread_cond_wait(&start_timeout_convar, &timeout_mutex);
        }
        local_timeout_sec = shared_timeout_sec;
        shared_timeout_sec = 0;
        timeout_expired = false;
        timeout_set = false;
        pthread_mutex_unlock(&timeout_mutex);
        std::cout << "Start timeout " << local_timeout_sec << std::endl;
        sleep(local_timeout_sec); // Wait for the timeout duration
        std::cout << "Timeout " << local_timeout_sec << " seconds" << std::endl;
        pthread_mutex_lock(&worker_mutex);
        timeout_expired = true;
        pthread_cond_signal(&worker_convar);
        pthread_mutex_unlock(&worker_mutex);

        // If 'q' is entered, stop the timeout thread as well
        if (shared_char == 'q') {
            break;
        }
    }
    return nullptr;
}

/**
 * @brief Main worker thread
 * Receives the chars from the console and timeouts using @ref worker_convar
 * conditional variable
 * @param arg
 * @return void*
 */
void *worker(void *arg) {
    std::cout << "Starting worker thread" << std::endl;
    start_timeout(1);
    while (true) {

        pthread_mutex_lock(&worker_mutex);
        while ((!timeout_expired) && (!char_available)) {
            pthread_cond_wait(&worker_convar, &worker_mutex);
        }

        process_traffic_light(timeout_expired, char_available);

        timeout_expired = false;
        char_available = false;

        pthread_mutex_unlock(&worker_mutex);

        // If 'q' is entered, stop the timeout thread as well
        if (shared_char == 'q') {
            start_timeout(0);
            break;
        }
    }
    std::cout << "End worker" << std::endl;
    return nullptr;
}

/**
 * Main function for traffic light state machine
 * @param timeout_expired true when the last timeout started with @ref
 * start_timeout() has expired
 * @param button_pressed true when the user has pressed the button
 */
void process_traffic_light(bool timeout_expired, bool button_pressed) {

    if (button_pressed)
        controller->button_pressed();
    if (timeout_expired)
        controller->timeout_expired();
}

/*
int main() {
    state_machine =
        std::make_shared<RuntimeStateMachine>(TrafficState::CAR_RED);

    auto traffic_handler = std::make_unique<TrafficLightActionHandler>(
        std::make_unique<ConsoleDisplayService>(),
        std::make_unique<FunctionTimerService>(start_timeout));

    auto handler_raw = traffic_handler.get();

    controller = std::make_unique<TrafficLightController>(
        state_machine, std::move(traffic_handler));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_GREEN, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_YELLOW));
    // FIXED: Safe conditional transition using raw pointer
    state_machine->add_transition(std::make_unique<TrafficLightTransition>(
        TrafficState::CAR_YELLOW, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_RED, TrafficState::WALK_PREP,
        [handler_raw]() -> bool {
            return handler_raw->has_pedestrian_request();
        }));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_RED, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_RED_YELLOW));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK_PREP, SystemEvent::TIME_EXPIRED,
        TrafficState::WALK));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK, SystemEvent::TIME_EXPIRED,
        TrafficState::WALK_FINISH));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK_FINISH, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_RED_YELLOW));

    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_RED_YELLOW, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_GREEN));
    pthread_t reader_thread, worker_thread, timeout_thread;

    pthread_mutex_init(&worker_mutex, nullptr);
    pthread_mutex_init(&timeout_mutex, nullptr);
    pthread_cond_init(&worker_convar, nullptr);
    pthread_cond_init(&start_timeout_convar, nullptr);

    pthread_create(&reader_thread, nullptr, read_char, nullptr);
    pthread_create(&worker_thread, nullptr, worker, nullptr);
    pthread_create(&timeout_thread, nullptr, timeout_generator, nullptr);

    pthread_join(reader_thread, nullptr);
    pthread_join(worker_thread, nullptr);
    pthread_join(timeout_thread, nullptr);

    pthread_mutex_destroy(&worker_mutex);
    pthread_mutex_destroy(&timeout_mutex);
    pthread_cond_destroy(&worker_convar);
    pthread_cond_destroy(&start_timeout_convar);

    return 0;
}
*/
