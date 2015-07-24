#ifndef controller_h
#define controller_h

#include <Arduino.h>

class Controller
{
public:
	void setupController();
	void updateController();
	bool readStartButton() const {return MyStartButton;}
	float readLStickY() const {return MyLStickY;}
	float readLStickX() const {return MyLStickX;}
	float readRStickY() const {return MyRStickY;}
	float readRStickX() const {return MyRStickX;}
	bool readXButton() const {return MyXButton;}
private:
	bool MyStartButton;
	float MyLStickY;
	float MyLStickX;
	float MyRStickY;
	float MyRStickX;
	bool MyXButton;
};
#endif