#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string.h>

#define INPUT 0
#define OUTPUT 1

/**
 * Function to export a GPIO
 * gpio: number of the gpio to export
**/
void exportGpio(int gpio);

/**
 * Function to unexport a GPIO
 * gpio: number of the gpio to unexport
**/
void unexportGpio(int gpio);

/**
 * Function to set if a gpio must be input or output
 * pin: number of the gpio
 * mode: INPUT or OUTPUT
**/
void pinMode(int pin, int mode);