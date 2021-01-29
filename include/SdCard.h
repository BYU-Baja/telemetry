#include "Controller.h"
#include <stdint.h>

class SdCardController: public Controller {
    public:
        void setup();
        void update();

        void write(FILE *);
        void read(FILE *);
        void close(FILE *);
        FILE * openFile(string);
};