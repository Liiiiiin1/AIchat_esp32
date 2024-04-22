#include "TTS.h"
#define RXD2 16
#define TXD2 17

TTSModule::TTSModule(){
}

void TTSModule::init() {
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  //speak("OK了老铁们");
  }

void TTSModule::speak(String data) {
 
  Serial.println("Speaking: " + data);
  String utf8_str = data;
  String gb2312_str = GB.get(utf8_str);
  unsigned char head[gb2312_str.length()+6];
  unsigned char gb2312_data[gb2312_str.length() + 1];
  memset(gb2312_data, 0, sizeof(gb2312_data));
  strncpy((char*)gb2312_data, gb2312_str.c_str(), gb2312_str.length());
  head[0] = 0xFD;
  unsigned int dat_len = gb2312_str.length() + 3;
  head[1] = dat_len >> 8;
  head[2] = dat_len;
  head[3] = 0x01;
  head[4] = 0x01;
  for (int i = 0; i < gb2312_str.length(); i++) {
    head[i + 5] = gb2312_data[i];
  }
  head[gb2312_str.length() + 5] = head[0];
  for (int i = 1; i < gb2312_str.length() + 5; i++) {
    head[gb2312_str.length() + 5] ^= head[i];
  }
  for (int j = 0; j < gb2312_str.length() + 6; j++) {
    Serial2.write(head[j]);
  }
}

int TTSModule::workstate() {
  unsigned char head[4];
  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x02;
  head[3] = 0x21;
  for (int j = 0; j < 4; j++) {
    Serial2.write(head[j]);
  }
  while (Serial2.available() < 1) {
    delay(150);
  }
  byte response = Serial2.read();
  if (response == 0x4E) {
    return 1;
  } else if (response == 0x4F) {
    return 0;
  } else {
    return 2;
  }
}
