#ifndef TTS_MODULE_H
#define TTS_MODULE_H

#include "UTF8ToGB2312.h"

class TTSModule {
public:
  TTSModule();
  void init();
  
  void speak(String data);

private:
  int workstate();

};

#endif
