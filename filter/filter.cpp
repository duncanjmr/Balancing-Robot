#include "Arduino.h"
#include "filter.h"

#include <Wire.h>
#include <FreeSixIMU.h>
#include <FIMU_ITG3200.h>
#include <FIMU_ADXL345.h>

int yes;
int no;


FreeSixIMU IMU = FreeSixIMU();

int16_t rawAcc[3] = {0, 0, 0};
float rawGyro[3] =  {0.0, 0.0, 0.0};

const float GYRO_SCALE = 1;
const float ACC_SCALE = 0.1;

const float RADIAN_TO_DEGREE = float(180 / 3.1415);

double pitch = 0;

void updateIMU() {
  IMU.acc.readAccel(&rawAcc[0], &rawAcc[1], &rawAcc[2]);
  IMU.gyro.readGyro(&rawGyro[0], &rawGyro[1], &rawGyro[2]);
}

float getGyroYRate() {
  return rawGyro[0];
}

float loopTime;
float nowTime;
float lastStartTime;
float baseangle;
float dAngle;
float angle;
float lastAngle;
float lastAcc;
float calAngle;
float initialAngle = 0;

float getAccY() {
  float accXval = rawAcc[0] / ACC_SCALE;
  float accYval = rawAcc[1] / ACC_SCALE;
  accYval--; //-1g when lying down
  float accZval = rawAcc[2] / ACC_SCALE;

  float R = sqrt(pow(accXval, 2) + pow(accYval, 2) + pow(accZval, 2)); // Calculate the length of force vector
  float angleY = acos(accYval / R) * RADIAN_TO_DEGREE;
  return angleY;
}

void Filter::setupIMU(double calibration) {

  // put your setup code here, to run once:

  Serial.begin(9600);

  // IMU initialization
  Wire.begin(); // IMU connection
  delay(5);
  IMU.init(); // begin the IMU
  delay(5);

  updateIMU();
  delay(200); 

  calAngle = 0;
  initialAngle = 0;
  
  for(int i = 0; i < 50; i++) {
    updateIMU();
    calAngle = getAccY() + initialAngle;
    initialAngle = calAngle;
    delay(1);
  }
  baseangle = calAngle / 50 + calibration;
  pinMode(13, OUTPUT);
  /*
  digitalWrite(49, HIGH);
  digitalWrite(48, LOW);
  digitalWrite(50, LOW);
  digitalWrite(51, HIGH);
  analogWrite(2, 100);
  analogWrite(3, 100);

  delay(100);

    digitalWrite(49, HIGH);
  digitalWrite(48, LOW);
  digitalWrite(50, LOW);
  digitalWrite(51, HIGH);
  analogWrite(2, 0);
  analogWrite(3, 0);
*/
}

float lastreqVel;
float reqVelbegin;
float reqVeltime;
bool motion;
int stoptime;
bool restart = 0;
float newposition1;
float slowtime;
int lastreqVel1;

double reqVelA;
float reqVelB;
float reqVelB2;
float VOffset;
bool stay;
float reqVel1;
float lastvelocity;
float dvelocity;

float dposition;
float lastposition;
float fullposition;
float lastfullposition;
float velocityrestarttime;
float nowtime;
int velocityresettime;
float velocityrestarttime1;
float reqVelB2speed;
float acceleration;

