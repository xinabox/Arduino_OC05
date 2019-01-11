/*
  This is a library for the OC04\5
  Servo Driver
  
  The board uses I2C for communication.
  
  The board communicates with the follwoing I2C device:
  - PCA9685
*/

#include "xOC05.h"
/*--Public Class Function--*/

/*************************************************************
  Constructor
*************************************************************/
xOC05::xOC05(void)
{
  PCA9685_I2C_ADDRESS = PCA9685_I2C_ADDR;
}

xOC05::xOC05(uint8_t addr)
{
  PCA9685_I2C_ADDRESS = addr;
}

/*************************************************************
  Configure Driver
*************************************************************/
bool xOC05::begin(void)
{
  xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, PCA9685_RESTART);
  return true;
}

/*************************************************************
  Reset Driver
*************************************************************/
void xOC05::reset(void)
{
  uint8_t mode1_content = readPCA9685(PCA9685_MODE1);
  if((mode1_content >> 7) == 0x01) {  // test for logic 1 in restart bit.
    xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, ((mode1_content & 0x7F) | (0xEF)));
    delay(500);
    mode1_content = readPCA9685(PCA9685_MODE1);
    xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, (mode1_content | PCA9685_RESTART) & 0xFF);
  } else {
    xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, (PCA9685_RESTART | mode1_content));
    delay(10);
  }
}

void xOC05::sleep(void)
{
  xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, ((readPCA9685(PCA9685_MODE1) & 0x7F) | PCA9685_SLEEP));
}

/*************************************************************
  Read the Pulse Width Modulation Frequency
*************************************************************/
void xOC05::setPWMFreq(float frequency)
{
  frequency *= 0.8;
  float clock_freq = 25000000;  // PCA9685 internal clock frequency (Hz)
  uint16_t steps = pow(2, 12);  // 4096

  // prescale value to write to prescale register
  uint8_t prescale_val = round(clock_freq/(steps*frequency)) - 1;

  // read the mode register to retain its configuation
  uint8_t mode1_content = readPCA9685(PCA9685_MODE1);

  // put PCA9685 to sleep before writing to the prescale register
  xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, ((mode1_content & 0x7F) | PCA9685_SLEEP));

  // configure prescale register
  xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_PRESCALE, prescale_val);
  
  xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, mode1_content | PCA9685_RESTART);
  delay(500);

  // control register automatically increments 
  xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, (PCA9685_AUTOINCR | mode1_content) | 0x80);
}

/*************************************************************
  Set the Pulse Width Modulation
*************************************************************/
void xOC05::setPWM(uint8_t channel, uint16_t off_point)
{
	uint16_t on_point = 0;
	
  Wire.beginTransmission(PCA9685_I2C_ADDRESS);
  Wire.write(LED8_ON_L + (4 * (channel - 1)));
  Wire.write(on_point);
  Wire.write(on_point >> 8);
  Wire.write(off_point);
  Wire.write(off_point >> 8);
  Wire.endTransmission();
}

/*************************************************************
  Set the Servo Position in Degrees
*************************************************************/
void xOC05::setPosition(uint8_t channel, int pos) {
  pos = constrain(pos, -90, 90);
  pos = map(pos, -90, 90, SERVOMIN, SERVOMAX);
  
  setPWM((channel), (uint16_t)pos);
}

/*************************************************************
  Use OC05 as GPIO
*************************************************************/
void xOC05::digitalWrite(uint8_t channel, bool state)
{
  if(state == HIGH) {
    setPWM(channel, 4096);
  } else if(state == LOW) {
    setPWM(channel, 0);
  }
}

/*--Private Class Fucntion--*/

/*************************************************************
  Read PCA9685 registers
*************************************************************/
uint8_t xOC05::readPCA9685(uint8_t cmd)
{
  uint8_t content = xCore.read8(PCA9685_I2C_ADDRESS, cmd);
  return content;
}
