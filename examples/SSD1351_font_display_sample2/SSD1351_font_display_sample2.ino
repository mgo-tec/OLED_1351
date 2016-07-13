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

char* cc[8] = {"フリー日本語漢字","東雲(ｼﾉﾉﾒ)ﾌｫﾝﾄ★","16x16ﾄﾞｯﾄ です。","全角-半角混在OK!","半角ｶﾀｶﾅもOK！！","記号●◆▲♪◎☆","16bit カラーOLED","複雑漢字(鰺鰤鰻)"};

uint8_t sj_txt[32]; //Shift_JISコード
uint16_t sj_length; //Shift_JISコードの長さ
uint8_t font_buf[32][16]; //東雲フォントデータ

const uint8_t sclk=14;
const uint8_t  mosi = 13;
const uint8_t  cs = 15;
const uint8_t DCpin =  4;
const uint8_t RSTpin =  5;

void setup() {
  Serial.begin(115200);
  ssd1351.SSD1351_Init(sclk, mosi, cs, DCpin, RSTpin); //SSD1351を初期化
  delay(300);
  ssd1351.SSD1351_BlackOut(); //黒画面出力

  uint8_t Red, Green, Blue; //Max Red = 31, Max Green = 63, MaxBlue = 31
  int8_t i,j;
  int8_t ci=0;

  Red = 31;
  Green = 63;
  Blue = 31;

  for(j=0; j<8; j++){
    u8ts.UTF8_to_SJIS_str_cnv(UTF8SJIS_file, cc[j], sj_txt, &sj_length); //UTF8コードをShift_JISコードに変換
    SFR.SjisToShinonome16FontRead_ALL(ZenkakuFontFile, HalfFontFile, 0, 0, sj_txt, sj_length, font_buf); //S_JISコードから東雲フォント抽出
      //文字列を１列一気に表示させる
    ssd1351.SSD1351_8x16_DisplayOut_1col_LtoR(0, j*16, Red, Green, Blue, sj_length, font_buf);
 
  }
}

void loop() {

}
