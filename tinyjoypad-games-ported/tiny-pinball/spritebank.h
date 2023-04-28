//          >>>>>  T-I-N-Y  P-I-N-B-A-L-L for ATTINY85  GPL v3 <<<<
//                    Programmer: Daniel C 2018-2021
//             Contact EMAIL: electro_l.i.b@tinyjoypad.com
//                      https://www.tinyjoypad.com
//           https://sites.google.com/view/arduino-collection

//  Tiny PINBALL is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//for TINY JOYPAD rev2 (attiny85)
//the code work at 16MHZ internal
//Program the chip with an arduino uno in "Arduino as ISP" mode.

#include <avr/pgmspace.h>

const uint8_t ScreenBallA  [] PROGMEM =  {
0xFF, 0x03, 0x01, 0x01, 0x03, 0xFF, 0xFF, 0x03, 0x19, 0x19, 0x03, 0xFF, 0xFF, 0x03, 0xD9, 0xD9,
0x03, 0xFF, 0xFF, 0x03, 0xD9, 0xD9, 0x03, 0xFF, 0xFF, 0x03, 0xD9, 0xD9, 0x03, 0xFF, 
};

const uint8_t ScreenBallB  [] PROGMEM =  {
0xFF, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF, 0x80, 0x80, 0x80,
0x80, 0xFF, 0xFF, 0x80, 0x86, 0x86, 0x80, 0xFF, 0xFF, 0x80, 0xB6, 0xB6, 0x80, 0xFF, 
};

const uint8_t PusherA [] PROGMEM =  {
0xFF, 0x01, 0x01, 0x01, 0x01, 0xFF, 0xFF, 0x05, 0x03, 0x05, 0x03, 0xFF, 0xFF, 0x15, 0x0B, 0x15,
0x0B, 0xFF, 0xFF, 0x55, 0x2B, 0x55, 0x2B, 0xFF, 0xFF, 0x55, 0xAB, 0x55, 0xAB, 0xFF, 0xFF, 0x55,
0xAB, 0x55, 0xAB, 0xFF, 0xFF, 0x55, 0xAB, 0x55, 0xAB, 0xFF, 0xFF, 0x55, 0xAB, 0x55, 0xAB, 0xFF
};

const uint8_t PusherB [] PROGMEM =  {
0xFF, 0xC0, 0x80, 0x80, 0xC0, 0xFF, 0xFF, 0xC0, 0x80, 0x80, 0xC0, 0xFF, 0xFF, 0xC0, 0x80, 0x80,
0xC0, 0xFF, 0xFF, 0xC0, 0x80, 0x80, 0xC0, 0xFF, 0xFF, 0xC1, 0x80, 0x81, 0xC0, 0xFF, 0xFF, 0xC5,
0x82, 0x85, 0xC2, 0xFF, 0xFF, 0xD5, 0x8A, 0x95, 0xCA, 0xFF, 0xFF, 0xD5, 0xAA, 0xD5, 0xEA, 0xFF
};

const uint8_t FLIPDETGAUCHE  [] PROGMEM =  {
0x30, 0x38, 0x7C, 0x7F, 0x73, 0x60, 
};
const uint8_t FLIPDETDROIT  [] PROGMEM =  {
0x0C, 0x1C, 0x3E, 0xFE, 0xCE, 0x06, 
};
const uint8_t FLIPGAUCHE  [] PROGMEM =  {
0x30, 0x18, 0x0C, 0x07, 0x03, 0x00, 0x00, 0x00, 0x78, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x08, 0x1F,
0x33, 0x60, 
};
const uint8_t FLIPDROITE  [] PROGMEM =  {
0x0C, 0x18, 0x30, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x1E, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x10, 0xF8,
0xCC, 0x06, 
};

const uint8_t READY [] PROGMEM =  {
  10,2,
0xFF, 0x01, 0x7D, 0x45, 0x75, 0x05, 0x45, 0x7D, 0x01, 0xFF, 0xFF, 0x80, 0xAF, 0x89, 0xA9, 0xA9,
0xA9, 0xAF, 0x80, 0xFF, };

