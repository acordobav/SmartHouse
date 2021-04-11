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
    if (write(fd, pin, strlen(pin)) < 0) {
        perror("Error writing to /sys/class/gpio/export");
        exit(1);
    }

    close(fd);
}

int main()
{
    exportGpio(84);
    return 0;
}