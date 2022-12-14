//==============================================================================
//
//  CRYSTALFONTZ
//  The controller is a Neotec NT7108
//    https://www.crystalfontz.com/controllers/Neotec/NT7108/
//
//  Seeeduino v4.2, an open-source 3.3v capable Arduino clone.
//    https://www.seeedstudio.com/Seeeduino-V4.2-p-2517.html
//    https://github.com/SeeedDocument/SeeeduinoV4/raw/master/resources/Seeeduino_v4.2_sch.pdf
//
//==============================================================================
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>
//==============================================================================
// LCD parallel 6800 pins and control lines on Seeeduino:
// Seeduino    |  Port  | LCD
// ------------+--------+-------------
// #8/D8       |  PB0   | D/I (data/instruction)
// #9/D9       |  PB1   | R/W (read/write)
// #10/D10     |  PB2   | E (enable signal)
// #11/D11     |  PB3   | CS1 (column select 1)
// #12/D12     |  PB4   | CS2 (column select 2)
// #13/D13     |  PB5   | RES (reset signal)
// #0-7/D0-7   |  PD0-7 | DB0-DB7 (data bus lines)
//
// you also need to control the contrast with a variable resistor,
// please refer the datasheet schematic for connection details
//==============================================================================

#include <Arduino.h>

// define pins for commands and data transfer
#define RS (8)
#define RW (9)
#define EN (10)
#define CS1 (11)
#define CS2 (12)
#define RST (13)
#define DATAPINS (PORTD)

#define RS_MASK (0x01)
#define CLR_RS (PORTB &= ~(RS_MASK))
#define SET_RS (PORTB |= (RS_MASK))

#define RW_MASK (0x02)
#define CLR_RW (PORTB &= ~(RW_MASK))
#define SET_RW (PORTB |= (RW_MASK))

#define EN_MASK (0x04)
#define CLR_EN (PORTB &= ~(EN_MASK))
#define SET_EN (PORTB |= (EN_MASK))

#define CS1_MASK (0x08)
#define CLR_CS1 (PORTB &= ~(CS1_MASK))
#define SET_CS1 (PORTB |= (CS1_MASK))

#define CS2_MASK (0x10)
#define CLR_CS2 (PORTB &= ~(CS2_MASK))
#define SET_CS2 (PORTB |= (CS2_MASK))

#define RST_MASK (0x20)
#define CLR_RST (PORTB &= ~(RST_MASK))
#define SET_RST (PORTB |= (RST_MASK))

#define HRES (128)
#define VRES (64)

// define column addresses
#define col1 (0x01) // column 1
#define col2 (0x02) // column 2
#define bcol (0x03) // both columns

// page x addresses
// page 0 = 0xb8
// page 1 = 0xb9
// page 2 = 0xba
// page 3 = 0xbb
// page 4 = 0xbc
// page 5 = 0xbd
// page 6 = 0xbe
// page 7 = 0xbf

void sendCommand(uint8_t col, uint8_t cmd)
{
    CLR_RS;
    if (col == col1) //
    {
        SET_CS1;
        CLR_CS2;
    }
    else if (col == col2) //
    {
        SET_CS2;
        CLR_CS1;
    }
    else if (col == bcol)
    {
        SET_CS1;
        SET_CS2;
    }
    delay(10);
    DATAPINS = cmd;
    SET_EN;
    delay(10);
    CLR_EN;
}

void sendData(uint8_t col, uint8_t data)
{
    SET_RS;
    if (col == col1)
    {
        SET_CS1;
        CLR_CS2;
    }
    else if (col == col2)
    {
        SET_CS2;
        CLR_CS1;
    }
    else if (col == bcol)
    {
        SET_CS1;
        SET_CS2;
    }
    delay(10);
    DATAPINS = data;
    SET_EN;
    delay(10);
    CLR_EN;
}

void init_NT7101()
{
    sendCommand(bcol, 0x3f); // display on
    delay(100);
    sendCommand(bcol, 0x40); // y zero address
    delay(100);
    sendCommand(bcol, 0xb8); // x zero address
    delay(100);
    sendCommand(bcol, 0xc0); // display start line set to zero
    delay(100);
}

void startAddress(uint8_t page)
{
    sendCommand(bcol, page);
}

void fillhalf()
{
    uint8_t x, y;
    startAddress(0xb8); // page 0 address
    for (y = 0; y < 8; y++)
    {
        startAddress(0xb8 + y); // cycle between pages 0-7
        for (x = 0; x < 64; x++)
        {
            sendData(col1, 0xff);
        }
        for (x = 0; x < 64; x++)
        {
            sendData(col2, 0x00); //
        }
    }
}

void crossDot()
{
    uint8_t x, y;
    startAddress(0xb8);
    for (y = 0; y < 8; y++)
    {
        startAddress(0xb8 + y);
        for (x = 0; x < 32; x++)
        {
            sendData(col1, 0xaa);
            sendData(col1, 0x55);
        }
        for (x = 0; x < 32; x++)
        {
            sendData(col2, 0xaa);
            sendData(col2, 0x55);
        }
    }
}

