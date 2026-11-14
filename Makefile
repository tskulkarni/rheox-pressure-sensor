# Compiler and Flags
CC = gcc

INC_DIRS = -I ./

CFLAGS = -Wall -Wextra -std=gnu99 -g -O0

# Target executable name
TARGET = build/senor.out


# Source and Object files
SRCS = main.c pressure_sensor.c protocol.c serial.c crc.c
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Compile and link directly from source files
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

# Phony targets to clean up build artifacts
.PHONY: clean
clean:
	rm -f  $(TARGET)