/************************************************************************************\
tweeter.ino utilizes an Anduino Uno R3 using an Ethernet shield to connect to twitter
in order to control a coffee pots functions. Using a custom-build PCB, the arduino was
connected to the coffee pot.

This code was written by Eric Bouwkamp & Kristen Herder of Calvin College
For: Engineering 332, Professor Yoon Kim. 
Date: May 6, 2013
\************************************************************************************/
#include <SPI.h>
#include <Ethernet.h>
#include <Time.h>
#include "server.h"
#include "CoffeePot.h"

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x24, 0x1F };
Coffee coffeepot;
EthernetClient client;
char serverName[] = "api.twitter.com";
boolean requested; 
unsigned long lastAttemptTime = 0;
String currentLine = "";
String tweet = "";
String valueReq = "";
String dateString = "";
boolean readingTweet = false;
boolean readingReq = false;
boolean readingDate = false;
boolean timeSetBoolean = false;
int hoursVal;
int minutesVal;

void setup() {
  currentLine.reserve(256);
  tweet.reserve(150);
  Serial.begin(9600);
  Serial.println("Welcome to the ArduCoffee Coffee Pot!");
  Serial.println("Now Initializing!");
  Serial.print("Connecting to the Internet: ");
  Ethernet.begin(mac);
  Serial.print("Done!\n");
  connectToServer();  
}

void loop() {
  coffeepot.checkTime();
  if (client.connected()) {
    if (client.available()) {
      char inChar = client.read();
      currentLine += inChar; 
      if (inChar == '\n') {
        currentLine = "";
      } 
      if ( currentLine.endsWith("<text>")) {
        readingTweet = true; 
        tweet = "";
      }
      if (currentLine == "date: "){
        readingDate = true;
        dateString = "";
      }
      if (readingTweet) {
        if (inChar != '<') {
          if (inChar != '>')
            tweet += inChar;
        } 
        else {
          readingTweet = false;
          Serial.print("\tYour Tweet Was: ");
          Serial.print(tweet);
          Serial.print("\n\tCurrent Arduino Time is: ");
          Serial.print(hour());
          Serial.print(":");
          Serial.print(minute());
          Serial.print(":");
          Serial.print(second());
          Serial.print("\n");
          coffeepot.coffeePotAction(tweet);  
          client.stop(); 
        }
      }
      if (readingDate){
       if (inChar != '\n')
          dateString += inChar;
       else{
         readingDate = false;
         if (!timeSetBoolean){
             timeSetBoolean = true;
             coffeepot.convertTime(dateString.substring(18,20),dateString.substring(21,23), hoursVal, minutesVal,1);
             setTime(hoursVal, minutesVal, 0,1,1,13);
         }
       }       
      }
    }   
  }
  else if (millis() - lastAttemptTime > 25000) {
    connectToServer();
  }
}

void connectToServer() {
  Serial.print("\nConnecting to Twitter: ");
  if (client.connect("api.twitter.com", 80)) {
    client.println("GET /1/statuses/user_timeline.xml?screen_name=ArduCoffee&count=1 HTTP/1.1");
    client.println("HOST: api.twitter.com");
    client.println("Connection: close");
    client.println();
  }
  Serial.print("Done!\n ");
  lastAttemptTime = millis();
}
  
  
  
  
