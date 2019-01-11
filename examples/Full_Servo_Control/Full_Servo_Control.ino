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

unsigned long tick_Move;

#define SERVO_MAX 450
#define SERVO_MIN 130
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
  if ((millis() - tick_Move) > 1000) {
    if (dir == 0) {
      SERVO_LEFT();
      dir = 1;
    } else if (dir == 1) {
      SERVO_RIGHT();
      dir = 0;
    }
    tick_Move = millis();
  }
}

// Turn servo in the left direction
void SERVO_LEFT(void) {
  for (uint16_t pulselen = SERVO_MAX; pulselen > SERVO_MIN; pulselen--) {
    OC05.setPWM(SERVO_CHANNEL, pulselen);
    delay(10);
  }
}

// Turn servo in the right direction
void SERVO_RIGHT(void) {
  for (uint16_t pulselen = SERVO_MIN; pulselen < SERVO_MAX; pulselen++) {
    OC05.setPWM(SERVO_CHANNEL, pulselen);
    delay(10);
  }
}
