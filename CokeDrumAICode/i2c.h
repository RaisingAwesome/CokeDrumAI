#pragma once

#include <stdbool.h>
#include "epoll_timerfd_utilities.h"
//// OLED
#include "oled.h"

#define LSM6DSO_ID         0x6C   // register value
#define LSM6DSO_ADDRESS	   0x6A	  // I2C Address
float readDistance(void);
int initI2c(void);
void closeI2c(void);
extern int i2cFd;