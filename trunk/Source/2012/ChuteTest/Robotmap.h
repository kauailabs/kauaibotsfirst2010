#ifndef ROBOTMAP_H
#define ROBOTMAP_H


/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
 
#define LIFTING_UP_SOLENOID_CHANNEL 5
#define TRIGGER_ON_SOLENOID_CHANNEL 1
#define TRIGGER_OFF_SOLENOID_CHANNEL 2
#define STEERING_JAGUAR_CHANNEL 7
#define LIFTING_DOWN_SOLENOID_CHANNEL 6
#define STEERING_POTENTIOMETER_CHANNEL 1
#define STEERING_POTENTIOMETER_MODULE 2
#define FIRST_SOLENOID_MODULE 1
// For example to map the left and right motors, you could define the
// following variables to use with your drivetrain subsystem.
// #define LEFTMOTOR 1
// #define RIGHTMOTOR 2

// If you are using multiple modules, make sure to define both the port
// number and the module. For example you with a rangefinder:
// #define RANGE_FINDER_PORT 1
// #define RANGE_FINDER_MODULE 1

#endif
