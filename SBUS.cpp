/*
 * SBUS.cpp
 * Sean Kent
 * seankent@mit.edu
 */

/*
 * The SBUS protocol uses inverted serial logic with a baud rate of 100000, 8 data bits, even parity bit,
 * and 2 stop bits. The SBUS packet is 25 bytes long consisting of:
 *
 * Byte[0]: SBUS Header, 0x0F
 * Byte[1-22]: 16 servo channels, 11 bits per servo channel
 * Byte[23]:
 *     + Bit 7: digital channel 17 (0x80)
 *     + Bit 6: digital channel 18 (0x40)
 *     + Bit 5: frame lost (0x20)
 *     + Bit 4: failsafe activated (0x10)
 *     + Bit 0 - 3: n/a
 * Byte[24]: SBUS End Byte, 0x00
 */

#include "SBUS.h"

// SBUS object, input the serial bus
SBUS::SBUS (HardwareSerial* serial)
{
    this->serial = serial;
}

// starts the serial communication
void SBUS::begin()
{
    serial->flush();
    serial->begin(baud, SERIAL_8E2);
}

// read the SBUS data
bool SBUS::read()
{
    if (parse()) // parse() returns true if data has been updated with a new message
        {
        // 16 channels of 11 bit data
        channels[0]  = (uint16_t) ((data[0]       | data[1]  << 8)                  & 0x07FF);
        channels[1]  = (uint16_t) ((data[1]  >> 3 | data[2]  << 5)                  & 0x07FF);
        channels[2]  = (uint16_t) ((data[2]  >> 6 | data[3]  << 2 | data[4]  << 10) & 0x07FF);
        channels[3]  = (uint16_t) ((data[4]  >> 1 | data[5]  << 7)                  & 0x07FF);
        channels[4]  = (uint16_t) ((data[5]  >> 4 | data[6]  << 4)                  & 0x07FF);
        channels[5]  = (uint16_t) ((data[6]  >> 7 | data[7]  << 1 | data[8]  << 9)  & 0x07FF);
        channels[6]  = (uint16_t) ((data[8]  >> 2 | data[9]  << 6)                  & 0x07FF);
        channels[7]  = (uint16_t) ((data[9]  >> 5 | data[10] << 3)                  & 0x07FF);
        channels[8]  = (uint16_t) ((data[11]      | data[12] << 8)                  & 0x07FF);
        channels[9]  = (uint16_t) ((data[12] >> 3 | data[13] << 5)                  & 0x07FF);
        channels[10] = (uint16_t) ((data[13] >> 6 | data[14] << 2 | data[15] << 10) & 0x07FF);
        channels[11] = (uint16_t) ((data[15] >> 1 | data[16] << 7)                  & 0x07FF);
        channels[12] = (uint16_t) ((data[16] >> 4 | data[17] << 4)                  & 0x07FF);
        channels[13] = (uint16_t) ((data[17] >> 7 | data[18] << 1 | data[19] << 9)  & 0x07FF);
        channels[14] = (uint16_t) ((data[19] >> 2 | data[20] << 6)                  & 0x07FF);
        channels[15] = (uint16_t) ((data[20] >> 5 | data[21] << 3)                  & 0x07FF);

        frameLost = (data[22] & 0x20) >> 5; // update frameLost bit (data[22], bit 5)
        failSafe = (data[22] & 0x10) >> 4; // update failSafe bit (data[22], bit 4)
    
        return true;
        }
    else
        {
        return false; // the channels were not updated
        }
}

// parse the SBUS data
bool SBUS::parse()
{
    while (serial->available() > 0) // see if serial data is available to read
        {
        curByte = serial->read(); // read() returns the first byte of serial data available
        
        if (parseState == -1) // waiting for the header byte to arrive
            {
            if (curByte == header && prevByte == footer)
                {
                parseState++; // once we see the header, we can begin updating data with the new information
                }
            }
        else
            {
            // update data (one byte at a time)
            if (parseState < dataSize) // parseState = 1 cooresponds to the first byte in data
                {
                data[parseState] = curByte;
                parseState++;
                }
            else
                {
                parseState = -1; // reset parseState
                
                if (curByte == footer) return true; // all the data has been updated
                else return false; // the last byte was not the SBUS end byte (something must have gone wrong)
                }
            }
        prevByte = curByte;
        }
    return false; // no data is available to read
}
