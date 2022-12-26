/*
Copyright 2022 Huckies <https://github.com/Huckies>
Copyright 2022 OctopusZ <https://github.com/OctopusZ>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "battery_measure.h"

#if defined BATTERY_MEASURE_PIN

__attribute__((weak)) const uint16_t battery_map[] = {
    2515, 2528, 2541, 2554, 2567, 2580, 2593, 2606, 2619, 2632,
    2645, 2658, 2672, 2685, 2698, 2711, 2724, 2737, 2750, 2763,
    2776, 2789, 2802, 2816, 2829, 2842, 2855, 2868, 2881, 2894,
    2907, 2920, 2933, 2946, 2960, 2973, 2986, 2999, 3012, 3025,
    3038, 3051, 3064, 3077, 3090, 3104, 3117, 3130, 3143, 3156,
    3169, 3182, 3195, 3208, 3221, 3234, 3248, 3261, 3274, 3287,
    3300, 3313, 3326, 3339, 3352, 3365, 3378, 3392, 3405, 3418,
    3431, 3444, 3457, 3470, 3483, 3496, 3509, 3522, 3536, 3549,
    3562, 3575, 3588, 3601, 3614, 3627, 3640, 3653, 3666, 3680,
    3693, 3706, 3719, 3732, 3745, 3758, 3771, 3784, 3797, 3810
};

static bool battery_indicator_enable = 0;
static uint16_t battery_indicator_timer = 0;

static inline void battery_config_channel(pin_t pin)
{
    switch (pin) {
        case A7:
            ADC_ChannelCfg(11);
            break;
        case A8:
            ADC_ChannelCfg(12);
            break;
        case A9:
            ADC_ChannelCfg(13);
            break;
        case A4:
            ADC_ChannelCfg(0);
            break;
        case A5:
            ADC_ChannelCfg(1);
            break;
        case A6:
            ADC_ChannelCfg(10);
            break;
        case A0:
            ADC_ChannelCfg(9);
            break;
        case A1:
            ADC_ChannelCfg(8);
            break;
        case A2:
            ADC_ChannelCfg(7);
            break;
        case A3:
            ADC_ChannelCfg(6);
            break;
        case A15:
            ADC_ChannelCfg(5);
            break;
        case A14:
            ADC_ChannelCfg(4);
            break;
        case A13:
            ADC_ChannelCfg(3);
            break;
        case A12:
            ADC_ChannelCfg(2);
            break;
    }
}

uint16_t battery_get_min()
{
    return battery_map[0];
}

uint16_t battery_get_max()
{
    return battery_map[BATTERY_MAP_SIZE - 1];
}

// just in case there is no gpio interrupt handler defined
__attribute__((weak)) __INTERRUPT __HIGH_CODE void GPIOA_IRQHandler()
{
    R16_PA_INT_IF = R16_PA_INT_IF;
    R16_PB_INT_IF = R16_PB_INT_IF;
}

__attribute__((weak)) __INTERRUPT __HIGH_CODE void GPIOB_IRQHandler()
{
    R16_PA_INT_IF = R16_PA_INT_IF;
    R16_PB_INT_IF = R16_PB_INT_IF;
}

__attribute__((noreturn)) __HIGH_CODE static void battery_handle_critical()
{
    uint8_t temp = RB_WAKE_EV_MODE;

#ifdef POWER_DETECT_PIN
    temp |= RB_SLP_GPIO_WAKE;
    if (POWER_DETECT_PIN & 0x80000000) {
        PFIC_EnableIRQ(GPIO_B_IRQn);
    } else {
        PFIC_EnableIRQ(GPIO_A_IRQn);
    }
    setPinInputLow(POWER_DETECT_PIN);
    setPinInterruptRisingEdge(POWER_DETECT_PIN);
#endif
    do {
        sys_safe_access_enable();
        R8_SLP_WAKE_CTRL = temp;
        sys_safe_access_disable();
    } while (R8_SLP_WAKE_CTRL != temp);

    uint8_t x32Kpw, x32Mpw;

    FLASH_ROM_SW_RESET();
    x32Kpw = R8_XT32K_TUNE;
    x32Mpw = R8_XT32M_TUNE;
    x32Mpw = (x32Mpw & 0xfc) | 0x03;     // 150%额定电流
    if (R16_RTC_CNT_32K > 0x3fff) {      // 超过500ms
        x32Kpw = (x32Kpw & 0xfc) | 0x01; // LSE驱动电流降低到额定电流
    }

    WAIT_FOR_DBG;

    sys_safe_access_enable();
    R8_BAT_DET_CTRL = 0;
    sys_safe_access_enable();
    R8_XT32K_TUNE = x32Kpw;
    R8_XT32M_TUNE = x32Mpw;
    sys_safe_access_disable();
    SetSysClock(CLK_SOURCE_HSE_6_4MHz);

    PFIC->SCTLR |= (1 << 2); //deep sleep

    sys_safe_access_enable();
    R8_SLP_POWER_CTRL = RB_RAM_RET_LV | 0x01;
    sys_safe_access_enable();
    R16_POWER_PLAN = RB_PWR_PLAN_EN | RB_PWR_MUST_0010;
    __WFI();
    __nop();
    __nop();
    FLASH_ROM_SW_RESET();
    sys_safe_access_enable();
    R8_RST_WDOG_CTRL |= RB_SOFTWARE_RESET;
    sys_safe_access_disable();

    __builtin_unreachable();
}

__attribute__((weak)) void battery_init()
{
    setPinInput(BATTERY_MEASURE_PIN);
    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_2);
}

__attribute__((weak)) uint16_t battery_measure()
{
    uint16_t abcBuff[15];
    int16_t RoughCalib_Value = ADC_DataCalib_Rough();

    battery_config_channel(BATTERY_MEASURE_PIN);
    for (uint8_t i = 0; i < 15; i++) {
        abcBuff[i] = ADC_ExcutSingleConver() + RoughCalib_Value;
    }
    R8_ADC_CFG &= ~(RB_ADC_BUF_EN | RB_ADC_POWER_ON);

    uint16_t adc_data = 0;

    for (uint8_t i = 0; i < 10; i++) {
        adc_data += abcBuff[i + 5];
    }

    return adc_data;
}

__attribute__((weak)) uint8_t battery_calculate(uint16_t adcVal)
{
    if ((adcVal < battery_map[0] * 10)
#ifdef POWER_DETECT_PIN
        && !readPin(POWER_DETECT_PIN)
#endif
    ) {
        PRINT("Battery level critical.\n");
        battery_handle_critical();
    }

    for (uint32_t i = 1; i < BATTERY_MAP_SIZE; i++) {
        if (adcVal < battery_map[i] * 10) {
            return (uint8_t)(i * 100 / BATTERY_MAP_SIZE);
        }
    }
    return 100;
}

void battery_indicator_toggle(bool status)
{
    if (status) {
        battery_indicator_enable = true;
        battery_indicator_timer = timer_read();
    } else {
        battery_indicator_enable = false;
    }
}

bool battery_indicator_state()
{
    return battery_indicator_enable;
}

bool battery_indicator_timeout()
{
    return (timer_elapsed(battery_indicator_timer) > BATTERY_INDICATOR_TIMEOUT);
}

#endif
