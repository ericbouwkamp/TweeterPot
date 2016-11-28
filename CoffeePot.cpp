/************************************************************************************\
CoffeePot.cpp contains the definitions for a Coffee class and its functions in order 
to control the coffee maker using an arduino.

This code was written by Eric Bouwkamp & Kristen Herder of Calvin College
For: Engineering 332, Professor Yoon Kim. 
Date: April 29, 2013
\************************************************************************************/
#include "CoffeePot.h"

int autoOn =  A0;
int hourSet = 3;
int autoOff = 2;
int keepWarm = A1;
int minSet = 4;
int program = 5;
int onOff = 6;
int powerMain = 7;

/****************************************************************\
	Coffee() initializes the coffee class and the private
	variables that control the coffee pots status
\****************************************************************/
Coffee::Coffee(){
  coffeePotOn = false;
  brewing = false; 
  programmed = false;
  pinMode(autoOn, OUTPUT);
  pinMode(keepWarm, OUTPUT); 
  pinMode(autoOff, OUTPUT);
  pinMode(hourSet, OUTPUT);
  pinMode(minSet, OUTPUT);
  pinMode(program, OUTPUT);
  pinMode(onOff, OUTPUT);
  pinMode(powerMain, OUTPUT);
  digitalWrite(autoOn, LOW);
  digitalWrite(keepWarm, LOW);
  digitalWrite(autoOff, LOW);
  digitalWrite(hourSet, LOW);
  digitalWrite(minSet, LOW);
  digitalWrite(program, LOW);
  digitalWrite(onOff, LOW);
  digitalWrite(powerMain, LOW);
 }

/****************************************************************\
	coffeePotAction() is the Main Controller function that Calls 
	functions based on the Tweet recieved from twitter and the 
	Coffee Pot's Current State.													
\****************************************************************/
void Coffee::coffeePotAction(String tweet){
	tweet.toLowerCase();
	if (tweet == "brew coffee on" && !brewing ){
		if (!coffeePotOn){
			coffeePotPowerOn();
		}
		brewCoffeeOn();
	}
	else if (tweet == "brew coffee off" && brewing){
		brewCoffeeOff();
	}
	else if (tweet == "power on"  && !coffeePotOn){
		coffeePotPowerOn();
	}
	else if (tweet == "power off" && coffeePotOn){
		coffeePotPowerOff();
	}
	else if (tweet.substring(0,14) == "make coffee at" && !programmed)
		programCoffeeMaker(tweet);
	else if (tweet == "turn off auto program" && programmed)
		unprogramCoffeeMaker();
}

/****************************************************************\
	setCoffeeMakerTime() is a function that sets the coffee 
	makers time appropriately from the time read from twitter.
\****************************************************************/
void Coffee::setCoffeeMakerTime()
{
   int hourVal = hour();
   int minutesVal = minute();
   if (hourVal != 0)
      for (unsigned i = 0; i < hourVal; i++)
      {
	digitalWrite(hourSet, HIGH);
	delay(410);
	digitalWrite(hourSet, LOW);  
      }
  
   if (minutesVal != 0)
      for (unsigned i = 0; i <= minutesVal; i++)
      {
        digitalWrite(minSet, HIGH);
        delay(425);
        digitalWrite(minSet, LOW);
      }  
}

