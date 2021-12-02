#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include "kmotor.h"

LiquidCrystal_I2C lcd(0x27,16,2);
kmotor _kmotor(true);

int R;
int L, SL, SR;
int dif;
int cnt = 0;
int dem=0;

void setup() {
  _kmotor.cauhinh();
  Serial.begin(9600);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  _kmotor.tien(1, 200);
  _kmotor.tien(0, 200);
  delay(400);
}

void chayrandom() {
  if (analogRead(A6) < 800) {
    if (analogRead(A4) > 800) {
      while (analogRead(A6) > 800) {
        _kmotor.tien(0, -150);
        _kmotor.tien(1, 150);
      }
    } else {
      while (analogRead(A6) > 800) {
        _kmotor.tien(0, 150);
        _kmotor.tien(1, -150);
      }
    }
  }
}

void doLine() {
  if (analogRead(A4) <= 800) {
    if (analogRead(A5) <= 800) {
      _kmotor.tien(1, 200);
      _kmotor.tien(0, 200);
    } else {
      _kmotor.tien(1, -200);
      _kmotor.tien(0, 200);
    }
  } else {
    if (analogRead(A5) <= 800) {
      _kmotor.tien(1, 200);
      _kmotor.tien(0, -200);
    } else {
      _kmotor.tien(0, -200);
      _kmotor.tien(1, -200);
      cnt=cnt+1;
      if(cnt>200){
          _kmotor.tien(1, 200);
          _kmotor.tien(0, 200);
          delay(500);
          while(true){
            _kmotor.stop();
          };
      }
    }
    chayrandom();
  }
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

void tunel1() {
    R = getDistance(2,12);
    L = getDistance(10,9);
    SR = getDistance(13,11);
    SL = getDistance(4,5);

    if (L >= 35 && L >= 35) {
      _kmotor.run(0, 200); 
      delay(100);
      _kmotor.stop();
    }
    if (L <= 12 || R <= 12 ){
      if (L > R){
        _kmotor.run(2, 180);
        delay(100);
        _kmotor.stop();
      } else {
        _kmotor.run(3, 180);
        delay(100);
        _kmotor.stop();
      }
    } 
    else 
        if (SL <= 4 || SR <= 4 ){
            if (SL > SR){
              _kmotor.run(2, 180);
              delay(100);
              _kmotor.stop();
            } else {
              _kmotor.run(3, 180);
              delay(100);
              _kmotor.stop();
            }
    }
    else{
      _kmotor.run(0, 200);  
      delay(100);
      _kmotor.stop();
    }
}

void loop(){   
  int Light = analogRead(A0);
  Serial.println(Light);
  if(Light > 300||(analogRead(A6) < 800&&analogRead(A7)<800&&analogRead(A4)<800&&analogRead(A5)<800)){  
    analogWrite(A1,1000);
    tunel1();
  }
  else{
    analogWrite(A1,0);
    doLine();
  }
}
