#include "../include/sm-gpio.h"

void exportGpio(int gpio)
{
    if ((gpio < 0) || (gpio > 63))
    {
        perror("GPIO number must be 0-63");
        exit(1);
    }

    // Opening the /sys/class/gpio/export file
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0)
    {
        perror("Unable to open /sys/class/gpio/export");
        exit(1);
    }

    // Conversion of the gpio number into string
    char pin[3];
    sprintf(pin, "%d", gpio);

    // Export the pin by writing to /sys/class/gpio/export
    if (write(fd, pin, strlen(pin)) < 0)
    {
        perror("Error writing to /sys/class/gpio/export");
        exit(1);
    }

    close(fd);
}

void unexportGpio(int gpio)
{
    if ((gpio < 0) || (gpio > 63))
    {
        perror("GPIO number must be 0-63");
        exit(1);
    }

    // Opening the /sys/class/gpio/unexport file
    int fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd == -1)
    {
        perror("Unable to open /sys/class/gpio/unexport");
        exit(1);
    }

    // Conversion of the gpio number into string
    char pin[3];
    sprintf(pin, "%d", gpio);

    // Unexport the pin by writing to /sys/class/gpio/unexport
    if (write(fd, pin, strlen(pin)) < 0)
    {
        perror("Error writing to /sys/class/gpio/unexport");
        exit(1);
    }

    close(fd);
}

void pinMode(int pin, int mode)
{
    // Checks the mode desired for the pin
    if (mode != INPUT && mode != OUTPUT)
    {
        perror("Pin mode must be INPUT or OUTPUT");
        exit(1);
    }

    // Exports the pin
    exportGpio(pin);

    // Conversion of the gpio number into string
    char gpio[3];
    sprintf(gpio, "%d", pin);

    // The gpio file name is obtained
    const char *gpioString = "gpio";
    char *gpioFileName = malloc(strlen(gpioString) + strlen(gpio) + 1);
    strcpy(gpioFileName, gpioString);
    strcat(gpioFileName, gpio);

    // The gpio file mode is obtained
    const char *path1 = "/sys/class/gpio/";
    const char *path2 = "/direction";
    char *gpioModeFile = malloc(
        strlen(path1) + strlen(gpioFileName) + strlen(path2) + 1);
    strcpy(gpioModeFile, path1);
    strcat(gpioModeFile, gpioFileName);
    strcat(gpioModeFile, path2);

    int fd = open(gpioModeFile, O_WRONLY);
    if (fd == -1)
    {
        perror("Unable to open gpio mode file");
        exit(1);
    }

    // Writing the pin mode to the file
    char *pinMode = "out";
    if (!mode)
        pinMode = "in";
    if (write(fd, pinMode, strlen(pinMode)) < 0)
    {
        perror("Unable to set pin mode");
        exit(1);
    }

    free(gpioFileName);
    free(gpioModeFile);
    close(fd);
}

int main()
{
    pinMode(22, OUTPUT);
    unexportGpio(22);
    return 0;
}