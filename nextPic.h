#include <avr/pgmspace.h>

const uint16_t nextPicWidth = 20;
const uint16_t nextPicHeight = 16;

const unsigned short nextPic[0x140] PROGMEM ={
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0010 (16)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xF800, 0x0000, 0xFA60, 0xFA60, 0x0000, 0x0000, 0x0000, 0xFF40, 0xFE60, 0xFF40,   // 0x0020 (32)
0x0000, 0x0000, 0x0000, 0xFA60, 0xFA60, 0x0000, 0xF800, 0x0000, 0x0000, 0xF800, 0x0000, 0xF800, 0x0000, 0x0000, 0xFB60, 0x0000,   // 0x0030 (48)
0xFE80, 0x0000, 0x0000, 0x0000, 0xFE80, 0x0000, 0xFB60, 0x0000, 0x0000, 0xF800, 0x0000, 0xF800, 0x0000, 0x0000, 0xF800, 0x0000,   // 0x0040 (64)
0x0000, 0x0000, 0x0000, 0xFD80, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFD80, 0x0000, 0x0000, 0x0000, 0x0000, 0xF800, 0x0000,   // 0x0050 (80)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0060 (96)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x07F7, 0x0000, 0x0000, 0x07FA, 0x0000, 0x07FE, 0x07BF, 0x075F, 0x0000, 0x06BF,   // 0x0070 (112)
0x0000, 0x0000, 0x061F, 0x0000, 0x055F, 0x04FF, 0x03DF, 0x0000, 0x0000, 0x0000, 0x07F7, 0x07F7, 0x0000, 0x07FA, 0x0000, 0x07FE,   // 0x0080 (128)
0x0000, 0x0000, 0x0000, 0x0000, 0x06BF, 0x061F, 0x0000, 0x0000, 0x0000, 0x04FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x07F7, 0x0000,   // 0x0090 (144)
0x07FA, 0x07FA, 0x0000, 0x07FE, 0x07BF, 0x0000, 0x0000, 0x06BF, 0x0000, 0x0000, 0x061F, 0x0000, 0x0000, 0x04FF, 0x0000, 0x0000,   // 0x00A0 (160)
0x0000, 0x0000, 0x07F7, 0x0000, 0x0000, 0x07FA, 0x0000, 0x07FE, 0x0000, 0x0000, 0x0000, 0x06BF, 0x0000, 0x0000, 0x061F, 0x0000,   // 0x00B0 (176)
0x0000, 0x04FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x07F7, 0x0000, 0x0000, 0x07FA, 0x0000, 0x07FE, 0x07BF, 0x075F, 0x0000, 0x06BF,   // 0x00C0 (192)
0x0000, 0x0000, 0x061F, 0x0000, 0x0000, 0x04FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x00D0 (208)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x77E0, 0x0000, 0x0000,   // 0x00E0 (224)
0xA7E0, 0xA7E0, 0xB7E0, 0xB7E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xB7E0, 0xB7E0, 0xA7E0, 0xA7E0, 0x0000, 0x0000, 0x77E0,   // 0x00F0 (240)
0x0000, 0x77E0, 0x0000, 0x8FE0, 0x0000, 0x0000, 0x0000, 0xCFE0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xCFE0, 0x0000, 0x0000,   // 0x0100 (256)
0x0000, 0x8FE0, 0x0000, 0x77E0, 0x0000, 0x77E0, 0x8FE0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xE7E0, 0x0000, 0x0000, 0x0000,   // 0x0110 (272)
0xE7E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8FE0, 0x77E0, 0x0000, 0x77E0, 0x0000, 0xFA40, 0xFB80, 0xFCA0, 0xFDA0, 0xFF60,   // 0x0120 (288)
0x0000, 0xF7E0, 0xE7E0, 0xF7E0, 0x0000, 0xFF60, 0xFDA0, 0xFCA0, 0xFB80, 0xFA40, 0x0000, 0x77E0, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0130 (304)
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // 0x0140 (320)
};