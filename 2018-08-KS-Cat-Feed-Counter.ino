//************************************************************************************
// KS-Cat-Feed-Counter
//************************************************************************************
// Remark:
// Use linux to upload to Arduino Pro Mini - Windows has problems to recognize the
// uploader-Module sometimes
// open a Linux-Terminal-Window with Ctrl-Alt-t
// execute: sudo chmod 666 /dev/ttyUSB0 // depends on where your com-port is
//
// When the lid of the foodbox is opened for longer than the "minOpeningTime" the
// counter variable "feedingTimes" will be increased.
// There is a status button to light up the LED's according to the "feedingTimes".
// With the decrease button you can decrease the "feedingTimes".
//************************************************************************************
// Libraries
//************************************************************************************
// To save power - as the gadget runs on battery
#include <LowPower.h>
//************************************************************************************
// Configuration variables
//************************************************************************************
// Define after how much time of open food box you want to increase the feedingTimes
int minOpeningTime = 6000;

// The time that the LED's will lighten up on lid openin/closing and on increasing
// or decreasing the "feedingTimes"
int ledLightenUpTime = 1200;

//************************************************************************************
// General variables
//************************************************************************************
// Counter for the amount of feeding times
int feedingTimes = 0;

// To make sure that feedingTimes will only be increased once per lid opening.
boolean upCountDone = false;

// To avoid multiple decreases
boolean decreaseDone = false;

//************************************************************************************
// Inputs and ouputs
//************************************************************************************
// Output definitions
int ledOne = 6;
int ledTwo = 5;
int ledThree = 4;
int ledFour = 3;

// Input definitions
int decreaseLedState = 7;
int showLedState = 8;
int lidPosition = 9;
//************************************************************************************
// General setup
//************************************************************************************
void setup()
{
  // define the output pins
  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);
  pinMode(ledThree, OUTPUT);
  pinMode(ledFour, OUTPUT);

  // define the input pins
  pinMode(decreaseLedState, INPUT);
  pinMode(showLedState, INPUT);
  pinMode(lidPosition, INPUT);

  // initialize the output pins
  digitalWrite(ledOne, LOW);
  digitalWrite(ledTwo, LOW);
  digitalWrite(ledThree, LOW);
  digitalWrite(ledFour, LOW);

  // activate pull-up Resistors for switches
  digitalWrite(decreaseLedState, HIGH);
  digitalWrite(showLedState, HIGH);
  digitalWrite(lidPosition, HIGH);
}
//************************************************************************************
// Main program loop
//************************************************************************************
void loop()
{
  // Activate low power consumption
  LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);

  //**********************************************************************************
  // Check if the lid is open for more than minOpeningTime and increase feedingTimes
  //**********************************************************************************
  // Save the Time the lid is opened
  if ((digitalRead(lidPosition)) == HIGH && upCountDone == false)
  {
    // Lighten up the active LED's
    lightenUpLeds(ledLightenUpTime);

    // check if feeding is done
    delay(minOpeningTime);

    if ((digitalRead(lidPosition)) == HIGH)
    {
      if (feedingTimes >= 4)
      {
        feedingTimes = 1;
      }
      else
      {
        feedingTimes = feedingTimes + 1;
      }
      upCountDone = true;
      
      // Lighten up the active LED's
      lightenUpLeds(ledLightenUpTime);
    }
  }
  
  // When lid is closed after an upcount the upCountDone has to be set to false
  if ((digitalRead(lidPosition)) == LOW && upCountDone == true)
  {
    upCountDone = false;

    // Lighten up the active LED's
    lightenUpLeds(ledLightenUpTime);
}

  //**********************************************************************************
  // Light up the feedingTimes number of LED's during ledLightenUpTime
  //**********************************************************************************
  if ((digitalRead(showLedState) == LOW))
  {
    // Lighten up the active LED's
    lightenUpLeds(ledLightenUpTime);
  }
  
  //**********************************************************************************
  // Decrease feedingTimes by button (just for correction)
  //**********************************************************************************
  if (digitalRead(decreaseLedState) == LOW && decreaseDone == false)
  {
    if (feedingTimes >= 1 && feedingTimes <= 4)
    {
      // Decrease the total amount of enlighten LED's
      feedingTimes = feedingTimes - 1;
    }
    else
    {
      feedingTimes = 4;
    }
    decreaseDone = true;
    
    // Lighten up the active LED's
    lightenUpLeds(ledLightenUpTime);
  }

  if (digitalRead(decreaseLedState) == HIGH && decreaseDone == true)
  {
    decreaseDone = false;
  }
}
//************************************************************************************
// Function to lighten up LED's
//************************************************************************************
void lightenUpLeds(int ledLightenUpTime)
{
  switch (feedingTimes)
  {
    case 1:
      // lighten up LED 1
      digitalWrite(ledOne, HIGH);
      delay(ledLightenUpTime);
      digitalWrite(ledOne, LOW);
      break;
    case 2:
      // lighten up LED 1 and 2
      digitalWrite(ledOne, HIGH);
      digitalWrite(ledTwo, HIGH);
      delay(ledLightenUpTime);
      digitalWrite(ledOne, LOW);
      digitalWrite(ledTwo, LOW);
      break;
    case 3:
      // lighten up LED 1, 2 and 3
      digitalWrite(ledOne, HIGH);
      digitalWrite(ledTwo, HIGH);
      digitalWrite(ledThree, HIGH);
      delay(ledLightenUpTime);
      digitalWrite(ledOne, LOW);
      digitalWrite(ledTwo, LOW);
      digitalWrite(ledThree, LOW);
      break;
    case 4:
      // lighten up LED 1, 2, 3 and 4
      digitalWrite(ledOne, HIGH);
      digitalWrite(ledTwo, HIGH);
      digitalWrite(ledThree, HIGH);
      digitalWrite(ledFour, HIGH);
      delay(ledLightenUpTime);
      digitalWrite(ledOne, LOW);
      digitalWrite(ledTwo, LOW);
      digitalWrite(ledThree, LOW);
      digitalWrite(ledFour, LOW);
      break;
    default:
      // impossible state - don't do anything
      break;
  }
}
//************************************************************************************
