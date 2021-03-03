#include "frame.h"

// extern "C" {
// Converts a frame to a buffer.
// frame - frame to convert
// buf - buffer to put the frame into. buf should be large enough to accomadate the entire frame.
// a good size is the max frame size FRAME_MAX_LEN.
// buf_length - length of the buffer when done.
extern "C" void frame_to_buff(rf_frame frame, uint8_t *buf, uint16_t *buf_length) {
    buf[0] = START_OF_FRAME; // Start of frame.
    // Set id in buffer
    buf[1] = frame.flag;
    buf[2] = frame.id;
    // Insert data length
    buf[DATA_BEGIN_OFFSET - 1] = frame.data_length;
    // Insert data from frame into buffer.
    for (uint8_t i = 0; i < frame.data_length; i++) {
        buf[DATA_BEGIN_OFFSET + i] = frame.data[i];
    }
    *buf_length = DATA_BEGIN_OFFSET + frame.data_length + 1;
    buf[*buf_length - 1] =
        END_OF_FRAME; // put a newline at the end. This is the end of frame indicator.
}
// }