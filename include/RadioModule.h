#ifndef __RADIO_MODULE_H__
#define __RADIO_MODULE_H__

#include <Arduino.h>
#include "Controller.h"
#include "sx_pro.h"
#include "frame.h"
class RadioModule: public Controller {
    private: 
      void readParam(char *param, SXPro mod);
      Stream *radioModulePort;

    public:
      RadioModule(Stream &radioModule);
      void setup();
      void update();
      bool checkRadio();
      void setMode();
      void sendMessage(uint8_t* msg, uint16_t len);
      void sendFrame(rf_frame frame);
};

#endif