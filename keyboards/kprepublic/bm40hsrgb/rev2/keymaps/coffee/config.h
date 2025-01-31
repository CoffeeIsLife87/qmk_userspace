// Copyright 2024 Silas Fankhauser (@CoffeeIsLife87)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define LAYER_STATE_8BIT

// This doesn't work in rules.mk because it isn't ever processed.
#define TURBO_ENABLE
#define NUMBERPAD_ENABLE

#ifdef TURBO_ENABLE
#   ifndef TAP_DANCE_ENABLE
#       error Turbo uses tap dance for some of its keys. Please set TAP_DANCE_ENABLE = yes in rules.mk
#       undef TURBO_ENABLE
#   endif // TAP_DANCE_ENABLE
#endif // TURBO_ENABLE

#ifdef NUMBERPAD_ENABLE
#   ifndef TAP_DANCE_ENABLE
#       error Numberpad uses tap dance for some of its keys. Please set TAP_DANCE_ENABLE = yes in rules.mk 
#       undef NUMBERPAD_ENABLE
#   endif // TAP_DANCE_ENABLE
#endif // NUMBERPAD_ENABLE

// RGB Matrix effects disable
#ifdef RGB_MATRIX_ENABLE
#    define UNDERGLOW_DISABLE

#   ifdef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#   undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#   endif

#    define RGB_TRIGGER_ON_KEYDOWN

#    define RGB_MATRIX_LED_FLUSH_LIMIT 10

#    undef ENABLE_RGB_MATRIX_SOLID_COLOR
#    undef ENABLE_RGB_MATRIX_ALPHAS_MODS
#    undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#    undef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#    undef ENABLE_RGB_MATRIX_BREATHING
#    undef ENABLE_RGB_MATRIX_BAND_SAT
#    undef ENABLE_RGB_MATRIX_BAND_VAL
#    undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#    undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#    undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#    undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL

#    undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#    undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#    undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#    undef ENABLE_RGB_MATRIX_DUAL_BEACON
#    undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
#    undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#    undef ENABLE_RGB_MATRIX_RAINDROPS
#    undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#    undef ENABLE_RGB_MATRIX_HUE_BREATHING
#    undef ENABLE_RGB_MATRIX_HUE_PENDULUM
#    undef ENABLE_RGB_MATRIX_HUE_WAVE
#    undef ENABLE_RGB_MATRIX_PIXEL_RAIN
#    undef ENABLE_RGB_MATRIX_PIXEL_FLOW
#    undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL

#    undef ENABLE_RGB_MATRIX_TYPING_HEATMAP
#    undef ENABLE_RGB_MATRIX_DIGITAL_RAIN

#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#    undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#    undef ENABLE_RGB_MATRIX_SPLASH
#    undef ENABLE_RGB_MATRIX_SOLID_SPLASH
#    undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#endif

// If RGBLight is enabled
#ifdef RGBLIGHT_ENABLE
//#    define RGBLED_NUM RGB_MATRIX_LED_COUNT

#    define RGBLIGHT_EFFECT_SNAKE
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#endif

// Feature disable
#ifndef NO_PRINT
#    define NO_PRINT
#endif

#ifndef NO_DEBUG
#    define NO_DEBUG
#endif

#ifndef NO_ACTION_ONESHOT
#    define NO_ACTION_ONESHOT
#endif

// Configure features
#ifdef DYNAMIC_MACRO_ENABLE
#    define DYNAMIC_MACRO_USER_CALL
#    define DYNAMIC_MACRO_NO_NESTING
#endif

#ifdef MOUSEKEY_ENABLE
#    define MOUSEKEY_INTERVAL 16 // 60 FPS

// Speed up the slowest preset
#    define MK_C_OFFSET_0 4
#    define MK_C_INTERVAL_0 16

// Speed up the medium preset
#    define MK_C_OFFSET_1 16

#    define MK_3_SPEED
#endif