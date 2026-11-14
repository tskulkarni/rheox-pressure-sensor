#include "pressure_sensor.h"
#include "crc.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


uint8_t tx_buffer[MAX_TX_RX_PACKET_LEN];
uint8_t rx_buffer[MAX_TX_RX_PACKET_LEN];
uint8_t rx_data[ MAX_IN_PACKET_LEN];

static DataPacket data_packet;

static void clear_buffers(){
    memset(tx_buffer,0x00,MAX_TX_RX_PACKET_LEN);
    memset(rx_buffer,0x00,MAX_TX_RX_PACKET_LEN);
    memset(rx_data,0x00,MAX_IN_PACKET_LEN);
    memset((void *)&data_packet,0x00,sizeof(data_packet));
}

///Init Commands
int cmd_network_reset(PressureSensor *self){

    size_t tx_buffer_len =0;
    clear_buffers();

    packet_init(&self->protocol_packet,0x00,0xFF,NETWORK_RESET);
    packet_serialize(&self->protocol_packet,NULL,tx_buffer,&tx_buffer_len);
    //Send the Packet (No Response expected)
    if( serial_write_bytes(self->fd,tx_buffer,tx_buffer_len))
        return 0;
    else
        return -1;
}

int cmd_network_get_sn(PressureSensor *self)
{
    size_t tx_buffer_len =0;
    int bytes_read =0;
    size_t data_len =0;
    int ret;
    
    clear_buffers();
    packet_init(&self->protocol_packet,0x00,0xFF,NETWORK_GET_SN);
    packet_serialize(&self->protocol_packet,NULL,tx_buffer,&tx_buffer_len);
    
    if(! serial_write_bytes(self->fd,tx_buffer,tx_buffer_len))
        return -1;
    
    //Read the data
    bytes_read = serial_read_bytes(self->fd,rx_buffer,NETWORK_GET_SN_RX_LEN);
    if(bytes_read != NETWORK_GET_SN_RX_LEN){
        return -1;
    }
    memset(rx_data,0x00,sizeof(rx_data));
    ret = packet_deserialize(rx_buffer,bytes_read,rx_data,&data_len);
    if(data_len == self->sn_len){
        memcpy(self->sn,rx_data,data_len);
    }
    printf("Received SN: %s\n",(char *)rx_data);
    return ret;
}

int cmd_network_src_addr_set(PressureSensor *self)
{
    size_t tx_buffer_len =0;
    int bytes_read =0;
    size_t data_len =0;
    int ret;
    
    clear_buffers();
    packet_init(&self->protocol_packet,0x00,0xFF,NETWORK_SET_SRC_ADDR);
    for(size_t i=0;i<self->sn_len;i++){
        data_packet.data[i] = self->sn[i];
    }
    data_packet.data[self->sn_len] = NETWORK_TARGET_ADDR;
    data_packet.data_len = self->sn_len +1;
    packet_serialize(&self->protocol_packet,&data_packet,tx_buffer,&tx_buffer_len);
    
    if(! serial_write_bytes(self->fd,tx_buffer,tx_buffer_len))
        return -1;
    
    //Read the data
    bytes_read = serial_read_bytes(self->fd,rx_buffer,NETWORK_SET_SRC_ADDR_RX_LEN);
    if(bytes_read != NETWORK_SET_SRC_ADDR_RX_LEN){
        return -1;
    }
    memset(rx_data,0x00,sizeof(rx_data));
    ret = packet_deserialize(rx_buffer,bytes_read,rx_data,&data_len);
    if(data_len < self->sn_len + 1){
        return -1;
    }
    else{
        if(rx_data[self->sn_len] != NETWORK_TARGET_ADDR){
            fprintf(stderr,"Address Set does not match..\n");
            return -1;
        }
    }
    return 0;

}

int cmd_network_tr_set(PressureSensor *self)
{
    size_t tx_buffer_len =0;
    int bytes_read =0;
    size_t data_len =0;
    int ret;

    clear_buffers();
    packet_init(&self->protocol_packet,0x00,NETWORK_TARGET_ADDR,NETWORK_SET_TOKEN_RING);
    data_packet.data[0]= NETWORK_TOKEN_ADDRESS;
    data_packet.data_len = 1;
    packet_serialize(&self->protocol_packet,&data_packet,tx_buffer,&tx_buffer_len);
    if(! serial_write_bytes(self->fd,tx_buffer,tx_buffer_len))
        return -1;
    
    //Read the data
    bytes_read = serial_read_bytes(self->fd,rx_buffer,NETWORK_SET_TOKEN_RING_RX_LEN);
    if(bytes_read != NETWORK_SET_TOKEN_RING_RX_LEN){
        return -1;
    }
    memset(rx_data,0x00,sizeof(rx_data));
    ret = packet_deserialize(rx_buffer,bytes_read,rx_data,&data_len);
    if(ret <0){
        return ret;
    }
    if(data_len <  1){
        return -1;
    }
    else{
        if(rx_data[0] != NETWORK_TOKEN_ADDRESS){
            fprintf(stderr,"Token Ring Set does not match..\n");
            return -1;
        }
    }
    return 0;

}

int cmd_acquistion_start(PressureSensor *self){
    

}

int cmd_acquisition_stop(PressureSensor *self){

}

int cmd_acquisition_get(PressureSensor *self){

}


/// @brief Initialze Comm with Pressure sensor
/// @param self 
/// @param serial_fd Valid Com Port Descriptor
/// @return 0 if success
int init_pressure_sensor(PressureSensor *self, serial_fd_t serial_fd)
{
    self->fd = serial_fd;
    self->sn_len = 8;
    self->index=0;
    //Send a Network reset command
    if(cmd_network_reset(self) < 0)
        return -1;
    ///Insert a delay before sending next command
    sleep(2);
    if( cmd_network_get_sn(self) < 0){
        fprintf(stderr,"Error getting Unit SN..\n");
        return -1;
    }
    if( cmd_network_src_addr_set(self) < 0){
        fprintf(stderr,"Error setting Unit addr..\n");
        return -1;
    }
    if( cmd_network_tr_set(self) < 0){
        fprintf(stderr,"Error setting Unit Token Ring..\n");
        return -1;
    }
    return 0;
}

