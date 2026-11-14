#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "serial.h"
#include "protocol.h"
#include "pressure_sensor.h"

PressureSensor pressure_sensor;
int main(void)
{
    serial_fd_t fd = serial_open();
    
    if (fd < 0) {
        fprintf(stderr, "Failed to open serial port %s (this is expected if no device is connected)\n", SERIAL_PORT);
        return 0;
    }
    
    printf("Serial port opened successfully\n");
    printf("Device: %s\n", SERIAL_PORT);
    
    init_pressure_sensor(&pressure_sensor,fd);

    
    serial_close(fd);
    
    printf("Done\n");
    
    return 0;
}
