#include "main.h"

#include <stdbool.h>

#include "can.h"
#include "clock.h"
#include "gpio.h"
#include "i2c.h"
#include "interrupts.h"
#include "error_handler.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stm32g4xx_hal.h>


void heartbeat_task(void *pvParameters) {
	(void) pvParameters;
	while(true)
	{
//        if (!fake_CAN_send(3, 2, 0xf5aa)) error_handler();
        if (!core_CAN_send(CAN2, 3, 2, 0xf5aa)) error_handler();
		GPIO_toggle_heartbeat();
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

int main(void)
{
    HAL_Init();
	// Drivers
	if (!core_clock_init()) error_handler();
    if (!core_CAN_init(CAN2)) error_handler();

    heartbeat_init(GPIOB, GPIO_PIN_9);
    GPIO_set_heartbeat(GPIO_PIN_SET);

	int err = xTaskCreate(heartbeat_task, 
        "heartbeat", 
        1000,
        NULL,
        4,
        NULL);
    if (err != pdPASS) {
        error_handler();
    }

    // hand control over to FreeRTOS
    vTaskStartScheduler();

    // we should not get here ever
    error_handler();
}

// Called when stack overflows from rtos
// Not needed in header, since included in FreeRTOS-Kernel/include/task.h
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName)
{
	(void) xTask;
	(void) pcTaskName;

    error_handler();
}