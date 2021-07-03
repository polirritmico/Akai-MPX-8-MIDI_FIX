#include <TM1637Display.h>

TM1637Display display(CLK, DIO);

//***********************************************************************
// Begin the display object, setup the brightness and display the startup menssage. Show the current INFO option
void startDisplay(){
    display.setBrightness(0xff); // bytes beetwen 0-255
    display.setSegments( new uint8_t[4] {0x00, 0x00, 0x00, 0x00} ); // Clean the display
    delay(500);
    switch(DEBUG_MODE){
        case 1:
            display.setSegments(MESSAGES[3]);
            delay(500);
            break;
        default:
            for(uint8_t i = 0; i != 35; i++){
                display.setSegments(new uint8_t[4] {STARTUP[i], STARTUP[i+1], STARTUP[i+2], STARTUP[i+3]});
                delay(110);
            }
            break;
    }
    delay(200);
    // Display the current Mode
    switch(currentViewMode){
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

//***********************************************************************
// option MENU
// A lot of switches inside a while(true). The submenus are inside its own while. Its the same code for each menu
void confMenu(){
    // First a small animation.
    display.setSegments(MESSAGES[1]);
    delay(220);
    for(uint8_t i = 0; i != 8; i++){
        display.setSegments(new uint8_t[4] { MAIN_MENU_ANIMATION[i], MAIN_MENU_ANIMATION[i+1], 
                                             MAIN_MENU_ANIMATION[i+2], MAIN_MENU_ANIMATION[i+3] } );
        delay(DELAY_ANIMATION);
    }

    // The current option display in the menu (or submenu)
    uint8_t current = 0;
    display.setSegments(MAIN_MENU[0]);
    delay(DELAY_BUTTON);
    while(1){
        if(digitalRead(BTN_DOWN) == ON){
            switch(current){
                case 0: // if the current is MIDI
                    display.setSegments(MAIN_MENU[1]);
                    current = 1;
                    break;
                case 1:
                    display.setSegments(MAIN_MENU[2]);
                    current = 2;
                    break;
                case 2:
                    display.setSegments(MAIN_MENU[3]);
                    current = 3;
                    break;
                case 3:
                    display.setSegments(MAIN_MENU[0]);
                    current = 0;
                    break;
            }
            delay(DELAY_BUTTON);
        }
        if(digitalRead(BTN_UP) == ON){
            switch(current){
                case 0:
                    display.setSegments(MAIN_MENU[3]);
                    current = 3;
                    break;
                case 1:
                    display.setSegments(MAIN_MENU[0]);
                    current = 0;
                    break;
                case 2:
                    display.setSegments(MAIN_MENU[1]);
                    current = 1;
                    break;
                case 3:
                    display.setSegments(MAIN_MENU[2]);
                    current = 2;
                    break;
            }
            delay(DELAY_BUTTON);
        }
        if(digitalRead(BTN_MODE) == ON){
            switch(current){
///////////////////////////////////////////////////////////////
// MIDI Submenu
                case 0: 
                    current = 0;
                    display.setSegments(MIDI_MENU[0]);
                    delay(DELAY_DISPLAY);
                    while(1){
                        if(digitalRead(BTN_DOWN) == ON){
                            switch(current){
                                case 0:
                                    display.setSegments(MIDI_MENU[1]);
                                    current = 1;
                                    break;
                                case 1:
                                    display.setSegments(MIDI_MENU[2]);
                                    current = 2;
                                    break;
                                case 2:
                                    display.setSegments(MIDI_MENU[3]);
                                    current = 3;
                                    break;
                                case 3:
                                    display.setSegments(MIDI_MENU[4]);
                                    current = 4;
                                    break;
                                case 4:
                                    display.setSegments(MIDI_MENU[5]);
                                    current = 5;
                                    break;
                                case 5:
                                    display.setSegments(MIDI_MENU[6]);
                                    current = 6;
                                    break;
                                case 6:
                                    display.setSegments(MIDI_MENU[0]);
                                    current = 0;
                                    break;
                            }
                            delay(DELAY_BUTTON);
                        }
                        if(digitalRead(BTN_UP) == ON){
                            switch(current){
                                case 0:
                                    display.setSegments(MIDI_MENU[6]);
                                    current = 6;
                                    break;
                                case 1:
                                    display.setSegments(MIDI_MENU[0]);
                                    current = 0;
                                    break;
                                case 2:
                                    display.setSegments(MIDI_MENU[1]);
                                    current = 1;
                                    break;
                                case 3:
                                    display.setSegments(MIDI_MENU[2]);
                                    current = 2;
                                    break;
                                case 4:
                                    display.setSegments(MIDI_MENU[3]);
                                    current = 3;
                                    break;
                                case 5:
                                    display.setSegments(MIDI_MENU[4]);
                                    current = 4;
                                    break;
                                case 6:
                                    display.setSegments(MIDI_MENU[5]);
                                    current = 5;
                                    break;
                            }
                            delay(DELAY_BUTTON);
                        }
                        if(digitalRead(BTN_MODE) == ON){
                            switch(current){
///////////////////////////////////////////////////////////////
// MIDI : FIX Submenu
                                case 0:
                                    if(currentMidiMode != Fix){
                                        currentMidiValue = 0x6C;
                                    }
                                    currentMidiMode = Fix;
                                    display.showNumberDec(currentMidiValue);
                                    delay(DELAY_DISPLAY);
                                    while(1){
                                        if(digitalRead(BTN_DOWN) == ON && currentMidiValue > 0x05){
                                            currentMidiValue -= 0x06;
                                            display.showNumberDec(currentMidiValue);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_UP) == ON && currentMidiValue < 0x7E){
                                            currentMidiValue += 0x06;
                                            display.showNumberDec(currentMidiValue);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_MODE) == ON){
                                            display.setSegments(MESSAGES[2]);
                                            delay(DELAY_DISPLAY);
                                            switch(currentViewMode){
                                                case Velocity:
                                                    display.setSegments(VIEW_MENU[0]);
                                                    return;
                                                case Channel:
                                                    display.setSegments(VIEW_MENU[1]);
                                                    return;
                                                case Pitch:
                                                    display.setSegments(VIEW_MENU[2]);
                                                    return;
                                            }
                                        }
                                    }
                                    break;
///////////////////////////////////////////////////////////////
// MIDI : COMPRESSION Submenu
// More info in the MIDI.h
                                case 1:{
                                    if(currentMidiMode != Compression){
                                       currentMidiValue = 0b01001010;// 87 thsr, gain 0, ratio 4:1
                                    }
                                    currentMidiMode = Compression;
                                    
                                    display.setSegments(MIDI_COMP[0]);
                                    delay(1000);
///////////////////////////////////////////////////////////////
// Threshold
                                    display.showNumberDec(((currentMidiValue & 0b00001111) * 8) + 7);
                                    delay(DELAY_DISPLAY);
                                    while(1){
                                        if(digitalRead(BTN_DOWN) == ON && (currentMidiValue & 0b00001111) > 0x00){
                                            currentMidiValue -= 0x01;
                                            display.showNumberDec(((currentMidiValue & 0b00001111) * 8) + 7);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_UP) == ON && (currentMidiValue & 0b00001111) < 0x0F){
                                            currentMidiValue += 0x01;
                                            display.showNumberDec(((currentMidiValue & 0b00001111) * 8) + 7);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_MODE) == ON){
                                            // Set the Threshold value
                                            threshold = (((currentMidiValue & 0b00001111) * 8) + 7);
                                            break;
                                        }
                                    }
///////////////////////////////////////////////////////////////
// Gain
                                    display.setSegments(MIDI_COMP[1]);
                                    delay(1000);
                                    display.showNumberDec(((currentMidiValue & 0b00110000) >> 4) * 10 );
                                    delay(DELAY_DISPLAY);
                                    while(1){
                                        if(digitalRead(BTN_DOWN) == ON && ((currentMidiValue & 0b00110000) >> 4) > 0x00 ){
                                            currentMidiValue -= 0b00010000;
                                            display.showNumberDec(((currentMidiValue & 0b00110000) >> 4) * 10 );
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_UP) == ON && (((currentMidiValue & 0b00110000) >> 4) < 0x03)){
                                            currentMidiValue += 0b00010000;
                                            display.showNumberDec(((currentMidiValue & 0b00110000) >> 4) * 10 );
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_MODE) == ON){
                                            // set the Gain value
                                            gain = (((currentMidiValue >> 4) & 0b00000011) * 10);
                                            break;
                                        }
                                    }
///////////////////////////////////////////////////////////////
// Ratio
                                    display.setSegments(MIDI_COMP[2]);
                                    delay(1000);
                                    if((currentMidiValue >> 6) == 0) display.setSegments(MIDI_COMP[3]);
                                    else display.showNumberDec((currentMidiValue >> 6) * 4);
                                    delay(DELAY_DISPLAY);
                                    while(1){
                                        if(digitalRead(BTN_DOWN) == ON && (currentMidiValue >> 6) > 0x00 ){
                                            currentMidiValue -= 0b01000000;
                                            if((currentMidiValue >> 6) == 0) display.setSegments(MIDI_COMP[3]);
                                            else display.showNumberDec((currentMidiValue >> 6) * 4);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_UP) == ON && (currentMidiValue >> 6) < 0x03){
                                            currentMidiValue += 0b01000000;
                                            if((currentMidiValue >> 6) == 0) display.setSegments(MIDI_COMP[3]);
                                            else display.showNumberDec((currentMidiValue >> 6) * 4);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_MODE) == ON){
                                            // set the Ratio value
                                            ratio = ((currentMidiValue >> 6) * 4);
                                            break;
                                        }
                                    }
                                    display.setSegments(MESSAGES[2]);
                                    }
                                    break;
///////////////////////////////////////////////////////////////
// MIDI : GATE Submenu
                                case 2:
                                    if(currentMidiMode != Gate){
                                        currentMidiValue = 0x1E;
                                    }
                                    currentMidiMode = Gate;
                                    display.showNumberDec(currentMidiValue);
                                    delay(DELAY_DISPLAY);
                                    while(1){
                                        if(digitalRead(BTN_DOWN) == ON && currentMidiValue > 0x05){
                                            currentMidiValue -= 0x06;
                                            display.showNumberDec(currentMidiValue);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_UP) == ON && currentMidiValue < 0x7E){
                                            currentMidiValue += 0x06;
                                            display.showNumberDec(currentMidiValue);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_MODE) == ON){
                                            display.setSegments(MESSAGES[2]);
                                            delay(DELAY_DISPLAY);
                                            switch(currentViewMode){
                                                case Velocity:
                                                    display.setSegments(VIEW_MENU[0]);
                                                    return;
                                                case Channel:
                                                    display.setSegments(VIEW_MENU[1]);
                                                    return;
                                                case Pitch:
                                                    display.setSegments(VIEW_MENU[2]);
                                                    return;
                                            }
                                        }
                                    }
                                    break;
