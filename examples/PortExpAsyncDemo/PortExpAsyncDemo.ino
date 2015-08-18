/*
 * Asynchronous Drive Demonstration
 *  modified for use with I2C Port Expander Extensions
 * Copyright Richard Morris 2015. All Rights Reserved
 * http://armdroid1.blogspot.co.uk
 *
 */

#include <Wire.h>
#include "Armdroid.h"
#include "ArmdroidPortExp.h"

// port expander address:
const int i2c_addr = 0x38;

// initialize Armdroid library:
ArmdroidPortExp myArm;

// variables for receiving and decoding commands:
unsigned int rxCmdPos;
int rxCmdVal;
int rxCmdArg[5];

// variable to store previous state:
ArmAsyncState previous;

void setup()
{
  Serial.begin(9600);

  myArm.begin(i2c_addr);
  myArm.setSpeed(120);
  myArm.torqueMotors(true);
}

void loop()
{
  if (!Serial) {
    // while the serial stream is not open, do nothing:
    while(!Serial);
    Serial.println(F("Welcome, Armdroid!"));

    // reset command variables
    rxCmdPos = rxCmdVal = 0;
    memset(rxCmdArg, 0, sizeof(rxCmdArg));
  }

  if (Serial.available()) {
    const char ch = Serial.read();
    if (isDigit(ch))
      rxCmdVal = (rxCmdVal * 10) + (ch  - '0');  // accumulate value
    else if (ch == '-')
      rxCmdVal = rxCmdVal * -1;
    else if (ch == ',') {
      rxCmdArg[rxCmdPos++] = rxCmdVal;           // shift received value into
                                                 //  arguments array
      if (rxCmdPos == 6)
        rxCmdPos = 0;                            // wrap argument index
      
      rxCmdVal = 0;                              // reset accumulator
    }
    else if (ch == 'D') {
      Serial.println( F("drive motors") );

      MTR_CHANNELS channels = { rxCmdArg[0], rxCmdArg[1], rxCmdArg[2], rxCmdArg[3], rxCmdArg[4], rxCmdVal };
      myArm.Start(channels);

      // reset accumulator
      rxCmdPos = rxCmdVal = 0;
    }
    else if (ch == 'P') {
      Serial.println( F("pause running motors") );
      myArm.Pause();
    }
    else if (ch == 'C') {
      Serial.println( F("continue driving motors") );
      myArm.Resume();
    }
    else if (ch == 'S') {
      Serial.println( F("stop dead all motors!") );
      myArm.Stop();
    }
  }

  //  method called every loop iteration:
  myArm.driveMotorsAsynchronous();
  
  // report state changes:
  ArmAsyncState current = myArm.getAsyncState();
  if (current != previous) {

  	Serial.print( F("DRIVE STATUS = ") );
  	switch (current)
  	{
  		case ASYNC_DRIVE_STOPPED:
  			Serial.println( F("STOPPED") );
  			break;
  		case ASYNC_DRIVE_RUNNING:
  			Serial.println( F("RUNNING") );
  			break;
  		case ASYNC_DRIVE_PAUSED:
  			Serial.println( F("PAUSED") );
  			break;
  	}
  	
  	previous = current;
  }
  
}
