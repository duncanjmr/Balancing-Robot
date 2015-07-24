#ifndef filter_h
#define filter_h

#include "Arduino.h"

class Filter
{
public:
	void setupIMU(double calibration);
	void getAngles(bool still, double control, double velocity, double reqVel, float averagePos);
	float sendData() const {return angle;}
	float sendDerivative() const {return derivative;}
	float sendresetPos() const {return newposition1;}
	bool sendMotion() const {return stay;}

private:
	float angle;
	float derivative;
	int resetPos1;
	float newposition1;
	bool stay;
};
#endif