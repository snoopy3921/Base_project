/*
 * Copyright (c) 2023 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "device_definition.h"
#include "hdlcd_helper.h"

#include "hdlcd_drv.h"

/* RGB565 format
    Pixel information:
    <--NOT USED--> <--  RED   --> <-- GREEN --> <-- BLUE -->
    31 .. .. .. 16 15 .. .. .. 11 10 .. .. .. 5 4 .. .. .. 0
*/
static const struct hdlcd_pixel_cfg_t hdlcd_pixel_cfg_rgb565 = {.red.default_color = 0xff,
                                                                .red.bit_size = 0x5,
                                                                .red.offset = 0xb,
                                                                .green.default_color = 0x0,
                                                                .green.bit_size = 0x6,
                                                                .green.offset = 0x5,
                                                                .blue.default_color = 0x0,
                                                                .blue.bit_size = 0x5,
                                                                .blue.offset = 0x0};
/* RGB32 format */
static const struct hdlcd_pixel_cfg_t hdlcd_pixel_cfg_rgb32 = {.red.default_color = 0xff,
                                                               .red.bit_size = 0x8,
                                                               .red.offset = 0x10,
                                                               .green.default_color = 0xff,
                                                               .green.bit_size = 0x8,
                                                               .green.offset = 0x8,
                                                               .blue.default_color = 0xff,
                                                               .blue.bit_size = 0x8,
                                                               .blue.offset = 0x0};
/* A2R10G10B10 format. HDLCD only handles top 8 bits. */
static const struct hdlcd_pixel_cfg_t hdlcd_pixel_cfg_a2r10g10b10 = {.red.default_color = 0xff,
                                                                     .red.bit_size = 0x8,
                                                                     .red.offset = 0x16,
                                                                     .green.default_color = 0xff,
                                                                     .green.bit_size = 0x8,
                                                                     .green.offset = 0xc,
                                                                     .blue.default_color = 0xff,
                                                                     .blue.bit_size = 0x8,
                                                                     .blue.offset = 0x2};

const hdlcd_mode_t HDLCD_MODES[] = {{.pixel_format = BYTESPERPIXEL2 << PIXEL_FORMAT_BYTES_PER_PIXEL_Pos,
                                     .default_highlight_color = RGB565_YELLOW,
                                     .default_mask_color = RGB565_RED,
                                     .bytes_per_pixel = 2,
                                     .pixel_cfg = &hdlcd_pixel_cfg_rgb565},
                                    {.pixel_format = BYTESPERPIXEL4 << PIXEL_FORMAT_BYTES_PER_PIXEL_Pos,
                                     .default_highlight_color = RGB32_YELLOW,
                                     .default_mask_color = RGB32_RED,
                                     .bytes_per_pixel = 4,
                                     .pixel_cfg = &hdlcd_pixel_cfg_rgb32},
                                    {.pixel_format = BYTESPERPIXEL4 << PIXEL_FORMAT_BYTES_PER_PIXEL_Pos,
                                     .default_highlight_color = A2R10G10B10_YELLOW,
                                     .default_mask_color = A2R10G10B10_RED,
                                     .bytes_per_pixel = 4,
                                     .pixel_cfg = &hdlcd_pixel_cfg_a2r10g10b10}};

void hdlcd_helper_init(void){
    enum hdlcd_error_t hdlcd_err;

    printf("Starting HDLCD config!\r\n");

    hdlcd_err = hdlcd_init(&HDLCD_DEV);
    if (hdlcd_err != HDLCD_ERR_NONE) {
        printf("HDLCD init error! \r\n");
        while (1);
    }

    hdlcd_err = hdlcd_static_config(&HDLCD_DEV);
    if (hdlcd_err != HDLCD_ERR_NONE) {
        printf("HDLCD static config error! \r\n");
        while (1);
    }

    printf("Starting ISP test!\r\n");
}

void hdlcd_show(uint32_t address, uint32_t width, uint32_t height, uint32_t mode)
{
    struct hdlcd_resolution_cfg_t custom_resolution_cfg;

    enum hdlcd_error_t hdlcd_err = 0;
    enum hdlcd_pixel_format format = HDLCD_PIXEL_FORMAT_RGB565;

    //printf("Displaying image...\r\n");
    if (format == HDLCD_PIXEL_FORMAT_NOT_SUPPORTED) {
        printf("Unsupported pixel format: 0x%x\r\n", mode);
        return;
    }

    hdlcd_disable(&HDLCD_DEV);

    // TODO: Choose standard resolution
    // Note: This only works, because FVP ignores all timing values
    custom_resolution_cfg.v_data = height;
    custom_resolution_cfg.h_data = width;
    hdlcd_set_custom_resolution(&HDLCD_DEV, &custom_resolution_cfg);

    struct hdlcd_buffer_cfg_t hdlcd_buff = {.base_address = address,
                                            .line_length = width * HDLCD_MODES[format].bytes_per_pixel,
                                            .line_count = height - 1,
                                            .line_pitch = width * HDLCD_MODES[format].bytes_per_pixel,
                                            .pixel_format = HDLCD_MODES[format].pixel_format};

    hdlcd_err = hdlcd_buffer_config(&HDLCD_DEV, &hdlcd_buff);
    if (hdlcd_err != HDLCD_ERR_NONE) {
        printf("HDLCD buffer config error! \r\n");
        while (1)
            ;
    }
    hdlcd_err = hdlcd_pixel_config(&HDLCD_DEV, HDLCD_MODES[format].pixel_cfg);
    if (hdlcd_err != HDLCD_ERR_NONE) {
        printf("HDLCD pixel config error! \r\n");
        while (1)
            ;
    }

    hdlcd_enable(&HDLCD_DEV);

    //printf("Image displayed\r\n");
}
