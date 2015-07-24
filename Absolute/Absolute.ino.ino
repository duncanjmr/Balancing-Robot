#include <controller.h>
#include <filter.h>
#include <encoders.h>
#include <Wire.h>
#include <FreeSixIMU.h>
#include <FIMU_ITG3200.h>
#include <FIMU_ADXL345.h>
#include <PS2.h>

//////////////////Presets///////////////////

const float Calibration = -2.5;
const float Antidrift = 0;

////////////////////////////////////////////

float pi = 3.1415;

float delta; 

float Input;
float Output;
float velocity;
float output;
float forwbackw;
float pos;
float RequestedSpeed;
bool Movement = 0;


int resettime;
bool StartButton = false;
int stillness;



Filter filter;
Encoders encoders;
Controller controller;

void updateRightEncoder(){
  encoders.UpdateRightEncoder(stillness);
}

void updateLeftEncoder(){
  encoders.UpdateLeftEncoder(stillness);
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  controller.setupController();
  encoders.setupEncoder();
  filter.setupIMU(Calibration);
  
  attachInterrupt(53, updateRightEncoder, CHANGE);
  attachInterrupt(24, updateLeftEncoder, CHANGE);

  pinMode(48, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  
  resettime = millis();
  StartButton = 0;
}

int reset;
int slowing;
int lastslowing;
float newposition;
float averagepos;
float leftspeed;
float rightspeed;

void loop() {
  // put your main code here, to run repeatedly:

encoders.getPosandV(StartButton, stillness, newposition);
velocity = encoders.sendV();
pos = encoders.sendPos();
averagepos = encoders.sendAvgPos();

controller.updateController();
StartButton = - 1 * controller.readStartButton() + 1;
float LStickY = controller.readLStickY();
float LStickX = controller.readLStickX() - 128;
float RStickY = controller.readRStickY();
float RStickX = controller.readRStickX() - 128;
bool XButton = controller.readXButton();

RequestedSpeed = LStickY / 51 - 2.49;

stillness = 0;
  
filter.getAngles(stillness, pos, velocity, RequestedSpeed, averagepos);
Input = filter.sendData();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
delta = filter.sendDerivative();
newposition = filter.sendresetPos();


Output = -sin(Input * 2 * pi / 180) * 255 * 4.25 * sin(Input * pi / 180) ;

leftspeed = Output + RStickX;
rightspeed = Output - RStickX;

// right motor
if(rightspeed < 0){
  digitalWrite(49, LOW);
  digitalWrite(48, HIGH);
  analogWrite(3, -rightspeed);
}
if(rightspeed > 0){digitalWrite(49, HIGH);
  digitalWrite(48, LOW);
  analogWrite(3, rightspeed);
}

if(leftspeed < 0){
  digitalWrite(50, HIGH);
  digitalWrite(51, LOW);
  analogWrite(2, -leftspeed);
}
if(leftspeed > 0){
  digitalWrite(50, LOW); 
  digitalWrite(51, HIGH);
  analogWrite(2, leftspeed);
}
/*
if(Output < 0){
  digitalWrite(49, LOW);
  digitalWrite(48, HIGH);
  digitalWrite(50, HIGH);
  digitalWrite(51, LOW);
  analogWrite(2, -Output);
  analogWrite(3, -Output);
}

if(Output > 0){
  digitalWrite(49, HIGH);
  digitalWrite(48, LOW);
  digitalWrite(50, LOW); 
  digitalWrite(51, HIGH);
  analogWrite(2, Output);
  analogWrite(3, Output);
}
*/


  Serial.print(Input);
  Serial.print("    /    "); 
  Serial.print(rightspeed);
  Serial.print("    /    ");
  Serial.print(leftspeed);
  Serial.print("    /    ");
  Serial.println(LStickX); 
  

 

 
}
