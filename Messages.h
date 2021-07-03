enum Option { Velocity, Channel, Pitch, Fix, Compression, Gate, Min, Bypass, 
              FixChannel, ThruOff, ThruOn, ThruUSB };

static const uint8_t MESSAGES[5][4] = { 
    { 0x00, 0x79, 0x33, 0x33 }, //  ERR
    { 0x37, 0x79, 0x54, 0x3E }, // MENU
    { 0x00, 0x00, 0x3F, 0x75 }, //   OK
    { 0x5E, 0x7C, 0x3E, 0x7D }, // DBUG
    { 0x00, 0x00, 0x00, 0x00 }  // clear
};

static const uint8_t STARTUP[] = { 0x00, 0x00, 0x00, 0x00, 0x40, 0x37, 0x06, 0x5E, 
                                   0x06, 0x00, 0x71, 0x30, 0x76, 0x40, 0x00, 0x7C, 
                                   0x6E, 0x00, 0x79, 0x7C, 0x33, 0x77, 0x6E, 0x00, 
                                   0x6D, 0x3F, 0x38, 0x3E, 0x39, 0x06, 0x3F, 0x37, 
                                   0x79, 0x6D, 0x00, 0x00, 0x00, 0x00 };

static const uint8_t MAIN_MENU_ANIMATION[] = { 0x37, 0x79, 0x54, 0x3E, 
                                        0x00, 0x40, 0x00, 0x37, 0x06, 0x5E, 0x06 };

static const uint8_t MAIN_MENU[4][4] = { { 0x37, 0x06, 0x5E, 0x06 },   // Midi
                                         { 0x78, 0x76, 0x33, 0x3E },   // Thru
                                         { 0x6D, 0x77, 0x3E, 0x79 },   // Save
                                         { 0x7C, 0x77, 0x39, 0x75 } }; // Back
                                         //0x06, 0x37, 0x71, 0x3F //Info

static const uint8_t VIEW_MENU[3][4] = { { 0x3E, 0x79, 0x38, 0x00 },   // Vel
                                         { 0x39, 0x76, 0x77, 0x37 },   // Chan
                                         { 0x37, 0x3F, 0x78, 0x79 } }; // Note                                         

static const uint8_t MIDI_MENU[7][4] = { { 0x71, 0x06, 0x76, 0x00 },   // Fix   0
                                         { 0x39, 0x3F, 0x37, 0x73 },   // Comp  1
                                         { 0x7D, 0x77, 0x78, 0x79 },   // Gate  2
                                         { 0x37, 0x06, 0x37, 0x00 },   // Min   3
                                         { 0x39, 0x76, 0x77, 0x37 },   // Chan  4
                                         { 0x7C, 0x6E, 0x73, 0x6D },   // Byps  5
                                         { 0x7C, 0x77, 0x39, 0x75 } }; // Back  6
                                         
static const uint8_t MIDI_COMP[4][4] = { { 0x78, 0x76, 0x38, 0x5E },   // Thld
                                         { 0x7D, 0x77, 0x06, 0x37 },   // Gain
                                         { 0x33, 0x77, 0x78, 0x00 },   // Rat
                                         { 0x00, 0x06, 0x37, 0x71 } }; //  Inf

static const uint8_t THRU_MODE[3][4] = { { 0x00, 0x3F, 0x71, 0x71 },   //  off
                                         { 0x00, 0x00, 0x3F, 0x37 },   //   on
                                         { 0x00, 0x3E, 0x6D, 0x7C } }; //  USB

/***********************************************************************

    CHARACTERS
    From the TM1637Display.h library, this are the segments values:

    SEG_A   0b 0000 0001
    SEG_B   0b 0000 0010
    SEG_C   0b 0000 0100
    SEG_D   0b 0000 1000
    SEG_E   0b 0001 0000
    SEG_F   0b 0010 0000
    SEG_G   0b 0100 0000
    
            A
           ---
        F |   | B
           -G-
        E |   | C
           ---
            D

    This are the values in hexadecimal for each letter:
   
    Characters: 
    0x77  A
    0x7C  B alternative 0x7F
    0x39  C alternative 0x58
    0x5E  D
    0x79  E
    0x71  F
    0x7D  G
    0x76  H alternative 0x74,
    0x30  I alternative 0x06
    0x1E  J
    0x75  K
    0x38  L
    0x37  M avoid
    0x37  N alternative 0x54
    0x3F  O alternative 0x5C
    0x73  P
    0x67  Q
    0x33  R
    0x6D  S
    0x78  T
    0x3E  U
    0x3E  V alternative 0x1C
    0x2A  W avoid
    0x76  X avoid
    0x6E  Y
    0x5B  Z avoid
    
    0x80  Clock Dots
    
 ***********************************************************************/

 void setNoteOnHandler(Option _thruMode);
