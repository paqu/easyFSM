

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
/*
typedef enum Trafic_Light_State {
    RED,
    RED_YELLOW,
    GREEN,
    YELLOW
} trafic_light_state_t;

trafic_light_state_t trafic_light_state = RED;
trafic_light_state_t next_trafic_light_state = RED;

typedef enum Pedestrian_Light_State {
    WALK_GREEN,
    DONT_WALK_RED
} pedestrian_light_state_t;
pedestrian_light_state_t pedestrian_light_state = DONT_WALK_RED;

void handle_button_pressed(trafic_light_state_t *tl_state,
pedestrian_light_state_t  *pl_state )
{

    std::cout << "handled button press" << std::endl;
}
*/

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

/**
 * Main function for traffic light state machine
 * @param timeout_expired true when the last timeout started with @ref
 * start_timeout() has expired
 * @param button_pressed true when the user has pressed the button
 */

void process_traffic_light(bool timeout_expired, bool button_pressed) {

  static bool pedestrian_request = false;

  if (button_pressed) {
    std::cout << "Pedestrian button pressed!" << std::endl;
    pedestrian_request = true;
  }

  state = next_state;

  if (timeout_expired) {

    if (state == RED_DONT_WALK) {
      std::cout << "Trafic light:Red, wait " << LightTimings::RED_DURATION
                << std::endl;
      if (pedestrian_request == true) {

        start_timeout(1);
        next_state = RED_WALK;

      } else {
        start_timeout(LightTimings::RED_DURATION);
        next_state = RED_YELLOW;
        std::cout << "Pedestrian Light: Dont Walk" << std::endl;
      }
    } else if (state == RED_WALK) {
      std::cout << "Pedestrian Light: Walk for " << LightTimings::WALK_DURATION
                << std::endl;
      start_timeout(LightTimings::WALK_DURATION);
      pedestrian_request = false;
      next_state = RED_YELLOW;
    } else if (state == RED_YELLOW) {
      std::cout << "Trafic light:Red Yellow, wait "
                << LightTimings::RED_YELLOW_DURATION << " secodns" << std::endl;
      std::cout << "Pedestrian Light: Dont Walk" << std::endl;
      start_timeout(LightTimings::RED_YELLOW_DURATION);
      next_state = GREEN;

    } else if (state == GREEN) {
      std::cout << "Trafic light:Green, wait " << LightTimings::GREEN_DURATION
                << " secodns" << std::endl;
      std::cout << "Pedestrian Light: Dont Walk" << std::endl;
      start_timeout(LightTimings::GREEN_DURATION);
      next_state = YELLOW;

    } else if (state == YELLOW) {
      std::cout << "Trafic light: Yellow, wait "
                << LightTimings::YELLOW_DURATION << " secodns" << std::endl;
      std::cout << "Pedestrian Light: Dont Walk" << std::endl;
      start_timeout(LightTimings::YELLOW_DURATION);
      next_state = RED_DONT_WALK;
    }
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
