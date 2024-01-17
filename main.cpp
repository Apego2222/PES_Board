#include "mbed.h"

#include "pm2_drivers/PESBoardPinMap.h"
#include "pm2_drivers/DebounceIn.h"
#include "pm2_drivers/Servo.h"

bool do_execute_main_task = false; // this variable will be toggled via the user button (blue button) and
                                   // decides whether to execute the main task or not
bool do_reset_all_once = false;    // this variable is used to reset certain variables and objects and
                                   // shows how you can run a code segment only once

// objects for user button (blue button) handling on nucleo board
DebounceIn user_button(USER_BUTTON);  // create InterruptIn interface object to evaluate user button falling and
                                      // rising edge (no blocking code in ISR)
void user_button_pressed_fcn();       // custom functions which get executed when user
                                      // button gets pressed, definition below

// Additional function declaration
float ir_sensor_compensation(float _ir_distance_mV);

// main runs as an own thread
int main()
{
    enum RobotState {
        INITIAL,      
        EXECUTION,
        SLEEP,
        EMERGENCY
    } robot_state = RobotState::INITIAL;

    // attach button fall function to user button object, button has a pull-up resistor
    user_button.fall(&user_button_pressed_fcn);

    // while loop gets executed every main_task_period_ms milliseconds (simple
    // aproach to repeatedly execute main)
    const int main_task_period_ms = 10; // define main task period time in ms e.g. 50 ms
                                        // -> main task runs 20 times per second
    Timer main_task_timer;              // create Timer object which we use to run the main task every main_task_period_ms
    Timer timer;

    // led on nucleo board
    // create DigitalOut object to command user led
    DigitalOut user_led(USER_LED);

    // additional led's
    // create DigitalOut object to command extra led (you need to add an aditional
    // resistor, e.g. 220...500 Ohm) an led as an anode (+) and a cathode (-), the
    // cathode is connected to ground via a resistor
    DigitalOut led1(PB_8);
    //DigitalOut led2(PB_9);

    // mechanical button
    DigitalIn mechanical_button(PC_5); // create DigitalIn object to evaluate extra mechanical button, you
                                       // need to specify the mode for proper usage, see below
    mechanical_button.mode(PullUp);    // set pullup mode: sets pullup between pin and 3.3 V, so that there
                                       // is a defined potential

    // HERE DEFINE OBJECTS (remember about giving comments and proper names)
    // Sharp GP2Y0A41SK0F, 4-40 cm IR Sensor
    float ir_distance_mV = 0.0f; // define variable to store measurement (in mV)
    float ir_distance_cm = 0.0f;
    AnalogIn ir_analog_in(PC_1); // create AnalogIn object to read in infrared distance sensor, 0...3.3V are mapped to 0...1

    float min_range = 10.0f;
    float max_range = 35.0f;

    // Servo
    Servo servo_D0(PB_D0);
    Servo servo_D1(PB_D1);

    // Those variables should be filled out with values obtained in the
    // calibration process, these are minimal pulse width and maximal pulse width.
    //  Futuba S3001
    float servo_D0_ang_min = 0.0150f;
    float servo_D0_ang_max = 0.1150f;
    // Modelcraft RS2 MG/BB
    float servo_D1_ang_min = 0.0325f;
    float servo_D1_ang_max = 0.1250f;

    // setNormalisedPulseWidth: before calibration (0,1) -> (min pwm, max pwm)
    servo_D0.calibratePulseMinMax(servo_D0_ang_min, servo_D0_ang_max);
    servo_D1.calibratePulseMinMax(servo_D1_ang_min, servo_D1_ang_max);

    // setNormalisedPulseWidth: after calibration (0,1) -> (servo_D0_ang_min, servo_D0_ang_max)

    //Variables needed to manage the servo
    float servo_angle = 0.0f; // servo S1 normalized input: 0...1
    int servo_counter = 0; // define servo counter, this is an additional variable
                       // to make the servos move
    const int loops_per_seconds = static_cast<int>(ceilf(1.0f / (0.001f * (float)main_task_period_ms)));


    // start timer
    main_task_timer.start();
    timer.start();

    // this loop will run forever
    while (true) {
        main_task_timer.reset();

        if (do_execute_main_task) {
		
            // visual feedback that the main task is executed, setting this once would actually be enough
            // visual feedback that the main task is executed, setting this once would actually be enough
            led1 = 1;

            // read analog input 
            ir_distance_mV = 1.0e3f * ir_analog_in.read() * 3.3f;
            ir_distance_cm = ir_sensor_compensation(ir_distance_mV);

            switch (robot_state) {
                case RobotState::INITIAL: {
                    //Servo enabling statement
                    if (!servo_D0.isEnabled())
                        servo_D0.enable();
                    if (!servo_D1.isEnabled())
                        servo_D1.enable();
                    robot_state = RobotState::EXECUTION;
                    break;
                }
                case RobotState::EXECUTION: {
                    //Function to map the distnace to servo movement
                    float servo_ang = (1 / (max_range - min_range)) * ir_distance_cm - (min_range / (max_range - min_range));
                    servo_D0.setNormalisedPulseWidth(servo_ang);

                    if (mechanical_button.read()){
                        robot_state = RobotState::SLEEP;
                    }
                    else if (ir_distance_cm > max_range) {
                        robot_state = RobotState::EMERGENCY;
                    }
                    break;
                }
                case RobotState::SLEEP: {

                    if (mechanical_button.read()){
                        robot_state = RobotState::EXECUTION;
                    }
                    break;
                }
                case RobotState::EMERGENCY: {
                    user_button_pressed_fcn();  
                    break;
                }
                default: {
                    break; // do nothing
                }
            }


        } else {
            if (do_reset_all_once) {
                do_reset_all_once = false;
                
                led1 = 0;
                servo_D0.disable();
                servo_D1.disable();
                servo_angle = 0.0f;
                ir_distance_mV = 0.0f;
                ir_distance_cm = 0.0f;
            }
        }

        // toggling the user led
        user_led = !user_led;

        //Printing command 
        printf("IR distance mV: %f IR distance cm: %f \n", ir_distance_mV, ir_distance_cm);

        // read timer and make the main thread sleep for the remaining time span (non blocking)
        int main_task_elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(main_task_timer.elapsed_time()).count();
        thread_sleep_for(main_task_period_ms - main_task_elapsed_time_ms);
    }
}