void Filter::getAngles(bool still, double position, double velocity, double reqVel, float averagePos)
{
  

  // put your main code here, to run repeatedly
  nowTime = millis();
  loopTime = (nowTime - lastStartTime) / 1000;
  lastStartTime = nowTime;


  updateIMU();
  float yes = getGyroYRate();
  float no = getAccY();

  dAngle = 0.99 * ((yes) * loopTime) + 0.01 * (no - lastAcc);
  derivative = dAngle / loopTime;

  float fullAngle = dAngle + lastAngle;


  reqVelB2speed = (reqVelB2 - reqVelB) / loopTime;
  reqVelB = reqVelB2;

  VOffset =  reqVel - reqVelB;

  /*

    if(reqVel != 0 or reqVel != reqVelB){
      if(VOffset > 0.09){
        reqVelB2 = reqVelB + 0.1;
      }
      else if(VOffset < -0.09){
        reqVelB2 = reqVelB - 0.1;
      }
      if(VOffset > -0.09 && VOffset < 0.09){
        reqVelB2 = reqVelB;
      }
    }
*/

  reqVelB2 = reqVelB + 0.05 * VOffset;


/*
  reqVel1 = reqVel * 10;
  reqVelB = reqVelB2;
  VOffset = reqVel1 - velocity;

  if(still == 1){
    if(reqVel != 0 or reqVel != reqVelB){
      if(VOffset > 0.299){
        reqVelB2 = reqVelB + sqrt(VOffset) * 0.1;
      }
      else if(VOffset < 0.299){
        reqVelB2 = reqVelB - sqrt(VOffset) * 0.1;
      }
      if(VOffset > -3 && VOffset < 3){
        reqVelB2 = reqVelB;
      }
      angle = fullAngle + baseangle - averagePos + reqVelB / 10 + 0.001 * dvelocity / loopTime;
    }
  }

  if(still == 0){
      angle = fullAngle + baseangle - 0.4 * position - 0.07 * velocity + reqVelB / 10;
  }
*/
  dposition = position - lastposition;

  reqVel1 = reqVelB2 /  (2 * loopTime);

  lastposition = position;

  fullposition = (dposition - reqVelB2 / 2) + lastfullposition;

    lastfullposition = fullposition;
    acceleration = (velocity - lastvelocity) / loopTime;
    lastvelocity = velocity;

   angle = fullAngle + baseangle - 0.4  * fullposition - 0.07 * (velocity - reqVel1); //+ 0.001 * acceleration;


/*  if(abs(reqVelB2) > 0.2){
    motion = 1;
    velocityrestarttime1 = 0;
    velocityresettime = 0;

  }
  if(abs(reqVelB2) < 0.2){
    motion = 0;
  }

  if(velocityresettime == 0 && motion == 0){
    velocityresettime = millis();
  }

  if(velocityresettime != 0){
    velocityrestarttime1 = millis() - velocityresettime;
  }
    if((millis() - velocityresettime) > 1000 && velocityresettime != 0){
    velocityrestarttime1 = 1000;
  }


  angle = fullAngle + baseangle - (0.4 - reqVelB2 / 10) * fullposition - 0.08 * (velocity) * velocityrestarttime1 / 1000;
*/
Serial.print(reqVelB2);
    Serial.print("     ");
    Serial.print(reqVel1);
    Serial.print("     ");
    Serial.print(acceleration);


/*
  if(reqVel < 0.05 && reqVel > -0.05){

    if(motion == 1){
      stoptime = millis();

    }
    if((millis() - stoptime) < 500){
      slowtime = millis() - stoptime;

      angle = fullAngle + baseangle - 0.4 * position - lastreqVel1 - lastreqVel1 * ((500 - slowtime) / 500);
    }
    if((millis() - stoptime) > 2000){
       angle = fullAngle + baseangle - 0.4 * position - 0.07 * velocity;
    }
    else{
      angle = fullAngle + baseangle;
    }
  }

    
    if(millis() - stoptime > 2000){
      angle = fullAngle + baseangle - 0.4 * position - 0.07 * velocity;
      restart = 0;
    }
    if(millis() - stoptime < 2000){
      angle = fullAngle + baseangle - (millis() - stoptime) * 0.07 * velocity / 2000;
      restart = 1;
    }
    

  if(abs(lastreqVel - reqVel) > 0.05) {
    reqVelbegin = millis();
    lastreqVel = reqVel;
  }

  if(millis() - reqVelbegin < 500){
    reqVeltime = (nowTime - reqVelbegin) / 500;
  } 
  else{
    reqVeltime = 1;
  }

  if(reqVel > 0.05 || reqVel < -0.05){
    angle = fullAngle + baseangle - 0.4 * position - reqVeltime * (reqVel);
    motion = 1;
    restart = 0;
    lastreqVel1 = reqVel;
  }
  else{
    motion = 0;
  }



  if(position < 0){
      angle = fullAngle + baseangle + setting + pow(position / 5, 2);
    }

  if(position > 0){
      angle = fullAngle + baseangle + setting - pow(position / 5, 2);
    }
*/

  /*
  if(position < 8 && position > -8){
    if(velocity < 20 && velocity > -20){
      if(position < 0){
        angle = fullAngle + baseangle + setting - pow(position, 2);
      }
      else{
          angle = fullAngle + baseangle + setting + pow(position, 2);
        }
      }
    if(velocity > 20 || velocity < -20){  
      if(position < 0){
        angle = fullAngle + baseangle + setting + pow(position, 2);
      }
      else{
        angle = fullAngle + baseangle + setting - pow(position, 2);
      }
    }
  
  }


  if(position > 8 || position < -8){
    if(position < 0){
      angle = fullAngle + baseangle + setting  - 1.778;
    }
    else{
      angle = fullAngle + baseangle + setting + 1.778;
    }
  }
*/

lastAngle = fullAngle;
lastAcc = no;
}







