/*  Adafruit OLED Breakout Board – 16-bit Color 1.5″ w/microSD holder を使用するためのサンプルスケッチ
 *  日本語専用です。
 *  UTF8toSJISライブラリ、ShinonomeFONTreadライブラリを事前にインストールする必要があります。
 *　事前にSPIFFSファイルシステムアップローダーでフォントファイルをフラッシュにアップロードする必要があります。
 *  詳しくは https://wwww.mgo-tec.com
 */
#include <UTF8toSJIS.h>
#include <ShinonomeFONTread.h>
#include <OLED_SSD1351.h>

UTF8toSJIS u8ts;
ShinonomeFONTread SFR;
OLED_SSD1351 ssd1351;

const char* UTF8SJIS_file = "/Utf8Sjis.tbl"; //UTF8 Shift_JIS 変換テーブルファイル名を記載しておく
const char* ZenkakuFontFile = "/shnmk16.bdf"; //全角フォントファイル名を定義
const char* HalfFontFile = "/shnm8x16r.bdf"; //半角フォントファイル名を定義

char* cc[8] = {"フリー日本語漢字","東雲(ｼﾉﾉﾒ)ﾌｫﾝﾄ★","16x16ﾄﾞｯﾄ です。","全角-半角混在OK!","半角ｶﾀｶﾅもOK！！","16bit カラー★♪","倍角表示"};

uint8_t sj_txt[32]; //Shift_JISコード
uint16_t sj_length; //Shift_JISコードの長さ
uint8_t font_buf[32][16]; //東雲フォントデータ

const uint8_t sclk=14;
const uint8_t  mosi = 13;
const uint8_t  cs = 15;
const uint8_t DCpin =  4;
const uint8_t RSTpin =  5;

uint8_t R_ctr = 255, G_ctr = 170, B_ctr = 85;
uint8_t R_ud = 0, G_ud = 0, B_ud = 0;
uint8_t scl = 0;
uint32_t sclTime = 0;
uint32_t contrastTime = 0;

void setup() {
  Serial.begin(115200);
  ssd1351.SSD1351_Init(sclk, mosi, cs, DCpin, RSTpin); //SSD1351を初期化
  delay(300);
  ssd1351.SSD1351_BlackOut(); //黒画面出力

  uint8_t Red, Green, Blue; //Max Red = 31, Max Green = 63, MaxBlue = 31
  int8_t i,j;
  int8_t ci=0;

  for(j=0; j<7; j++){
    u8ts.UTF8_to_SJIS_str_cnv(UTF8SJIS_file, cc[j], sj_txt, &sj_length); //UTF8コードをShift_JISコードに変換
    SFR.SjisToShinonome16FontRead_ALL(ZenkakuFontFile, HalfFontFile, 0, 0, sj_txt, sj_length, font_buf); //S_JISコードから東雲フォント抽出

    switch(j){
      case 0:
        Red = 31, Green = 0, Blue = 0;
        break;
      case 1:
        Red = 0, Green = 63, Blue = 0;
        break;
      case 2:
        Red = 0, Green = 0, Blue = 31;
        break;
      case 3:
        Red = 31, Green = 63, Blue = 31;
        break;
    }
    if(j < 4){
      //文字列を１列一気に表示させる
      ssd1351.SSD1351_8x16_DisplayOut_1col_LtoR(0, j*16, Red, Green, Blue, sj_length, font_buf);
    }else{
      for(i=0; i<16; i++){
        if(ci<2){
          Red = 31; Green = 0; Blue = 0;
          ci++;
        }else if(ci<4){
          Red = 0; Green = 63; Blue = 0;
          ci++;
        }else if(ci<6){
          Red = 0; Green = 0; Blue = 31;
          ci++;
        }else if(ci<8){
          Red = 31; Green = 63; Blue = 0;
          ci++;
        }else if(ci<10){
          Red = 0; Green = 63; Blue = 31;
          ci++;
        }else if(ci<12){
          Red = 31; Green = 0; Blue = 31;
          ci = 0;
        }
        if(j < 6 ){
          //半角一文字ずつ表示させる
          ssd1351.SSD1351_8x16_DisplayOut(i*8, j*16, Red, Green, Blue, font_buf[i]);
        }else{
          if(i > 7) break;
          ssd1351.SSD1351_8x16_2x2_DisplayOut(i*16, 96, Red, Green, Blue, font_buf[i]);
        }
      }
    }
  }
  delay(2000);
  //RAM_Hscrolleはメインloop外でもRAM側で勝手にスクロールする
  ssd1351.SSD1351_RAM_Hscrolle(0, 128, 0, 1); //(uint8_t StartY, uint8_t EndY, uint8_t Interval, uint8_t direction)
  sclTime = millis();
  contrastTime = millis();
}

void loop() {
  if(millis()-contrastTime > 5){
    switch(R_ctr){
      case 254:
        R_ud = 1;
        break;
      case 1:
        R_ud =0;
        break;
    }
    switch(G_ctr){
      case 254:
        G_ud = 1;
        break;
      case 1:
        G_ud =0;
        break;
    }
    switch(B_ctr){
      case 254:
        B_ud = 1;
        break;
      case 1:
        B_ud =0;
        break;
    }
    if(R_ud == 1){
      R_ctr--;
    }else{
      R_ctr++;
    }
    if(G_ud == 1){
      G_ctr--;
    }else{
      G_ctr++;
    }
    if(B_ud == 1){
      B_ctr--;
    }else{
      B_ctr++;
    }
    ssd1351.SSD1351_RGBcontrast(R_ctr, G_ctr, B_ctr);//OLEDのコントラスト調整関数
    contrastTime = millis();
  }
  
  if(millis()-sclTime > 100){
    if(scl==127) scl=0;
    //RAM_VscrolleはHscrolleと異なり、自動ではスクロールしないので動作プログラムを組む必要がある。
    ssd1351.SSD1351_RAM_Vscrolle(scl++, 1); //(uint8_t StartY, uint8_t OffsetY)
    sclTime = millis();
  }
}
