#include "Led.h"
#include "NodeMcuConstants.h"

/*
 Name:    SmartLock.ino
 Created: 3/28/2017 12:55:19 PM
 Author:  Nir Shchori
*/

// the setup function runs once when you press reset or power the board
Led *blue = NULL ;
Led *red = NULL;
void setup() {
  blue =new Led(D1);
  red =new Led(D0);
}

// the loop function runs over and over again until power down or reset
void loop() {
  delay(100);
  
  (*blue).turnON();
  (*red).turnOFF();
  delay(100);
  (*blue).turnOFF();
  (*red).turnON();
}

