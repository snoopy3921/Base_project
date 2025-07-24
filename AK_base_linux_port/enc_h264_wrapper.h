#include <stdint.h>
#include <stdlib.h>

#include "codec_def.h"
#include "codec_api.h"
#include "codec_app_def.h"
#include "extern.h"
#include "typedefs.h"

#include "memory_align.h"
#include "picture_handle.h"
#include "customEncoder.h"

#define ENABLE_H264_ENCODER 1


#if ENABLE_H264_ENCODER
// Constants
#define WIDTH     320
#define HEIGHT    320
#define FRAMERATE 20
#define BITRATE   400000

// Static buffers (no dynamic allocation)
static uint8_t yBuffer[WIDTH * HEIGHT];
static uint8_t uBuffer[(WIDTH / 2) * (HEIGHT / 2)];
static uint8_t vBuffer[(WIDTH / 2) * (HEIGHT / 2)];

extern void ReadYUVFrame(uint8_t* y, uint8_t* u, uint8_t* v);
extern void SendEncodedData(uint8_t* data, int len);
extern void EncoderTaskInternal();

// Example a wrapper h264 API encoder


#endif // end of ENABLE_H264_ENCODER