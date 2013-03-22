#ifndef _JOYSTICK_CONSTANTS_H 
#define _JOYSTICK_CONSTANTS_H  
///////////////////////////////////////////////////////////////////////////////////// 
//Joystick Assignments 
///////////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////////////  
#define DEADBAND  .025 
///////////////////////////////////////////////////////////////////////////////////// 
// Joystick Modifiers 
// Result(when outside of the dead zone) = Mult(Adjust*Original^Pow + (1-Adjust)Original) 
// Result(with within the dead zone) = 0
///////////////////////////////////////////////////////////////////////////////////// 
// Forward Backwards 
const double JoystickForward_Adjust = 0.40;  
const double JoystickForward_Power = 3;  
const double JoystickForward_Multiplier = 1; 
const double JoystickForward_Deadband = DEADBAND;  
// Side to Side 
const double JoystickStrafe_Adjust = 0.40;  
const double JoystickStrafe_Power = 3;  
const double JoystickStrafe_Multiplier = 1;  
const double JoystickStrafe_Deadband = DEADBAND;  
// Rotation 
const double JoystickRotate_Adjust = .40; 
const double JoystickRotate_Power = 3; 
const double JoystickRotate_Multiplier = .35; 
const double JoystickRotate_Deadband = DEADBAND; 
/////////////////////////////////////////////////////////////////////////////////////  
#endif  