///////////////////////////////////////////////////////////////
// MIDI : FIXED MIN. Submenu
                                case 3:
                                    if(currentMidiMode != Min){
                                        currentMidiValue = 0x1E;
                                    }
                                    currentMidiMode = Min;
                                    display.showNumberDec(currentMidiValue);
                                    delay(DELAY_DISPLAY);
                                    while(1){
                                        if(digitalRead(BTN_DOWN) == ON && currentMidiValue > 0x05){
                                            currentMidiValue -= 0x06;
                                            display.showNumberDec(currentMidiValue);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_UP) == ON && currentMidiValue < 0x7E){
                                            currentMidiValue += 0x06;
                                            display.showNumberDec(currentMidiValue);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_MODE) == ON){
                                            display.setSegments(MESSAGES[2]);
                                            delay(DELAY_DISPLAY);
                                            switch(currentViewMode){
                                                case Velocity:
                                                    display.setSegments(VIEW_MENU[0]);
                                                    return;
                                                case Channel:
                                                    display.setSegments(VIEW_MENU[1]);
                                                    return;
                                                case Pitch:
                                                    display.setSegments(VIEW_MENU[2]);
                                                    return;
                                            }
                                        }
                                    }
                                    break;
///////////////////////////////////////////////////////////////
// MIDI : FIXED CHANNEL Submenu
                                case 4:
                                    if(currentMidiMode != FixChannel){
                                        currentMidiValue = 0x0A;// Default value
                                    }
                                    currentMidiMode = FixChannel;
                                    
                                    display.showNumberDec(currentMidiValue);
                                    delay(DELAY_DISPLAY);
                                    while(1){
                                        if(digitalRead(BTN_DOWN) == ON && currentMidiValue > 0x01){
                                            currentMidiValue -= 0x01;
                                            display.showNumberDec(currentMidiValue);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_UP) == ON && currentMidiValue < 0x10){
                                            currentMidiValue += 0x01;
                                            display.showNumberDec(currentMidiValue);
                                            delay(DELAY_BUTTON);
                                        }
                                        if(digitalRead(BTN_MODE) == ON){
                                            display.setSegments(MESSAGES[2]);
                                            delay(DELAY_DISPLAY);
                                            switch(currentViewMode){
                                                case Velocity:
                                                    display.setSegments(VIEW_MENU[0]);
                                                    return;
                                                case Channel:
                                                    display.setSegments(VIEW_MENU[1]);
                                                    return;
                                                case Pitch:
                                                    display.setSegments(VIEW_MENU[2]);
                                                    return;
                                            }
                                        }
                                    }
                                    break;
