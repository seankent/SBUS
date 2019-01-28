/*
 * SBUS.h
 * Sean Kent
 * seankent@mit.edu
 */

#ifndef SBUS_h
#define SBUS_h

#include "Arduino.h"

class SBUS
{
private:
    HardwareSerial* serial;
    const uint32_t baud = 100000; // baud rate
    const uint8_t header = 0x0F; // SBUS header
    const uint8_t footer = 0x00; // SBUS end byte
    static const uint8_t dataSize = 23; // number of bytes in an SBUS message (not including the header and footer bytes)
    uint8_t data[dataSize];
    int parseState = -1; // parseState is -1 if waiting for the header byte to arrive
    uint8_t curByte, prevByte = footer;

public:
    uint16_t channels[16];
    bool frameLost;
    bool failSafe; // loss of connection betweeen the transmitter and receiver
    const uint16_t channelMin = 172, channelMax = 1811; // the min and max value that a channel can be set to

public:
    SBUS (HardwareSerial* serial);
    void begin();
    bool read();
    bool parse();
};

#endif /* SBUS_h */


