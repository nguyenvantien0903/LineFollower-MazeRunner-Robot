#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include "kmotor.h"
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,16,2);
kmotor _kmotor(true);

int initial_motor_speed = 200;

float Kp=10,Kd=50,Ki=0;
float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;
int TB4,TB5,TB6,TB7;
int maxVal4=0,maxVal5=0,maxVal6=0,maxVal7=0;
int minVal4=2000,minVal5=2000,minVal6=2000,minVal7=2000;
int pre=0;
int sensor[4] = {0, 0, 0, 0};

void READ() {
    minVal4=min(minVal4,analogRead(A4));
    minVal5=min(minVal5,analogRead(A5));
    minVal6=min(minVal6,analogRead(A6));
    minVal7=min(minVal7,analogRead(A7));
    maxVal4=max(maxVal4,analogRead(A4));
    maxVal5=max(maxVal5,analogRead(A5));
    maxVal6=max(maxVal6,analogRead(A6));
    maxVal7=max(maxVal7,analogRead(A7));
}

void docSoLieu(){
  TB4=850;
  TB5=850;
  TB6=850;
  TB7=850;
}

void read_sensor_values()
{
  if(analogRead(A5)>=TB5)sensor[0]=1;
  else sensor[0]=0;
  if(analogRead(A7)>=TB7)sensor[1]=1;
  else sensor[1]=0;
  if(analogRead(A6)>=TB6)sensor[2]=1;
  else sensor[2]=0;
  if(analogRead(A4)>=TB4)sensor[3]=1;
  else sensor[3]=0;


  
    Serial.print(sensor[0]);
    Serial.print("\t");
    Serial.print(sensor[1]);
    Serial.print("\t");
    Serial.print(sensor[2]);
    Serial.print("\t");
    Serial.println(sensor[3]);
  if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0))
    error = 3;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0))
    error = 2;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0))
    error = 1;
  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0))
    error = 0;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0))
    error = -1;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1))
    error = -2;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1))
    error = -3;
  else error = 100;
}

void calculate_pid()
{
  P = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;
}

void motor_control()
{
  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value-30;

  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);
  Serial.println(left_motor_speed);
  Serial.println(right_motor_speed);
  if(left_motor_speed<=right_motor_speed)pre=1;
  else pre=2;
  _kmotor.tien(0, left_motor_speed);//M1
  _kmotor.tien(1, right_motor_speed);//M2

}

float getDistance(int trig,int echo){
  float dem=0;
    pinMode(trig,OUTPUT);
    digitalWrite(trig,LOW);
    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);
    pinMode(echo, INPUT);
    dem = pulseIn(echo,HIGH,30000)/58.0;
    if(dem==0) dem=30;
    return dem;
}

void setup() {
  _kmotor.cauhinh();
  Serial.begin(9600);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  docSoLieu();
}

void loop(){
    read_sensor_values();
    if(error==100){
      if(pre==1){
        _kmotor.tien(0, 0);//M1
        _kmotor.tien(1, 200);//M2
      }else{
        _kmotor.tien(0, 200);//M1
        _kmotor.tien(1, 0);//M2
      }
    }else{
      calculate_pid();
      motor_control(); 
    }
    
}