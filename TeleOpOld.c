#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Motor,  motorA,           ,             tmotorNXT, PIDControl)
#pragma config(Motor,  motorC,          motorGrab,     tmotorNXT, PIDControl)
#pragma config(Motor,  mtr_S1_C1_1,     motorL,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorR,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorRO,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorRA,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servoWrist,           tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Tele-Operation Mode Code Template
//
// This file contains a template for simplified creation of an tele-op program for an FTC
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

#define JOYSTICK_MIN 10
int wrist_pos=24;          //starting position for wrist servo
bool rotateOn = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of tele-op mode, you may want to perform some initialization on your robot
// and the variables within your program.
//
// In most cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{
	// Place code here to initialize servos to starting positions.
	// Disabling sensors because of memory bug.
	SensorType[S2] = sensorNone;
	SensorType[S3] = sensorNone;
	SensorType[S4] = sensorNone;

	servoChangeRate[servoWrist] =1;          // Slow the Servo Change Rate down to only 4 positions per update.
	servo[servoWrist] = wrist_pos;                              // Move servo1 to position to starting position
}

// Operate the two drive motors
void driveMotors()
{
	int joyLeft = joystick.joy1_y1;
	int joyRight = joystick.joy1_y2;

	// If the left or right joysticks are engaged beyond a minimum threshold, operate the drive
	// motors; else stop them. The threshold is needed because a joystick that's not being touched
	// can still show small non-zero values.
	// Uses exponential increase in speed to help smooth the control at slow speeds.
	if ((abs(joyLeft) > JOYSTICK_MIN) || (abs(joyRight) > JOYSTICK_MIN))
	{
		if (abs(joyLeft) < 80)
		{
			if (joyLeft > 0)
				joyLeft = 1*pow(1+.056,joyLeft);
			else
				joyLeft = -1*pow(1+.056,abs(joyLeft));
		}

		if (abs(joyRight) < 80)
		{
			if (joyRight > 0)
				joyRight = 1*pow(1+.056,joyRight);
			else
				joyRight = -1*pow(1+.056,abs(joyRight));
		}

		motor[motorL] = (joyLeft)*100/127;
		motor[motorR] = (joyRight)*100/127;
	}
	else
	{
		motor[motorL] = 0;
		motor[motorR] = 0;
	}
}

//Arm Rotate
void rotateArm()
{
	const int rotateSpeed = 5;
	int topHat = joystick.joy1_TopHat;


	if (topHat < 0)
	{
		rotateOn = false;
		motor[motorRO] = 0;
		return;
	}

	if(topHat == 2)
	{
		motor[motorRO] = rotateSpeed;    // rotate arm clockwise
		rotateOn = true;
	}
	if(topHat == 6)
	{
		motor[motorRO] = -rotateSpeed ;     // rotate arm counter-clockwise
		rotateOn = true;
	}
}

//Arm Raise
void raiseArm()
{
	const int raiseSpeed = 20;
	const int lowerSpeed = -5;
	int topHat = joystick.joy1_TopHat;

	if (topHat < 0)
	{
		motor[motorRA] = 0;
		return;
	}

	if(topHat == 0)
		motor[motorRA] = raiseSpeed ;    // Raise Arm

	if(topHat == 4)
		motor[motorRA] = lowerSpeed ;     // Lower Arm
}

void moveGrabber()
{
	//ROBOT ARM GRAB CODE
	int buttonValues = joystick.joy1_Buttons;
	int grabUp = buttonValues & 64;
	int grabDown = buttonValues & 16;
	int closeGrabber =  buttonValues & 32;
	int openGrabber = buttonValues & 128;

	if ((grabUp + grabDown + closeGrabber + openGrabber) == 0)
	{
		//No buttons pressed, stop motors.
		motor[motorGrab] = 0;
		return;
	}

	if (grabUp && wrist_pos <= 255)
	{
		wrist_pos += 1;
	}

	if (grabDown && wrist_pos >= 0)
	{
		wrist_pos -= 1;
	}
	servo[servoWrist] = wrist_pos;

	//if (closeGrabber)
	//{
	//	motor(motorGrab) = 25;
	//}

	//if (openGrabber)
	//{
	//	motor(motorGrab) = -25;
	//}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the tele-op robot operation. Customize as appropriate for
// your specific robot.
//
// Game controller / joystick information is sent periodically (about every 50 milliseconds) from
// the FMS (Field Management System) to the robot. Most tele-op programs will follow the following
// logic:
//   1. Loop forever repeating the following actions:
//   2. Get the latest game controller / joystick settings that have been received from the PC.
//   3. Perform appropriate actions based on the joystick + buttons settings. This is usually a
//      simple action:
//      *  Joystick values are usually directly translated into power levels for a motor or
//         position of a servo.
//      *  Buttons are usually used to start/stop a motor or cause a servo to move to a specific
//         position.
//   4. Repeat the loop.
//
// Your program needs to continuously loop because you need to continuously respond to changes in
// the game controller settings.
//
// At the end of the tele-op period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	initializeRobot();

	//waitForStart();   // wait for start of tele-op phase

	while (true)
	{
		///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////
		////                                                   ////
		////      Add your robot specific tele-op code here.   ////
		////                                                   ////
		///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////

		getJoystickSettings(joystick);   // Obtain current game controller settings

		// Display the settings on the NXT Brick
		nxtDisplayString(0, "joy1_x1: %d", joystick.joy1_x1);
		nxtDisplayString(1, "joy1_y1: %d", joystick.joy1_y1);
		nxtDisplayString(2, "joy1_x2: %d", joystick.joy1_x2);
		nxtDisplayString(3, "joy1_y2: %d", joystick.joy1_y2);
		nxtDisplayString(4, "Buttons: %d", joystick.joy1_Buttons);
		nxtDisplayString(5, "TopHat:  %d", joystick.joy1_TopHat);
		// Drive Motors
		driveMotors();

		// Grabber Control
		moveGrabber();

		// Rotate arm
		rotateArm();

		// Raise or Lower arm
		raiseArm();
	}
}
