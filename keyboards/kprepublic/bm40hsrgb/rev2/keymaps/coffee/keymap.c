// Copyright 2024 Silas Fankhauser (@CoffeeIsLife87)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    NULLKEY = SAFE_RANGE,    // An empty key to start (and maybe end) the enum
    #ifdef DYNAMIC_MACRO_ENABLE
    MCR_PLY,             // Macro play
    MCR_REC,             // Macro record
    MCR_SWT,             // Swap active macro
    #endif // DYNAMIC_MACRO_ENABLE
    #ifdef TURBO_ENABLE
    TRBO_SS, // Turbo Start/Stop/Set
    TRBO_SU, // Turbo Speed Up
    TRBO_SD, // Turbo Speed Down
    #endif // TURBO_ENABLE
    #ifdef MOUSEKEY_ENABLE
    MS_ACL_U, // Accel up
    MS_ACL_D, // Accel Down
    #endif // MOUSEKEY_ENABLE
};

enum layout_names {
    _MAIN = 0,       // Keys Layout: The main keyboard layout that has all the characters
    _SUB,            // Extension to Main
    _CTR,            // Macros, RGB, Audio controls, layer access. More or less the control center of my keyboard
    #ifdef NUMBERPAD_ENABLE
    _NUM,
    #endif // NUMBERPAD_ENABLE
    #ifdef MOUSEKEY_ENABLE
    _MSE,
    #endif // MOUSEKEY_ENABLE
    _END,
};

#ifdef TAP_DANCE_ENABLE
enum tapdance_actions {
    TD_BLANK,
    //#ifdef TURBO_ENABLE
    //TD_TRBO_SSSK, // Turbo Start/Stop / Set Key
    //#endif // TURBO_ENABLE
    #ifdef NUMBERPAD_ENABLE
    TD_NM_PLMN, // Plus / Minus
    TD_NM_ASFS, // Asterisk / Forward Slash
    TD_NM_CMDT, // Comma / Dot
    TD_NM_PTHS, // Parenthesis
    #endif // NUMBERPAD_ENABLE
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_BLANK] = {{0}},
    //#ifdef TURBO_ENABLE
    //[TD_TRBO_SSSK] = ACTION_TAP_DANCE_DOUBLE(TRBO_SS, TRBO_SK),
    //#endif // TURBO_ENABLE
    #ifdef NUMBERPAD_ENABLE
    [TD_NM_PLMN] = ACTION_TAP_DANCE_DOUBLE(KC_PPLS, KC_PMNS),
    [TD_NM_ASFS] = ACTION_TAP_DANCE_DOUBLE(KC_PAST, KC_PSLS),
    [TD_NM_CMDT] = ACTION_TAP_DANCE_DOUBLE(KC_COMM, KC_DOT),
    [TD_NM_PTHS] = ACTION_TAP_DANCE_DOUBLE(LSFT(KC_9), LSFT(KC_0)),
    #endif // NUMBERPAD_ENABLE
};
#endif // TAP_DANCE_ENABLE

#ifdef TURBO_ENABLE
static bool turbo_active = false;
static bool turbo_assigning = false;
static uint16_t turbo_assign_timer = 0; // Takes the place of tap dance inputs since tap dance hates process_record_user :(
static uint8_t turbo_key = XXXXXXX;
static uint16_t turbo_tapping_timer = 0;
static uint16_t turbo_tapping_speeds[] = {1, 100, 200, 500, 1000};
const uint8_t turbo_tapping_range = ((sizeof(turbo_tapping_speeds) / sizeof(turbo_tapping_speeds[0])) - 1);
static uint16_t turbo_speed_index = 3;
#else // TURBO_ENABLE
#define TRBO_SU XXXXXXX
#define TRBO_SD XXXXXXX
#endif // TURBO_ENABLE

