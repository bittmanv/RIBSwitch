#ifndef RIBControl_h
#define RIBControl_h
#include <inttypes.h>

// maximum message length which can be sent 
#define maxMsgSize 40

// define pulse length constants

#define pulse2 23
#define pulse4 46

#define syncLength 6


#define transmitCount 6

#define timedelay 110

class RibControl
{
	public:
		RibControl(int transmitterPin);
		void sendMessage(int dipSetup, uint8_t buttonNum);
  private:
  	int transPin;
  	void sendCode(uint8_t message[]);
	
} ;

#endif