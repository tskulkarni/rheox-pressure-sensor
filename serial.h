#include <stdint.h>
#include <stdbool.h>

#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE B921600

typedef int serial_fd_t;

extern serial_fd_t serial_open(void);
extern bool serial_close(serial_fd_t fd);
extern bool serial_configure(serial_fd_t fd);
extern bool serial_write_bytes(serial_fd_t fd, const uint8_t *data, int len);
extern int serial_read_bytes(serial_fd_t fd, uint8_t *buffer, int max_len);
