#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "RTE_Components.h"
#include CMSIS_device_header

#include "device_cfg.h"
#include "device_definition.h"
#include "arm_mps3_io_drv.h"

#include "arm_mps3_io_reg_map.h"

#include "os_tick.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#include "hdlcd_helper.h"

#include "arm_vsi.h"
#include "video_drv.h"
#ifdef __cplusplus
}
#endif

#define ENABLE_H264_ENCODER 1

#if ENABLE_H264_ENCODER
#include "enc_h264_wrapper.h"
#define ENCODER_TASK_NAME           ("SwEncoder")
#define ENCODER_TASK_STACK_SIZE     (configMINIMAL_STACK_SIZE)
#define ENCODER_TASK_PRIORITY       (tskIDLE_PRIORITY + 1)
#endif
#define VIDEO_STREAMING_TASK_NAME       ("VideoStreaming")
#define VIDEO_STREAMING_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define VIDEO_STREAMING_TASK_PRIORITY   (tskIDLE_PRIORITY + 2)

#define HDLCD_TASK_NAME             ("HDLCD")
#define HDLCD_TASK_STACK_SIZE       (configMINIMAL_STACK_SIZE)
#define HDLCD_TASK_PRIORITY         (tskIDLE_PRIORITY + 1)




#if ENABLE_H264_ENCODER
static void encoder_h264_task(void *pvParameters)
{
    printf("[Enter Func] encoder_h264_task for testing!!!!\r\n");
    (void) pvParameters;    
    int count_frame = 0;

    for(;;)
    {
        /* Block task for USER_LED_TOGGLE_PERIOD_MS. */
        vTaskDelay(5000);
        
        if(count_frame++ % 15 == 0) {
            printf("[encoder_h264_task] have a trigger msg encoder yuv-frame[%d]\r\n",count_frame/15);
            //EncoderTaskInternal();
        } else {
            printf("[encoder_h264_task] is running but nothing doing\r\n");
        }
    }
}
#endif


#define IMAGE_WIDTH (192U)
#define IMAGE_HEIGHT (192U)
#define FRAME_RATE (30U)
#define CHANNELS_IMAGE_DISPLAYED (3U)
#define IMAGE_DATA_SIZE (IMAGE_WIDTH*IMAGE_HEIGHT*CHANNELS_IMAGE_DISPLAYED)

static uint8_t ImageBuf[IMAGE_DATA_SIZE];   // Buffer for holding an input frame


static void video_streaming_task(void *pvParameters)
{
    (void) pvParameters;
    if (VideoDrv_Initialize(NULL) == VIDEO_DRV_OK) {
        printf("[OK] Init video driver\n");
    }
    /* Configure video driver for input */
    if (VideoDrv_Configure(VIDEO_DRV_IN0,  IMAGE_WIDTH, IMAGE_HEIGHT, VIDEO_DRV_COLOR_BGR565, FRAME_RATE) == VIDEO_DRV_OK) {
        printf("[OK] Configure video input\n");
    }
    /* Set input video buffer */
    if (VideoDrv_SetBuf(VIDEO_DRV_IN0,  ImageBuf, IMAGE_DATA_SIZE) == VIDEO_DRV_OK) {
        printf("[OK] Set buffer for video input\n");
    }
    /* Start video capture */
    if (VideoDrv_StreamStart(VIDEO_DRV_IN0, VIDEO_DRV_MODE_CONTINUOS) == VIDEO_DRV_OK) {
        printf("[OK] Start frame capture\n");
    }
    for(;;)
    {
        vTaskDelay(0xffffffff); 
    }
}

static void hdlcd_task(void *pvParameters)
{
    (void) pvParameters;
    hdlcd_helper_init();
    for(;;)
    {
        vTaskDelay(5);
        hdlcd_show((uint32_t)ImageBuf, IMAGE_WIDTH, IMAGE_HEIGHT, HDLCD_PIXEL_FORMAT_RGB565);
    }
}


extern "C" int stdout_init();
int main()
{
    //__enable_irq();
    stdout_init();

    BaseType_t retval;

    /* Create the RTOS tasks */
#if ENABLE_H264_ENCODER
    retval = xTaskCreate(encoder_h264_task, ENCODER_TASK_NAME, ENCODER_TASK_STACK_SIZE, NULL, ENCODER_TASK_PRIORITY, NULL );
#endif
    retval = xTaskCreate(video_streaming_task, VIDEO_STREAMING_TASK_NAME, VIDEO_STREAMING_TASK_STACK_SIZE, NULL, VIDEO_STREAMING_TASK_PRIORITY, NULL );
    retval = xTaskCreate(hdlcd_task, HDLCD_TASK_NAME, HDLCD_TASK_STACK_SIZE, NULL, HDLCD_TASK_PRIORITY, NULL );

    /* Start the scheduler */
    vTaskStartScheduler();

    while (1) {

        for(int i = 0; i < 1000000; i++);
        
        //printf("Main function running...\n");
        // Add your application logic here.
    }
}