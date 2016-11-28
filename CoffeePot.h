/************************************************************************************\
CoffeePot.h contains the declarations for a Coffee class and its functions in order 
to control the coffee maker using an arduino.

This code was written by Eric Bouwkamp & Kristen Herder of Calvin College
For: Engineering 332, Professor Yoon Kim. 
Date: April 29, 2013
\************************************************************************************/
#ifndef Coffee_h
#define Coffee_h

#include <Time.h>
#include "Arduino.h"
#include <string>

class Coffee{
	public:
		Coffee();
		void coffeePotAction(String tweet);
		void brewCoffeeOn();
		void brewCoffeeOff();
		void coffeePotPowerOn();
		void coffeePotPowerOff(); 
		void programCoffeeMaker(String tweet);
		void unprogramCoffeeMaker();
		void setCoffeeMakerTime();
		void convertTime(String hoursString, String minutesString, int & hourValue, int & minuteValue, int adjust = 1);
		void checkTime();
	private:
		int programHour;
		int programMinute; 
		boolean coffeePotOn;
		boolean brewing;
		boolean programmed;
};


#endif
