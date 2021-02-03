#include "Controller.h"
#include <stdint.h>

class SdCardController: public Controller {
    public:
        void setup();
        void update();

        void write(File *);
        void read(File *);
        void close(File *);
        File * openFile(string);
};