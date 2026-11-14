static uint64_t get_time_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}



bool send_packet(serial_fd_t fd, const ProtocolPacket *pkt)
{
    uint8_t buffer[270];
    uint8_t len;
    
    packet_write_bytes(pkt, buffer, &len);
    
    return serial_write_bytes(fd, buffer, len);
}

bool receive_packet(serial_fd_t fd, ProtocolPacket *pkt, uint32_t timeout_ms)
{
    uint8_t buffer[270];
    uint64_t start_time = get_time_ms();
    int bytes_read = 0;
    
    while (get_time_ms() - start_time < timeout_ms) {
        int n = serial_read_bytes(fd, &buffer[bytes_read], sizeof(buffer) - bytes_read);
        if (n > 0) {
            bytes_read += n;
            
            if (bytes_read >= 3) {
                uint16_t pkt_len = (buffer[1] << 8) | buffer[2];
                if (bytes_read >= pkt_len + 3) {
                    memcpy(pkt, buffer, bytes_read);
                    return true;
                }
            }
        } else {
            usleep(1000);
        }
    }
    
    return false;
}

bool cmd_system_reset(serial_fd_t fd, uint8_t token_ring, uint8_t source, uint8_t network)
{
    ProtocolPacket pkt;
    packet_build(&pkt, token_ring, source, network, 0, CMD_SYSTEM_RESET, NULL, 0);
    return send_packet(fd, &pkt);
}

bool cmd_acquisition_start(serial_fd_t fd, uint8_t token_ring, uint8_t source, uint8_t network)
{
    ProtocolPacket pkt;
    packet_build(&pkt, token_ring, source, network, 0, CMD_ACQUISITION_START, NULL, 0);
    return send_packet(fd, &pkt);
}

bool cmd_acquisition_stop(serial_fd_t fd, uint8_t token_ring, uint8_t source, uint8_t network)
{
    ProtocolPacket pkt;
    packet_build(&pkt, token_ring, source, network, 0, CMD_ACQUISITION_STOP, NULL, 0);
    return send_packet(fd, &pkt);
}

int cmd_acquisition_get(serial_fd_t fd, uint8_t token_ring, uint8_t source, uint8_t network, uint8_t *data, uint8_t *len)
{
    ProtocolPacket pkt;
    packet_build(&pkt, token_ring, source, network, 0, CMD_ACQUISITION_GET, NULL, 0);
    
    if (!send_packet(fd, &pkt)) {
        return -1;
    }
    
    ProtocolPacket response;
    if (!receive_packet(fd, &response, RESPONSE_TIMEOUT_MS)) {
        return -1;
    }
    
    uint16_t data_len = (response.length[0] << 8) | response.length[1];
    data_len -= 8;
    
    if (data && len && data_len > 0) {
        memcpy(data, response.data, data_len);
        *len = data_len;
    }
    
    return data_len;
}