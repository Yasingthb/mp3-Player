#include "Arduino.h"
#include "SoftwareSerial.h"
#include <Keypad.h>
char TUS; int SAYI;
const byte ROWS = 9;
const byte COLS = 3;
char keys[ROWS][COLS] = {
{'1','A','J'},
{'2','B','K'},
{'3','C','L'},
{'4','D','M'},
{'5','E','N'},
{'6','F','O'},
{'7','G','O'},
{'8','H','R'},
{'9','I','S'}};

byte colPins[COLS] = {2, 3, 4};
byte rowPins[ROWS] = {5, 6, 7, 8, 9, 14, 15, 12, 13}; 
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

SoftwareSerial Audio(10, 11); // RX, TX
void setup()
  {
  Serial.begin(9600);// Rs232 Portu
  Audio.begin(9600); // Ses Kartı 
  KOMUT(0x3F,0);     // 0:Initialize  
  KOMUT(0x06,30);    // 030:Volume
  KOMUT(0x07,1);     // 1-5:Equaliser
  KOMUT(0x11,1);     // 0:Repeat/1:PlayStop
  KOMUT(0x03,0);     // 0-2999:ilk Sarkı
  }

void loop()
  {
  TUS = kpd.getKey(); SAYI=0; // Matristen gelen CHAR bilgisi alınıyor, Sayı sıfırlanıyor
  switch (TUS)                // CHAR bilgi tablo karşılığı sayısal değere çevriliyor
    {
    case '1':SAYI= 1; break; case 'A':SAYI=10; break; case 'J':SAYI=19; break;
    case '2':SAYI= 2; break; case 'B':SAYI=11; break; case 'K':SAYI=20; break;
    case '3':SAYI= 3; break; case 'C':SAYI=12; break; case 'L':SAYI=21; break;
    case '4':SAYI= 4; break; case 'D':SAYI=13; break; case 'M':SAYI=22; break;
    case '5':SAYI= 5; break; case 'E':SAYI=14; break; case 'N':SAYI=23; break;
    case '6':SAYI= 6; break; case 'F':SAYI=15; break; case 'O':SAYI=24; break;
    case '7':SAYI= 7; break; case 'G':SAYI=16; break; case 'P':SAYI=25; break;
    case '8':SAYI= 8; break; case 'H':SAYI=17; break; case 'R':SAYI=26; break;
    case '9':SAYI= 9; break; case 'I':SAYI=18; break; case 'S':SAYI=27; break;
    }
  if (SAYI) { Serial.println(SAYI); KOMUT(0x03,SAYI); } // Sayısal değer Rs232 portuna ve Ses Kartına gönderiliyor
  }

void KOMUT(byte CMD, int DAT)
  {
  byte A=highByte(DAT); byte B=lowByte(DAT);      // 0-65535 (Max 2999) arası gelen değer iki byte haline getiriliyor 
  word CRC = -(0xFF + 0x06 + CMD + 0x00 + A + B); // iletişim protokolu haberleşme paket bütünlüğü hesaplanıyor
  byte H=highByte(CRC); byte L=lowByte(CRC);      // Hesaplanan paket değeri iki Byte haline getiriliyor
  byte DATA[10] = { 0x7E, 0xFF, 0x06, CMD, 0x00, A, B, H, L, 0xEF}; // Ses Kartına gönderilecek bilgi hazırlanıyor
  for (byte i=0; i<10; i++) { Audio.write(DATA[i]); }; delay(100);  // 10 adet bilgi Ses Kartına gönderiliyor
  }

// NOT: Ses kartı girişi için; RX bacagına giren uca 1K Seri, 
// RX bacağından GND'ye 2.2K Direnç takılınca parazit kalkıyor.
// ARDUINO TX (11) ----[2k2]---+---[1k]----- GND
//                             |              |
// SES KARTI                  RX             GND
