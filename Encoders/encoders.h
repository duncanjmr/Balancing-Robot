#ifndef encoders_h
#define encoders_h

#include <Arduino.h>

class Encoders
{
public:
	void setupEncoder();
	void UpdateRightEncoder(bool reset);
	void UpdateLeftEncoder(bool reset);
	void getPosandV(bool reset1, bool slowdownreset, float newposition);
	float sendPos() const {return Position;}
	float sendV() const {return Velocity;}
	float sendAvgPos() const {return averageposition;}
private:
	float Velocity;
	float Position;
	float averageposition;

};
#endif