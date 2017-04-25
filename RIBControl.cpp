#include "RIBControl.h"


#if ARDUINO >= 100
#include "Arduino.h"
#else
extern "C" {
#include "WConstants.h"
}
#endif

// #define RIB_DEBUG

const PROGMEM byte syncCode[]={45,22,22,22,22,70};

const PROGMEM byte buttAcode[]={68,46,22,22,22,22,22,110,0};
const PROGMEM byte buttBcode[]={45,23,45,45,22,22,22,110,0};
const PROGMEM byte buttCcode[]={45,23,22,22,45,46,22,110,0};
const PROGMEM byte buttDcode[]={45,23,22,22,22,22,45,134,0};
const byte* const button_table[] PROGMEM = {buttAcode, buttBcode, buttCcode, buttDcode};


RibControl::RibControl(int transmitterPin)
	{
		transPin=transmitterPin;
	}
	
void RibControl::sendMessage(int dipSetup, uint8_t buttonNum)
	{
		
   // msg position counter
  int msgPos=0;
  // total lenght of msh in micros
  int msgLength=0;
  // current value
  int curval=0;
  int workDip=dipSetup;
  // array for pulse message
  uint8_t builtMsg[maxMsgSize];
  // first clear generated message
  for(int i=0;i<maxMsgSize;i++)
  {
    builtMsg[i]=0;
  }

  // first member is  pulse 
  builtMsg[msgPos++]=pulse4; 
  msgLength+=pulse4;
  // pin 1 is there everytime, so first pulse will be set directly
  // first high pulse is depends on pin0 directly
  #ifdef RIB_DEBUG
    Serial.print(".");
  #endif
   if((workDip&1)==1)
   {
      curval=1;
      builtMsg[msgPos++]=pulse4;
      msgLength+=pulse4;
   }
   else
   {
      curval=0;
      builtMsg[msgPos++]=pulse2;
      msgLength+=pulse2;    
   }
   #ifdef RIB_DEBUG
    Serial.print("/");
   #endif
   for(int pin=1;pin<9;pin++)
   {
      #ifdef RIB_DEBUG
        Serial.println(pin+1);
        Serial.print(F("A"));
      #endif
       // low pulse config
       //if(curval==dipSetup[pin])
       if((workDip&0b11)==0b11||((workDip&0b0000000011)|0b00)==0b00)
       {
        #ifdef RIB_DEBUG
          Serial.print(F("aa"));
        #endif
         // if pin value is same as previous, short low pulse
          builtMsg[msgPos++]=pulse2;
          msgLength+=pulse2;
          #ifdef RIB_DEBUG    
            Serial.print(F(".aa"));
          #endif
       }
       else
       {
           // if old value is high and new value should be low 
           // if(curval==1&&dipSetup[pin]==0)
           if((workDip&0b11)==0b01)
           {
            #ifdef RIB_DEBUG
              Serial.print(F("bb"));
            #endif
              // long low pulse will be added 
            builtMsg[msgPos++]=pulse4;
            msgLength+=pulse4;
            // first 0 after 1 should be skipped 
            // pin++;
            workDip=workDip>>1;
            curval=0;
            #ifdef RIB_DEBUG
              Serial.print(F(".bb"));
            #endif
           }
           else 
           {
            #ifdef RIB_DEBUG
              Serial.print(F("c"));
            #endif
            // switching from 0 to 1  
            builtMsg[msgPos++]=pulse2;
            msgLength+=pulse2;
            // curval=1;
            #ifdef RIB_DEBUG
             Serial.print(F("c"));
            #endif
           }
       }
       #ifdef RIB_DEBUG
          Serial.print(F("B"));
       #endif
       // high pulse config is simple, and configured based on value of 
       // current pin, 
       // if(dipSetup[pin]==1&&curval==0)
       if((workDip&0b11)==0b10)
       {
          #ifdef RIB_DEBUG
             Serial.print(F("d"));
          #endif
          builtMsg[msgPos++]=pulse4;
          msgLength+=pulse4;
          curval=1;
          #ifdef RIB_DEBUG
             Serial.print(F(".d"));
          #endif
       }
       else
       {
          #ifdef RIB_DEBUG
             Serial.print(F("e"));
          #endif
          builtMsg[msgPos++]=pulse2;
          msgLength+=pulse2;
         // curval=dipSetup[pin];
          #ifdef RIB_DEBUG
              Serial.print(F(".e"));
          #endif
       }
       #ifdef RIB_DEBUG
          Serial.println(msgPos);
          Serial.print(pin);
          Serial.println(F("C"));
       #endif
       workDip=workDip>>1;
   }
   // if 10 is on 
   //if(dipSetup[9]==1)
   if((workDip&0b11)>0)
   {
      // is dip 9 also on
      // if(dipSetup[8]==1)
      // {
         // low pulse for 9
         builtMsg[msgPos++]=pulse2;
         msgLength+=pulse2;
         // high pulse for 10
         builtMsg[msgPos++]=pulse2;
         msgLength+=pulse2;
         builtMsg[msgPos++]=pulse2;
         msgLength+=pulse2;

         
     }
      else if(workDip&0b11) 
      {
         builtMsg[msgPos++]=pulse2;
         msgLength+=pulse2;
          // dip 9 is off 
         builtMsg[msgPos++]=pulse4;
         msgLength+=pulse4;
         // high pulse for 10
         builtMsg[msgPos++]=pulse2;
         msgLength+=pulse2;
          
      }
    
   
   else if(workDip&&0b00)
   {
      // pulse for 10 will be added just for case when 9 is also off
      //if(dipSetup[8]==0)
      //{
         builtMsg[msgPos++]=pulse2;
         
         // high pulse for 10
         builtMsg[msgPos++]=pulse2;
         // msgLength+=pulse2;
         builtMsg[msgPos++]=pulse2;
         // msgLength+=pulse2;
        msgLength+=(3*pulse2);
    }
    else 
    {
         builtMsg[msgPos++]=pulse4;
         msgLength+=pulse4;

    }
   
    // parity bit high in button setup
    #ifdef RIB_DEBUG
          Serial.print(F("F"));
    #endif    
    if(msgLength<500)
     {
        builtMsg[msgPos++]=pulse4;
     }
     else
     {
        builtMsg[msgPos++]=pulse2;
     }
    #ifdef RIB_DEBUG
    Serial.print(F("D")); 
    #endif
    // now copy medium sync part 
     for(int i=0;i<syncLength;i++)
     {
        builtMsg[msgPos++]=pgm_read_byte_near(syncCode + i);
     }
     #ifdef RIB_DEBUG
       Serial.print(F("E"));
     #endif
    // now copy buton code 
    //  for(int i=0;i<buttonLength;i++)
     //{
      strcpy_P(builtMsg+msgPos,(char*)pgm_read_word(&(button_table[buttonNum])));
       //  builtMsg[msgPos++]=buttAcode[i];
     // }
     // physical msg send 
    #ifdef RIB_DEBUG
      Serial.println("------");
	    for(int i=0;i<maxMsgSize;i+=2)
	    {
		     Serial.print(builtMsg[i]);
		     Serial.print(F(","));
		     Serial.println(builtMsg[i+1]);
	    }
    #endif
    sendCode(builtMsg);
  } // SendMessage method
  