const uint8_t  start [] PROGMEM= {
0xAB, 0xAB, 0x2B, 0x2B, 0x0B, 0x0B, 0x03, 0x03, 0x83, 0xC3, 0xE3, 0x73, 0x33, 0x3B, 0x3B, 0x3B,
0x3B, 0x13, 0x03, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x0F, 0x07,
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
0x07, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0x03, 0xD9, 0xD9, 0x03, 0xFF,
0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x04, 0x06, 0x03,
0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x1E, 0x1E, 0x0C, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0xB6, 0xB6, 0x80, 0xFF,
0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x20, 0x60, 0xC0,
0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x78, 0x78, 0x30, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0xAB, 0xAB, 0x01, 0xFF,
0xD5, 0xD5, 0xD4, 0xD4, 0xD0, 0xD0, 0xC0, 0xC0, 0xC1, 0xC3, 0xC7, 0xCE, 0xCC, 0xDC, 0xDC, 0xDC,
0xDC, 0xC8, 0xC0, 0xC0, 0xE0, 0xF0, 0xB8, 0xBC, 0xBC, 0xBC, 0xB8, 0xB8, 0xB0, 0xB0, 0xB0, 0xB0,
0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0,
0x80, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0xF0, 0xFC, 0xFF, 0xFF, 0xFF, 0xC0, 0xAA, 0xAA, 0xC0, 0xFF
,/*boucing pushing*/
0xAB, 0xAB, 0x2B, 0x2B, 0x0B, 0x0B, 0x03, 0x03, 0x83, 0xC3, 0xE3, 0x73, 0x33, 0x3B, 0x3B, 0x3B,
0x3B, 0x13, 0x03, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x0F, 0x07,
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
0x07, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0x03, 0xD9, 0xD9, 0x03, 0xFF,
0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x04, 0x06, 0x03,
0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xC0, 0x60, 0xA0, 0xA0, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x33, 0x2D, 0x2D, 0x33, 0x1E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0xB6, 0xB6, 0x80, 0xFF,
0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x20, 0x60, 0xC0,
0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x03, 0x06, 0x05, 0x05, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x78, 0xCC, 0xB4, 0xB4, 0xCC, 0x78,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0xAB, 0xAB, 0x01, 0xFF,
0xD5, 0xD5, 0xD4, 0xD4, 0xD0, 0xD0, 0xC0, 0xC0, 0xC1, 0xC3, 0xC7, 0xCE, 0xCC, 0xDC, 0xDC, 0xDC,
0xDC, 0xC8, 0xC0, 0xC0, 0xE0, 0xF0, 0xB8, 0xBC, 0xBC, 0xBC, 0xB8, 0xB8, 0xB0, 0xB0, 0xB0, 0xB0,
0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0,
0x80, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0xF0, 0xFC, 0xFF, 0xFF, 0xFF, 0xC0, 0xAA, 0xAA, 0xC0, 0xFF
};
const uint8_t  intro [] PROGMEM= {
65,4,
0x00, 0xFC, 0xFE, 0x1E, 0x0E, 0xEE, 0x2E, 0xEE, 0x6E, 0xAE, 0x6E, 0xEE, 0xEE, 0xEE, 0x0E, 0xFE,
0xFE, 0xFE, 0x06, 0xFA, 0x9C, 0x0C, 0x0C, 0x08, 0x98, 0xF2, 0x02, 0x06, 0xFE, 0xFE, 0xFE, 0xFE,
0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x0E, 0xFE, 0x3E, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xDE, 0xDE, 0x1E,
0xDE, 0xDE, 0xDE, 0x1E, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x7E, 0xFE, 0xFC,
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x4E, 0x6D, 0x6E, 0xEF, 0x44, 0xEF, 0xEF, 0xFF, 0x00,
0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xC7, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x3F, 0xFF, 0xFF,
0x7F, 0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x08, 0x0F, 0x10, 0x1F, 0x20, 0x7F, 0x83, 0xFF, 0xFF, 0xFF,
0xAB, 0xAA, 0xAA, 0xAA, 0xCB, 0xFF, 0xFF, 0xFF, 0x8D, 0xDD, 0xDD, 0xDD, 0xCD, 0xFD, 0xD0, 0xFF,
0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xE8, 0xED, 0xEC, 0xED, 0x4C, 0xFF, 0xFF, 0xFF,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xE1, 0x1F, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x3F,
0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x30, 0x00, 0xC1, 0xFF, 0xFF,
0xFF, 0x8C, 0xAA, 0x88, 0xBA, 0xCC, 0xFF, 0xFF, 0x9F, 0xB6, 0xB6, 0x36, 0x56, 0x58, 0xEF, 0xFF,
0xFF, 0xFF, 0x00, 0x00, 0x3F, 0x7F, 0x60, 0x60, 0x67, 0x64, 0x66, 0x66, 0x66, 0x64, 0x66, 0x66,
0x67, 0x70, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x78, 0x77, 0x6F, 0x67, 0x60,
0x70, 0x7F, 0x7F, 0x7E, 0x7C, 0x7C, 0x78, 0x78, 0x78, 0x79, 0x7C, 0x7C, 0x7E, 0x7F, 0x7F, 0x7F,
0x7F, 0x7F, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x7E,
0x7F, 0x7F, 0x3F, 0x00, 
};


const uint8_t  GameOver [] PROGMEM= {
  19,4,
0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x7D, 0x45, 0x75, 0x05, 0x45,
0x7D, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x60, 0x90, 0x90, 0x90, 0x90, 0x60, 0x00, 0x00, 0x29, 0x29,
0x2F, 0xA9, 0xA9, 0x46, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xE4, 0x2A, 0x2A, 0x6A, 0x2A, 0xEA, 0x00,
0x00, 0x3A, 0x0A, 0x0A, 0x1A, 0x0A, 0x39, 0x00, 0xFF, 0xFF, 0x80, 0x80, 0x92, 0x8A, 0x86, 0x8E,
0x92, 0x8E, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 
};
