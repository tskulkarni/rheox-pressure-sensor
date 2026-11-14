#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include "protocol.h"
#include "serial.h"

#define CMD_SYSTEM_RESET       0x10
#define CMD_ACQUISITION_START  0x44
#define CMD_ACQUISITION_STOP   0x46
#define CMD_ACQUISITION_GET    0x45

#define RESPONSE_TIMEOUT_MS    1000



#define NETWORK_RESET           0x45
#define NETWORK_GET_SN          0x51
#define NETWORK_SET_SRC_ADDR    0x52
#define NETWORK_SET_TOKEN_RING  0x4B
#define NETWORK_SERVICE_NORMAL  0x48


/// Set Addresses to Our Device
#define NETWORK_TARGET_ADDR     0x01
#define NETWORK_TOKEN_ADDRESS      0x02

///Expeced data sizes from the Sensor
#define PACKET_OVERHEAD               0x05 // Start, Length(2), CRC (2)
#define NETWORK_GET_SN_RX_LEN         0x0C + PACKET_OVERHEAD
#define NETWORK_SET_SRC_ADDR_RX_LEN   0x0D + PACKET_OVERHEAD
#define NETWORK_SET_TOKEN_RING_RX_LEN 0x05 + PACKET_OVERHEAD

#define ACQUISITION_START_RX_LEN      0x0F + PACKET_OVERHEAD
#define ACQUISITION_GET_RX_LEN        0x11 + PACKET_OVERHEAD
#define ACQUISITION_STOP_RX_LEN       0x09 + PACKET_OVERHEAD


#define  ACQUISITION_DECIMITATION     1    //25hz

typedef struct {
    serial_fd_t fd;
    size_t sn_len;
    uint8_t sn[16];
    uint32_t index;
    ProtocolPacket protocol_packet;
    DataPacket data_packet;
}PressureSensor;


int init_pressure_sensor(PressureSensor *self, serial_fd_t serial_fd);
int cmd_network_get_sn(PressureSensor *self);
int cmd_network_reset(PressureSensor *self);



#endif
