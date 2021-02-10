#ifndef __CAN_BUS_CONTROLLER_H__
#define __CAN_BUS_CONTROLLER_H__

#include "Controller.h"
#include "MessageHandler.h"
#include <FlexCAN_T4.h>
#include <stdint.h>

class CANBusController: public Controller, public MessageHandler<CANMessage> {
    public:
        void setup();
        void update();
        template<typename T>
        void onMessage(void(*handler)(CANMessage));
        CANBusController(int baudrate);
    private:
        uint32_t baudrate;
        void _handleMessage(CAN_message_t &msg);
};

class CANMessage {
    public:
        CANMessage(CAN_message_t);
};

#endif