///////////////////////////////////////////////////////////////
// MIDI : BYPASS Submenu
                                case 5:
                                    currentMidiMode = Bypass;
                                    display.setSegments(MESSAGES[2]);
                                    break;
///////////////////////////////////////////////////////////////
// MIDI : Back
                                case 6:
                                    switch(currentViewMode){
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
                                    delay(DELAY_BUTTON);
                                    return;
                            }
                            // After the switch:
                            delay(DELAY_DISPLAY);
                            switch(currentViewMode){
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
                            return;
                        }
                    }
                    break;
// END OF MIDI MENU
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// THRU
                case 1: // Thru
                    current = 0;
                    display.setSegments(THRU_MODE[0]);                    
                    delay(DELAY_DISPLAY);
                    while(1){
                        if(digitalRead(BTN_DOWN) == ON){
                            switch(current){
                                case 0: // if the current is On
                                    display.setSegments(THRU_MODE[1]);
                                    current = 1;
                                    break;
                                case 1:
                                    display.setSegments(THRU_MODE[2]);
                                    current = 2;
                                    break;
                                case 2:
                                    display.setSegments(THRU_MODE[0]);
                                    current = 0;
                                    break;
                            }
                            delay(DELAY_BUTTON);
                        }
                        if(digitalRead(BTN_UP) == ON){
                            switch(current){
                                case 0:
                                    display.setSegments(THRU_MODE[2]);
                                    current = 2;
                                    break;
                                case 1:
                                    display.setSegments(THRU_MODE[0]);
                                    current = 0;
                                    break;
                                case 2:
                                    display.setSegments(THRU_MODE[1]);
                                    current = 1;
                                    break;
                            }
                            delay(DELAY_BUTTON);
                        }
                        if(digitalRead(BTN_MODE) == ON){
                            switch (current){
                                case 0:// off
                                    currentThruMode = ThruOff;
                                    break;
                                case 1:// on
                                    currentThruMode = ThruOn;
                                    break;
                                case 2:// usb
                                    currentThruMode = ThruUSB;
                                    break;
                            }
                            // change the note handler
                            setNoteOnHandler(currentThruMode);

                            display.setSegments(MESSAGES[2]);
                            delay(DELAY_DISPLAY);
                            switch(currentViewMode){
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
                            return;
                         }
                    }
///////////////////////////////////////////////////////////////
// SAVE
                case 2: // Save
                    saveData(currentViewMode, currentMidiMode, currentMidiValue, currentThruMode);
                    display.setSegments(MESSAGES[2]);
                    delay(DELAY_DISPLAY);
                    switch(currentViewMode){
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
                    return;
///////////////////////////////////////////////////////////////
// BACK
                case 3: // Back
                    switch(currentViewMode){
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
                    delay(DELAY_BUTTON);
                    return;
            }
        }
    }   
}
