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

const long oneSecond = 1000;
const long x2delay = 500;
const long lcddelay = 500;
const int analogInPin = A0;
const int analogOutPin = 11;
const long Nmode=5;

int sensorValue = 0;        
int outputValue = 0; 

unsigned long previousMillis = 0,prex2=0,currentMillis,prelcd=0;
int cnt=0,timedelay=1500,x1,x2,x1pushed=0,x2pushed=0,cnt2,cntchange=0;
int gio,phut,giay,setgio=12,setphut=12,setgiay=12;
double smalltime=0;

void setup()
{
  Serial.begin(115200);
  lcd.begin(16, 2);
  pinMode(RED1, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(YELLOW1, OUTPUT);
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
/*
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
*/
void mode1(){
  //if(currentMillis-prelcd>=lcddelay){
  //  prelcd=currentMillis;
    lcd.setCursor(0, 0);lcd.print("Time:           ");
    lcd.setCursor(6, 0);lcd.print(gio);lcd.print(":");
    lcd.setCursor(9, 0);lcd.print(phut);lcd.print(":");
    lcd.setCursor(12, 0);lcd.print(giay);
    //Serial.println("Time: "+String(gio)+":"+String(phut)+":"+String(giay));

    lcd.setCursor(0, 1);lcd.print("SetT:           ");
    lcd.setCursor(6, 1);lcd.print(setgio);lcd.print(":");
    lcd.setCursor(9, 1);lcd.print(setphut);lcd.print(":");
    lcd.setCursor(12, 1);lcd.print(setgiay);
 // }
  //Serial.println("SetT: "+String(setgio)+":"+String(setphut)+":"+String(setgiay));
}


void mode2(){
  lcd.setCursor(0, 0);lcd.print("CaidatGio:      ");
  lcd.setCursor(0, 1);lcd.print("SetT:           ");
  lcd.setCursor(6, 1);lcd.print(setgio);
 // Serial.println("CaidatGio: ");
 //Serial.println("setT: "+String(setgio));
}

void mode3(){
  lcd.setCursor(0, 0);lcd.print("CaidatPhut:     ");
  lcd.setCursor(0, 1);lcd.print("SetT:           ");
  lcd.setCursor(6, 1);lcd.print(setphut);
  //Serial.println("CaidatPhut: ");
  //Serial.println("setT: "+String(setphut));
}


void mode4(){
  lcd.setCursor(0, 0);lcd.print("CaidatGiay:     ");
  lcd.setCursor(0, 1);lcd.print("SetT:           ");
  lcd.setCursor(6, 1);lcd.print(setgiay);
  //Serial.println("CaidatGiay: ");
  //Serial.println("setT: "+String(setgiay));
}

void mode5(){
  
  lcd.setCursor(0, 0);lcd.print("    Test ADC:   ");
  lcd.setCursor(0, 1);lcd.print("                ");
  lcd.setCursor(6, 1);lcd.print(outputValue);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(analogOutPin, outputValue);
  //Serial.println("CaidatGiay: ");
  //Serial.println("setT: "+String(setgiay));
}

void updateTime(){
  giay++;
  if(giay==60){
    giay=0;
    phut++;
  }
  if(phut==60){
  	phut=0;
    gio++; 
  }
  if(gio==24){
   	gio=0; 
  }
}

void loop()
{
  //read sensor
  x1=digitalRead(button1);
  x2=digitalRead(button2);
  sensorValue = analogRead(analogInPin);
  
  //time counter
  currentMillis = millis();
  if(currentMillis - previousMillis >= oneSecond){   
    updateTime();
    if(gio==setgio&&phut==setphut&&giay==setgiay){
      Serial.print("It's time to duelllllllll!!!");
    }
    Serial.println(giay);
    previousMillis = currentMillis;
	Serial.println(currentMillis);
  }
  
  // set current time:command be like *change-time:hh:mm:ss/
  // set wakeup time:command be like *wake-time:hh:mm:ss/
  if(Serial.available()>0){ 
    String command=Serial.readStringUntil('/');   
    Serial.println(command);
    if(getValue(command,':',0)=="*change-time"){
      gio=getValue(command,':',1).toInt();
      phut=getValue(command,':',2).toInt();
      giay=getValue(command,':',3).toInt();
    }else{
      setgio=getValue(command,':',1).toInt();
      setphut=getValue(command,':',2).toInt();
      setgiay=getValue(command,':',3).toInt();
    }
  }
  
  //execute
  if(x1==0){
    x1pushed=1;
  }
  if(x1==1&&x1pushed==1){
    x1pushed=0;
    cnt++;
  }
  if(x2==0&&currentMillis-prex2>=x2delay){
    prex2=currentMillis;
    if(cnt%Nmode==1){
      setgio++;
      if(setgio==24){
        setgio=0;
      }
    }
    if(cnt%Nmode==2){
      setphut++;
        if(setphut==60){
          setphut=0;
        }
    }
    if(cnt%Nmode==3){
      setgiay++;
      if(setgiay==60){
        setgiay=0;
      }
    }
  }
  
  //output
  
  if(currentMillis-prelcd>=lcddelay){
    prelcd=currentMillis;
    if(cnt%Nmode==0)mode1();
    if(cnt%Nmode==1)mode2();
    if(cnt%Nmode==2)mode3();
    if(cnt%Nmode==3)mode4();
    if(cnt%Nmode==4)mode5();
  }
  delay(1);
  
}
