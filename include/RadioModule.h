#include "Controller.h"
#include "sx_pro.h"

class RadioModule: public Controller {
    private: void readParam(char *param, SXPro mod);

    public:
      void setup();
      void update();

};