#ifdef NUMBERPAD_ENABLE
#define TOG_NUM TG(_NUM)
#define NM_PLMN TD(TD_NM_PLMN) // Plus / Minus 
#define NM_ASFS TD(TD_NM_ASFS) // Asterisk / Forward Slash
#define NM_CMDT TD(TD_NM_CMDT) // Comma / Dot
#define NM_PTHS TD(TD_NM_PTHS) // Parenthesis
bool num_lock_on = false;
#else
#define TOG_NUM XXXXXXX
#endif // NUMBERPAD_ENABLE

#ifdef MOUSEKEY_ENABLE
#include "action.h"
#define TOG_MSE TG(_MSE)
static int current_accel = 0;
#else
#define TOG_MSE XXXXXXX
#endif // MOUSEKEY_ENABLE

#ifdef DYNAMIC_MACRO_ENABLE
// Macro 1 is = 1, Macro 2 = -1, No macro = 0
static bool MACRO1 = true;
static bool RECORDING = false;

static uint16_t PLY = DM_PLY1;
static uint16_t REC = DM_REC1;

bool dynamic_macro_record_start_user(int8_t direction) {
    REC = DM_RSTP;
    RECORDING = true;
    return true;
}
bool dynamic_macro_record_end_user(int8_t direction) {
    RECORDING = false;
    return false;
}
#else
#define MCR_PLY KC_NO
#define MCR_REC KC_NO
#define MCR_SWT KC_NO
#endif // DYNAMIC_MACRO_ENABLE

const uint16_t PROGMEM keymaps[_END][MATRIX_ROWS][MATRIX_COLS] = {
    [_MAIN] = LAYOUT_planck_mit(
        KC_ESC , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
        KC_TAB , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_ENT ,
        KC_LCTL, KC_LGUI, XXXXXXX,KC_LALT ,MO(_SUB),      KC_SPC     ,MO(_CTR), KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT
    ),
    [_SUB] = LAYOUT_planck_mit(
        KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_DEL,
        XXXXXXX, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_MINS, KC_EQL , KC_LBRC, KC_RBRC, KC_PGUP,
        _______, KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , XXXXXXX, KC_HOME, KC_END , KC_BSLS, KC_PGDN,
        _______, _______, XXXXXXX, _______, _______,      _______    , _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_CTR] = LAYOUT_planck_mit(
        XXXXXXX, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, XXXXXXX, XXXXXXX, KC_VOLU, XXXXXXX, TRBO_SS, MCR_REC,
        XXXXXXX, RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, TRBO_SU, MCR_PLY,
        XXXXXXX, XXXXXXX, RGB_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD, XXXXXXX, TRBO_SD, MCR_SWT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TOG_NUM,      TOG_MSE,     _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    )
    #ifdef NUMBERPAD_ENABLE
    ,[_NUM] = LAYOUT_planck_mit(
        _______, XXXXXXX,  KC_P7 ,  KC_P8 ,  KC_P9, NM_CMDT , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
        _______, XXXXXXX,  KC_P4 ,  KC_P5 ,  KC_P6, NM_PLMN , XXXXXXX, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, XXXXXXX,
        _______, XXXXXXX,  KC_P1 ,  KC_P2 ,  KC_P3, NM_ASFS , XXXXXXX, NM_PTHS, KC_HOME, KC_END , KC_BSLS, _______,
        XXXXXXX, XXXXXXX, KC_PEQL,  KC_P0 , TOG_NUM,      XXXXXXX,     KC_NUM , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    )
    #endif // NUMBERPAD_ENABLE
    #ifdef MOUSEKEY_ENABLE
    ,[_MSE] = LAYOUT_planck_mit(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, XXXXXXX, XXXXXXX, KC_BTN1, KC_WH_D, KC_WH_U, KC_BTN2, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MS_ACL_D,      TOG_MSE,    MS_ACL_U,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    )
    #endif // MOUSEKEY_ENABLE
};

