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
