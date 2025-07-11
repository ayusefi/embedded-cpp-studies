#include <FreeRTOS.h>
#include <semphr.h>

SemaphoreHandle_t xConfigMutex;
int configValue = 0;

void setConfig(int value) {
    xSemaphoreTake(xConfigMutex, portMAX_DELAY);
    configValue = value;
    xSemaphoreGive(xConfigMutex);
}

void mainTask(void *pvParameters) {
    // Example usage
    setConfig(42);
    // ...other code...
}

int main() {
    xConfigMutex = xSemaphoreCreateMutex();
    // Create mainTask and start scheduler
    xTaskCreate(mainTask, "MainTask", 1000, NULL, 1, NULL);
    vTaskStartScheduler();
    // Should never reach here
    for(;;);
    return 0;
}