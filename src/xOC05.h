/*
	This is a library for the OC05
	Servoi Driver
	
	The board uses i2c for communication.
	
	The board communicates with the following I2C device:
	- PCA9685
*/

#ifndef xOC05_h
#define xOC05_h

#include <xCore.h>
#include <Arduino.h>
#include <inttypes.h>

// Mode Registers and Settings
#define PCA9685_MODE1		0x00
#define PCA9685_MODE2		0x01
#define PCA9685_SLEEP		0x10
#define PCA9685_AUTOINCR	0x20
#define PCA9685_RESTART		0x80

// I2C Addresses
#define PCA9685_I2C_ADDR	0x78
#define PCA9685_SUBADR1		0x02
#define PCA9685_SUBADR2		0x03
#define PCA9685_SUBADR3		0x04
#define PCA9685_ALLCALLADR	0x05

// LED8 Channel Registers. OC05 uses channels 8-15.
#define LED8_ON_L		0x26
#define LED8_ON_H		0x27
#define LED8_OFF_L		0x28
#define LED8_OFF_H		0x29

// Prescale Register
#define PCA9685_PRESCALE	0xFE

class xOC05: public xCoreClass
{
	public:
	 	/**
		* Constructor
		* Creates a new instance of Driver class.
		*/	
		xOC05();
		xOC05(uint8_t addr);

	 	/**
		* Runs the setup of the driver. 
		* Call this in setup(), before controlling the driver.
		* @return, true if setup successful.
		*/	
		bool begin(void);

	 	/**
		* Restarts the driver. 
		* Call when restart desired.
		*/
		void reset(void);

	 	/**
		* Puts driver to sleep. 
		* Call whenever restart desired.
		*/
		void sleep(bool sleep_state);

		/**
		* Sets the frequency of PWM
		* Call and input a frequency before using PWM
		* @param channel, channel selection 1-8
		* @param on_point, point within the 4096 boundary at which the channel turns on
		* @param off_point, point within the 4096 boundary at whichs the channels turns off
		*/
		void setPWMFreq(float frequency);

		/**
		* Sets the PWM
		* @param channel, channel selection 1-8
		* @param on_point, point within the 4096 boundary at which the channel turns on
		* @param off_point, point within the 4096 boundary at whichs the channels turns off
		*/
		void setPWM(uint8_t channel, uint16_t on_point, uint16_t off_point);
		
		/**
		* Use channels as GPIO
		* @param channel, channel selection 1-8 (Conforms with channel numbers on the xchip)
		* @param state, input HIGH or LOW
		void setChannel(uint8_t channel, bool state);

	private:

		// Read content from PCA9685
		uint8_t readPCA9685(uint8_t cmd);

		// Device I2C Address
		uint8_t PCA9685_I2C_ADDRESS;
};
#endif
