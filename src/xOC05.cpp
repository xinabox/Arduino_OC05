#include "xOC05.h"

xOC05::xOC05(void)
{
  PCA9685_I2C_ADDRESS = 0x78;
}

xOC05::xOC05(uint8_t addr)
{
  PCA9685_I2C_ADDRESS = addr;
}

bool xOC05::begin(void)
{
  reset();
  return true;
}

void xOC05::reset(void)
{
  uint8_t mode1_content = readPCA9685(PCA9685_MODE1);
  if((mode1_content >> 7) == 0x01) {
    xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, ((mode1_content & 0x7F) | (0xEF)));
    delay(500);
    xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, (mode1_content | PCA9685_RESTART));
  } else {
    xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, 0x80);
    delay(10);
  }
}

void xOC05::sleep(bool sleep_state = false)
{
  xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, ((readPCA9685(PCA9685_MODE1) & 0x7F) | PCA9685_SLEEP));
}

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
  
  xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, mode1_content);
  delay(5);

  // control register automatically increments 
  xCore.write8(PCA9685_I2C_ADDRESS, PCA9685_MODE1, (PCA9685_AUTOINCR | mode1_content) | 0x80);
}

void xOC05::setPWM(uint8_t channel, uint16_t on_point, uint16_t off_point)
{
  Wire.beginTransmission(PCA9685_I2C_ADDRESS);
  Wire.write(LED8_ON_L + (4 * (channel - 1)));
  Wire.write(on_point);
  Wire.write(on_point >> 8);
  Wire.write(off_point);
  Wire.write(off_point >> 8);
  Wire.endTransmission();
}

uint8_t xOC05::readPCA9685(uint8_t cmd)
{
  uint8_t content = xCore.read8(PCA9685_I2C_ADDRESS, cmd);
  return content;
}