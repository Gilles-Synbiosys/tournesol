#include <cstdio>
#include "pico/stdlib.h"
#include "motor2040.hpp"
#include "button.hpp"
#include "pid.hpp"
#include "hardware/gpio.h"
#include "tusb.h"
#include "stdlib.h"
#include "string.h"

/*
A demonstration of driving all four of Motor 2040's motor outputs through a
sequence of velocities, with the help of their attached encoders and PID control.

Press "Boot" to exit the program.
*/

using namespace motor;
using namespace encoder;

enum Wheels
{
  RL = 1,
  RR = 0,
};

// The gear ratio of the motor
constexpr float GEAR_RATIO = 50.0f;

// Ratio of the turret gear

constexpr float TURRET_GEAR_RATIO = 1.0f;

// The counts per revolution of the motor's output shaft
constexpr float COUNTS_PER_REV = MMME_CPR * GEAR_RATIO;

// The scaling to apply to the motor's speed to match its real-world speed
constexpr float SPEED_SCALE = 5.4f;

// How many times to update the motor per second
const uint UPDATES = 500;
constexpr float UPDATE_RATE = 1.0f / (float)UPDATES;

// The time to travel between each random value
constexpr float TIME_FOR_EACH_MOVE = 2.0f;
const uint UPDATES_PER_MOVE = TIME_FOR_EACH_MOVE * UPDATES;

// How many of the updates should be printed (i.e. 2 would be every other update)
const uint PRINT_DIVIDER = 4;

// The speed to drive the wheels at
constexpr float DRIVING_SPEED = 1.0f;

// PID values
constexpr float VEL_KP = 100.0f; // Velocity proportional (P) gain
constexpr float VEL_KI = 0.0f;  // Velocity integral (I) gain
constexpr float VEL_KD = 0.4f;  // Velocity derivative (D) gain

// Create an array of motor pointers
const pin_pair motor_pins[] = {motor2040::MOTOR_A, motor2040::MOTOR_B,
                               motor2040::MOTOR_C, motor2040::MOTOR_D};
const uint NUM_MOTORS = 2u; // count_of(motor_pins);
Motor *motors[NUM_MOTORS];

// Create an array of encoder pointers
const pin_pair encoder_pins[] = {motor2040::ENCODER_A, motor2040::ENCODER_B,
                                 motor2040::ENCODER_C, motor2040::ENCODER_D};
const char *ENCODER_NAMES[] = {"RR", "RL", "FL", "FR"};
const uint NUM_ENCODERS = count_of(encoder_pins);
Encoder *encoders[NUM_ENCODERS];

// Create the user button
Button user_sw(motor2040::USER_SW);

// Create an array of PID pointers
PID vel_pids[NUM_MOTORS];

// Helper functions for driving in common directions

void set_speed(float s_azimuth, float s_elevation)
{
  vel_pids[RL].setpoint = TURRET_GEAR_RATIO * s_azimuth - s_elevation;
  vel_pids[RR].setpoint = TURRET_GEAR_RATIO * s_azimuth + s_elevation;
  printf("motor RL: %f\n", vel_pids[RL].setpoint);
  printf("motor RR: %f\n", vel_pids[RR].setpoint);
}

void stop()
{
  // vel_pids[FL].setpoint = 0.0f;
  // vel_pids[FR].setpoint = 0.0f;
  vel_pids[RL].setpoint = 0.0f;
  vel_pids[RR].setpoint = 0.0f;
}

