#include <cstdint>
#include "main.h"

class LEDController {
private:
    static void initGPIO() {
        LED::GPIO_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin = LED::PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(LED::GPIO_PORT, &GPIO_InitStruct);
    }

public:
    static void init() {
        initGPIO();
    }

    static void toggle() {
        HAL_GPIO_TogglePin(LED::GPIO_PORT, LED::PIN);
    }
};

extern "C" {
    void SysTick_Handler(void) {
        HAL_IncTick();
    }
}

int main() {
    HAL_Init();
    LEDController::init();

    while (true) {
        LEDController::toggle();
        HAL_Delay(1000);
    }

    return 0;
}