#define LAYER (get_highest_layer(layer_state))
#define LAYER_SIZE (MATRIX_ROWS * MATRIX_COLS)
#define CHECK_LED() \
    if ((i >= RGB_MATRIX_LED_COUNT) \
    || ((g_led_config.flags[pos] == LED_FLAG_NONE) || (g_led_config.flags[pos] == LED_FLAG_UNDERGLOW))) \
        continue

#ifdef RGB_MATRIX_ENABLE
#ifdef UNDERGLOW_DISABLE
void keyboard_pre_init_user(void) {
    for (int key_id = 0; key_id < RGB_MATRIX_LED_COUNT; key_id++ ) {
        if (g_led_config.flags[key_id] == LED_FLAG_UNDERGLOW) {
            g_led_config.flags[key_id] = LED_FLAG_NONE;
        }
    }
}
#endif

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (LAYER != _MAIN) {
        int DimmedMax = UINT8_MAX - (UINT8_MAX - rgb_matrix_config.hsv.v);

        for (uint8_t i = led_min; i <= led_max; i++) {
            uint8_t pos = ((uint8_t*)g_led_config.matrix_co)[i];

            CHECK_LED(); // Check LED before moving on
            uint16_t KC = pgm_read_word(&((uint16_t*)keymaps)[(LAYER_SIZE * LAYER) + i]);

            if (KC == KC_NO) {
                RGB_MATRIX_INDICATOR_SET_COLOR(pos, 0, 0, 0 );
            }

            #ifdef DYNAMIC_MACRO_ENABLE
            else if ((KC == MCR_SWT) && (!MACRO1)) {
                RGB_MATRIX_INDICATOR_SET_COLOR(pos, DimmedMax, DimmedMax, DimmedMax);
            } else if ((KC == MCR_REC) && (RECORDING)) {
                RGB_MATRIX_INDICATOR_SET_COLOR(pos, DimmedMax, 0, 0);
            }
            #endif // DYNAMIC_MACRO_ENABLE

            #ifdef TURBO_ENABLE
            else if ((KC == TRBO_SS) && turbo_active) {
                if (timer_elapsed(turbo_tapping_timer) < (turbo_tapping_speeds[turbo_speed_index] / 2)) {
                    RGB_MATRIX_INDICATOR_SET_COLOR(pos, DimmedMax, DimmedMax, DimmedMax);
                }
            }
            else if ((KC == TRBO_SS) && turbo_assigning) {
                RGB_MATRIX_INDICATOR_SET_COLOR(pos, DimmedMax, 0, 0);
            }
            #endif // TURBO_ENABLE

            #ifdef NUMBERPAD_ENABLE
            else if ((KC == KC_NUM) && (!num_lock_on)) {
                RGB_MATRIX_INDICATOR_SET_COLOR(pos, DimmedMax, DimmedMax, DimmedMax);
            }
            #endif // NUMBERPAD_ENABLE

        }
    }
    return false;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    #ifdef DYNAMIC_MACRO_ENABLE
        if (keycode == MCR_REC) keycode = REC;
        if (keycode == MCR_PLY) keycode = PLY;
        if (!process_dynamic_macro(keycode, record)) {
            REC = (MACRO1 ? DM_REC1 : DM_REC2);
            return false;
        }
    #endif // DYNAMIC_MACRO_ENABLE

    #ifdef TURBO_ENABLE
        if (record->event.pressed && turbo_assigning) {
            if (keycode == TRBO_SS) {
                turbo_assigning = false;
                return false;
            } else if (
                IS_BASIC_KEYCODE(keycode)    || 
                IS_SYSTEM_KEYCODE(keycode)   ||
                IS_CONSUMER_KEYCODE(keycode) ||
                IS_MOUSE_KEYCODE(keycode)    ||
                IS_MODIFIER_KEYCODE(keycode) ||
                IS_JOYSTICK_KEYCODE(keycode)
            ) {
                turbo_key = keycode;
                turbo_assigning = false;
                return false;
            }
        }

        if (record->event.pressed && (keycode == TRBO_SS)) {
            if ((turbo_assign_timer != 0) && (timer_elapsed(turbo_assign_timer) <= 200)) { // Second hit. Assigning now
                turbo_assigning = true;
                turbo_assign_timer = 0;
            }
            if (!turbo_assigning && (turbo_assign_timer == 0)) { // The first time we hit it. Or it's been past the ignore time since the past hit
                turbo_assign_timer = timer_read();
            }
            return false;
        }
    #endif // TURBO_ENABLE

    #ifdef MOUSEKEY_ENABLE
        if (keycode == MS_ACL_U || keycode == MS_ACL_D) {
            if (record->event.pressed) {
                if ( (keycode == MS_ACL_U) && (current_accel < 2) ) { current_accel += 1; }
                if ( (keycode == MS_ACL_D) && (current_accel > 0) ) { current_accel -= 1; }
            }
            keycode = KC_ACL0 + current_accel;
            action_t mousekey_action = action_for_keycode(keycode);
            process_action(record, mousekey_action);
        }
    #endif

    switch (keycode) {
        #ifdef DYNAMIC_MACRO_ENABLE
            case MCR_SWT:
                if (!RECORDING && record->event.pressed) {
                    // if the button is pressed and we're not recording
                    MACRO1 = !MACRO1;
                    if (MACRO1) {
                        REC = DM_REC1;
                        PLY = DM_PLY1;
                    } else {
                        REC = DM_REC2;
                        PLY = DM_PLY2;
                    }
                }
                return false;
        #endif

        #ifdef TURBO_ENABLE
            case TRBO_SU:
                if (record->event.pressed) {
                    if (turbo_speed_index > 0) {
                        turbo_speed_index -= 1;
                    }
                }
                return false;
            case TRBO_SD:
                if (record->event.pressed) {
                    if (turbo_speed_index < turbo_tapping_range) {
                        turbo_speed_index += 1;
                    }
                }
                return false;
        #endif // TURBO_ENABLE

        #if defined(RGB_MATRIX_ENABLE) && defined(RGBLIGHT_ENABLE) // this only needs to be defined if both are enabled
            case RGB_TOG: // We can intercept this keycode ig? Cool :)
                if (record->event.pressed) {
                    if (rgb_matrix_is_enabled()) {
                        rgb_matrix_disable/*_noeeprom*/();
                        rgblight_enable/*_noeeprom*/();
                    } else if (rgblight_is_enabled()) {
                        rgb_matrix_disable/*_noeeprom*/();
                        rgblight_disable/*_noeeprom*/();
                    } else {
                        rgb_matrix_enable/*_noeeprom*/();
                        rgblight_disable/*_noeeprom*/();
                    }
                }
                return false;
        #endif

        default:
            return true; //Process all other keycodes normally
    }
}

bool led_update_user(led_t led_state) {
    #ifdef NUMBERPAD_ENABLE
    num_lock_on = led_state.num_lock;
    #endif // NUMBERPAD_ENABLE
    return true;
}

void housekeeping_task_user() {
    #ifdef TURBO_ENABLE
    if ((turbo_assign_timer != 0) && (timer_elapsed(turbo_assign_timer) > 200)) {
        turbo_assign_timer = 0;
        turbo_active = !turbo_active;
        if (turbo_active) {
            if (turbo_key == KC_NO) {
                turbo_tapping_timer = 0;
                turbo_active = false;
            } else {
                turbo_tapping_timer = timer_read();
            }
        } else {
            turbo_tapping_timer = 0;
        }
    }

    if (turbo_active) {
        if (timer_elapsed(turbo_tapping_timer) >= turbo_tapping_speeds[turbo_speed_index]) {
            tap_code(turbo_key);
            turbo_tapping_timer = timer_read();
        }
    }
    #endif // TURBO_ENABLE
}