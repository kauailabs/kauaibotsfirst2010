#ifndef ROBOTMAP_H
#define ROBOTMAP_H

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
 
/////////////////////////////////////////////////////////////////////////////////////
// CAN Bus Addresses (Drive Subsystem)
/////////////////////////////////////////////////////////////////////////////////////
#define LEFT_REAR_CAN_ADDRESS 5
#define RIGHT_REAR_CAN_ADDRESS 2
#define RIGHT_FRONT_CAN_ADDRESS 3
#define LEFT_FRONT_CAN_ADDRESS 4

#endif
