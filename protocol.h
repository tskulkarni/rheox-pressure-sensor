#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

#include <stdbool.h>

#define PROTOCOL_START 0xAA


#define PACKET_LENGTH_NO_DATA   0x04
#define DATA_PACKET_OVERHEAD    0x02  //OVerhead for Index an Command 


#define MAX_IN_PACKET_LEN       128   //
#define MAX_TX_RX_PACKET_LEN    MAX_IN_PACKET_LEN + 16

typedef struct 
{
    uint8_t data_len;
    uint8_t data[255];
} DataPacket;

typedef struct {
    uint8_t start;
    uint16_t length;
    uint8_t token_ring;
    uint8_t target;
    uint8_t source;
    uint8_t network;
    uint16_t crc;
} __attribute__((packed)) ProtocolPacket;


typedef struct {
    uint8_t data[255];
    uint8_t length;
    uint8_t token_ring;
    uint8_t source;
    uint8_t network;
    uint8_t index;
    uint8_t command;
    uint16_t crc;
} ResponsePacket;

extern void packet_init(ProtocolPacket *pkt, uint8_t token_ring, uint8_t target,uint8_t network);

extern void packet_serialize(ProtocolPacket *pkt, DataPacket *data_packet, uint8_t *out_buffer,  size_t *out_buffer_len);

extern int packet_deserialize(uint8_t *rx_buffer, size_t rx_buffer_len, uint8_t *out_data, size_t *out_data_len);

#endif
