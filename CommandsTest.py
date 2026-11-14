#!/usr/bin/python3
import serial
SEED = 0xFFFF
GP = 0xA001

CMD_NETWOTK_RESET = [ 0xAA, 0x00, 0x04, 0x00, 0xFF, 0x00, 0x45]
CMD_NetworkSerialNumberGet = [0xAA, 0x00, 0x04, 0x00, 0xFF, 0x00, 0x51]


def crc_calculate(byte: int, crc: int) -> int:
    crc ^= (byte & 0xFF)
    for _ in range(8):
        carry = crc & 0x0001
        crc >>= 1
        if carry:
            crc ^= GP
    return crc & 0xFFFF


def get_packet_crc(crc_packet):
    crc = SEED
    for b in crc_packet:
        b = b & 0xFF
        crc = crc_calculate(b,crc)
    return crc
    
def send_data(tx_data:bytes):
    # 1. Open the serial port
# Windows ports look like 'COM3'; Linux/macOS look like '/dev/ttyUSB0' or '/dev/ttyACM0'
    with serial.Serial(port='/dev/ttyUSB0', baudrate=921600, timeout=1) as ser:
        # Optional: Wait 2 seconds if connecting to a microcontroller (like Arduino) that resets on open
        #time.sleep(2)

        # 2. Write byte data
        # Option A: Raw hex bytes using a bytes literal
        print("TX Datat:", tx_data.hex(' '))
        ser.write(tx_data)

        # 3. Read byte data
        raw_response = ser.read(256)
        print("Raw bytes:", raw_response)

        # Convert received bytes to a list of integers or hex string for inspection
        print("As integers:", list(raw_response))
        print("As hex:", raw_response.hex(' '))




#full_packet = [0xaa,0x00,0x08,0x01,0x01,0x00,0x00,0x75,0x29,0x01,0x02,0xda,0x92]
#send_data(full_packet)

#crc_packet = full_packet[3:-2]
#print(crc_packet
#print(hex(crc))
#full_packet = [0xaa,0x00,0x07,0x01,0x01,0x00,0x00,0x76,]
# Send a test command,This is the entire cmd without CRC and wait for RX

def send_cmd(cmd_data):
    tx_cmd = cmd_data
    crc_packet = cmd_data[3:]
    crc = get_packet_crc(crc_packet)
    print(f'{crc=}')
    tx_cmd.append( crc & 0xFF) # Low Byte First
    tx_cmd.append((crc >> 8) & 0xFF)

    tx_cmd = bytes(tx_cmd)
    print(tx_cmd.hex())
    print(f'{tx_cmd=}')
    print(len(tx_cmd))
    send_data(tx_cmd)



send_cmd(CMD_NETWOTK_RESET)
send_cmd(CMD_NetworkSerialNumberGet)

