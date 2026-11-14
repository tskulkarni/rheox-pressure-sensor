#include <stdint.h>

#define SEED 0xFFFF
#define GP 0xA001

uint16_t Crc16 = SEED;

uint16_t CrcCalculate(uint8_t Byte, uint16_t CRC)
{
    uint8_t Carry;
    int Bit_Index;
    CRC = CRC ^ (Byte & 0xFF);
    for (Bit_Index = 0; Bit_Index < 8; Bit_Index++) {
        Carry = CRC & 0x0001;
        CRC = CRC >> 1;
        if (Carry)
            CRC = CRC ^ GP;
    }
    return CRC;
}

uint16_t GetPacketCRC(uint8_t *packet, int packet_len)
{
    uint16_t crc = SEED;
    for (int i = 0; i < packet_len; i++) {
        crc = CrcCalculate(packet[i], crc);
    }
    return crc;
}