int parse_message(const char *message, float *vx, float *vy)
{
    // Copy the input message to a modifiable buffer
    char buffer[64];
    strncpy(buffer, message, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

    // Temporary buffer to hold parts of the message
    char *token;

    // Extract the vx part
    token = strtok(buffer, ";");
    if (token == NULL || sscanf(token, "vx%f", vx) != 1)
    {
        return 1; // Error parsing vx
    }

    // Extract the vy part
    token = strtok(NULL, ";");
    if (token == NULL || sscanf(token, "vy%f", vy) != 1)
    {
        return 1; // Error parsing vy
    }

    return 0; // Success
}


char str1[50];
int main()
{
  stdio_init_all();
  tusb_init();
  uint8_t message[64] = {0};
  float vx = 0.0, vy = 0.0;
  // Fill the arrays of motors, encoders, and pids, and initialise them
  for (auto i = 0u; i < NUM_MOTORS; i++)
  {
    motors[i] = new Motor(motor_pins[i], NORMAL_DIR, SPEED_SCALE);
    motors[i]->init();

    encoders[i] = new Encoder(pio0, i, encoder_pins[i], PIN_UNUSED, NORMAL_DIR, COUNTS_PER_REV, true);
    encoders[i]->init();

    vel_pids[i] = PID(VEL_KP, VEL_KI, VEL_KD, UPDATE_RATE);
  }

  // Reverse the direction of the B and D motors and encoders

  // Enable all motors
  for (auto i = 0u; i < NUM_MOTORS; i++)
  {
    motors[i]->enable();
  }

  uint update = 0;
  uint print_count = 0;
  uint sequence = 0;
  float speed = 0.0f;
  float speed2 = 0.0f;
  Encoder::Capture captures[NUM_MOTORS];

  // Continually move the motor until the user button is pressed
  while (!user_sw.raw())
  {

    tud_task();// TinyUSB device task
    if (tud_cdc_connected())
        {
            int bytes_available = tud_cdc_available();
            if (bytes_available)
            {
                for (int i = 0; i < sizeof(message); i++)
                {
                    message[i] = 0;
                }
                int num_read = tud_cdc_read(message, sizeof(message));
                if (num_read > 0)
                {
                    // Process the received command
                    printf("Received command: %.*s\n", num_read, message);
                    //process_command(message, num_read);

                    if (parse_message((const char *)message, &vx, &vy) == 0)
                    {
                        printf("vx = %f\n", vx);
                        printf("vy = %f\n", vy);
                        set_speed(vx,vy);
                    }
                    else
                    {
                        fprintf(stderr, "Failed to parse the message\n");
                    }
                }
            }
        }

    // Capture the state of all the encoders
    for (auto i = 0u; i < NUM_MOTORS; i++)
    {
      captures[i] = encoders[i]->capture();
    }

    for (auto i = 0u; i < NUM_MOTORS; i++)
    {
      // Calculate the acceleration to apply to the motor to move it closer to the velocity setpoint
      float accel = vel_pids[i].calculate(captures[i].revolutions_per_second());

      // Accelerate or decelerate the motor
      motors[i]->speed(motors[i]->speed() + (accel * UPDATE_RATE));
    }

    for (int inc = 0; inc < 10; inc++)
    {
      // Print out the current motor values and their setpoints, but only on every multiple
      if (print_count == 0)
      {
        for (auto i = 0u; i < NUM_ENCODERS; i++)
        {
          printf("%s = %f, ", ENCODER_NAMES[i], captures[i].revolutions_per_second());
          printf("motor RL: %f\n", vel_pids[RL].setpoint);
          printf("motor RR: %f\n", vel_pids[RR].setpoint);
        }
        printf("\n");
      }

      // Increment the print count, and wrap it
      print_count = (print_count + 1) % PRINT_DIVIDER;
      sleep_ms(UPDATE_RATE * 1000.0f);
    }

    update++;   // Move along in time

    // Have we reached the end of this movement?
    if(update >= UPDATES_PER_MOVE) {
      update = 0;  // Reset the counter

      // Move on to the next part of the sequence
      sequence += 1;

      // Loop the sequence back around
      if(sequence >= 9) {
        sequence = 0;
      }
    }

    // // Set the motor speeds, based on the sequence
    // switch(sequence) {
    // case 0:
    //   set_speed(speed, 0.0f);
    //   printf("azimut full positive\n");
    //   break;
    // case 2:
    //   set_speed(-speed, 0.0f);
    //   printf("azimut full negative\n");
    //   break;
    // case 3:
    //   set_speed(0.0f, speed);
    //   printf("elevation full positive\n");
    //   break;
    // case 4:
    //   set_speed(0.0f, -speed);
    //   printf("elevation full negative\n");
    //   break;
    // case 5:
    //   set_speed(speed, speed);
    //   printf("azimut and elevation full positive\n");
    //   break;
    // case 6:
    //   set_speed(-speed, -speed);
    //   printf("azimut and elevation full negative\n");
    //   break;
    // case 7:
    //   set_speed(speed, -speed);
    //   break;
    // case 8:
    //   set_speed(-speed, speed);
    //   break;
    // default:
    //   stop();
    //   printf("stop\n");
    //   break;
    // }

    sleep_ms(UPDATE_RATE * 1000.0f);
  }

  // Stop all the motors
  for (auto m = 0u; m < NUM_MOTORS; m++)
  {
    motors[m]->disable();
  }
}
