#include <stdint.h>

#include <iostream>

#include <pthread.h>
#include <unistd.h>

char shared_char;
bool char_available = false;
bool timeout_expired = false;
bool timeout_set = false;
uint32_t shared_timeout_sec = 0;

pthread_mutex_t worker_mutex;
pthread_mutex_t timeout_mutex;
pthread_cond_t worker_convar;
pthread_cond_t start_timeout_convar;

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
 * TODO: implement traffic light functionality here.
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

struct LightTimings {
    static constexpr uint32_t RED_DURATION = 8;
    static constexpr uint32_t RED_YELLOW_DURATION = 2;
    static constexpr uint32_t GREEN_DURATION = 10;
    static constexpr uint32_t YELLOW_DURATION = 2;
    static constexpr uint32_t WALK_DURATION = 5;
};

enum ActionState { RED_DONT_WALK, RED_WALK, RED_YELLOW, GREEN, YELLOW };
ActionState state = RED_DONT_WALK;
ActionState next_state = RED_DONT_WALK;

// Structure to hold state information for display
struct StateInfo {
    const char *traffic_light_status;
    const char *pedestrian_status;
    int duration;
};

// Function to get display information for current state
StateInfo get_state_info(ActionState current_state, bool pedestrian_request) {
    switch (current_state) {
    case RED_DONT_WALK:
        if (pedestrian_request) {
            return {"Red", "Don't Walk",
                    1}; // Short duration before walk signal
        } else {
            return {"Red", "Don't Walk", LightTimings::RED_DURATION};
        }
    case RED_WALK:
        return {"Red", "Walk", LightTimings::WALK_DURATION};
    case RED_YELLOW:
        return {"Red Yellow", "Don't Walk", LightTimings::RED_YELLOW_DURATION};
    case GREEN:
        return {"Green", "Don't Walk", LightTimings::GREEN_DURATION};
    case YELLOW:
        return {"Yellow", "Don't Walk", LightTimings::YELLOW_DURATION};
    default:
        return {"Unknown", "Unknown", 0};
    }
}

// Function to handle state transitions
ActionState get_next_state(ActionState current_state, bool pedestrian_request) {
    switch (current_state) {
    case RED_DONT_WALK:
        return pedestrian_request ? RED_WALK : RED_YELLOW;
    case RED_WALK:
        return RED_YELLOW;
    case RED_YELLOW:
        return GREEN;
    case GREEN:
        return YELLOW;
    case YELLOW:
        return RED_DONT_WALK;
    default:
        return RED_DONT_WALK;
    }
}

void print_state_info(const StateInfo &info, bool button_just_pressed = false) {
    if (button_just_pressed) {
        std::cout << "Pedestrian button pressed!" << std::endl;
    }

    std::cout << "Traffic light: " << info.traffic_light_status << ", wait "
              << info.duration << " seconds" << std::endl;
    std::cout << "Pedestrian Light: " << info.pedestrian_status << std::endl;
}

/**
 * Main function for traffic light state machine
 * @param timeout_expired true when the last timeout started with @ref
 * start_timeout() has expired
 * @param button_pressed true when the user has pressed the button
 */
void process_traffic_light(bool timeout_expired, bool button_pressed) {
    static bool pedestrian_request = false;
    bool button_just_pressed = false;

    // Handle button press
    if (button_pressed) {
        pedestrian_request = true;
        button_just_pressed = true;
    }

    // Update current state
    state = next_state;

    // Handle timeout and state transitions
    if (timeout_expired) {
        // Get state information for display
        StateInfo info = get_state_info(state, pedestrian_request);

        // Print current state
        print_state_info(info, button_just_pressed);

        // Calculate next state
        next_state = get_next_state(state, pedestrian_request);

        // Clear pedestrian request after granting walk signal
        if (state == RED_WALK) {
            pedestrian_request = false;
        }

        // Start timeout for current state
        start_timeout(info.duration);
    } else if (button_just_pressed) {
        // Print button press notification even if timeout hasn't expired
        std::cout << "Pedestrian button pressed!" << std::endl;
    }
}

int main() {
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
