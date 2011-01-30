#ifndef LOGITECH_GAMEPAD_H_
#define LOGITECH_GAMEPAD_H_

#include "Joystick.h"

/**
 * Handles input from a Logitech Gamepad connected to the Driver Station.
 * This class is a convenience class for the Logitech Dual Action Gamepad. It
 * wraps a Joystick object to provide methods to access the inputs on the
 * gamepad. 
 */
class LogitechGamepad
{
public:
	typedef enum
	{
		kCenter, kUp, kUpLeft, kLeft, kDownLeft, kDown, kDownRight, kRight,
		kUpRight
	} DPadDirection;

	LogitechGamepad(unsigned port);
	virtual ~LogitechGamepad();

	float GetLeftX(void);
	float GetLeftY(void);
	float GetRightX(void);
	float GetRightY(void);

	bool GetNumberedButton(unsigned buttonNumber);
	bool GetLeftPush();
	bool GetRightPush();

	DPadDirection GetDPad();

private:
	static const unsigned kNumButtons = 12;
	static const unsigned kLeftJoystickButton = 11;
	static const unsigned kRightJoystickButton = 12;
	static const unsigned kNumAxes = 4;
	typedef enum
	{
		kLeftXAxis = 1, kLeftYAxis, kRightXAxis, kRightYAxis
	} GamepadAxisType;
	Joystick *joystick;
};

#endif // LOGITECH_GAMEPAD_H_
