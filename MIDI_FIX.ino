/***********************************************************************

    MIDI_FIX v0.1

    Initially done to connect any drumtrigger to the AKAI MPX8 Sampler.

    This FIX work with the Arduino UNO and a Sparkfun MIDI Shield. Also
    I add a 4x7 segments display with the TM1637 I2c for show the
    current stats and setup the machine (Through 3 pushbuttons) and a
    led for MIDI input.

    The MIDI_FIX take each note_on midi message and instantly send a
    note_off MIDI message with the same note, velocity and chanel info.
    This work for conecting drum triggers with machines that have
    troubles reading MIDI messages in the "Running status" mode (like
    the AKAI MPX8).
    I also add some basic MIDI functions:

        1. FIX mode: In this mode any output note_on message will have
        the current fix velocity value selected in the setup menu.

        2. COMPRESSOR mode: You can setup the Threshold, the Input Gain
        and the ratio (4:1, 8:1, 12:1 and inf).

        3. GATE mode: The MIDI_FIX will ignore all the MIDI note_on
        messages below the velocity setup parameter.

        4. MIN mode: In this mode any note_on message with a velocity
        value below the current setup, will be fixed to the MIN value.

        5. Bypass mode: In this mode no changes on the velocity are
        made. This is not like a through MIDI port.

    Each menu and submenus functions are in a big while.

    This code is in the public domain

    Any questions about this, send it to ebray187@gmail.com with the
    subject: Arduino MIDI_FIX

    30/08/2016 Santiago de Chile
    Eduardo Bray

 ***********************************************************************/


//***********************************************************************
// GLOBAL CONST & VARIABLES

// Debug mode: 0 OFF, 1 ON
static const bool DEBUG_MODE = false;

// Delay values
static const uint16_t DELAY_BUTTON = 200;
static const uint16_t DELAY_DISPLAY = 800;
static const uint16_t DELAY_ANIMATION = 35;
#define NOP __asm__ __volatile__ ("nop\n\t")// delay 62.5ns on a 16MHz AtMega

bool displayStateOn = true;

//***********************************************************************
// ARDUINO CONECTIONS.
// Pin numbers for the 7 segment display:

static const uint8_t CLK  = 11;
static const uint8_t DIO = 10;
// Pin numbers for LEDs
static const uint8_t LED1 = 5;
static const uint8_t LED2 = 12;
// Pin numbers for buttons
static const uint8_t BTN_UP = 2;
static const uint8_t BTN_DOWN = 3;
static const uint8_t BTN_MODE = 4;

// The buttons and leds on the MIDI Shield are active LOW, so we use
// this values:
static const bool ON = LOW;
static const bool OFF = HIGH;

//***********************************************************************
// COMPRESOR VALUES. As global variables so that are not recalculated
// for each note. "More memory less processor".

uint8_t ratio;
uint8_t threshold;
uint8_t gain;

//***********************************************************************
// Load Libraries: <TM1637Display.h>, <MIDI.h> Arduino MIDI library 4.2
#include "Messages.h"

// As a global variable so it can be used anytime by the print function
// called by the MIDI event handler.
Option currentViewMode;
Option currentMidiMode;
Option currentThruMode;
uint8_t currentMidiValue;

#include "Data.h"
#include "Display.h"
#include "MIDI.h"


//***********************************************************************
// Init
void setup() {
    //start the leds & buttons
    pinMode(13, OUTPUT); digitalWrite(13, LOW);//Shutdown the L LED
    pinMode(BTN_MODE, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_UP, INPUT_PULLUP);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);

    switch (DEBUG_MODE) {
        case 0:
            loadData();
            break;
        default:
            currentViewMode = Velocity;
            currentThruMode = ThruOff;
            currentMidiMode = Compression;
            currentMidiValue = 0b01001010;// Thsr 87, Gain 0, Ratio 4:1
            //currentMidiMode = Bypass;
            //currentMidiValue = 0x00;
            break;
    }

    // Get the values from the currentMidiValue byte.
    // First two bites for the ratio. (0 = inf);
    ratio = ((currentMidiValue >> 6) * 4);
    // The four bites from the right are the threshold value
    threshold = (((currentMidiValue & 0b00001111) * 8) + 7);
    // The two remaining bites are the gain value
    gain = (((currentMidiValue >> 4) & 0b00000011) * 10);

    // Finally we start the display and the MIDI.
    startDisplay();
    startMIDI();
}

//***********************************************************************
// Main
void loop() {
    // For Debugging
    //    if (digitalRead(BTN_UP) == ON) {
    //        // Velocity
    //        //            for (uint8_t i = 0x50; i < 0x80; i++) {
    //        //                handleNoteOn(0x0A, 0x2A, i);
    //        //                delay(400);
    //        //            }
    //        // Note pitch
    //        for (uint8_t i = 0x24; i < 0x48; i++) {
    //            handleNoteOn(0x0A, i, 0x68);//byte channel, byte pitch, byte velocity
    //            delay(400);
    //        }
    //    }
    if (digitalRead(BTN_MODE) == ON) {
        if (displayStateOn) {
            confMenu();
        }
        else {
            displayStateOn = true;
            display.setBrightness(0xFF);
            switch (currentViewMode) {
                case Velocity:
                    display.setSegments(VIEW_MENU[0]);
                    break;
                case Channel:
                    display.setSegments(VIEW_MENU[1]);
                    break;
                case Pitch:
                    display.setSegments(VIEW_MENU[2]);
                    break;
            }
        }
        delay(DELAY_BUTTON);
    }
    else if (digitalRead(BTN_UP) == ON) {
        display.setBrightness(0xFF);
        switch (displayStateOn) {
            case true: {
                    switch (currentViewMode) {
                        case Velocity:
                            currentViewMode = Channel;
                            display.setSegments(VIEW_MENU[1]);
                            break;
                        case Channel:
                            currentViewMode = Pitch;
                            display.setSegments(VIEW_MENU[2]);
                            break;
                        case Pitch:
                            currentViewMode = Velocity;
                            display.setSegments(VIEW_MENU[0]);
                            break;
                    }
                    break;
                }
            case false: {
                    displayStateOn = true;
                    switch (currentViewMode) {
                        case Velocity:
                            display.setSegments(VIEW_MENU[0]);
                            break;
                        case Channel:
                            display.setSegments(VIEW_MENU[1]);
                            break;
                        case Pitch:
                            display.setSegments(VIEW_MENU[2]);
                            break;
                    }
                    break;
                }
        }
        delay(DELAY_BUTTON);
    }
    else if (digitalRead(BTN_DOWN) == ON) {
        if (displayStateOn) {
            display.setBrightness(0x00);
            display.setSegments(MESSAGES[4]);
        }
        else {
            display.setBrightness(0xFF);
            switch (currentViewMode) {
                case Velocity:
                    display.setSegments(VIEW_MENU[0]);
                    break;
                case Channel:
                    display.setSegments(VIEW_MENU[1]);
                    break;
                case Pitch:
                    display.setSegments(VIEW_MENU[2]);
                    break;
            }
        }
        displayStateOn = !displayStateOn;
        delay(DELAY_BUTTON);
    }
    MIDI.read();
}
