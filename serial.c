#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "serial.h"

serial_fd_t serial_open(void)
{
    int fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY );
    if (fd < 0) {
        return -1;
    }
    
    if (!serial_configure(fd)) {
        close(fd);
        return -1;
    }
    
    return fd;
}

bool serial_close(serial_fd_t fd)
{
    if (fd < 0) {
        return false;
    }
    return close(fd) == 0;
}

bool serial_configure(serial_fd_t fd)
{
    struct termios tty;
    
    if (tcgetattr(fd, &tty) != 0) {
        return false;
    }
    
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHOK;
    tty.c_lflag &= ~ECHOCTL;
    tty.c_lflag &= ~ECHOKE;
    tty.c_lflag &= ~ISIG;
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;
    
    tty.c_cc[VTIME] = 30;
    tty.c_cc[VMIN] = 0;
    
    cfsetispeed(&tty, BAUD_RATE);
    cfsetospeed(&tty, BAUD_RATE);
    
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        return false;
    }
    
    return true;
}

bool serial_write_bytes(serial_fd_t fd, const uint8_t *data, int len)
{
    if (fd < 0 || !data || len <= 0) {
        return false;
    }
    
    int written = write(fd, data, len);
    return written == len;
}

int serial_read_bytes(serial_fd_t fd, uint8_t *buffer, int max_len)
{
    if (fd < 0 || !buffer || max_len <= 0) {
        return -1;
    }
    
    return read(fd, buffer, max_len);
}
