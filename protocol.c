#include "protocol.h"
#include "crc.h"
#include <string.h>
#include <stdint.h>

void packet_init(ProtocolPacket *pkt, uint8_t token_ring, uint8_t target,uint8_t network)
{
    memset(pkt, 0, sizeof(ProtocolPacket));
    pkt->start = PROTOCOL_START;
    pkt->token_ring = token_ring;
    pkt->target = target;
    pkt->source = 0;
    pkt->network = network;
    pkt->length = 0x00;
    pkt->crc = 0x00;
}


void packet_serialize(ProtocolPacket *pkt, DataPacket *data_packet, uint8_t *out_buffer,  size_t *out_buffer_len){
    
    int packet_index = 0;
    if(data_packet == NULL){
        pkt->length =  PACKET_LENGTH_NO_DATA;
    }
    else{
        pkt->length = PACKET_LENGTH_NO_DATA  + data_packet->data_len;
    }

    //Serilize the Packet 
    out_buffer[packet_index++] = pkt->start;
    out_buffer[packet_index++] = (uint8_t) ((pkt->length >> 8) & 0xFF);  //Lenght_H
    out_buffer[packet_index++] = (uint8_t) (pkt->length & 0xFF);         //Lenght _L
    out_buffer[packet_index++] = pkt->token_ring;
    out_buffer[packet_index++] = pkt->target;
    out_buffer[packet_index++] = pkt->source;
    out_buffer[packet_index++] = pkt->network;
    if(data_packet){
        for(int i=0;i<data_packet->data_len;i++){
            out_buffer[packet_index++] = data_packet->data[i];
        }
    }
    //Calculate the CRC over the packet fields after the length bytes
    pkt->crc = GetPacketCRC(&out_buffer[3], packet_index - 3);
    out_buffer[packet_index++] = (uint8_t) (pkt->crc & 0xFF);  //CRC_L
    out_buffer[packet_index++] = (uint8_t) ((pkt->crc >> 8) & 0xFF);
    
    *out_buffer_len = packet_index; 
}

int packet_deserialize(uint8_t *rx_buffer, size_t rx_buffer_len, uint8_t *out_data, size_t *out_data_len)
{
    uint16_t packet_len;
    uint8_t data_len;
    const int PACKET_LEN_INDEX = 1;
    const int PACKET_DATA_INDEX = 7;
    packet_len = ((rx_buffer[PACKET_LEN_INDEX] <<8 ) & 0xFF00) + rx_buffer[PACKET_LEN_INDEX + 1];
    if(packet_len > MAX_IN_PACKET_LEN){
        return -1;
    }
    if(rx_buffer_len < (size_t) (packet_len + 2)){
        return -1;
    }
    data_len = packet_len - PACKET_LENGTH_NO_DATA;
    for(int i=0;i<data_len;i++){
        out_data[i] = rx_buffer[PACKET_DATA_INDEX+i];
    }
    *out_data_len = data_len;
    return 0;
}