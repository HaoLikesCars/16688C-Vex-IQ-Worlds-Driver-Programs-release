///tune///
double kp = 1.3;
double ki = 0.0;
double kd = 0.0;

//turn costants
double tp = 0.73; //0.8
//double ti = 0;
double td = 0.7;
double ktp = 1.3;
///tune///



//double proportion;
double totalError;
double rtotalError;
double integral;
double rintegral;
double integralActiveZone = 200;//adjust later
double integralPowerLimit = 50;//adjust later
double derivative;
double rderivative;
double leftPower;
double rightPower;
double terror; //turn error >:D
double gyroValue;
double gyroerror;
double turn_TotalError;
double tderivative;
double power;
double tlasterror;

double total;
int side;

task gyroTask() //stolen from backwards crew :/
{
	gyroValue = getGyroHeading(gyro);
}

void Drive(double target,double angle) {
	double error = target;
	double lastError = target;
	double rError = target;
	double rLastError = target;
	resetMotorEncoder(right);
	resetMotorEncoder(left);
	while(abs(error) > 2||abs(Rerror) > 2||abs(derivative)>1){
		error = target - getMotorEncoder(left);
		rError = target - getMotorEncoder(right);
		if(error == 0){
			break;
		}


		//left integral
		if(abs(error) < integralActiveZone && error != 0){
			totalError += error;
		}
		else totalError = 0.0;

		if(totalError > integralPowerLimit){
			totalError = integralPowerLimit;
		}
		if(totalError < -integralPowerLimit){
			totalError = -integralPowerLimit;
		}

		integral = ki * totalError;
		///////

		//right integral
		if(abs(rError) < integralActiveZone && rError != 0){
			rtotalError += rerror;
		}
		else rtotalError = 0.0;

		if(rtotalError > integralPowerLimit){
			rtotalError = integralPowerLimit;
		}
		if(rtotalError < -integralPowerLimit){
			rtotalError = -integralPowerLimit;
		}

		rintegral = ki * rtotalError;
		///////

		//derivative
		derivative = (error - lastError);
		lastError = error;

		rderivative =(rerror - rlastError);
		rlastError = rerror;

		if(error == 0){
			derivative = 0;
			rderivative = 0;
		}

		//terror = turn error lol
		terror = angle-getGyroHeadingFloat(gyro);
		if (terror > 180){
			terror = terror - 360;
		}
		else if (terror < -180){
			terror = terror + 360;
		}

		terror = terror*ktp;
		leftPower = 0.5 * (kp * error + integral + kd*derivative);
		rightPower = 0.5 * (kp * rError + rintegral + kd*rderivative);
		if(leftPower>80){
			leftPower=80;
		}
		if(rightPower>80){
			rightPower=80;
		}
		setMotorSpeed(left,leftPower-terror*leftpower/80);
		setMotorSpeed(right,rightPower+terror*leftpower/80);

		wait1Msec(30);
	}
	StopMultipleMotors(left,right);
}
void drive(double target, int heading, int speed)
{

	resetMotorEncoder(left);
	resetMotorEncoder(right);
	while(abs(target-getMotorEncoder(left))>1&&abs(target-getMotorEncoder(right))>1){
		setMotorTarget(left,target,speed);
		setMotorTarget(right,target,speed);

		terror = heading-getGyroHeadingFloat(gyro);
		if (terror > 180){
			terror = terror - 360;
		}
		else if (terror < -180){
			terror = terror + 360;
		}

		terror=terror*ktp

		setMotorTarget(left,target,speed-terror);
		setMotorTarget(right,target,speed+terror);

	}
	setMotorBrakeMode(left,motorBrake);
	setMotorBrakeMode(right,motorBrake);
	setMotorSpeed(left,0);
	setMotorSpeed(right,0);
}

void strafe(double target, int heading, int speed)
{
	total=getMotorEncoder(Omni)+target;
	setMotorBrakeMode(left,motorBrake);
	setMotorBrakeMode(right,motorBrake);
	setMotorBrakeMode(Omni,motorBrake);
	setMotorTarget(Omni,total,speed);
	while(abs(total-getMotorEncoder(Omni))>5)
	{
		terror = heading-getGyroHeadingFloat(gyro);
		if (terror > 180){
			terror = terror - 360;
		}
		else if (terror < -180){
			terror = terror + 360;
		}

		terror=terror*ktp

		setMotorSpeed(left,-terror/2);
		setMotorSpeed(right,terror/2);
	}

	setMotorBrakeMode(Omni,motorBrake);
	setMotorSpeed(Omni,0);
	setMotorBrakeMode(left,motorBrake);
	setMotorBrakeMode(right,motorBrake);
	setMotorSpeed(left,0);
	setMotorSpeed(right,0);


}




void constant_drive(double target, int speed, int angle, double k){
	float change;
	ktp = ktp;
	resetMotorEncoder(left);
	resetMotorEncoder(right);
	while(abs(target-getmotorencoder(left))>10)
	{
		terror = angle-getGyroHeadingFloat(gyro);

		if (terror > 180){
			terror = terror - 360;
		}
		else if (terror < -180){
			terror = terror + 360;
		}
		change = terror*ktp;
		setMotor(left,speed-change);
		setMotor(right,speed+change);
	}
	stopMotor(left);
	stopMotor(right);
	setMotorBrakeMode(left,motorBrake);
	setMotorBrakeMode(right,motorBrake);
	wait1Msec(300);
}


void Turn_to(double target,double p, double d){ //currently a PD loop.
	terror = target - getGyroHeading(gyro);
	tlasterror = terror;
	double tp = p;
	double td = d;

	while(abs(terror) > 1||abs(terror-tlasterror)>0.3){
		terror = target - getGyroHeading(gyro);

		if (terror > 180){
			terror = terror - 360;
		}
		else if (terror < -180){
			terror = terror + 360;
		}

		tderivative = td * (terror - tlastError);
		tlastError = terror;

		if(terror == 0){
			tderivative = 0;
		}
		power = tp * terror + td * tderivative;

		setMotorSpeed(left,power*-1);
		setMotorSpeed(right,power*1);

		wait1Msec(30);

	}
	stopMotor(left);
	stopMotor(right);

}
