#include <Arduino.h>
const int PS2DataPin = D3;
const int PS2IRQpin  = D2;
#define FREQUENCY    160   
#include "ESP8266WiFi.h"
#include <PS2Keyboard.h>

extern "C" {
#include "user_interface.h"
void exec6502(int32_t tickcount);
void reset6502();
}




//CBM Textset1 ROM
unsigned char charROM [] = { 0x1C , 0x22 , 0x4A , 0x56 , 0x4C , 0x20 , 0x1E , 0x0 , 0x18 , 0x24 , 0x42 , 0x7E , 0x42 , 0x42 , 0x42 , 0x0 , 0x7C , 0x22 , 0x22 , 0x3C , 0x22 , 0x22 , 0x7C , 0x0 , 0x1C , 0x22 , 0x40 , 0x40 , 0x40 , 0x22 , 0x1C , 0x0 , 0x78 , 0x24 , 0x22 , 0x22 , 0x22 , 0x24 , 0x78 , 0x0 , 0x7E , 0x40 , 0x40 , 0x78 , 0x40 , 0x40 , 0x7E , 0x0 , 0x7E , 0x40 , 0x40 , 0x78 , 0x40 , 0x40 , 0x40 , 0x0 , 0x1C , 0x22 , 0x40 , 0x4E , 0x42 , 0x22 , 0x1C , 0x0 , 0x42 , 0x42 , 0x42 , 0x7E , 0x42 , 0x42 , 0x42 , 0x0 , 0x1C , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x1C , 0x0 , 0xE , 0x4 , 0x4 , 0x4 , 0x4 , 0x44 , 0x38 , 0x0 , 0x42 , 0x44 , 0x48 , 0x70 , 0x48 , 0x44 , 0x42 , 0x0 , 0x40 , 0x40 , 0x40 , 0x40 , 0x40 , 0x40 , 0x7E , 0x0 , 0x42 , 0x66 , 0x5A , 0x5A , 0x42 , 0x42 , 0x42 , 0x0 , 0x42 , 0x62 , 0x52 , 0x4A , 0x46 , 0x42 , 0x42 , 0x0 , 0x18 , 0x24 , 0x42 , 0x42 , 0x42 , 0x24 , 0x18 , 0x0 , 0x7C , 0x42 , 0x42 , 0x7C , 0x40 , 0x40 , 0x40 , 0x0 , 0x18 , 0x24 , 0x42 , 0x42 , 0x4A , 0x24 , 0x1A , 0x0 , 0x7C , 0x42 , 0x42 , 0x7C , 0x48 , 0x44 , 0x42 , 0x0 , 0x3C , 0x42 , 0x40 , 0x3C , 0x2 , 0x42 , 0x3C , 0x0 , 0x3E , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x0 , 0x42 , 0x42 , 0x42 , 0x42 , 0x42 , 0x42 , 0x3C , 0x0 , 0x42 , 0x42 , 0x42 , 0x24 , 0x24 , 0x18 , 0x18 , 0x0 , 0x42 , 0x42 , 0x42 , 0x5A , 0x5A , 0x66 , 0x42 , 0x0 , 0x42 , 0x42 , 0x24 , 0x18 , 0x24 , 0x42 , 0x42 , 0x0 , 0x22 , 0x22 , 0x22 , 0x1C , 0x8 , 0x8 , 0x8 , 0x0 , 0x7E , 0x2 , 0x4 , 0x18 , 0x20 , 0x40 , 0x7E , 0x0 , 0x42 , 0x18 , 0x24 , 0x42 , 0x7E , 0x42 , 0x42 , 0x0 , 0x42 , 0x18 , 0x24 , 0x42 , 0x42 , 0x24 , 0x18 , 0x0 , 0x18 , 0x24 , 0x3C , 0x42 , 0x7E , 0x42 , 0x42 , 0x0 , 0x0 , 0x8 , 0x1C , 0x2A , 0x8 , 0x8 , 0x8 , 0x8 , 0x0 , 0x0 , 0x10 , 0x20 , 0x7F , 0x20 , 0x10 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x8 , 0x8 , 0x8 , 0x8 , 0x0 , 0x0 , 0x8 , 0x0 , 0x24 , 0x24 , 0x24 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x24 , 0x24 , 0x7E , 0x24 , 0x7E , 0x24 , 0x24 , 0x0 , 0x8 , 0x1E , 0x28 , 0x1C , 0xA , 0x3C , 0x8 , 0x0 , 0x0 , 0x62 , 0x64 , 0x8 , 0x10 , 0x26 , 0x46 , 0x0 , 0x30 , 0x48 , 0x48 , 0x30 , 0x4A , 0x44 , 0x3A , 0x0 , 0x4 , 0x8 , 0x10 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x4 , 0x8 , 0x10 , 0x10 , 0x10 , 0x8 , 0x4 , 0x0 , 0x20 , 0x10 , 0x8 , 0x8 , 0x8 , 0x10 , 0x20 , 0x0 , 0x8 , 0x2A , 0x1C , 0x3E , 0x1C , 0x2A , 0x8 , 0x0 , 0x0 , 0x8 , 0x8 , 0x3E , 0x8 , 0x8 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x8 , 0x8 , 0x10 , 0x0 , 0x0 , 0x0 , 0x7E , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x18 , 0x18 , 0x0 , 0x0 , 0x2 , 0x4 , 0x8 , 0x10 , 0x20 , 0x40 , 0x0 , 0x3C , 0x42 , 0x46 , 0x5A , 0x62 , 0x42 , 0x3C , 0x0 , 0x8 , 0x18 , 0x28 , 0x8 , 0x8 , 0x8 , 0x3E , 0x0 , 0x3C , 0x42 , 0x2 , 0xC , 0x30 , 0x40 , 0x7E , 0x0 , 0x3C , 0x42 , 0x2 , 0x1C , 0x2 , 0x42 , 0x3C , 0x0 , 0x4 , 0xC , 0x14 , 0x24 , 0x7E , 0x4 , 0x4 , 0x0 , 0x7E , 0x40 , 0x78 , 0x4 , 0x2 , 0x44 , 0x38 , 0x0 , 0x1C , 0x20 , 0x40 , 0x7C , 0x42 , 0x42 , 0x3C , 0x0 , 0x7E , 0x42 , 0x4 , 0x8 , 0x10 , 0x10 , 0x10 , 0x0 , 0x3C , 0x42 , 0x42 , 0x3C , 0x42 , 0x42 , 0x3C , 0x0 , 0x3C , 0x42 , 0x42 , 0x3E , 0x2 , 0x4 , 0x38 , 0x0 , 0x0 , 0x0 , 0x8 , 0x0 , 0x0 , 0x8 , 0x0 , 0x0 , 0x0 , 0x0 , 0x8 , 0x0 , 0x0 , 0x8 , 0x8 , 0x10 , 0xE , 0x18 , 0x30 , 0x60 , 0x30 , 0x18 , 0xE , 0x0 , 0x0 , 0x0 , 0x7E , 0x0 , 0x7E , 0x0 , 0x0 , 0x0 , 0x70 , 0x18 , 0xC , 0x6 , 0xC , 0x18 , 0x70 , 0x0 , 0x3C , 0x42 , 0x2 , 0xC , 0x10 , 0x0 , 0x10 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0x0 , 0x0 , 0x0 , 0x8 , 0x1C , 0x3E , 0x7F , 0x7F , 0x1C , 0x3E , 0x0 , 0x10 , 0x10 , 0x10 , 0x10 , 0x10 , 0x10 , 0x10 , 0x10 , 0x0 , 0x0 , 0x0 , 0xFF , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0x0 , 0x0 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x4 , 0x4 , 0x4 , 0x4 , 0x4 , 0x4 , 0x4 , 0x4 , 0x0 , 0x0 , 0x0 , 0x0 , 0xE0 , 0x10 , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x4 , 0x3 , 0x0 , 0x0 , 0x0 , 0x8 , 0x8 , 0x8 , 0x10 , 0xE0 , 0x0 , 0x0 , 0x0 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0xFF , 0x80 , 0x40 , 0x20 , 0x10 , 0x8 , 0x4 , 0x2 , 0x1 , 0x1 , 0x2 , 0x4 , 0x8 , 0x10 , 0x20 , 0x40 , 0x80 , 0xFF , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0xFF , 0x1 , 0x1 , 0x1 , 0x1 , 0x1 , 0x1 , 0x1 , 0x0 , 0x3C , 0x7E , 0x7E , 0x7E , 0x7E , 0x3C , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0x0 , 0x36 , 0x7F , 0x7F , 0x7F , 0x3E , 0x1C , 0x8 , 0x0 , 0x40 , 0x40 , 0x40 , 0x40 , 0x40 , 0x40 , 0x40 , 0x40 , 0x0 , 0x0 , 0x0 , 0x0 , 0x3 , 0x4 , 0x8 , 0x8 , 0x81 , 0x42 , 0x24 , 0x18 , 0x18 , 0x24 , 0x42 , 0x81 , 0x0 , 0x3C , 0x42 , 0x42 , 0x42 , 0x42 , 0x3C , 0x0 , 0x8 , 0x1C , 0x2A , 0x77 , 0x2A , 0x8 , 0x8 , 0x0 , 0x2 , 0x2 , 0x2 , 0x2 , 0x2 , 0x2 , 0x2 , 0x2 , 0x8 , 0x1C , 0x3E , 0x7F , 0x3E , 0x1C , 0x8 , 0x0 , 0x8 , 0x8 , 0x8 , 0x8 , 0xFF , 0x8 , 0x8 , 0x8 , 0xA0 , 0x50 , 0xA0 , 0x50 , 0xA0 , 0x50 , 0xA0 , 0x50 , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x0 , 0x0 , 0x1 , 0x3E , 0x54 , 0x14 , 0x14 , 0x0 , 0xFF , 0x7F , 0x3F , 0x1F , 0xF , 0x7 , 0x3 , 0x1 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0x80 , 0xAA , 0x55 , 0xAA , 0x55 , 0xAA , 0x55 , 0xAA , 0x55 , 0x1 , 0x1 , 0x1 , 0x1 , 0x1 , 0x1 , 0x1 , 0x1 , 0x0 , 0x0 , 0x0 , 0x0 , 0xAA , 0x55 , 0xAA , 0x55 , 0xFF , 0xFE , 0xFC , 0xF8 , 0xF0 , 0xE0 , 0xC0 , 0x80 , 0x3 , 0x3 , 0x3 , 0x3 , 0x3 , 0x3 , 0x3 , 0x3 , 0x8 , 0x8 , 0x8 , 0x8 , 0xF , 0x8 , 0x8 , 0x8 , 0x0 , 0x0 , 0x0 , 0x0 , 0xF , 0xF , 0xF , 0xF , 0x8 , 0x8 , 0x8 , 0x8 , 0xF , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xF8 , 0x8 , 0x8 , 0x8 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0xFF , 0x0 , 0x0 , 0x0 , 0x0 , 0xF , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0xFF , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0x8 , 0xF8 , 0x8 , 0x8 , 0x8 , 0xC0 , 0xC0 , 0xC0 , 0xC0 , 0xC0 , 0xC0 , 0xC0 , 0xC0 , 0xE0 , 0xE0 , 0xE0 , 0xE0 , 0xE0 , 0xE0 , 0xE0 , 0xE0 , 0x7 , 0x7 , 0x7 , 0x7 , 0x7 , 0x7 , 0x7 , 0x7 , 0xFF , 0xFF , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0xFF , 0xFF , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0xFF , 0xFF , 0x1 , 0x1 , 0x1 , 0x1 , 0x1 , 0x1 , 0x1 , 0xFF , 0x0 , 0x0 , 0x0 , 0x0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF , 0xF , 0xF , 0xF , 0x0 , 0x0 , 0x0 , 0x0 , 0x8 , 0x8 , 0x8 , 0x8 , 0xF8 , 0x0 , 0x0 , 0x0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF , 0xF , 0xF , 0xF , 0xE3 , 0xDD , 0xB5 , 0xA9 , 0xB3 , 0xDF , 0xE1 , 0xFF , 0xE7 , 0xDB , 0xBD , 0x81 , 0xBD , 0xBD , 0xBD , 0xFF , 0x83 , 0xDD , 0xDD , 0xC3 , 0xDD , 0xDD , 0x83 , 0xFF , 0xE3 , 0xDD , 0xBF , 0xBF , 0xBF , 0xDD , 0xE3 , 0xFF , 0x87 , 0xDB , 0xDD , 0xDD , 0xDD , 0xDB , 0x87 , 0xFF , 0x81 , 0xBF , 0xBF , 0x87 , 0xBF , 0xBF , 0x81 , 0xFF , 0x81 , 0xBF , 0xBF , 0x87 , 0xBF , 0xBF , 0xBF , 0xFF , 0xE3 , 0xDD , 0xBF , 0xB1 , 0xBD , 0xDD , 0xE3 , 0xFF , 0xBD , 0xBD , 0xBD , 0x81 , 0xBD , 0xBD , 0xBD , 0xFF , 0xE3 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xE3 , 0xFF , 0xF1 , 0xFB , 0xFB , 0xFB , 0xFB , 0xBB , 0xC7 , 0xFF , 0xBD , 0xBB , 0xB7 , 0x8F , 0xB7 , 0xBB , 0xBD , 0xFF , 0xBF , 0xBF , 0xBF , 0xBF , 0xBF , 0xBF , 0x81 , 0xFF , 0xBD , 0x99 , 0xA5 , 0xA5 , 0xBD , 0xBD , 0xBD , 0xFF , 0xBD , 0x9D , 0xAD , 0xB5 , 0xB9 , 0xBD , 0xBD , 0xFF , 0xE7 , 0xDB , 0xBD , 0xBD , 0xBD , 0xDB , 0xE7 , 0xFF , 0x83 , 0xBD , 0xBD , 0x83 , 0xBF , 0xBF , 0xBF , 0xFF , 0xE7 , 0xDB , 0xBD , 0xBD , 0xB5 , 0xDB , 0xE5 , 0xFF , 0x83 , 0xBD , 0xBD , 0x83 , 0xB7 , 0xBB , 0xBD , 0xFF , 0xC3 , 0xBD , 0xBF , 0xC3 , 0xFD , 0xBD , 0xC3 , 0xFF , 0xC1 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xFF , 0xBD , 0xBD , 0xBD , 0xBD , 0xBD , 0xBD , 0xC3 , 0xFF , 0xBD , 0xBD , 0xBD , 0xDB , 0xDB , 0xE7 , 0xE7 , 0xFF , 0xBD , 0xBD , 0xBD , 0xA5 , 0xA5 , 0x99 , 0xBD , 0xFF , 0xBD , 0xBD , 0xDB , 0xE7 , 0xDB , 0xBD , 0xBD , 0xFF , 0xDD , 0xDD , 0xDD , 0xE3 , 0xF7 , 0xF7 , 0xF7 , 0xFF , 0x81 , 0xFD , 0xFB , 0xE7 , 0xDF , 0xBF , 0x81 , 0xFF , 0xBD , 0xE7 , 0xDB , 0xBD , 0x81 , 0xBD , 0xBD , 0xFF , 0xBD , 0xE7 , 0xDB , 0xBD , 0xBD , 0xDB , 0xE7 , 0xFF , 0xE7 , 0xDB , 0xC3 , 0xBD , 0x81 , 0xBD , 0xBD , 0xFF , 0xFF , 0xF7 , 0xE3 , 0xD5 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xFF , 0xFF , 0xEF , 0xDF , 0x80 , 0xDF , 0xEF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xFF , 0xFF , 0xF7 , 0xFF , 0xDB , 0xDB , 0xDB , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xDB , 0xDB , 0x81 , 0xDB , 0x81 , 0xDB , 0xDB , 0xFF , 0xF7 , 0xE1 , 0xD7 , 0xE3 , 0xF5 , 0xC3 , 0xF7 , 0xFF , 0xFF , 0x9D , 0x9B , 0xF7 , 0xEF , 0xD9 , 0xB9 , 0xFF , 0xCF , 0xB7 , 0xB7 , 0xCF , 0xB5 , 0xBB , 0xC5 , 0xFF , 0xFB , 0xF7 , 0xEF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFB , 0xF7 , 0xEF , 0xEF , 0xEF , 0xF7 , 0xFB , 0xFF , 0xDF , 0xEF , 0xF7 , 0xF7 , 0xF7 , 0xEF , 0xDF , 0xFF , 0xF7 , 0xD5 , 0xE3 , 0xC1 , 0xE3 , 0xD5 , 0xF7 , 0xFF , 0xFF , 0xF7 , 0xF7 , 0xC1 , 0xF7 , 0xF7 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xF7 , 0xF7 , 0xEF , 0xFF , 0xFF , 0xFF , 0x81 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xE7 , 0xE7 , 0xFF , 0xFF , 0xFD , 0xFB , 0xF7 , 0xEF , 0xDF , 0xBF , 0xFF , 0xC3 , 0xBD , 0xB9 , 0xA5 , 0x9D , 0xBD , 0xC3 , 0xFF , 0xF7 , 0xE7 , 0xD7 , 0xF7 , 0xF7 , 0xF7 , 0xC1 , 0xFF , 0xC3 , 0xBD , 0xFD , 0xF3 , 0xCF , 0xBF , 0x81 , 0xFF , 0xC3 , 0xBD , 0xFD , 0xE3 , 0xFD , 0xBD , 0xC3 , 0xFF , 0xFB , 0xF3 , 0xEB , 0xDB , 0x81 , 0xFB , 0xFB , 0xFF , 0x81 , 0xBF , 0x87 , 0xFB , 0xFD , 0xBB , 0xC7 , 0xFF , 0xE3 , 0xDF , 0xBF , 0x83 , 0xBD , 0xBD , 0xC3 , 0xFF , 0x81 , 0xBD , 0xFB , 0xF7 , 0xEF , 0xEF , 0xEF , 0xFF , 0xC3 , 0xBD , 0xBD , 0xC3 , 0xBD , 0xBD , 0xC3 , 0xFF , 0xC3 , 0xBD , 0xBD , 0xC1 , 0xFD , 0xFB , 0xC7 , 0xFF , 0xFF , 0xFF , 0xF7 , 0xFF , 0xFF , 0xF7 , 0xFF , 0xFF , 0xFF , 0xFF , 0xF7 , 0xFF , 0xFF , 0xF7 , 0xF7 , 0xEF , 0xF1 , 0xE7 , 0xCF , 0x9F , 0xCF , 0xE7 , 0xF1 , 0xFF , 0xFF , 0xFF , 0x81 , 0xFF , 0x81 , 0xFF , 0xFF , 0xFF , 0x8F , 0xE7 , 0xF3 , 0xF9 , 0xF3 , 0xE7 , 0x8F , 0xFF , 0xC3 , 0xBD , 0xFD , 0xF3 , 0xEF , 0xFF , 0xEF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0xFF , 0xFF , 0xFF , 0xF7 , 0xE3 , 0xC1 , 0x80 , 0x80 , 0xE3 , 0xC1 , 0xFF , 0xEF , 0xEF , 0xEF , 0xEF , 0xEF , 0xEF , 0xEF , 0xEF , 0xFF , 0xFF , 0xFF , 0x0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0xFF , 0xFF , 0xDF , 0xDF , 0xDF , 0xDF , 0xDF , 0xDF , 0xDF , 0xDF , 0xFB , 0xFB , 0xFB , 0xFB , 0xFB , 0xFB , 0xFB , 0xFB , 0xFF , 0xFF , 0xFF , 0xFF , 0x1F , 0xEF , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xFB , 0xFC , 0xFF , 0xFF , 0xFF , 0xF7 , 0xF7 , 0xF7 , 0xEF , 0x1F , 0xFF , 0xFF , 0xFF , 0x7F , 0x7F , 0x7F , 0x7F , 0x7F , 0x7F , 0x7F , 0x0 , 0x7F , 0xBF , 0xDF , 0xEF , 0xF7 , 0xFB , 0xFD , 0xFE , 0xFE , 0xFD , 0xFB , 0xF7 , 0xEF , 0xDF , 0xBF , 0x7F , 0x0 , 0x7F , 0x7F , 0x7F , 0x7F , 0x7F , 0x7F , 0x7F , 0x0 , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFF , 0xC3 , 0x81 , 0x81 , 0x81 , 0x81 , 0xC3 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0xFF , 0xC9 , 0x80 , 0x80 , 0x80 , 0xC1 , 0xE3 , 0xF7 , 0xFF , 0xBF , 0xBF , 0xBF , 0xBF , 0xBF , 0xBF , 0xBF , 0xBF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFC , 0xFB , 0xF7 , 0xF7 , 0x7E , 0xBD , 0xDB , 0xE7 , 0xE7 , 0xDB , 0xBD , 0x7E , 0xFF , 0xC3 , 0xBD , 0xBD , 0xBD , 0xBD , 0xC3 , 0xFF , 0xF7 , 0xE3 , 0xD5 , 0x88 , 0xD5 , 0xF7 , 0xF7 , 0xFF , 0xFD , 0xFD , 0xFD , 0xFD , 0xFD , 0xFD , 0xFD , 0xFD , 0xF7 , 0xE3 , 0xC1 , 0x80 , 0xC1 , 0xE3 , 0xF7 , 0xFF , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0x0 , 0xF7 , 0xF7 , 0xF7 , 0x5F , 0xAF , 0x5F , 0xAF , 0x5F , 0xAF , 0x5F , 0xAF , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xFF , 0xFF , 0xFE , 0xC1 , 0xAB , 0xEB , 0xEB , 0xFF , 0x0 , 0x80 , 0xC0 , 0xE0 , 0xF0 , 0xF8 , 0xFC , 0xFE , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xF , 0xF , 0xF , 0xF , 0xF , 0xF , 0xF , 0xF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0x0 , 0x0 , 0x0 , 0x0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0x7F , 0x7F , 0x7F , 0x7F , 0x7F , 0x7F , 0x7F , 0x7F , 0x55 , 0xAA , 0x55 , 0xAA , 0x55 , 0xAA , 0x55 , 0xAA , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFF , 0xFF , 0xFF , 0xFF , 0x55 , 0xAA , 0x55 , 0xAA , 0x0 , 0x1 , 0x3 , 0x7 , 0xF , 0x1F , 0x3F , 0x7F , 0xFC , 0xFC , 0xFC , 0xFC , 0xFC , 0xFC , 0xFC , 0xFC , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF0 , 0xF7 , 0xF7 , 0xF7 , 0xFF , 0xFF , 0xFF , 0xFF , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x7 , 0xF7 , 0xF7 , 0xF7 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0x0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xF0 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0x0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0x7 , 0xF7 , 0xF7 , 0xF7 , 0x3F , 0x3F , 0x3F , 0x3F , 0x3F , 0x3F , 0x3F , 0x3F , 0x1F , 0x1F , 0x1F , 0x1F , 0x1F , 0x1F , 0x1F , 0x1F , 0xF8 , 0xF8 , 0xF8 , 0xF8 , 0xF8 , 0xF8 , 0xF8 , 0xF8 , 0x0 , 0x0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0x0 , 0x0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0xFF , 0x0 , 0x0 , 0x0 , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0xFE , 0x0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xF , 0xF , 0xF , 0xF , 0xF0 , 0xF0 , 0xF0 , 0xF0 , 0xFF , 0xFF , 0xFF , 0xFF , 0xF7 , 0xF7 , 0xF7 , 0xF7 , 0x7 , 0xFF , 0xFF , 0xFF , 0xF , 0xF , 0xF , 0xF , 0xFF , 0xFF , 0xFF , 0xFF , 0xF , 0xF , 0xF , 0xF , 0xF0 , 0xF0 , 0xF0 , 0xF0 };

uint8_t screenmem[1000];
uint8_t colormem[1000];
uint8_t RAM[16384];
uint8_t BOARDER;
uint8_t BGCOLOR;
uint8_t VIC_D020;
uint8_t VIC_D021;


extern "C" {
  void videoinit();
  }

PS2Keyboard keyboard;

void setup() {
  keyboard.begin(PS2DataPin, PS2IRQpin, PS2Keymap_German);
  WiFi.forceSleepBegin();             
  delay(1);                               
  system_update_cpu_freq(FREQUENCY);
  videoinit();
  reset6502(); 
  }

void loop() {  
  exec6502(100);
  if (keyboard.available()) {
    char c = keyboard.read();
    RAM[198]=1;
    RAM[631]=c;
  }
}
