#include <MIDI.h>

// Generate the MIDI instance
MIDI_CREATE_DEFAULT_INSTANCE();
void handleNoteOn(byte channel, byte pitch, byte velocity);

void startMIDI() {
    MIDI.begin(MIDI_CHANNEL_OMNI);
    setNoteOnHandler(currentThruMode);
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
    digitalWrite(LED1, HIGH);

    switch (currentMidiMode) {
        case Fix:
            velocity = currentMidiValue;
            break;
        case Compression:
            {
                /**************************************************************

                    THE COMPRESSOR:

                    The byte code for the Ratio, gain and Threshold is this:

                    0000 0000
                    00        -> ratio: 00_limiter, 01_4:1, 02_8:1, 03_12:1
                      00      -> gain (* 20)
                         0000 -> threshold (* 6)

                    So, to get the ratio, threshold and gain values:
                        ratio = ((currentMidiValue >> 6) * 4);
                        threshold = (((currentMidiValue & 0b00001111) * 8) + 7);
                        gain = (((currentMidiValue >> 4) & 0b00000011) * 10);

                    This is done in the Compressor display submenu so there's
                    no need to calculate the same values for all the notes_on

                    The algorith avoid division in the calc of the compressor
                    gain because the AVR core of the Arduino doesn't have
                    implemented native division. So we are using shifts for
                    improve the performance. See the next article for more info
                    about the math:
                    http://www.codeproject.com/Articles/17480/Optimizing-integer-divisions-with-Multiply-Shift-i

                    This implementation is about 13 times faster than regular
                    division. Anyway, the standar math is in the default case
                    in the switch.

                 *************************************************************/

                //velocity is a uint8_t. We use this var to go bigger than 127.
                int _vel;

                // Compressor ON/OFF
                if (velocity >= threshold) {
                    switch (ratio) {
                        case 0: // limiter:
                            _vel = threshold + gain;
                            break;
                        case 4:
                            _vel = (velocity - threshold) >> 2;
                            _vel += threshold + gain;
                            break;
                        case 8:
                            _vel = (velocity - threshold) >> 3;
                            _vel += threshold + gain;
                            break;
                        case 12:
                            _vel = (( velocity - threshold) * 43) >> 9; //¿43? Magic number. see the description.
                            _vel += threshold;
                            break;
                        default:
                            _vel = ( threshold - velocity ) * (1 - ((float)1 / ratio));
                            _vel += velocity + gain;
                            break;
                    }
                }
                else {
                    _vel = velocity + gain;
                }

                //velocity = (_vel > 127 ? 127 : _vel);
                if (_vel > 127)
                    velocity = 127;
                else
                    velocity = _vel;
            }
            break;
        //***********************************************************************
        case Gate:
            if (velocity < currentMidiValue) {
                //delay(1); //??? Rlly inefficient
                NOP; // delay 62.5ns on a 16MHz AtMega
                digitalWrite(LED1, LOW);
                return;
            }
            break;
        case Min:
            velocity = ( velocity < currentMidiValue ? currentMidiValue : velocity );
            break;
        case FixChannel:
            channel = currentMidiValue;
            break;
    }

    // This should FIX the note off & Running status problems with the AKAI MPX8
    // The double note on and one note off
    MIDI.sendNoteOn(pitch, velocity, channel);
    MIDI.sendNoteOn(pitch, velocity, channel);
    MIDI.sendNoteOff(pitch, velocity, channel);
    //NOP;

    // Print signal Info
    switch (currentViewMode) {
        case Velocity:
            display.showNumberDec(velocity, 0, 3, 1);
            break;
        case Channel:
            display.showNumberDec(channel, 0, 3, 1);
            break;
        case Pitch:
            display.showNumberDec(pitch, 0, 3, 1);
            break;
    }
    digitalWrite(LED1, LOW);
}

void handleNoteOnThru(byte channel, byte pitch, byte velocity) {

    digitalWrite(LED1, HIGH);
    switch (currentViewMode) {
        case Velocity:
            display.showNumberDec(velocity, 0, 3, 1);
            break;
        case Channel:
            display.showNumberDec(channel, 0, 3, 1);
            break;
        case Pitch:
            display.showNumberDec(pitch, 0, 3, 1);
            break;
    }
    digitalWrite(LED1, LOW);
}

void handleNoteOnUSB(byte channel, byte pitch, byte velocity) {
    digitalWrite(LED1, HIGH);

    switch (currentMidiMode) {
        case Fix:
            velocity = currentMidiValue;
            break;
        case Compression:
            {
                int _vel;
                if (velocity >= threshold) {
                    switch (ratio) {
                        case 0: // limiter:
                            _vel = threshold + gain;
                            break;
                        case 4:
                            _vel = (velocity - threshold) >> 2;
                            _vel += threshold + gain;
                            break;
                        case 8:
                            _vel = (velocity - threshold) >> 3;
                            _vel += threshold + gain;
                            break;
                        case 12:
                            _vel = (( velocity - threshold) * 43) >> 9; //¿43? Magic number. see the description.
                            _vel += threshold;
                            break;
                        default:
                            _vel = ( threshold - velocity ) * (1 - ((float)1 / ratio));
                            _vel += velocity + gain;
                            break;
                    }
                }
                else {
                    _vel = velocity + gain;
                }

                if (_vel > 127)
                    velocity = 127;
                else
                    velocity = _vel;
            }
            break;
        //***********************************************************************
        case Gate:
            if (velocity < currentMidiValue) {
                //delay(1); //??? Rlly inefficient
                NOP; // delay 62.5ns on a 16MHz AtMega
                digitalWrite(LED1, LOW);
                return;
            }
            break;
        case Min:
            velocity = ( velocity < currentMidiValue ? currentMidiValue : velocity );
            break;
        case FixChannel:
            channel = currentMidiValue;
            break;
    }

    MIDI.sendNoteOn(pitch, velocity, channel);
    MIDI.sendNoteOff(pitch, velocity, channel);

    // Print signal Info
    switch (currentViewMode) {
        case Velocity:
            display.showNumberDec(velocity, 0, 3, 1);
            break;
        case Channel:
            display.showNumberDec(channel, 0, 3, 1);
            break;
        case Pitch:
            display.showNumberDec(pitch, 0, 3, 1);
            break;
    }
    digitalWrite(LED1, LOW);
}


void setNoteOnHandler(Option _thruMode) {
    switch (_thruMode) {
        case ThruOff:
            MIDI.setHandleNoteOn(handleNoteOn);
            MIDI.turnThruOff();
            return;
        case ThruOn:
            MIDI.turnThruOn();
            MIDI.setHandleNoteOn(handleNoteOnThru);
            return;
        case ThruUSB:
            MIDI.setHandleNoteOn(handleNoteOnUSB);
            MIDI.turnThruOff();
            return;
    }

}