/****************************************************************\
	convertTime() is a function that reads the time from twitter
	as strings, and returns the values as integers to be used 
	easily in other functions of the coffee pot class.
\****************************************************************/
void Coffee::convertTime(String hourString, String minutesString, int & hourValue, int & minuteValue, int adjust){
	int addFactorBig;
	int addFactorSmall;
	if (hourString == "00") hourValue = 0;
	else if (hourString == "01") hourValue = 1;
	else if (hourString == "02") hourValue = 2;
	else if (hourString == "03") hourValue = 3;
	else if (hourString == "04") hourValue = 4;
	else if (hourString == "05") hourValue = 5;
	else if (hourString == "06") hourValue = 6;
	else if (hourString == "07") hourValue = 7;
	else if (hourString == "08") hourValue = 8;
	else if (hourString == "09") hourValue = 9;
	else if (hourString == "10") hourValue = 10;
	else if (hourString == "11") hourValue = 11;
	else if (hourString == "12") hourValue = 12;
	else if (hourString == "13") hourValue = 13;
	else if (hourString == "14") hourValue = 14;
	else if (hourString == "15") hourValue = 15;  
	else if (hourString == "16") hourValue = 16;
	else if (hourString == "17") hourValue = 17;
	else if (hourString == "18") hourValue = 18;
	else if (hourString == "19") hourValue = 19;
	else if (hourString == "20") hourValue = 20;
	else if (hourString == "21") hourValue = 21;
	else if (hourString == "22") hourValue = 22;
	else if (hourString == "23") hourValue = 23;
  
	if (minutesString.substring(0, 1) == "0") addFactorBig = 0;
	else if (minutesString.substring(0, 1) == "1") addFactorBig = 10;
	else if (minutesString.substring(0, 1) == "2") addFactorBig = 20;
	else if (minutesString.substring(0, 1) == "3") addFactorBig = 30;
	else if (minutesString.substring(0, 1) == "4") addFactorBig = 40;
	else if (minutesString.substring(0, 1) == "5") addFactorBig = 50;
    
	if (minutesString.substring(1,2) == "0") addFactorSmall = 0;
	else if (minutesString.substring(1,2) == "1") addFactorSmall = 1;
	else if (minutesString.substring(1,2) == "2") addFactorSmall = 2;
	else if (minutesString.substring(1,2) == "3") addFactorSmall = 3;
	else if (minutesString.substring(1,2) == "4") addFactorSmall = 4;
	else if (minutesString.substring(1,2) == "5") addFactorSmall = 5;
	else if (minutesString.substring(1,2) == "6") addFactorSmall = 6;
	else if (minutesString.substring(1,2) == "7") addFactorSmall = 7;
	else if (minutesString.substring(1,2) == "8") addFactorSmall = 8;
	else if (minutesString.substring(1,2) == "9") addFactorSmall = 9;
  
	minuteValue = addFactorSmall + addFactorBig;
        
        if (adjust = 1)
        {
          if (hourValue >= 4)
	    hourValue = hourValue - 4;
	  else 
	    hourValue = (hourValue - 4) + 24;
        }
}

/****************************************************************\
	brewCoffeeOn() is a function that writes high and low values
	out to a pin of the arduino in order to turn on the brewing 
	process of the coffee maker.
\****************************************************************/
void Coffee::brewCoffeeOn(){
   digitalWrite(onOff, HIGH);
   delay(300);
   digitalWrite(onOff, LOW);
   brewing = true;
}

/****************************************************************\
	brewCoffeeOn() is a function that writes high and low values
	out to a pin of the arduino in order to turn off the brewing 
	process of the coffee maker.
\****************************************************************/
void Coffee::brewCoffeeOff(){
	digitalWrite(onOff, HIGH);
	delay(300);
	digitalWrite(onOff, LOW);
	brewing = false;
}

/****************************************************************\
	coffeePotPowerOn() is a function that writes a high value
	out to a pin of the arduino in order to turn on the main 
	power of the coffee pot.
\****************************************************************/
void Coffee::coffeePotPowerOn(){
	digitalWrite(powerMain, HIGH);
	coffeePotOn = true;
	Serial.println("\t\tCoffee Pot Power On");
	delay(2000);
	setCoffeeMakerTime();
        delay(2000);
}

/****************************************************************\
	coffeePotPowerOn() is a function that writes a low value
	out to a pin of the arduino in order to turn off the main 
	power of the coffee pot.
\****************************************************************/
void Coffee::coffeePotPowerOff(){
	digitalWrite(powerMain, LOW);
	coffeePotOn = false;
	Serial.println("\t\tCoffee Pot Power Off");
	delay(2000);
}

/****************************************************************\
	programCoffeeMaker() is a function that initializes private
	variables that will be tested frequently in order to allow
	a user to program the coffee pot to start at a certain time
\****************************************************************/
void Coffee::programCoffeeMaker(String tweet){
	convertTime(tweet.substring(15,17), tweet.substring(18,20), programHour, programMinute, 0);
        if (programHour <= 19)
          programHour = programHour + 4;
        else
          programHour = (programHour + 4) - 24;
        Serial.print("Coffee Maker Set to Turn on at: ");
        Serial.print(programHour);
        Serial.print(":");
        Serial.print(programMinute);
        Serial.print("\n");
	programmed = true;
}

/****************************************************************\
	unprogramCoffeeMaker() is a function that uninitializes 
	private variables to disable the automatic start of the
	coffee maker.
\****************************************************************/
void Coffee::unprogramCoffeeMaker(){
	programHour = 0;
	programMinute = 0;
	programmed = false; 
 }
 
/****************************************************************\
	checkTime() is a function that checks the current time 
	against the programmed time, and turns on the coffee pot if 
	the time matches
\****************************************************************/
void Coffee::checkTime(){  
	if (hour() == programHour && minute() == programMinute && !brewing && programmed){
		if (!coffeePotOn)
			coffeePotPowerOn();
		Serial.print("\t\tProgram on!");
		brewCoffeeOn();  
  }
}
