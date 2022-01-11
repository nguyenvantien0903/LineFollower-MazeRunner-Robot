// C++ code
//
/*
  This program blinks pin 13 of the Arduino (the
  built-in LED)
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(9,8,7,6,5,4);

#define RED1 13
#define GREEN1 12
#define YELLOW1 11
#define button1 3
#define button2 2

int cnt=0,timedelay=1500,x1,x2,x1pushed=0,x2pushed=0,cnt2;
int gio,phut,giay,setgio,setphut,setgiay;
double smalltime=0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(RED1, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(YELLOW1, OUTPUT);
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
}

void delayWhileRead(int delayT){
  for(int i=1;i<=1000;i++){
    x1=digitalRead(button1);
	x2=digitalRead(button2);
    if(x1==0){
      x1pushed=1;
    }
    if(x1==1&&x1pushed==1){
      x1pushed=0;
      cnt++; 
      lcd.setCursor(0, 1);
      lcd.print(cnt);
    }
    if(x2==0){
      timedelay=timedelay-100;
      if(timedelay<100)
        timedelay=1500;
      lcd.setCursor(5, 1);
      lcd.print(timedelay);
      delay(100);
    }
    
    delay(delayT/1000);
  }
}

void nhay(int k){
  digitalWrite(k, HIGH);
}

void tat(int k){
  digitalWrite(k, LOW);
}

void nhay1(int delayT){
  nhay(RED1);
  delayWhileRead(delayT);
  tat(RED1);
  nhay(GREEN1);
  delayWhileRead(delayT);
  tat(GREEN1);
  nhay(YELLOW1);
  delayWhileRead(delayT);
  tat(YELLOW1);
}

void nhay2(int delayT){
  nhay(YELLOW1);
  delayWhileRead(delayT);
  tat(YELLOW1);
  nhay(GREEN1);
  delayWhileRead(delayT);
  tat(GREEN1);
  nhay(RED1);
  delayWhileRead(delayT);
  tat(RED1);
}

void mode1(){
  lcd.setCursor(0, 0);lcd.print("Time: ");
  lcd.setCursor(6, 0);lcd.print(gio);lcd.print(":");
  lcd.setCursor(9, 0);lcd.print(phut);lcd.print(":");
  lcd.setCursor(12, 0);lcd.print(giay);
  
  lcd.setCursor(0, 1);lcd.print("SetT: ");
  lcd.setCursor(6, 1);lcd.print(setgio);lcd.print(":");
  lcd.setCursor(9, 1);lcd.print(setphut);lcd.print(":");
  lcd.setCursor(12, 1);lcd.print(setgiay);
}


void mode2(){
  lcd.setCursor(0, 0);lcd.print("CaidatGio: ");
  
  lcd.setCursor(0, 1);lcd.print("SetT: ");
  lcd.setCursor(6, 1);lcd.print(setgio);
}

void mode3(){
  lcd.setCursor(0, 0);lcd.print("CaidatPhut: ");
  
  lcd.setCursor(0, 1);lcd.print("SetT: ");
  lcd.setCursor(6, 1);lcd.print(setphut);
}


void mode4(){
  lcd.setCursor(0, 0);lcd.print("CaidatGiay: ");
  
  lcd.setCursor(0, 1);lcd.print("SetT: ");
  lcd.setCursor(6, 1);lcd.print(setgiay);
}

void updateTime(){
  if(giay==60){
    giay=0;
    phut++;
    lcd.clear();
  }
  if(phut==60){
  	phut=0;
    gio++; 
    lcd.clear();
  }
  if(gio==24){
   	gio=0; 
    lcd.clear();
  }
}

void delayStillCount(int time){
  smalltime+=1.0*time/900;//change this
  if(smalltime>=1){
    updateTime();
    giay++;
    Serial.print(giay);
    smalltime-=1;
  }
  delay(time);
}

void aaa(int time){
  for(int i=1;i<=time;i++){
    x1=digitalRead(button1);
  	x2=digitalRead(button2);
    if(x1==0){
      x1pushed=1;
    }
    if(x1==1&&x1pushed==1){
      x1pushed=0;
      cnt++;
      lcd.clear();
    }
    
    if(x2==0&&cnt%4!=0){
      delayStillCount(100);
      if(cnt%4==1){
        setgio++;
        if(setgio==24){
          setgio=0;
          lcd.clear();
        }
        mode2();
      }
      if(cnt%4==2){
        setphut++;
        if(setphut==60){
          setphut=0;
          lcd.clear();
        }
        mode3();
      }
      if(cnt%4==3){
        setgiay++;
        if(setgiay==60){
          setgiay=0;
          lcd.clear();
        }
        mode4();
      }
    }
    delayStillCount(1);
  }
}


void loop()
{
  aaa(900);
  if(cnt%4==0){
  	mode1();
  }
  if(cnt%4==1){
  	mode2();
  }
  if(cnt%4==2){
  	mode3();
  }
  if(cnt%4==3){
  	mode4();
  }
}