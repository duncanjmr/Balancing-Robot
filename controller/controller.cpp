#include <controller.h>
#include <PS2.h>
#include <Arduino.h>

PS2X ps2x;

#define pressures   true
#define rumble      false


int error = 0;
byte type = 0;
byte vibrate = 0;

float MyLStickY;
float MyLStickX;
float MyRStickY;
float MyRStickX;

bool MyStartButton;
bool MyXButton;
bool dMyXButton;
bool MylastXButton;

void Controller::setupController(){
	delay(100);

	error = ps2x.config_gamepad(pressures, rumble);
	
	if(error == 0){
    	Serial.println("Found Controller, configured successful ");
    } 

    else if(error == 1){
    	Serial.println("No controller found");
	}
	else{
		Serial.println("nothing is happening");
		delay(10000);
	}
	Serial.println("Neeeeeeext...");
	delay(200);

	MyXButton = 0;

	    
}

void Controller::updateController(){

	ps2x.read_gamepad(false, vibrate);

	MyStartButton = ps2x.Button(PSB_START);
	MyLStickY = ps2x.Analog(PSS_LY);
	MyLStickX = ps2x.Analog(PSS_LX);
	MyRStickY = ps2x.Analog(PSS_RY);
	MyRStickX = ps2x.Analog(PSS_RX);
	dMyXButton = ps2x.NewButtonState(PSB_BLUE);

	if(dMyXButton == 1){
		if(MylastXButton == 1){
			MyXButton = 0;
		}
		if(MylastXButton == 0){
			MyXButton = 1;
		}
	}
	MylastXButton = MyXButton;
}