# RIBSwitch
Arduino library for RIB Gate door (I have it) and probably garage doors (I don't have it). 

Gate used to test was installed in 2007, originally come with "Moon" remotes (http://www.remotecontrol-express.co.uk/remote+RIB+:+MOON+T433+2CH), later replaced by SUN remotes (http://www.ribind.com/products/accessories-and-boards-idc13/control-accessories-idc10/sun-idp51.html)



# Library usage
First check your remote (Moon is easier, because there are DIP switches directly in remote

Lib usage is simple:

#include "RIBControl.h"

then create instance of library, where RF pin is pin with RF 433 transmitter  

  RibControl remote(RFPin);

when neede to send code call method 

  	remote.sendMessage(dipSetup,buttonNum);

where dipSetup is binary number where 1 is set for DIP in On. Don't forget, that DIP with number 1 is left in remote, so your DIP setup 0b1100000000 need to be sent into method as value 0b0000000011. 
Buttons are numbered, when you have remote with holder towards you.

There is simple example how to use this library.

#Credits
RCSwitch from sui77 for Idea
In following address can be found helpfull scripts to 433 sniffing 
http://arduinobasics.blogspot.ch/2014/06/433-mhz-rf-module-with-arduino-tutorial_27.html

You can use this library for free (GPL licence)
