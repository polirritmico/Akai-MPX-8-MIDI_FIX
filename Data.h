/****************************************************************
 * 
 * First byte
 *   0x00 = no data
 *   0x01 = data
 * 
 * Second byte View mode
 *   0x00 = Velocity
 *   0x01 = Channel
 *   0x02 = Note
 * 
 * Third byte MIDI mode
 *   0x00 = Fix
 *   0x01 = Compression
 *   0x02 = Gate
 *   0x03 = Min
 *   0x04 = Bypass
 *   0x05 = FixChannel
 *   
 * Structure of the currentMidiValue:
 *   00000000
 *   00       -> ratio: 00_limiter, 01_4:1, 02_8:1, 03_12:1
 *     00     -> gain (* 20)
 *       0000 -> threshold (* 6)
 *        
 * Fourth byte Thru mode
 *   0x00 = Off
 *   0x01 = On
 *   
  ***************************************************************/

#include <EEPROM.h>

void saveData(Option _viewMode, Option _midiMode, uint8_t _midiValue, Option _thruMode) {
    uint8_t address = 0;

    EEPROM.write(address, 0x01);

    address++;
    switch (_viewMode) {
        case Velocity:
            EEPROM.write(address, 0x00);
            break;
        case Channel:
            EEPROM.write(address, 0x01);
            break;
        case Pitch:
            EEPROM.write(address, 0x02);
            break;
    }

    address++;
    switch (_midiMode) {
        case Fix:
            EEPROM.write(address, 0x00);
            break;
        case Compression:
            EEPROM.write(address, 0x01);
            break;
        case Gate:
            EEPROM.write(address, 0x02);
            break;
        case Min:
            EEPROM.write(address, 0x03);
            break;
        case Bypass:
            EEPROM.write(address, 0x04);
            break;
        case FixChannel:
            EEPROM.write(address, 0x05);
            break;
    }

    address++;
    if (_midiMode != Bypass)
        EEPROM.write(address, _midiValue);

    address++;
    switch (_thruMode) {
        case ThruOff:
            EEPROM.write(address, 0x00);
            break;
        case ThruOn:
            EEPROM.write(address, 0x01);
            break;
        case ThruUSB:
            EEPROM.write(address, 0x02);
            break;
    }
}

void loadData() {
    uint8_t address = 0;

    if (EEPROM.read(address) == 0x01 ) {
        address++;
        switch (EEPROM.read(address)) {
            case 0x00:
                currentViewMode = Velocity;
                break;
            case 0x01:
                currentViewMode = Channel;
                break;
            case 0x02:
                currentViewMode = Pitch;
                break;
        }
        address++;
        switch (EEPROM.read(address)) {
            case 0x00:
                currentMidiMode = Fix;
                break;
            case 0x01:
                currentMidiMode = Compression;
                break;
            case 0x02:
                currentMidiMode = Gate;
                break;
            case 0x03:
                currentMidiMode = Min;
                break;
            case 0x04:
                currentMidiMode = Bypass;
                break;
            case 0x05:
                currentMidiMode = FixChannel;
                break;
        }
        address++;
        if (currentMidiMode != Bypass)
            currentMidiValue = EEPROM.read(address);

        address++;
        switch (EEPROM.read(address)) {
            case 0x00:
                currentThruMode = ThruOff;
                break;
            case 0x01:
                currentThruMode = ThruOn;
                break;
            case 0x02:
                currentThruMode = ThruUSB;
                break;
        }
    }
    else {
        currentViewMode = Velocity;
        currentMidiMode = Bypass;
        currentMidiValue = 0x00;
        currentThruMode = ThruOff;
    }
}
