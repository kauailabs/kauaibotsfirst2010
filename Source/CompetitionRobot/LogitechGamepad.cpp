#include "LogitechGamepad.h"

/**
 * Constructs an instance of the LogitechGamepad class.
 * 
 * @param port The port on the driver station that the gamepad is plugged into.
 */
LogitechGamepad::LogitechGamepad(unsigned port)
{
	// Ideally, we'd use the Joystick(UINT32 port, UINT32 numAxisTypes, UINT32
	// numButtonTypes) constructor, but it's protected, so we assume the gamepad
	// has the same number of or fewer axes and buttons. 
	joystick = new Joystick(port);
}

LogitechGamepad::~LogitechGamepad()
{
}

/**
 * Gets the X-axis value of the gamepad's left joystick.
 * The left joystick cannot be used when the directional pad is active (i.e. the
 * red LED is illuminated). Press the mode button to toggle this behavior.
 * 
 * @return The left X-axis value. 
 */
float LogitechGamepad::GetLeftX()
{
	return joystick->GetRawAxis(kLeftXAxis);
}

/**
 * Gets the Y-axis value of the gamepad's left joystick.
 * The left joystick cannot be used when the directional pad is active (i.e. the
 * red LED is illuminated). Press the mode button to toggle this behavior.
 * 
 * @return The left Y-axis value.
 */
float LogitechGamepad::GetLeftY()
{
	return joystick->GetRawAxis(kLeftYAxis);
}

/**
 * Gets the X value of the gamepad's right joystick.
 * 
 * @return The right X-axis value.
 */
float LogitechGamepad::GetRightX()
{
	return joystick->GetRawAxis(kRightXAxis);
}

/**
 * Gets the Y value of the gamepad's right joystick.
 * 
 * @return The right Y-axis value.
 */
float LogitechGamepad::GetRightY()
{
	return joystick->GetRawAxis(kRightYAxis);
}

/**
 * Gets the state of the button with the given number.
 * 
 * @param buttonNumber The number of the button.
 * @return The state of the button.
 */
bool LogitechGamepad::GetNumberedButton(unsigned buttonNumber)
{
	return joystick->GetRawButton(buttonNumber);
}

/**
 * Gets whether or not the left joystick is depressed.
 * 
 * @return The state of the left axis joystick button.
 */
bool LogitechGamepad::GetLeftPush()
{
	return joystick->GetRawButton(kLeftJoystickButton);
}

/**
 * Gets whether or not the right joystick is depressed.
 *
 * @return The state of the right axis joystick button.
 */
bool LogitechGamepad::GetRightPush()
{
	return joystick->GetRawButton(kRightJoystickButton);
}

/**
 * Gets the state of the directional pad.
 * The left joystick cannot be used when the directional pad is active (i.e. the
 * red LED is illuminated). Press the mode button to toggle this behavior.
 *
 * @return The state of the directional pad.
 */
LogitechGamepad::DPadDirection LogitechGamepad::GetDPad()
{
	float x = GetLeftX();
	float y = GetLeftY();
	
	if (x < -0.5 && y < -0.5)
		return kUpLeft;
	if (x < -0.5 && y > 0.5)
		return kDownLeft;
	if (x > 0.5 && y > 0.5)
		return kDownRight;
	if (x > 0.5 && y < -0.5)
		return kUpRight;
	if (y < -0.5)
		return kUp;
	if (x < -0.5)
		return kLeft;
	if (y > 0.5)
		return kDown;
	if (x > 0.5)
		return kRight;
	
	return kCenter;
}
