#include "arduino_secrets.h"

/*

Sets the time from input and prints back time stamps for 5 seconds

Based on DS3231_set.pde
by Eric Ayars
4/11

Added printing back of time stamps and increased baud rate
(to better synchronize computer and RTC)
Andy Wickert
5/15/2011

Clean for SAMD arch, add explanation, respect code-style and 
fix interpretation of Serial input if used more than once
Olivier Staquet
4/26/2020

*/

#include <DS3231.h>
#include <Wire.h>

DS3231 clock;

byte year;
byte month;
byte date;
byte dow;
byte hour;
byte minute;
byte second;

bool century = false;
bool h12Flag;
bool pmFlag;

/*****************************************************************************************************
 * Setup
 *  - Open Serial and Wire connection
 *  - Explain to the user how to use the program
 *****************************************************************************************************/
void setup() {
  // Start the serial port
  Serial.begin(9600);

  // Start the I2C interface
  Wire.begin();

  // Request the time correction on the Serial
  delay(4000);
  Serial.println("Format YYMMDDwhhmmssx");
  
  // // UNCOMMENT TO SET CLOCK
  String inputStr = "2106271222300x";
  inputDateFromString(inputStr);
  clock.setClockMode(false);  // set to 24h
  clock.setYear(year);
  clock.setMonth(month);
  clock.setDate(date);
  clock.setDoW(dow);
  clock.setHour(hour);
  clock.setMinute(minute);
  clock.setSecond(second);
}

/*****************************************************************************************************
 * Loop
 *  - Each time we receive the time correction on the Serial
 *  - Set the clock of the DS3231
 *  - Echo the value from the DS3231 during 5 seconds
 *****************************************************************************************************/
void loop() {
  // Print Times
  Serial.print(clock.getYear(), DEC);
  Serial.print("-");
  Serial.print(clock.getMonth(century), DEC);
  Serial.print("-");
  Serial.print(clock.getDate(), DEC);
  Serial.print(" ");
  Serial.print(clock.getHour(h12Flag, pmFlag), DEC); //24-hr
  Serial.print(":");
  Serial.print(clock.getMinute(), DEC);
  Serial.print(":");
  Serial.println(clock.getSecond(), DEC);
  delay(1000);
  Serial.println(clock.getTemperature(), DEC);
  delay(1000);
}

/*****************************************************************************************************
 * inputDateFromSerial
 *  - Read and interpret the data from the Serial input
 *  - Store the data in global variables
 *****************************************************************************************************/
void inputDateFromString(String inputStr) {
	// the order YYMMDDwHHMMSS, with an 'x' at the end.
	byte temp1, temp2;
  // 	char inputStr[15];

  // Find the end of char "x"
  int posX = -1;
  for(uint8_t i = 0; i < 20; i++) {
    if(inputStr[i] == 'x') {
      posX = i;
      break;
    }
  }

  // Consider 0 character in ASCII
  uint8_t zeroAscii = '0';
 
	// Read Year first
	temp1 = (byte)inputStr[posX - 13] - zeroAscii;
	temp2 = (byte)inputStr[posX - 12] - zeroAscii;
	year = temp1 * 10 + temp2;
  
	// now month
	temp1 = (byte)inputStr[posX - 11] - zeroAscii;
	temp2 = (byte)inputStr[posX - 10] - zeroAscii;
	month = temp1 * 10 + temp2;
	
	// now date
	temp1 = (byte)inputStr[posX - 9] - zeroAscii;
	temp2 = (byte)inputStr[posX - 8] - zeroAscii;
	date = temp1 * 10 + temp2;
	
	// now Day of Week
	dow = (byte)inputStr[posX - 7] - zeroAscii;		
	
	// now Hour
	temp1 = (byte)inputStr[posX - 6] - zeroAscii;
	temp2 = (byte)inputStr[posX - 5] - zeroAscii;
	hour = temp1 * 10 + temp2;
	
	// now Minute
	temp1 = (byte)inputStr[posX - 4] - zeroAscii;
	temp2 = (byte)inputStr[posX - 3] - zeroAscii;
	minute = temp1 * 10 + temp2;
	
	// now Second
	temp1 = (byte)inputStr[posX - 2] - zeroAscii;
	temp2 = (byte)inputStr[posX - 1] - zeroAscii;
	second = temp1 * 10 + temp2;
}
