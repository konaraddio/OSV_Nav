#include <enes100.h>
#include <marker.h>
#include <rf_comm.h>

#include <SoftwareSerial.h>
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

SoftwareSerial mySerial(8, 9);//the ports to which the virtual RX and TX go in
Marker marker(108); //look at QR code's back for number
RF_Comm rf(&mySerial, &marker);

//assume OSV takes 1 second to rotate its wheels once
float timeToRotateOnce= 1000;

//OSV's CURRENT COORDINATES
float currentX;
float currentY;
float currentTheta;

//Point A
float Ax= 0.5;
float Ay= 0.325;

//Point B
float Bx= 0.5;
float By= 1.675;

void setup(){
    Serial.begin(9600);
    AFMS.begin();
}

void loop(){

    moveStraight(50, 50, 'F');
    //TODO Optimize exiting the wall after the basics work
    //EXIT THE WALL THROUGH POINT A (FOR NOW, WILL OPTIMIZE LATER)



    //TRAVEL TOWARDS FIRE SITE

    //FIRE SITE ROUND 1

    //FIRE SITE ROUND 2

    /*
    The statement below will run forever so this current loop() will never
    finish running. The purpose of this is for the loop() to never repeat.
    */
    while(1);
}

void updateCurrentLocation(){
    if(rf.updateLocation()){
        currentX= marker.x;
        currentY= marker.y;
        currentTheta= marker.theta;
    }else{
        //this shouldn't be happening....
        rf.println("oh lawrd");
        rf.println("updateCurrentLocation broke");
    }
}

/*
The functions below use speed (PWM: 0-255) and duration. For now, the duration
is time-based (milliseconds). It will be changed to rotations in the future.
The direction should be FORWARD or BACKWARD.
*/
void moveStraight(int speed, int duration, char direction){
    //motors on the left are motor[0] and motor [1] (top view of OSV with front facing North)
    //motors on the right are motor[2] and motor [3] (top view of OSV with front facing North)
    Adafruit_DCMotor *motor[4];
    for(int i= 0; i < 4; i++){
        motor[i]= AFMS.getMotor(i + 1);
        motor[i]->setSpeed(speed);
    }

    /*
    The if-statement has been kept outside of the loops intentionally. While this
    does take up more space, this is far more efficient than keeping the if-statements
    inside one loop.
    */
    if(direction == 'F'){
        for(int i= 0; i < 4; i++){
            //intentionally kept out of previous loop so motors are set up before moving
            motor[i]->run(FORWARD);
        }
    }else if(direction == 'B'){
        for(int i= 0; i < 4; i++){
            //intentionally kept out of previous loop so motors are set up before moving
            motor[i]->run(FORWARD);
        }
    }else{
        Serial.println("You need to use F or B, with quotes, for direction in moveStraight(~)");
    }

    delay(duration);

    //RELEASE is for turning off the motors
    for(int i= 0; i < 4; i++){
        //intentionally kept out of previous loop so motors are set up before moving
        motor[i]->run(RELEASE);
    }

    //the below is to check if it's working
    Serial.println("moveStraight(~) executed");
    Serial.println(speed);
    Serial.println(duration);
    Serial.println(direction);
    Serial.print("ms");
}