void vertline()
{
    uint8_t x, y;
    startAddress(0xb8);
    for (y = 0; y < 8; y++)
    {
        startAddress(0xb8 + y);
        for (x = 0; x < 32; x++)
        {
            sendData(col1, 0xff);
            sendData(col1, 0x00);
        }
        for (x = 0; x < 32; x++)
        {
            sendData(col2, 0xff);
            sendData(col2, 0x00);
        }
    }
}

void showpic(uint8_t pic[8][128])
{
    uint8_t x, y;
    startAddress(0xb8);
    for (y = 0; y < 8; y++)
    {
        startAddress(0xb8 + y);
        for (x = 0; x < 64; x++)
        {
            sendData(col1, pic[y][x]);
        }
        for (x = 64; x < 128; x++)
        {
            sendData(col2, pic[y][x]);
        }
    }
}

uint8_t picture[8][128] =
    { // CFA logo
        {0xF0, 0xFC, 0x0E, 0x07, 0x03, 0x03, 0x03, 0x07,
         0x0E, 0x0C, 0x00, 0xFF, 0xFF, 0x83, 0x83, 0x83,
         0x83, 0x83, 0xC7, 0xFE, 0x7C, 0x00, 0x03, 0x0F,
         0x3C, 0xF0, 0xC0, 0xC0, 0xF0, 0x3C, 0x0F, 0x03,
         0x00, 0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3,
         0xC7, 0x8E, 0x0C, 0x00, 0x03, 0x03, 0x03, 0xFF,
         0xFF, 0x03, 0x03, 0x03, 0x00, 0x00, 0xE0, 0xFC,
         0x1F, 0x1F, 0xFC, 0xE0, 0x00, 0x00, 0x00, 0x00,
         0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC3, 0xC3,
         0xC3, 0xC3, 0xC3, 0xC3, 0x03, 0x03, 0xF0, 0xFC,
         0x0E, 0x07, 0x03, 0x03, 0x07, 0x0E, 0xFC, 0xF0,
         0x00, 0x00, 0xFF, 0xFF, 0x0E, 0x38, 0xF0, 0xC0,
         0x00, 0x00, 0xFF, 0xFF, 0x00, 0x03, 0x03, 0x03,
         0xFF, 0xFF, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03,
         0x03, 0x03, 0xC3, 0xE3, 0x33, 0x1F, 0x0F, 0x03},
        {0x03, 0x0F, 0x1C, 0x38, 0x30, 0x30, 0x30, 0x38,
         0x1C, 0x0C, 0x00, 0x3F, 0x3F, 0x01, 0x01, 0x03,
         0x07, 0x0F, 0x1D, 0x38, 0x30, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x0C, 0x1C, 0x38, 0x30, 0x30, 0x30, 0x30,
         0x39, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x3F,
         0x3F, 0x00, 0x00, 0x00, 0x38, 0xBF, 0x87, 0x86,
         0x86, 0x06, 0x06, 0x07, 0xBF, 0xB8, 0x80, 0x80,
         0xBF, 0xBF, 0x30, 0x30, 0x30, 0xB0, 0xB0, 0xB0,
         0x30, 0x30, 0x00, 0x00, 0x3F, 0x3F, 0x80, 0x80,
         0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F,
         0x9C, 0xB8, 0x30, 0x30, 0x38, 0x9C, 0x8F, 0x83,
         0x00, 0x00, 0x3F, 0x3F, 0x80, 0x80, 0x80, 0x03,
         0x07, 0x1C, 0x3F, 0xBF, 0x80, 0x80, 0x00, 0x00,
         0x3F, 0x3F, 0x00, 0x00, 0x80, 0x80, 0x30, 0x3C,
         0x3E, 0x33, 0xB1, 0xB0, 0xB0, 0x30, 0x30, 0x30},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C,
         0x0C, 0x0C, 0x1C, 0x38, 0x70, 0x20, 0x00, 0x00,
         0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C, 0x0C, 0x0C,
         0x1C, 0x38, 0x70, 0xE0, 0xC0, 0x80, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3C, 0x7F, 0xC3, 0x80, 0x80, 0xC0,
         0x61, 0x21, 0xC0, 0xFE, 0x3F, 0x09, 0x08, 0x08,
         0x08, 0x80, 0xE0, 0x78, 0x3E, 0x27, 0x21, 0x3F,
         0xFE, 0xC0, 0x00, 0x3C, 0x7F, 0x83, 0x80, 0x88,
         0xC8, 0x79, 0x39, 0x00, 0x00, 0x06, 0xE3, 0xFF,
         0x1F, 0x01, 0x00, 0xC0, 0xE1, 0xB1, 0x98, 0x8F,
         0x07, 0x00, 0x76, 0xFF, 0x88, 0x88, 0x7F, 0x37,
         0x00, 0x7C, 0xFF, 0x8B, 0xC4, 0x7D, 0x39, 0x30,
         0x38, 0x2C, 0xE6, 0xFF, 0x3F, 0x21, 0x80, 0xE0,
         0x78, 0x3E, 0x27, 0x21, 0x3F, 0xFE, 0xC0, 0x00},
        {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x98, 0x9C,
         0x9E, 0x9F, 0x9F, 0x9F, 0x9F, 0x80, 0x80, 0x80,
         0x80, 0x00, 0x00, 0x00, 0xF8, 0xFC, 0xFE, 0xFF,
         0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x80, 0x80, 0x80,
         0x80, 0x80, 0x80, 0x9F, 0x9F, 0x9F, 0x9F, 0x9E,
         0x9C, 0x98, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
         0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
         0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00,
         0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
         0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
         0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
         0x80, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x38,
         0x1C, 0x8E, 0x06, 0x06, 0x8E, 0x1C, 0x38, 0xF0,
         0xE0, 0x80, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00,
         0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00,
         0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
         0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
         0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
         0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x99, 0x99, 0x99,
         0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99,
         0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99,
         0x00, 0xC0, 0xE0, 0xB0, 0xB8, 0x9C, 0x8E, 0x86,
         0x83, 0x83, 0x81, 0x81, 0x81, 0x83, 0x83, 0x87,
         0x8E, 0x9C, 0xB8, 0xF0, 0xE0, 0x80, 0x80, 0x80,
         0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
         0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
         0xC0, 0xF0, 0xBC, 0x9F, 0x87, 0x81, 0x80, 0x80,
         0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
         0x83, 0x8F, 0xBE, 0xF8, 0xE0, 0x80, 0x80, 0x80,
         0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
         0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
         0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
         0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
         0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x99, 0x99, 0x99,
         0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99,
         0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99,
         0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
         0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
         0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x0F, 0x1D,
         0x79, 0xF1, 0xC1, 0x81, 0x01, 0x01, 0x01, 0x01,
         0x01, 0x01, 0x81, 0xC1, 0xE1, 0x79, 0x1D, 0x0F,
         0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
         0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
         0x01, 0x01, 0x01, 0x01, 0x07, 0x1F, 0x7D, 0xF1,
         0xC1, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
         0x01, 0x01, 0x01, 0x01, 0xC1, 0xF1, 0x7F, 0x1F},
        {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x19, 0x39,
         0x79, 0xF9, 0xF9, 0xF9, 0xF9, 0x01, 0x01, 0x01,
         0x01, 0x00, 0x00, 0x00, 0x1F, 0x3F, 0x7F, 0xFF,
         0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x01, 0x01,
         0x01, 0x01, 0x01, 0xF9, 0xF9, 0xF9, 0xF9, 0x79,
         0x39, 0x19, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
         0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
         0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
         0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
         0x00, 0x01, 0x01, 0x01, 0x03, 0x07, 0x06, 0x07,
         0x06, 0x07, 0x03, 0x01, 0x00, 0x01, 0x00, 0x00,
         0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
         0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01,
         0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x03,
         0x07, 0x1E, 0x38, 0xF1, 0xC0, 0x80, 0x81, 0x80,
         0xC0, 0xF1, 0x78, 0x1F, 0x07, 0x01, 0x00, 0x01},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x01, 0x03, 0x07, 0x0E, 0x1C, 0x38,
         0x30, 0x30, 0x38, 0x1C, 0x0E, 0x04, 0x00, 0x00,
         0x01, 0x03, 0x07, 0x0E, 0x1C, 0x38, 0x30, 0x30,
         0x38, 0x1C, 0x0E, 0x07, 0x03, 0x01, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x4C, 0x92, 0x92, 0x92, 0x64, 0x00, 0xFE,
         0x04, 0x08, 0x04, 0xFE, 0x00, 0xFC, 0x22, 0x22,
         0x22, 0xFC, 0x00, 0xFE, 0x80, 0x80, 0x80, 0x80,
         0x00, 0xFE, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x78, 0x94, 0x92, 0x92, 0x60,
         0x00, 0xD8, 0x20, 0x20, 0xD8, 0x00, 0x00, 0x6C,
         0x92, 0x92, 0x92, 0x6C, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x02, 0x02, 0xFE, 0x02, 0x02, 0x00, 0xFE,
         0x92, 0x92, 0x92, 0x82, 0x01, 0xC7, 0x29, 0x11,
         0x29, 0xC6, 0x00, 0x02, 0x02, 0xFE, 0x02, 0x02}};

void setup()
{
    DDRB = 0x3f; // set pins 8-13 as outputs
    DDRD = 0xff; // set pins 0-7 as outputs
    // set a reasonable starting state for the pins
    PORTD = 0;
    CLR_RW; // we can set the command to write and not change this pin as no read instruction is present
    SET_RS;
    CLR_EN;
    CLR_CS1;
    CLR_CS2;
    init_NT7101(); // initialize the display
}

void loop()
{
    // you may have to disconnect pins 1 and 2 when uploading the code
    fillhalf();
    delay(2000);
    crossDot();
    delay(2000);
    vertline();
    delay(2000);
    showpic(picture);
    delay(10000);
}
