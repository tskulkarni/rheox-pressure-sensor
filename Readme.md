# Pressure Sensor Communication
This program is implemnted in C (gcc)  on Linux  (gcc). The program open the first serial port (/dev/ttyUSB0) at baud rate 921600. 

## Communication protocol
The program implements the following communication protocol to communicate with the pressure ssentor
Over All Tx structure
Start (1 byte) : 0xAA
Length (2 bytes): The calculated length from Token Ring 
Token Ring(1 byte): The token ring address
Source(1 Byte): the Source address
Network(1 Byte): the network address
Index (1 Bytes): the packet index, incremented on successive tx packets
Command(1 Byte): The command code (See file command_codes.h for command codes)
Data( variable): The command specific data 
CRC: The crc (see file crc.c for crc calculation)

The response if of the similar format, the data in the response is command specific 



  

