# librobotutils
A C library providing useful utility functions for the robot.

## Installation ##

This library is designed for Raspberry Pi with Raspbian.

* First, you need [wiringPi](http://wiringpi.com/download-and-install/),
if you don't have it already.

* Once wiringPi is installed, get back the sources :
`git clone git@github.com:TelecomParistoc/librobotutils.git`

* cd to the root of the repository and enter `make`

* finally enter `sudo make install`

## Usage ##

Don't forget to compile (actually, link) your C/C++ app with `-lrobotutils` option.
You can include all the headers you need with :
```c
#include <robotutils.h>
```

It is also possible to include headers individually with
```c
#include <robotutils/[header name].h>
```

### Timing functions ###

These functions provide synchronous and asynchronous
delays and time information.

#### `long long int getCurrentTime()` ####
Return current time, in ms.

#### `void waitFor(int millisecond)` ####
Wait for the given amount of time (in milliseconds). Please note that actual
delay may be slightly longer.

#### `void waitForMicro(int microseconds)` ####
wait for the given amount of time (in microseconds). Wait time should not exceed
1000000us (i.e. 1second).

#### `int scheduleIn(int milliseconds, void (*callback)(void))` ####
call a function after the given time, starting when
scheduleIn is called.

* milliseconds : the time to wait before calling the function in milliseconds
* callback : the function to call. It should be like void myFunction() {...}

**returns :** the scheduled function UID (positive), or -1 if an error occured

#### `int cancelScheduled(int uid)` ####
unscheduled a previously scheduled function (stop it from being called)

* uid : the unique ID of the task, returned by scheduleIn

**returns :** 0 if task was successfully unscheduled, -1 if it was not found in
the pending tasks

### I2C functions ###

These function allow to read and write I2C slaves, in a more flexible way than
wiringPi base functions.

#### `int8_t I2Cread8(uint8_t addr, uint8_t reg)` ####
Read a 8 bit register of an I2C slave.

* addr : slave address
* reg : register address

**returns :** register value

#### `int I2Cwrite8(uint8_t addr, uint8_t reg, uint8_t value)` ####
Write a 8 bit register of an I2C slave.

* addr : slave address
* reg : register address

**returns :** 0 for success, negative value for error.

#### `uint16_t I2Cread16(uint8_t addr, uint8_t reg)` ####
Read a 16 bit register of an I2C slave.

* addr : slave address
* reg : register address

**returns :** register value

#### `int I2Cwrite16(uint8_t addr, uint8_t reg, uint16_t value)` ####
Write a 16 bit register of an I2C slave.

* addr : slave address
* reg : register address

**returns :** 0 for success, negative value for error.

#### `uint32_t I2Cread32(uint8_t addr, uint8_t reg)` ####
Read a 32 bit register of an I2C slave. As wiringPi doesn't support 32 bits
access, the function actually issues two 16 bits reads at addr then addr + 2.

* addr : slave address
* reg : register address

**returns :** register value

#### `int I2Cwrite32(uint8_t addr, uint8_t reg, uint32_t value)` ####
Write a 8 bit register of an I2C slave. As wiringPi doesn't support 32 bits
access, the function actually issues two 16 bits writes at addr then addr + 2.

* addr : slave address
* reg : register address

**returns :** 0 for success, negative value for error.

#### `float I2CreadFloat(uint8_t addr, uint8_t reg)` ####
Read a 32-bit float register of an I2C Slace. Actually, it uses I2CRead32 and interprets the result as float

* addr : slave address
* reg : register address

**returns :** register value

#### `int I2CwriteFloat(uint8_t addr, uint8_t reg, float x)` ####
Write a 32-bit float register of an I2C slave. This work is done by converting the float to an uint32 and by using I2CWrite32.

* addr : slave address
* reg : register address
* x : the value to write

**returns :** 0 for success, negative value for error.

### Receiving/sending float on I2C slave ###
This is a basic example :
```

/************* USEFUL FUNCTIONS ************/

/* casts a float to an int32_t which has exactly the same binary representation */
static int32_t float_to_int32(float x){
  int32_t result;
  memcpy(&result, &x, sizeof(result));
  return result;
}

/* casts an int32_t to a float which has exactly the same binary representation */
static float int32_to_float(int32_t x){
  float result;
  memcpy(&result, &x, sizeof(result));
  return result;
}

/************* RX PART ************/
/* the float cur_pos.x will be read from I2C data*/
switch (addr) {
  ...
case CUR_POS_X_LOW_ADDR:
    tmp_cur_x = (rx_buffer[2] << 8) | rx_buffer[1];
    break;
case CUR_POS_X_HIGH_ADDR:
    tmp_cur_x |= (rx_buffer[2] << 24) | (rx_buffer[1] << 16);
    cur_pos.x = int32_to_float(tmp_cur_x);
    break;
}


/*************** TX PART **************/
/*the float cur_pos.x will be sent on I2C bus */
switch (addr) {
  ...
case CUR_POS_X_LOW_ADDR:
    saved_cur_x = float_to_int32(cur_pos.x);
    *value = saved_cur_x & 0x0000FFFFU;
    break;
case CUR_POS_X_HIGH_ADDR:
    *value = (saved_cur_x & 0xFFFF0000U) >> 16U;
    break;
}
/*then *value is sent on I2C*/
```  
