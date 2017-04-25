/* RIB Library demo sketch to show library usage
 *  Tested with Arduino Uno/Nano 16 MHz 
 *  
 *  Expected connection 
 *  VCC of Transmitter to +5v on arduino
 *  GND of Transmitter to gnd on arduino  
 *  Pin D10 to ATAD (DATA) of RF 433 Transmitter
 *  Pin D5  by button ( with common 10kohm pull down resistor
 * 
 */

#include "RIBControl.h"
// Transmiter pin
#define RFPin 10
// led demo pin 
#define ledPin 13
// button pin definition 
#define buttonPin  5
// initialize library object itself
RibControl remote(RFPin);
// expected DIP Setup of remote controller 
//  don't forget, that DIP 1 is on left on remote
//  so for following value was dips set as 0111111111 
int dipSetup=0b1111111110;
// button number on remote controller 
byte buttonNum=0;

void setup()
{
	  pinMode(RFPin,OUTPUT);
	  pinMode(ledPin, OUTPUT);
	  pinMode(buttonPin,INPUT);
	  
	  digitalWrite(RFPin, LOW);
	  digitalWrite(ledPin, LOW);
	  
	  Serial.begin(57600);
	  Serial.println(F("Setup done"));
}


void loop()
{
	if(digitalRead(buttonPin)==HIGH)
  {
  	digitalWrite(ledPin,HIGH);
  	remote.sendMessage(dipSetup,buttonNum);
  	digitalWrite(ledPin,LOW);
  }
}

