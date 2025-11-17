#pragma once

#include "stm32f4xx_hal.h"

namespace LED {
    constexpr uint16_t PIN = GPIO_PIN_5;
    static GPIO_TypeDef* GPIO_PORT = GPIOA;
    inline void GPIO_CLK_ENABLE() { __HAL_RCC_GPIOA_CLK_ENABLE(); }
}