#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     IRseeker,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     touchL,         sensorTouch)
#pragma config(Sensor, S4,     sonarSensor,         sensorSONAR)
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
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#define JOYSTICK_MIN 10
#define ENCODER_TICKS_INCH 102

void playMusic()
{
	PlayTone(695, 14); while(bSoundActive);
	PlayTone(695, 14); while(bSoundActive);
	PlayTone(695, 14); while(bSoundActive);
	PlayTone(929, 83); while(bSoundActive);
	PlayTone(1401, 83); while(bSoundActive);
	PlayTone(1251, 14); while(bSoundActive);
	PlayTone(1188, 14); while(bSoundActive);
	PlayTone(1054, 14); while(bSoundActive);
	PlayTone(1841, 83); while(bSoundActive);
	PlayTone(1401, 41); while(bSoundActive);
	PlayTone(1251, 14); while(bSoundActive);
	PlayTone(1188, 14); while(bSoundActive);
	PlayTone(1054, 14); while(bSoundActive);
	PlayTone(1841, 83); while(bSoundActive);
	PlayTone(1401, 41); while(bSoundActive);
	PlayTone(1251, 14); while(bSoundActive);
	PlayTone(1188, 14); while(bSoundActive);
	PlayTone(1251, 14); while(bSoundActive);
	PlayTone(1054, 55); while(bSoundActive);
	wait1Msec(280);
	PlayTone(695, 14); while(bSoundActive);
	PlayTone(695, 14); while(bSoundActive);
	PlayTone(695, 14); while(bSoundActive);
	PlayTone(929, 83); while(bSoundActive);
	PlayTone(1401, 83); while(bSoundActive);
	PlayTone(1251, 14); while(bSoundActive);
	PlayTone(1188, 14); while(bSoundActive);
	PlayTone(1054, 14); while(bSoundActive);
	PlayTone(1841, 83); while(bSoundActive);
	PlayTone(1401, 41); while(bSoundActive);
	PlayTone(1251, 14); while(bSoundActive);
	PlayTone(1188, 14); while(bSoundActive);
	PlayTone(1054, 14); while(bSoundActive);
	PlayTone(1841, 83); while(bSoundActive);
	PlayTone(1401, 41); while(bSoundActive);
	PlayTone(1251, 14); while(bSoundActive);
	PlayTone(1188, 14); while(bSoundActive);
	PlayTone(1251, 14); while(bSoundActive);
	PlayTone(1054, 55); while(bSoundActive);
}

// Returns an angle in degrees between 0 and 359
int norm_360(int deg) {
  return deg%360;
}

// Returns an angle in degrees between -180 and 179
int norm_180(int deg) {
  return norm_360(deg + 180)-180;
}

int convertDegree(float degree)
{
	return (int)(degreesToRadians(degree));
}

int convert(float inches)
{
	return (int)(inches * ENCODER_TICKS_INCH);
}

void GoInches(float inches, int speed)
{
	nMotorEncoder[motorL] = 0;
	nMotorEncoder[motorR] = 0;
	wait1Msec(200);
	motor[motorL] = speed;
	motor[motorR] = speed;
	while (abs(nMotorEncoder[motorR]) < (convert(inches)) || abs(nMotorEncoder[motorL]) < (convert(inches)))
	{
	}

	motor[motorL] = 0;
	motor[motorR] = 0;
	wait1Msec(200);
}

void goDegrees(float degree, int speed)
{
	nMotorEncoder[motorL] = 0;
	nMotorEncoder[motorL] = 0;
	wait1Msec(300);
	motor[motorL] = -speed;
	motor[motorR] = speed;
	while ((abs(nMotorEncoder[motorR]) < (convertDegree(degree))) || abs(nMotorEncoder[motorL]) < (convertDegree(degree)))
	{
	}

	motor[motorL] = 0;
	motor[motorR] = 0;
	wait1Msec(300);
}

void RotateMotor(int position)
{
	nMotorEncoder[motorD] = 0;  // Reset encoder
	nMotorEncoderTarget[motorD] = position;
	while(nMotorRunState[motorD] != runStateIdle)
	{
		// Wait until encoder value is reached.
		motor[motorD] = 75;
	}

	motor[motorD] = 0;
}

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

		motor[motorL] = (joyLeft)*75/127;
		motor[motorR] = (joyRight)*75/127;   // Convert motor range of -75 to 75
		wait1Msec(100);
	}
	else
	{
		motor[motorL] = 0;
		motor[motorR] = 0;
		wait1Msec(100);
	}
}

task main()
{


}