void RibControl::sendCode(uint8_t message[])
  {
  	    // The LED will be turned on to create a visual signal transmission indicator.
    // digitalWrite(ledPin, HIGH);
    
   //initialise the variables 
    int highLength = 0;
    int lowLength = 0;
    //The signal is transmitted 6 times in succession - this may vary with your remote.       
    unsigned long start=micros();
    int total=0;
//    int total=0;
    for(int j = 0; j<transmitCount; j++){
      int i=0;
      for(i = 0; i<maxMsgSize; i+=2){ 
        
        if(message[i]!=0)
        {
          
            highLength=message[i];
            lowLength=message[i+1];
            // total+=highLength;
            total+=highLength;
            total+=lowLength;


             /* Transmit a HIGH signal - the duration of transmission will be determined 
                by the highLength and timeDelay variables */
             digitalWrite(transPin, HIGH);     
             delayMicroseconds(highLength*timedelay); 

            /* T
             *  ransmit a LOW signal - the duration of transmission will be determined 

                by the lowLength and timeDelay variables */
             digitalWrite(transPin,LOW);     
             delayMicroseconds(lowLength*timedelay);  
           
             /*Serial.print(lowLength);
             Serial.print(",");
             Serial.println(highLength);*/
        }
       
      }
      // Serial.println(total);
    }
        unsigned long misEnd=micros();
        // Serial.print("Sent time is");
        //Serial.println(misEnd-start);

    //Turn the LED off after the code has been transmitted.
   //  digitalWrite(ledPin, LOW); 
    digitalWrite(transPin, LOW);     
 
  }  // end send code method
  
  
