/*************************************************************
	This is an examples for the OC05
	Servo Motor Controller
	
	You can buy one on our store!
	-----> https://xinabox.cc/products/OC05/
	
	This examples rotates a servo motor
	
	The sensor communicates over the I2C Bus.
	
*************************************************************/

/********************* Library Includes *********************/
#include <xCore.h>
#include <xOC05.h>  // https://github.com/xinabox/arduino-OC05

xOC05 OC05;

#define SERVO_CHANNEL 1

uint8_t dir = 0;

void setup() {
  // put your setup code here, to run once:
#ifdef ESP8266
  Wire.pins(2, 14);
  Wire.setClockStretchLimit(15000);
#endif

  Wire.begin();

  OC05.begin();

  OC05.setPWMFreq(60); 
}

void loop() {
  // put your main code here, to run repeatedly:
  OC05.setPosition(SERVO_CHANNEL, -90); //Set's servo position to -90 degrees
  delay(2000);
  
  OC05.setPosition(SERVO_CHANNEL, 0); //Set's servo position to 0 degrees
  delay(2000);
  
  OC05.setPosition(SERVO_CHANNEL, 90); //Set's servo position to 90 degrees
  delay(2000);
  
  OC05.setPosition(SERVO_CHANNEL, 0); //Set's servo position to 0 degrees
  delay(2000);
}
