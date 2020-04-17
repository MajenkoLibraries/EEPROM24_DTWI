#include <EEPROM24_DTWI.h>

void EEPROM24::begin() {
    _dtwi->beginMaster();
}

void EEPROM24::begin(uint8_t addr) {
    _i2caddr = addr;
    _dtwi->beginMaster();
}

size_t EEPROM24::write(size_t addr, uint8_t val) {
    if (addr >= _bytes) return 0;

    uint8_t i2caddr = _i2caddr;

    uint8_t reg = addr;
    if (addr > 255) {
        i2caddr |= 1;
    }
    
    uint8_t state = 0;
    uint32_t ts = millis();
    while (1) {
        if (millis() - ts > 100) {
            _dtwi->stopMaster();
        }
        switch (state) {
            case 0: // begin write 
                if (_dtwi->startMasterWrite(i2caddr)) {
                    state = 1;
                }
                break;
            case 1: // Send register address
                if (_dtwi->write(&reg, 1) == 1) {
                    state = 2;
                }
                break;
            case 2: // Send register address
                if (_dtwi->write(&val, 1) == 1) {
                    state = 3;
                }
                break;
            case 3: // Stop Master
                if (_dtwi->stopMaster()) {
                    return 1;
                }
                break;
        }
    }
    return 1;
}

size_t EEPROM24::write(size_t addr, const uint8_t *bytes, size_t len) {
    size_t c = 0;
    for (size_t i = 0; i < len; i++) {
        c += write(addr + i, bytes[i]);
    }
    return c;
}

size_t EEPROM24::write(size_t addr, const char *str) {
    size_t c = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        c += write(addr + i, str[i]);
    }
    return c;
}

uint8_t EEPROM24::read(size_t addr) {
    if (addr >= _bytes) return 0;


    uint8_t state = 0;
    uint8_t val = 0;
    uint32_t ts = millis();

    uint8_t i2caddr = _i2caddr;

    uint8_t reg = addr;
    i2caddr |= (addr >> 8);

    while (1) {
        if (millis() - ts > 100) {
            _dtwi->stopMaster();
            return 0;
        }
        switch (state) {
            case 0: // begin write 
                if (_dtwi->startMasterWrite(i2caddr)) {
                    state = 1;
                }
                break;
            case 1: // Send register address
                if (_dtwi->write(&reg, 1) == 1) {
                    state = 2;
                }
                break;
            case 2: // Stop Master
                if (_dtwi->stopMaster()) {
                    state = 3;
                }
                break;
            case 3:
                if (_dtwi->startMasterRead(i2caddr, 1)) {
                    state = 4;
                }
                break;
            case 4:
                if (_dtwi->available()) {
                    _dtwi->read(&val, 1);
                    state = 5;
                }
                break;
            case 5:
                if (_dtwi->stopMaster()) {
                    return val;
                }
                break;
        }
    }
    return 0;
}

size_t EEPROM24::read(size_t addr, uint8_t *b, size_t len) {
    if (addr >= _bytes) return 0;

    for (size_t i = 0; i < len; i++) {
        b[i] = read(addr + i);
    }
    return len;
}

void EEPROM24::end() {
    _dtwi->endMaster();
}