void user_button_pressed_fcn()
{
    // do_execute_main_task if the button was pressed
    do_execute_main_task = !do_execute_main_task;
    if (do_execute_main_task)
        do_reset_all_once = true;
}

// Function to compensate  
float ir_sensor_compensation(float _ir_distance_mV) {

    static const float a = 2.574e+04f;
    static const float b = -29.37f;

    static float ir_distance_cm = 0.0f;
    if (_ir_distance_mV + b == 0) ir_distance_cm = -1.0f;
    else ir_distance_cm = a / (_ir_distance_mV + b);

    return ir_distance_cm;
}


/*
#include "mbed.h"

#include "pm2_drivers/PESBoardPinMap.h"
#include "pm2_drivers/DebounceIn.h"
#include "pm2_drivers/EncoderCounter.h"
#include "pm2_drivers/DCMotor.h"


bool do_execute_main_task = false; // this variable will be toggled via the user button (blue button) and
                                   // decides whether to execute the main task or not
bool do_reset_all_once = false;    // this variable is used to reset certain variables and objects and
                                   // shows how you can run a code segment only once

// objects for user button (blue button) handling on nucleo board
DebounceIn user_button(USER_BUTTON);  // create InterruptIn interface object to evaluate user button falling and
                                      // rising edge (no blocking code in ISR)
void user_button_pressed_fcn();       // custom functions which get executed when user
                                      // button gets pressed, definition below

// main runs as an own thread
int main()
{
    // attach button fall function to user button object, button has a pull-up resistor
    user_button.fall(&user_button_pressed_fcn);

    // while loop gets executed every main_task_period_ms milliseconds (simple
    // aproach to repeatedly execute main)
    const int main_task_period_ms = 50; // define main task period time in ms e.g. 50 ms
                                        // -> main task runs 20 times per second
    Timer main_task_timer;              // create Timer object which we use to run the main task every main_task_period_ms
    Timer timer;

    // led on nucleo board
    // create DigitalOut object to command user led
    DigitalOut user_led(USER_LED);

    // additional led's
    // create DigitalOut object to command extra led (you need to add an aditional
    // resistor, e.g. 220...500 Ohm) an led as an anode (+) and a cathode (-), the
    // cathode is connected to ground via a resistor
    DigitalOut led1(PB_8);
    //DigitalOut led2(PB_9);

    // mechanical button
    DigitalIn mechanical_button(PC_5); // create DigitalIn object to evaluate extra mechanical button, you
                                       // need to specify the mode for proper usage, see below
    mechanical_button.mode(PullUp);    // set pullup mode: sets pullup between pin and 3.3 V, so that there
                                       // is a defined potential

    // HERE DEFINE OBJECTS (remember about giving comments and proper names)
    

    // start timer
    main_task_timer.start();
    timer.start();

    // this loop will run forever
    while (true) {
        main_task_timer.reset();

        if (do_execute_main_task) {
            
            // visual feedback that the main task is executed, setting this once would actually be enough
            led1 = 1;

        } else {
            if (do_reset_all_once) {
                do_reset_all_once = false;
                
            }
        }

        // toggling the user led
        user_led = !user_led;

        //Printing command 

        // read timer and make the main thread sleep for the remaining time span (non blocking)
        int main_task_elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(main_task_timer.elapsed_time()).count();
        thread_sleep_for(main_task_period_ms - main_task_elapsed_time_ms);
    }
}

void user_button_pressed_fcn()
{
    // do_execute_main_task if the button was pressed
    do_execute_main_task = !do_execute_main_task;
    if (do_execute_main_task)
        do_reset_all_once = true;
}
*/