#include "Controller.h"
#include <FlexCAN_T4.h>
#include <stdint.h>

class CANBusController: public Controller {
    public:
        void setup();
        void update();
        CANBusController(int baudrate);
    private:
        uint32_t baudrate;
        void _handleMessage(CAN_message_t &msg);
};

class CANMessage {
    public:
        CANMessage(CAN_message_t);
};
