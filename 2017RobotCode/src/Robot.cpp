#include <iostream>
#include <memory>
#include <string>

#include <VictorSP.h>
#include <Joystick.h>
#include <SampleRobot.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <ADXRS450_Gyro.h>
#include <Spark.h>
#include <NetworkTables/NetworkTable.h>
#include <DoubleSolenoid.h>
#include <CameraServer.h>
#include <XboxController.h>
#include <GenericHID.h>
#include <GamepadBase.h>


class Robot: public frc::SampleRobot {
	//Driving
	frc::RobotDrive myRobot { 0, 1, 2, 3 };
	frc::Joystick stick { 0 };
	frc::XboxController Xbox { 1 };
	//Motors and Stuff
	frc::VictorSP Kicker { 0 };
	frc::VictorSP Shooter { 1 };
	frc::VictorSP Climber { 2 };
	frc::VictorSP Intake { 3 };
	frc::VictorSP Agitator { 4 };
	frc::DoubleSolenoid Shifter { 0, 1 };

	//Addons
	frc::ADXRS450_Gyro gyro;
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";

public:
	Robot() {

		myRobot.SetExpiration(0.1);
	}

	void RobotInit() {
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
		frc::CameraServer::GetInstance()->StartAutomaticCapture();


	}

	//AUTONOMUS
	void Autonomous() {
		auto autoSelected = chooser.GetSelected();

		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
			std::cout << "Running custom Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 1.0); // spin at half speed
			frc::Wait(2.0);                // for 2 seconds
			myRobot.Drive(0.0, 0.0);  // stop robot
		} else {
			// Default Auto goes here
			std::cout << "Running default Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 0.0); // drive forwards half speed
			frc::Wait(2.0);                // for 2 seconds
			myRobot.Drive(0.0, 0.0);  // stop robot
		}
	}

	//TELEOP
	// FIXED: Vision Tracking on Bumpers and Shooting Mechanisms(Kicker and BallShooter) on respective Triggers

	//**DRIVER PREF**
	//Agitator on Y
	//Climber on B
	//Shifting on X
	//Intake on A
	//Left or Right Joystick for driving
	//Possible HomeButton to 0* robot...

	// Correct Format Complete (JoystickHand hand)			            }
	//if (Xbox.GetBumper(JoystickHand hand) const override
	//			{
	//				Shooter.Set(1);
	//				Shooter.Set(-1);
	//				Wait(0.1);
	//			Shooter.Set(0);
	//			}


	void OperatorControl() override
	{
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled())
		{
		// drive with arcade style (use right stick)
		myRobot.ArcadeDrive(stick);

		// wait for a motor update time
		frc::Wait(0.005);

		//Fix so kicker kicks at X degrees and returns
		if (Xbox.GetTriggerAxis(XboxController::JoystickHand(0))) //Left Trigger
		{
			Kicker.Set(-1);
			Wait(0.1);
			Kicker.Set(0);
		}
		//Vision Tracking for Gear
		if (Xbox.GetBumper(XboxController::JoystickHand(0)))
		{

		}

		//Shooter Trigger
		if (Xbox.GetTriggerAxis(XboxController::JoystickHand(1)))
		{
				Shooter.Set(1);
				Wait(0.1);
				Shooter.Set(0);

		}
		//Vision Tracking for Shooter
		if (Xbox.GetBumper(XboxController::JoystickHand(1)))
		{

		}



		if (Xbox.GetAButton()) //Hooper Alligator
		{
			Agitator.Set(-1);
			Agitator.Set(1);
			Wait(0.1);
			Agitator.Set(0);
		}

		if (Xbox.GetBButton()) //Climber
		{
			Climber.Set(-1);
			Climber.Set(1);
			Wait(0.1);
			Climber.Set(0);
		}

		if (Xbox.GetXButton()&& Shifter.Get()!=0) //Shifter
		{
			Shifter.Set(DoubleSolenoid::Value(0));
			Wait(0.005);
		}else
		{
			Shifter.Set(DoubleSolenoid::Value(1));
			Wait(0.005);
		}
		if (Xbox.GetYButton()) //Intake
		{
			Intake.Set(-1);
			Intake.Set(1);
			Wait(0.005);
			Intake.Set(0);

		}

		Wait(0.005);
	}

}

	/*
	 * Runs during test mode
	 */
	void Test()
    {
	}
};
START_ROBOT_CLASS(Robot)
