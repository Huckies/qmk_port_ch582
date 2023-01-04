/*
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

#pragma once

//* platform settings
// #define DEBUG        Debug_UART1
#define DCDC_ENABLE  1
#define FREQ_SYS     40000000
#define LSE_ENABLE   0
#define BLE_SLOT_NUM 4
// #define HSE_LOAD_CAPACITANCE 7.5 // in pF unit
// #define LSE_LOAD_CAPACITANCE 7   // in pF unit
// #define I2C_IO_REMAPPING
// #define SPI_IO_REMAPPING

/* USB Device descriptor parameter */
#define VENDOR_ID    0xCAFE
#define PRODUCT_ID   0x0B91
#define DEVICE_VER   0x0001
#define MANUFACTURER mk67lite
#define PRODUCT      mk67lite

#define QMK_VERSION    "0.0.1"
#define QMK_BUILDDATE  __DATE__
#define QMK_KEYBOARD_H "mk67lite.h"

#define MATRIX_ROWS 5
#define MATRIX_COLS 15
#define MATRIX_ROW_PINS      \
    {                        \
        B9, A7, B8, B16, B18 \
    }
#define MATRIX_COL_PINS                                              \
    {                                                                \
        A4, A5, A6, A0, A1, A3, B7, B5, B4, B3, B2, B1, B0, B14, B15 \
    }
#define DIODE_DIRECTION       COL2ROW
#define BOOTMAGIC_LITE_ROW    0
#define BOOTMAGIC_LITE_COLUMN 0
// #define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS

#ifdef ENCODER_ENABLE
#define ENCODERS_PAD_A \
    {                  \
        A10            \
    }
#define ENCODERS_PAD_B \
    {                  \
        A11            \
    }
#define ENCODER_RESOLUTION 4
#endif

#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP FALSE

#define WS2812_EN_PIN   B6
#define WS2812_EN_LEVEL 1

#define BATTERY_MEASURE_PIN A2
// #define POWER_DETECT_PIN    B12

#ifdef RGBLIGHT_ENABLE
#define RGBLED_NUM        17
#define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_RGB
// #define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_CHRISTMAS
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
// #define RGBLIGHT_EFFECT_RGB_TEST
#define RGBLIGHT_EFFECT_ALTERNATING
#define RGBLIGHT_EFFECT_TWINKLE
#define RGBLIGHT_LIMIT_VAL   128
#define RGBLIGHT_HUE_STEP    20
#define RGBLIGHT_SAT_STEP    20
#define RGBLIGHT_VAL_STEP    20
#define RGBLIGHT_DEFAULT_HUE 191
#define RGBLIGHT_DEFAULT_SAT 255
#define RGBLIGHT_DEFAULT_VAL 10
#define RGBLIGHT_DEFAULT_SPD 1
#endif

#ifdef RGB_MATRIX_ENABLE
#ifdef WS2812_DRIVER_PWM
#define WS2812_PWM_DRIVER 1
#endif
#define RGBLED_NUM                    67
#define DRIVER_LED_TOTAL              RGBLED_NUM
#define RGB_MATRIX_LED_COUNT          DRIVER_LED_TOTAL
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 32
#define RGB_MATRIX_STARTUP_VAL        RGB_MATRIX_MAXIMUM_BRIGHTNESS
#define RGB_MATRIX_HUE_STEP           10
#define RGB_MATRIX_SAT_STEP           8
#define RGB_MATRIX_VAL_STEP           4
#define RGB_MATRIX_SPD_STEP           10
#define RGB_DISABLE_WHEN_USB_SUSPENDED
#define ENABLE_RGB_MATRIX_ALPHAS_MODS
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_BREATHING
#define ENABLE_RGB_MATRIX_BAND_SAT
#define ENABLE_RGB_MATRIX_BAND_VAL
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#define ENABLE_RGB_MATRIX_CYCLE_ALL
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#define ENABLE_RGB_MATRIX_DUAL_BEACON
#define ENABLE_RGB_MATRIX_RAINBOW_BEACON
#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#define ENABLE_RGB_MATRIX_RAINDROPS
#define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#define ENABLE_RGB_MATRIX_HUE_BREATHING
#define ENABLE_RGB_MATRIX_HUE_PENDULUM
#define ENABLE_RGB_MATRIX_HUE_WAVE
#define ENABLE_RGB_MATRIX_PIXEL_RAIN
#define ENABLE_RGB_MATRIX_PIXEL_FLOW
#define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#endif

#ifdef AW20216
#define DRIVER_1_CS A12
#define DRIVER_2_CS A11
#define DRIVER_1_EN A10
#define DRIVER_2_EN A10

#define DRIVER_COUNT       2
#define DRIVER_1_LED_TOTAL 17
#define DRIVER_2_LED_TOTAL 0
#define DRIVER_LED_TOTAL   (DRIVER_1_LED_TOTAL + DRIVER_2_LED_TOTAL)
#endif

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 10

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

#include "pre_handler.h"