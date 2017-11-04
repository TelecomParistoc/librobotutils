#include "i2c-functions.h"
#include "timing.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h> 		//for memcpy

#define I2C_SLAVE	0x0703

// make sure there's only one user accessing I2C port
pthread_mutex_t I2Clock;

int i2c_device = 0;

int i2c_init(uint8_t addr) {
	if(pthread_mutex_init(&I2Clock, NULL)) {
		printf("ERROR : cannot create mutex\n");
		return -2;
	}
	i2c_device = wiringPiI2CSetup(addr);
	return i2c_device;
}

uint8_t I2Cread8(uint8_t addr, uint8_t reg) {
	uint8_t result;
	if(i2c_device <= 0) {
		if(i2c_init(addr) < 0) {
			printf("I2Cread8 : ERROR cannot open device\n");
			return -1;
		}
	} else if(ioctl(i2c_device, I2C_SLAVE, addr)<0) {
		printf("I2Cread8 : ERROR cannot connect to selected device at address 0x%x\n", addr);
		return -2;
	}
	pthread_mutex_lock(&I2Clock);
	result = wiringPiI2CReadReg8 (i2c_device, reg);
	pthread_mutex_unlock(&I2Clock);
	return result;
}
uint16_t I2Cread16(uint8_t addr, uint8_t reg) {
	uint16_t result;
	if(i2c_device <= 0) {
		if(i2c_init(addr) < 0) {
			printf("I2Cread16 : ERROR cannot open device\n");
			return -1;
		}
	} else if(ioctl(i2c_device, I2C_SLAVE, addr)<0) {
		printf("I2Cread16 : ERROR cannot connect to selected device at address 0x%x\n", addr);
		return -2;
	}
	pthread_mutex_lock(&I2Clock);
	result = wiringPiI2CReadReg16 (i2c_device, reg);
	pthread_mutex_unlock(&I2Clock);
	return result;
}
uint32_t I2Cread32(uint8_t addr, uint8_t reg) {
	uint32_t result;
	result = I2Cread16(addr, reg);
	result +=  I2Cread16(addr, reg + 2) << 16;
	return result;
}

float I2CreadFloat(uint8_t addr, uint8_t reg) {
	uint32_t result_as_int = I2Cread32(addr, reg);
	float result;
	memcpy(&result, &result_as_int, sizeof(result_as_int));
	return result;
}

static void releaseI2Clock() {
	pthread_mutex_unlock(&I2Clock);
}
int I2Cwrite8(uint8_t addr, uint8_t reg, uint8_t value) {
	int result;
	if(i2c_device <= 0) {
		if(i2c_init(addr) < 0) {
			printf("I2Cwrite8 : ERROR cannot open device\n");
			return -1;
		}
	} else if(ioctl(i2c_device, I2C_SLAVE, addr)<0) {
		printf("I2Cwrite8 : ERROR cannot connect to selected device at address 0x%x\n", addr);
		return -2;
	}
	pthread_mutex_lock(&I2Clock);
	result = wiringPiI2CWriteReg8 (i2c_device, reg, value);
	scheduleIn(2, releaseI2Clock);
	return result;
}
int I2Cwrite16(uint8_t addr, uint8_t reg, uint16_t value) {
	int result;
	if(i2c_device <= 0) {
		if(i2c_init(addr) < 0) {
			printf("I2Cwrite16 : ERROR cannot open device\n");
			return -1;
		}
	} else if(ioctl(i2c_device, I2C_SLAVE, addr)<0) {
		printf("I2Cwrite16 : ERROR cannot connect to selected device at address 0x%x\n", addr);
		return -2;
	}
	pthread_mutex_lock(&I2Clock);
	result = wiringPiI2CWriteReg16(i2c_device, reg, value);
	scheduleIn(2, releaseI2Clock);
	return result;
}
int I2Cwrite32(uint8_t addr, uint8_t reg, uint32_t value) {
	I2Cwrite16(addr, reg, value);
	return I2Cwrite16(addr, reg + 2, value >> 16);
}

int I2CwriteFloat(uint8_t addr, uint8_t reg, float value){
	uint32_t to_send;
	memcpy(&to_send, &value, sizeof(value));
	return I2Cwrite32(addr, reg, to_send);
}
