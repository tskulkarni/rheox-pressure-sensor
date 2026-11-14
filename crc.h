#ifndef CRC_H
#define CRC_H

#include <stdint.h>

#define SEED 0xFFFF
#define GP 0xA001

uint16_t CrcCalculate(uint8_t Byte, uint16_t CRC);
uint16_t GetPacketCRC(uint8_t *packet, int packet_len);

#endif
