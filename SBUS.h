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
    uint8_t parseState = 0, curByte, prevByte = footer; // parseState is 0 if waiting for the header byte to arrive

public:
    uint16_t channels[16];
    bool frameLost;
    bool failSafe; // loss of connection betweeen the transmitter and receiver
    
    
public:
    SBUS (HardwareSerial* serial);
    void begin();
    bool read();
    bool parse();
};

#endif /* SBUS_h */


