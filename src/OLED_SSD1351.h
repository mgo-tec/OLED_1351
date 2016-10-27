/*
  OLED_SSD1351.cpp - for ESP-WROOM-02 ( esp8266 ) or Arduino
  Beta version 1.54
  
License MIT [Modified person is Mgo-tec.]

Original License is BSD [Copyright (c) 2012, Adafruit Industries]

BSD License:
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holders nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef OLED_SSD1351_h_
#define OLED_SSD1351_h_
#include "Arduino.h"

class OLED_SSD1351
{
public:
  OLED_SSD1351();
  void SSD1351_Init(const uint8_t sclk, const uint8_t mosi, const uint8_t cs, const uint8_t dc, const uint8_t rst);
  void SSD1351_Init262(const uint8_t sclk, const uint8_t mosi, const uint8_t cs, const uint8_t dc, const uint8_t rst, uint8_t col_form);
  void SSD1351_BlackOut();
  void SSD1351_BlackOut262();
  void SSD1351_8x16_DisplayOut(uint8_t StartX, uint8_t StartY, uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t* buf);
  void SSD1351_8x16_DisplayOut_1col_LtoR(uint8_t StartX, uint8_t StartY, uint8_t Red, uint8_t Green, uint8_t Blue, uint16_t sjis_len, uint8_t buf[][16]);
  void SSD1351_8x16_DisplayOut_1col_LtoR262(uint8_t StartX, uint8_t StartY, uint8_t Red, uint8_t Green, uint8_t Blue, uint16_t sjis_len, uint8_t buf[][16]);
	void SSD1351_8x16_DisplayOut_1col_RtoL(uint8_t StartX, uint8_t StartY, uint8_t Red, uint8_t Green, uint8_t Blue, uint16_t sjis_len, uint8_t buf[][16]);
  void SSD1351_8x16_2x2_DisplayOut(uint8_t StartX, uint8_t StartY, uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t* buf);
  void SSD1351_RAM_Hscrolle(uint8_t StartY, uint8_t EndY, uint8_t Interval, uint8_t direction);
  void SSD1351_RAM_Vscrolle(uint8_t StartY, uint8_t OffsetY);
  void Scroller_8x16Dot_Replace(uint8_t drection, uint8_t next_buff1[][16], uint8_t scl_buff1[][16], uint8_t* Orign_buff1);
  void SSD1351_RGBcontrast(uint8_t Red, uint8_t Green, uint8_t Blue);
  void SSD1351_1pixel_DisplayOut(uint8_t x, uint8_t y, uint8_t Red, uint8_t Green, uint8_t Blue);
  void SSD1351_1pixel_DisplayOut262(uint8_t x, uint8_t y, uint8_t Red, uint8_t Green, uint8_t Blue);
  void SSD1351_lineH(uint8_t x1, uint8_t x2, uint8_t y, uint8_t Red, uint8_t Green, uint8_t Blue);
  void SSD1351_lineV(uint8_t x, uint8_t y1, uint8_t y2, uint8_t Red, uint8_t Green, uint8_t Blue);
  void SSD1351_RectLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t Red, uint8_t Green, uint8_t Blue);
  void SSD1351_RectFill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t Red, uint8_t Green, uint8_t Blue);
  void SSD1351_RectFill262(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t Red, uint8_t Green, uint8_t Blue);
	void SPIwrite(uint8_t c);
  void writeCommand(uint8_t c);
  void writeData(uint8_t c);
  
private:
};

#endif