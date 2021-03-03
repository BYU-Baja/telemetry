#include <stdint.h>

#ifndef __FRAME__H__
#define __FRAME__H__

typedef uint8_t frame_id;

#define FRAME_MAX_DATA_LENGTH 256
#define FRAME_MAX_LEN 263 // 1 + max data length + length of id + 1(data length byte) => 1(start of frame) + 256 + 4 + 1 + 1(end of frame)
#define BYTES_PER_UINT_32 4
#define DATA_BEGIN_OFFSET 4
#define START_OF_FRAME 0x00
#define END_OF_FRAME 0xFF

// This is a single frame.
// The frame will be sent over the RF transceiver.
typedef struct
{
  uint8_t flag;
  frame_id id;
  uint8_t data_length;
  uint8_t data[FRAME_MAX_DATA_LENGTH] __attribute__((aligned(8)));
} rf_frame;

// Convert between uint32_t and a byte array
union uint32_uint8_converter
{
  uint32_t value;
  uint8_t data[BYTES_PER_UINT_32 - 1];
};
// Convert between float and a byte array
union float_uint8_converter
{
  float value;
  uint8_t data[BYTES_PER_UINT_32 - 1];
};

// Converts a frame to a buffer.
// frame - frame to convert
// buf - buffer to put the frame into. buf should be large enough to accommodate the entire frame.
// a good size is the max frame size FRAME_MAX_LEN.
// buf_length - length of the buffer when done.
extern "C" void frame_to_buff(rf_frame frame, uint8_t *buf, uint16_t *buf_length);

#endif