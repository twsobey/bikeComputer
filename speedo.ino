/*
First crack at a bike computer
Version 0.8 - 15-Sept-2020

Still to do:
* DONE - total time
* DONE - instant speed
* DONE - total distance
* DONE - average speed
* DONE - max speed
* LCD output
* auto pause?
* export data
*/
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

const float mmToMiles = 25.4*12*5280;
const int   wheelCirc = 2111;           //2111mm circ for 700x25c wheel
const int   millisToMinutes = 60000;
const int   millisToHours = millisToMinutes * 60;
const float msToMph = 2236.936;         //millimeters per microsec to MPH

bool  pause = 0;
float dist = 0;
int   startTime = 0;
int   seconds;
int   timeHolder = 0;
float aveSpeed = 0;
float curSpeed;
long  speedTime;
float maxSpeed;


void setup() {
  lcd.init(); // initialize the lcd
  lcd.backlight();
  pinMode(11, INPUT_PULLUP);
  pause = digitalRead(11);
  Serial.begin(9600);
  while(pause == digitalRead(11));    //Wait to start timer until the wheel is moving
  Serial.println("Start...");
  startTime = millis();
  speedTime = micros();
}

void loop() {
  if(digitalRead(11)==0)
  {
    //Calculate total distance traveled based off of wheel size
    dist+=(wheelCirc/mmToMiles);      //Convert one wheel rotation to miles
    lcd.setCursor(0, 0);              // move cursor to   (0, 0)    
    //lcd.print("Dist: ");
    lcd.print(dist, 2);
    lcd.print(" m");

    Serial.print("Distance: ");
    Serial.print(dist, 4);
    Serial.print(" Miles\t");

    //Calculate total time since starting ride
    Serial.print("Time: ");
    seconds = (millis()-startTime)/1000;
    timeHolder = seconds/3600;
    lcd.setCursor(9, 0);              // move cursor to   (0, 0)
    lcd.print(timeHolder);
    lcd.print(":");
    Serial.print(timeHolder);
    Serial.print(":");
    timeHolder = (seconds/60)%60;
    if(timeHolder<10) {
      Serial.print("0");
      lcd.print("0");
    }
    lcd.print(timeHolder);
    lcd.print(":");
    Serial.print(timeHolder);
    Serial.print(":");
    timeHolder = seconds%60;
    if(timeHolder<10){
      Serial.print("0");
      lcd.print("0");
    }
    lcd.print(timeHolder);
    Serial.print(timeHolder);
    Serial.print("\t");

    //Calculate pace for the ride
    Serial.print("Pace: ");
    aveSpeed = (dist*3600)/(seconds);
    if(seconds<=1)
      {
       Serial.print("0.0");
       Serial.print(" MPH\t");
       maxSpeed = 0;
       //lcd.clear();
      }
    else 
      { 
       lcd.setCursor(0, 1);              // move cursor to   (0, 0)
       lcd.print(aveSpeed);
       lcd.print(" ");   
       Serial.print(aveSpeed);
       Serial.print(" MPH\t"); 
      }

    //Calculate current speed
    Serial.print("Speed: ");
    curSpeed = ((wheelCirc * 1.0) / (micros() - speedTime))*msToMph; // Added " * 1.0" to ensure working with float
    if(curSpeed>maxSpeed) maxSpeed = curSpeed;
    lcd.setCursor(6, 1);              
    lcd.print(curSpeed, 1);
    lcd.print("  ");
    lcd.setCursor(11, 1);             
    lcd.print(maxSpeed);
    lcd.print(" ");
    Serial.print(curSpeed);
    Serial.print(" MPH\t");
    Serial.print("Max Speed: ");
    Serial.print(maxSpeed);
    Serial.print(" MPH\t");
    speedTime = micros();
    
    
    Serial.println(":)");
    while(digitalRead(11)==0);        //Wait until the wheel moves past before starting to look again
  }
}
