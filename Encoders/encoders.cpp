#include <encoders.h>
#include <Arduino.h>

int val; 

  int rightencoderPinA = 53;
  int rightencoderPinB = 52;
  float rightencoderPos = 0;
  int rightencoderPinALast = LOW;
  int n = LOW;
  float r;

  int leftencoderPinA = 24;
  int leftencoderPinB = 22;
  float leftencoderPos = 0;
  int leftencoderPinALast = LOW;
  int m = LOW;
  float l;

 float f;
 float Position;
 float fun;
 float Velocity;
 int lastT;

int beginning;
float yup;
float t;
float mee;

 void Encoders::setupEncoder() { 
  pinMode(28, OUTPUT);
  digitalWrite(28,HIGH);

  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);
  
   pinMode (rightencoderPinA,INPUT);
   pinMode (rightencoderPinB,INPUT);

   pinMode (leftencoderPinA, INPUT);
   pinMode (leftencoderPinB, INPUT);

   Serial.begin (9600);
   
 } 

 
void Encoders::UpdateRightEncoder(bool reset) {

  n = digitalRead(rightencoderPinA);
  if ((rightencoderPinALast == LOW) && (n == HIGH)) {
    if (digitalRead(rightencoderPinB) == LOW) {
      if (reset == 0){
        rightencoderPos--;
      }
     fun--;
  }else {
    if (reset == 0){
      rightencoderPos++;
    }
    fun++;
   }
 }   
 rightencoderPinALast = n;

}


void Encoders::UpdateLeftEncoder(bool reset) {

   m = digitalRead(leftencoderPinA);

    if ((leftencoderPinALast == LOW) && (m == HIGH)) {
      if (digitalRead(leftencoderPinB) == LOW) {
        if (reset == 0){
        leftencoderPos++;
      }
       fun++;
      } else {
        if (reset == 0){
          leftencoderPos--;
      }
       fun--;
     }
   }   
   leftencoderPinALast = m;
 }


float rightPos;
float leftPos;
int timesincereset;
float Newposition;
float positionsum;
float positioncount;
float lastpositionsum;
float averageposition;

void Encoders::getPosandV(bool reset1, bool slowdownreset, float Newposition) {

  if(reset1 == 0){
    timesincereset = millis();
  }

  timesincereset = millis() - timesincereset;

  if (timesincereset < 2000){
    leftencoderPos = 0;
    rightencoderPos = 0;
    lastpositionsum = 0;
    positionsum = 0;
    positioncount = 0;
  }

  float elapsed = micros() - beginning;

  r = rightencoderPos / 480;
  l = leftencoderPos / 480;

  Position = (r + l) * 9 * 3.1415 / 2 + Newposition;
/*
  if(slowdownreset == 0){
    positioncount++;

    positionsum = Position + lastpositionsum;
    lastpositionsum = positionsum;
    averageposition = positionsum / positioncount;
  }

  /*
  Serial.print(l);
  Serial.print(" / ");
  Serial.print(leftencoderPos);
  Serial.print(" / ");
  Serial.println(Position);
  */

  mee = fun;
  yup = (mee / (480 * 2));
  float t = (elapsed / 1000000);
  Velocity = 9 * 3.415 * yup / t;

  beginning = micros();
  fun = 0;
}