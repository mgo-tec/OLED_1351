/*
  OLED_SSD1351.cpp - for ESP-WROOM-02 ( esp8266 ) or Arduino
  Beta version 1.40
  
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

#include "Arduino.h"
#include "OLED_SSD1351.h"

static uint8_t _DCpin;
static uint8_t _sclk;
static uint8_t _mosi;
static uint8_t _RSTpin;
static uint8_t _cs;

OLED_SSD1351::OLED_SSD1351(){}

//****************SSD1351初期化*************************************************
void OLED_SSD1351::SSD1351_Init(const uint8_t sclk, const uint8_t mosi, const uint8_t cs, const uint8_t dc, const uint8_t rst){

  _DCpin = dc; _sclk = sclk; _mosi = mosi; _RSTpin = rst; _cs = cs;
  pinMode(_DCpin, OUTPUT);
  pinMode(_sclk, OUTPUT);
  pinMode(_mosi, OUTPUT);
  pinMode(_RSTpin, OUTPUT);
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, LOW);
   
  digitalWrite(_RSTpin, HIGH);
  delay(500);
  digitalWrite(_RSTpin, LOW);
  delay(500);
  digitalWrite(_RSTpin, HIGH);
  delay(500);
  
  writeCommand(0xFD); //Set Command Lock
    writeData(0x12); //Unlock OLED driver IC MCU interface from entering command
  writeCommand(0xFD); //Set Command Lock
    writeData(0xB1); //Command A2,B1,B3,BB,BE,C1 accessible if in unlock state
  writeCommand(0xAE); //Sleep mode On (Display OFF)
  writeCommand(0xB3); //Front Clock Divider
    writeCommand(0xF1); // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
  writeCommand(0xCA); //Set MUX Ratio
    writeData(127);
  writeCommand(0xA0); //Set Re-map
    writeData(B01110100); //65k color
    //writeData(B10110100); //262k color
    //writeData(B11110100); //262k color, 16-bit format 2
  writeCommand(0x15); //Set Column
    writeData(0); //start
    writeData(127); //end
  writeCommand(0x75); //Set Row
    writeData(0); //start
    writeData(127); //end
  writeCommand(0xA1); //Set Display Start Line
    writeData(0);
  writeCommand(0xA2); //Set Display Offset
    writeData(0);
  writeCommand(0xB5); //Set GPIO
    writeData(0);
  writeCommand(0xAB); //Function Selection
    writeData(0x01); //Enable internal Vdd /8-bit parallel
    //writeData(B01000001); //Enable internal Vdd /Select 16-bit parallel interface
  writeCommand(0xB1); //Set Reset(Phase 1) /Pre-charge(Phase 2)
    //writeCommand(B00110010); //5 DCLKs / 3 DCLKs
    writeCommand(0x74);
  writeCommand(0xBE); //Set VCOMH Voltage
    writeCommand(0x05); //0.82 x VCC [reset]
  writeCommand(0xA6); //Reset to normal display
  writeCommand(0xC1); //Set Contrast
    writeData(0xC8); //Red contrast (reset=0x8A)
    writeData(0x80); //Green contrast (reset=0x51)
    writeData(0xC8); //Blue contrast (reset=0x8A)
  writeCommand(0xC7); //Master Contrast Current Control
    writeData(0x0F); //0-15
  writeCommand(0xB4); //Set Segment Low Voltage(VSL)
    writeData(0xA0);
    writeData(0xB5);
    writeData(0x55);
  writeCommand(0xB6); //Set Second Precharge Period
    writeData(0x01); //1 DCLKS
  writeCommand(0x9E); //Scroll Stop Moving
  writeCommand(0xAF); //Sleep mode On (Display ON)
}
//****************SSD1351初期化*************************************************
void OLED_SSD1351::SSD1351_Init262(const uint8_t sclk, const uint8_t mosi, const uint8_t cs, const uint8_t dc, const uint8_t rst, uint8_t col_form){

  _DCpin = dc; _sclk = sclk; _mosi = mosi; _RSTpin = rst; _cs = cs;
  pinMode(_DCpin, OUTPUT);
  pinMode(_sclk, OUTPUT);
  pinMode(_mosi, OUTPUT);
  pinMode(_RSTpin, OUTPUT);
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, LOW);
   
  digitalWrite(_RSTpin, HIGH);
  delay(500);
  digitalWrite(_RSTpin, LOW);
  delay(500);
  digitalWrite(_RSTpin, HIGH);
  delay(500);
  
  writeCommand(0xFD); //Set Command Lock
    writeData(0x12); //Unlock OLED driver IC MCU interface from entering command
  writeCommand(0xFD); //Set Command Lock
    writeData(0xB1); //Command A2,B1,B3,BB,BE,C1 accessible if in unlock state
  writeCommand(0xAE); //Sleep mode On (Display OFF)
  writeCommand(0xB3); //Front Clock Divider
    writeCommand(0xF1); // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
  writeCommand(0xCA); //Set MUX Ratio
    writeData(127);
  writeCommand(0xA0); //Set Re-map
  switch(col_form){
    case 0:
      writeData(B01110100); //65k color
      break;
    case 1:
      writeData(B10110100); //262k color
      break;
    case 2:
      writeData(B11110100); //262k color, 16-bit format 2
  }
  writeCommand(0xA0); //Set Re-map
    //writeData(B01110100); //65k color
    writeData(B10110100); //262k color
    //writeData(B11110100); //262k color, 16-bit format 2
  writeCommand(0x15); //Set Column
    writeData(0); //start
    writeData(127); //end
  writeCommand(0x75); //Set Row
    writeData(0); //start
    writeData(127); //end
  writeCommand(0xA1); //Set Display Start Line
    writeData(0);
  writeCommand(0xA2); //Set Display Offset
    writeData(0);
  writeCommand(0xB5); //Set GPIO
    writeData(0);
  writeCommand(0xAB); //Function Selection
    writeData(0x01); //Enable internal Vdd /8-bit parallel
    //writeData(B01000001); //Enable internal Vdd /Select 16-bit parallel interface
  writeCommand(0xB1); //Set Reset(Phase 1) /Pre-charge(Phase 2)
    //writeCommand(B00110010); //5 DCLKs / 3 DCLKs
    writeCommand(0x74);
  writeCommand(0xBE); //Set VCOMH Voltage
    writeCommand(0x05); //0.82 x VCC [reset]
  writeCommand(0xA6); //Reset to normal display
  writeCommand(0xC1); //Set Contrast
    writeData(0xC8); //Red contrast (reset=0x8A)
    writeData(0x80); //Green contrast (reset=0x51)
    writeData(0xC8); //Blue contrast (reset=0x8A)
  writeCommand(0xC7); //Master Contrast Current Control
    writeData(0x0F); //0-15
  writeCommand(0xB4); //Set Segment Low Voltage(VSL)
    writeData(0xA0);
    writeData(0xB5);
    writeData(0x55);
  writeCommand(0xB6); //Set Second Precharge Period
    writeData(0x01); //1 DCLKS
  writeCommand(0x9E); //Scroll Stop Moving
  writeCommand(0xAF); //Sleep mode On (Display ON)
}
//****************全画面消去*************************************************
void OLED_SSD1351::SSD1351_BlackOut(){
  writeCommand(0x15); //Set Column
    writeData(0x00);
    writeData(127);
  writeCommand(0x75); //Set Row
    writeData(0x00);
    writeData(127);
  writeCommand(0x5C); //Write RAM
  for(int i=0; i<128*128; i++){
    writeData(0x00);
    writeData(0x00);
    //writeData(0x00); //262k colorの場合３バイト分送信
  }
}
//****************全画面消去*************************************************
void OLED_SSD1351::SSD1351_BlackOut262(){
  writeCommand(0x15); //Set Column
    writeData(0x00);
    writeData(127);
  writeCommand(0x75); //Set Row
    writeData(0x00);
    writeData(127);
  writeCommand(0x5C); //Write RAM
  for(int i=0; i<128*128; i++){
    writeData(0x00);
    writeData(0x00);
    writeData(0x00); //262k colorの場合３バイト分送信
  }
}
//****************等倍フォント表示*************************************************
void OLED_SSD1351::SSD1351_8x16_DisplayOut(uint8_t StartX, uint8_t StartY, uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t* buf){
  int16_t i,j;
  uint8_t RGBbit1, RGBbit2;
  RGBbit1 = (Red<<3) | (Green>>3);
  RGBbit2 = (Green<<5) | Blue;
   
  writeCommand(0x15); //Set Column
    writeData(StartX);
    writeData(StartX+7);
  writeCommand(0x75); //Set Row
    writeData(StartY);
    writeData(StartY+15);
  writeCommand(0x5C); //Write RAM
   
  for(i=0; i<16; i++){
    for(j=7; j>=0; j--){
      if(buf[i] & _BV(j)){
        writeData(RGBbit1);
        writeData(RGBbit2);
      }else{
        writeData(0);
        writeData(0);
      }
    }
  }
}
//****************等倍フォント一列一括表示（左→右）*************************************************
void OLED_SSD1351::SSD1351_8x16_DisplayOut_1col_LtoR(uint8_t StartX, uint8_t StartY, uint8_t Red, uint8_t Green, uint8_t Blue, uint16_t sjis_len, uint8_t buf[][16]){
  //１列一括表示
  uint8_t i, kk;
  int16_t j; //これはsigned int でなければならない。
  uint8_t MaxSjLen;
  uint8_t MaxCol;
  uint8_t RGBbit1, RGBbit2;
  RGBbit1 = (Red<<3) | (Green>>3);
  RGBbit2 = (Green<<5) | Blue;
   
  double sx = StartX; //ceil関数はdouble型の引数でなければならない
  MaxSjLen = 16 - ceil(sx / 8);
  if(sjis_len < MaxSjLen) MaxSjLen = sjis_len;
  MaxCol = StartX + MaxSjLen*8-1;

  writeCommand(0x15); //Set Column
    writeData(StartX);
    writeData(MaxCol);
  writeCommand(0x75); //Set Row
    writeData(StartY);
    writeData(StartY+15);
  
  writeCommand(0x5C); //Write RAM
   
  for(kk=0; kk<16; kk++){
    for(i=0; i<MaxSjLen; i++){
      for(j=7; j>=0; j--){
        if(buf[i][kk] & _BV(j)){
          writeData(RGBbit1);
          writeData(RGBbit2);
        }else{
          writeData(0);
          writeData(0);
        }
      }
    }
  }
}
//****************等倍フォント一列一括表示（右→左）*************************************************
void OLED_SSD1351::SSD1351_8x16_DisplayOut_1col_RtoL(uint8_t StartX, uint8_t StartY, uint8_t Red, uint8_t Green, uint8_t Blue, uint16_t sjis_len, uint8_t buf[][16]){
  //これは電光掲示板よのうに文字スクロールする場合の関数で、右→左方向で文字を描く
  int16_t i,j,kk;
  uint8_t MaxSjLen;
  uint8_t MaxCol;
  uint8_t RGBbit1, RGBbit2;
  RGBbit1 = (Red<<3) | (Green>>3);
  RGBbit2 = (Green<<5) | Blue;
   
  double sx = StartX; //ceil関数はdouble型の引数でなければならない
  MaxSjLen = 16 - ceil(sx / 8);
  if(sjis_len < MaxSjLen) MaxSjLen = sjis_len;
  MaxCol = StartX + MaxSjLen*8-1;
  
  writeCommand(0x15); //Set Column
    writeData(StartX);
    writeData(MaxCol);
  writeCommand(0x75); //Set Row
    writeData(StartY);
    writeData(StartY+15);

  writeCommand(0x5C); //Write RAM
   
  for(kk=0; kk<16; kk++){
    for(i=MaxSjLen-1; i>=0; i--){
      for(j=7; j>=0; j--){
        if(buf[i][kk] & _BV(j)){
          writeData(RGBbit1);
          writeData(RGBbit2);
        }else{
          writeData(0);
          writeData(0);
        }
      }
    }
  }
}
//****************倍角フォント表示*************************************************
void OLED_SSD1351::SSD1351_8x16_2x2_DisplayOut(uint8_t StartX, uint8_t StartY, uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t* buf){
  int16_t i,j,ii;
  uint8_t RGBbit1, RGBbit2;
  RGBbit1 = (Red<<3) | (Green>>3);
  RGBbit2 = (Green<<5) | Blue;
   
  writeCommand(0x15); //Set Column
    writeData(StartX);
    writeData(StartX+15);
  writeCommand(0x75); //Set Row
    writeData(StartY);
    writeData(StartY+31);
  writeCommand(0x5C); //Write RAM
   
  for(i=0; i<16; i++){
    for(ii=0; ii<2; ii++){//倍角の場合２行同じものを描く
      for(j=7; j>=0; j--){
        if(buf[i] & _BV(j)){
          writeData(RGBbit1);
          writeData(RGBbit2);
          writeData(RGBbit1);
          writeData(RGBbit2);
        }else{
          writeData(0);
          writeData(0);
          writeData(0);
          writeData(0);
        }
      }
    }
  }
}
//****************SSD1351 RAM 水平スクロール*************************************************
void OLED_SSD1351::SSD1351_RAM_Hscrolle(uint8_t StartY, uint8_t EndY, uint8_t Interval, uint8_t direction){
  uint8_t dr;
  switch(direction){
    case 0:
      dr = 1;
      break;
    case 1:
      dr = 0xFF;
      break;
  }
  writeCommand(0x96);
    writeData(dr);//1～B00111111までは同じ方向、逆はB01000000～0xFFはどれでも同じ
    writeData(StartY); //Start row address
    writeData(EndY); //垂直値はどこまでをスクロール対象にするか。Start address + ドット数　<=128
    writeData(0); //リザーブ
    writeData(Interval); //Scrolling time interval 
  writeCommand(0x9F); // Start scroll
 
}
//****************SSD1351 RAM 水平スクロール*************************************************
void OLED_SSD1351::SSD1351_RAM_Vscrolle(uint8_t StartY, uint8_t OffsetY){
  writeCommand(0xFD); //Set Command Lock
    writeData(0x12); //Unlock OLED driver IC MCU interface from entering command
  writeCommand(0xA1); //
    writeData(StartY);
  writeCommand(0xA2); //
    writeData(OffsetY);
 
}
//****************電光掲示風 Dot Scroll Replace**************************************
void OLED_SSD1351::Scroller_8x16Dot_Replace(uint8_t drection, uint8_t next_buff1[][16], uint8_t scl_buff1[][16], uint8_t* Orign_buff1)
{
  int8_t i, j;

  for(i=15 ; i>=0 ; i--){ //デクリメントの方が処理速度早い
    next_buff1[15][i] = ( next_buff1[15][i] | ( scl_buff1[15][i] & B10000000 ));
    scl_buff1[15][i] = scl_buff1[15][i]<<1;
    scl_buff1[15][i] = ( scl_buff1[15][i] | (( Orign_buff1[i] & B10000000 )>>7));
    Orign_buff1[i] = Orign_buff1[i]<<1;
  }
  for(i=14 ; i>=0 ; i--){
    for(j=15 ; j>=0 ; j--){ //デクリメントの方が処理速度早い
      next_buff1[i][j] = ( next_buff1[i][j] | ( scl_buff1[i][j] & B10000000 ));
      scl_buff1[i][j] = scl_buff1[i][j]<<1;
      scl_buff1[i][j] = ( scl_buff1[i][j] | (( next_buff1[i+1][j] & B10000000 )>>7));
      next_buff1[i+1][j] = next_buff1[i+1][j]<<1;
    }
  }
}
//****************SSD1351 RGBコントラスト*************************************************
void OLED_SSD1351::SSD1351_RGBcontrast(uint8_t Red, uint8_t Green, uint8_t Blue){
  writeCommand(0xC1); //Set Contrast
    writeData(Red); //Red contrast (reset=0x8A)
    writeData(Green); //Green contrast (reset=0x51)
    writeData(Blue); //Blue contrast (reset=0x8A)
}
//****************1pixel表示*************************************************
void OLED_SSD1351::SSD1351_1pixel_DisplayOut(uint8_t x, uint8_t y, uint8_t Red, uint8_t Green, uint8_t Blue){
  uint8_t RGBbit1 = (Red<<3) | (Green>>3);
  uint8_t RGBbit2 = (Green<<5) | Blue;
  
  writeCommand(0x15); //Set Column
    writeData(x);
    writeData(x);
  writeCommand(0x75); //Set Row
    writeData(y);
    writeData(y);
  writeCommand(0x5C); //Write RAM
    writeData(RGBbit1);
    writeData(RGBbit2);
}
//****************1pixel表示*************************************************
void OLED_SSD1351::SSD1351_1pixel_DisplayOut262(uint8_t x, uint8_t y, uint8_t Red, uint8_t Green, uint8_t Blue){
  //Red, Green, Blue Max=63 (B00111111) 
  writeCommand(0x15); //Set Column
    writeData(x);
    writeData(x);
  writeCommand(0x75); //Set Row
    writeData(y);
    writeData(y);
  writeCommand(0x5C); //Write RAM 262k color serial
    writeData(Red);
    writeData(Green);
    writeData(Blue);
}
//****************水平直線描画*************************************************
void OLED_SSD1351::SSD1351_lineH(uint8_t x1, uint8_t x2, uint8_t y, uint8_t Red, uint8_t Green, uint8_t Blue){
  uint8_t RGBbit1 = (Red<<3) | (Green>>3);
  uint8_t RGBbit2 = (Green<<5) | Blue;
  
  writeCommand(0x15); //Set Column
    writeData(x1);
    writeData(x2);
  writeCommand(0x75); //Set Row
    writeData(y);
    writeData(y);
  writeCommand(0x5C); //Write RAM
    for(int i=0; i<=(x2-x1); i++){
      writeData(RGBbit1);
      writeData(RGBbit2);
    }
}
//****************垂直直線描画*************************************************
void OLED_SSD1351::SSD1351_lineV(uint8_t x, uint8_t y1, uint8_t y2, uint8_t Red, uint8_t Green, uint8_t Blue){
  uint8_t RGBbit1 = (Red<<3) | (Green>>3);
  uint8_t RGBbit2 = (Green<<5) | Blue;
  
  writeCommand(0x15); //Set Column
    writeData(x);
    writeData(x);
  writeCommand(0x75); //Set Row
    writeData(y1);
    writeData(y2);
  writeCommand(0x5C); //Write RAM
    for(int i=0; i<=(y2-y1); i++){
      writeData(RGBbit1);
      writeData(RGBbit2);
    }
}
//****************四角形、線画*************************************************
void OLED_SSD1351::SSD1351_RectLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t Red, uint8_t Green, uint8_t Blue){
  uint8_t RGBbit1 = (Red<<3) | (Green>>3);
  uint8_t RGBbit2 = (Green<<5) | Blue;
  
  SSD1351_lineH(x1, x2, y1, Red, Green, Blue);
  SSD1351_lineH(x1, x2, y2, Red, Green, Blue);
  SSD1351_lineV(x1, y1, y2, Red, Green, Blue);
  SSD1351_lineV(x2, y1, y2, Red, Green, Blue);
}
//****************四角形、塗りつぶし*************************************************
void OLED_SSD1351::SSD1351_RectFill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t Red, uint8_t Green, uint8_t Blue){
  uint8_t RGBbit1 = (Red<<3) | (Green>>3);
  uint8_t RGBbit2 = (Green<<5) | Blue;
  
  writeCommand(0x15); //Set Column
    writeData(x1);
    writeData(x2);
  writeCommand(0x75); //Set Row
    writeData(y1);
    writeData(y2);
  writeCommand(0x5C); //Write RAM
  for(int i=0; i<=(y2-y1); i++)
    for(int j=0; j<=(x2-x1); j++){
      writeData(RGBbit1);
      writeData(RGBbit2);
    }
}
//****************SPIデータ処理*************************************************
void OLED_SSD1351::SPIwrite(uint8_t c){
  digitalWrite(_sclk, HIGH);
  int8_t i; //signed intでなければならない。負の数になると255という値になって、例外エラーになる。
  for (i=7; i>=0; i--) {
      digitalWrite(_sclk, LOW);    
      if (c & _BV(i)) {
          digitalWrite(_mosi, HIGH);
      } else {
          digitalWrite(_mosi, LOW);
      }
      digitalWrite(_sclk, HIGH);
  }
}
 
void OLED_SSD1351::writeCommand(uint8_t c) {
    digitalWrite(_DCpin, LOW);
    digitalWrite(_cs, LOW);
    SPIwrite(c);
    digitalWrite(_cs, HIGH);
}
 
void OLED_SSD1351::writeData(uint8_t c) {
    digitalWrite(_DCpin, HIGH);
    digitalWrite(_cs, LOW);
    SPIwrite(c);
    digitalWrite(_cs, HIGH);
}
