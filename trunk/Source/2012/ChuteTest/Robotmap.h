#ifndef ROBOTMAP_H
#define ROBOTMAP_H


/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
 
#define LIFTING_UP_SOLENOID_CHANNEL 5
#define TRIGGER_RIGHT_UP_SOLENOID_CHANNEL 3
#define TRIGGER_RIGHT_DOWN_SOLENOID_CHANNEL 4
#define STEERING_SERVO_CHANNEL 7
#define LIFTING_DOWN_SOLENOID_CHANNEL 6
#define TRIGGER_LEFT_UP_SOLENOID_CHANNEL 1
#define TRIGGER_LEFT_DOWN_SOLENOID_CHANNEL 2
// For example to map the left and right motors, you could define the
// following variables to use with your drivetrain subsystem.
// #define LEFTMOTOR 1
// #define RIGHTMOTOR 2

// If you are using multiple modules, make sure to define both the port
// number and the module. For example you with a rangefinder:
// #define RANGE_FINDER_PORT 1
// #define RANGE_FINDER_MODULE 1

#endif
