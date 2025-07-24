#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
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

#include "cmsis_vio.h"
#include "timeout.h"
}

#include "enc_h264_wrapper.h"

extern void vioSetValue(uint32_t id, int32_t value);
extern int32_t vioGetValue(uint32_t id);

/*******************************************************************************
* Macros
*******************************************************************************/
#if ENABLE_H264_ENCODER
#define ENCODER_TASK_NAME           ("SwEncoder")
#define ENCODER_TASK_STACK_SIZE     (configMINIMAL_STACK_SIZE)
#define ENCODER_TASK_PRIORITY       (tskIDLE_PRIORITY + 1)
#endif
#define BLINKY_TASK_NAME            ("Blinky")
#define BLINKY_TASK_STACK_SIZE      (configMINIMAL_STACK_SIZE)
#define BLINKY_TASK_PRIORITY        (tskIDLE_PRIORITY + 1)
#define MAIN_TASK_NAME              ("Main")
#define MAIN_TASK_STACK_SIZE        (configMINIMAL_STACK_SIZE)
#define MAIN_TASK_PRIORITY          (tskIDLE_PRIORITY + 1)

/* USER LED toggle period in milliseconds */
#define USER_LED_TOGGLE_PERIOD_MS   1000u


#if ENABLE_H264_ENCODER
static void encoder_h264_task(void *pvParameters)
{
    printf("[Enter Func] encoder_h264_task for testing!!!!\r\n");
    (void) pvParameters;    
    int count_frame = 0;

    for(;;)
    {
        /* Block task for USER_LED_TOGGLE_PERIOD_MS. */
        vTaskDelay(10);
        
        if(count_frame++ % 15 == 0) {
            printf("[encoder_h264_task] have a trigger msg encoder yuv-frame[%d]\r\n",count_frame/15);
            EncoderTaskInternal();
        } else {
            printf("[encoder_h264_task] is running but nothing doing\r\n");
        }
    }
}
#endif

static void blinky_task(void *pvParameters)
{
    (void) pvParameters;

    /* Initialize the User LED */
    struct arm_mps3_io_reg_map_t* p_mps3_io_port =
                                (struct arm_mps3_io_reg_map_t*)MPS3_IO_DEV.cfg->base; // MPS3 IO base address

    for(;;)
    {
        vTaskDelay(USER_LED_TOGGLE_PERIOD_MS);
        vioSetValue(0, rand());
        /* Toggle the USER LED state */
        p_mps3_io_port->fpgaio_leds ^= 1;
    }
}

static void main_task(void *pvParameters)
{
    (void) pvParameters;

    for(;;)
    {
        /* Block task for USER_LED_TOGGLE_PERIOD_MS. */
        vTaskDelay(10);
        char buffer[32] = {0};
        snprintf(buffer, sizeof(buffer), "Vio value is set to %d\r\n", vioGetValue(0));
        printf("%s", buffer);

    }
}

extern "C" int stdout_init();

int main() {
    //__enable_irq();
    stdout_init();
    printf("Bare metal hello!!!!\r\n");

    BaseType_t retval;


    /* Create the RTOS tasks */
#if ENABLE_H264_ENCODER
    retval = xTaskCreate(encoder_h264_task, ENCODER_TASK_NAME, ENCODER_TASK_STACK_SIZE, NULL, ENCODER_TASK_PRIORITY, NULL );
#endif
    retval = xTaskCreate(blinky_task, BLINKY_TASK_NAME, BLINKY_TASK_STACK_SIZE, NULL, BLINKY_TASK_PRIORITY, NULL );
    retval = xTaskCreate(main_task, MAIN_TASK_NAME, MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL );
    /* Start the scheduler */
    vTaskStartScheduler();
    for (;;) {

    }
}
