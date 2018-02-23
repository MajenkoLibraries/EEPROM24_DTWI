#ifndef _EERROM24_H
#define _EEPROM24_H

#include <Arduino.h>
#include <DTWI.h>

class EEPROM24 {
    protected:  
        DTWI *_dtwi;
        uint8_t _i2caddr;
        size_t _bytes;

    public:
        EEPROM24(DTWI &dtwi, uint8_t a, size_t b) : _dtwi(&dtwi), _i2caddr(a), _bytes(b) {}
        EEPROM24(DTWI *dtwi, uint8_t a, size_t b) : _dtwi(dtwi), _i2caddr(a), _bytes(b) {}

        void begin();
        void begin(uint8_t addr);
        size_t write(size_t addr, uint8_t b);
        size_t write(size_t addr, const char *str);
        size_t write(size_t addr, const uint8_t *bytes, size_t len);
        uint8_t read(size_t addr);
        size_t read(size_t addr, uint8_t *bytes, size_t len);
        void end();
};
#endif
