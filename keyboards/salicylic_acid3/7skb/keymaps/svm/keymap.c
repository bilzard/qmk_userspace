#include QMK_KEYBOARD_H


#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// 1. レイヤーの定義を5枚（0-4）に合わせる
enum layer_number {
  _QWERTY = 0,
  _LAYER1,  // 1: 記号
  _LAYER2,  // 2: 数字
  _LAYER3,  // 3: ショートカット
  _LAYER4,  // 4: Fキー（新設用）
};

enum custom_keycodes {
    // --- 1. RemapのGUIから操作したいカスタムキー (USER枠を使う) ---
    MY_SPC = 0x7E00, // Kb 0に割り当てるスペースキー（T&H判定の対象）

    // --- 2. GUI操作は不要だが、コード内で定義が必要なもの (SAFE_RANGEを使う) ---
    // RGB_RST は SAFE_RANGE のあとに自動で連番が振られるようにする
    RGB_RST = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT(
  //,-----------------------------------------------------|   |--------------------------------------------------------------------------------.
       KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,        KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,  KC_EQL, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_LBRC, KC_RBRC,KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------|
     KC_LCTL,     KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,  KC_ENT,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT, MO(_LAYER1),
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
               KC_LALT, KC_LGUI,  KC_SPC,  KC_SPC,               KC_SPC,  KC_SPC,          KC_RGUI, KC_RALT
          //`---------------------------------------------|   |--------------------------------------------'
  ),

  [_LAYER1] = LAYOUT(
  //,-----------------------------------------------------|   |--------------------------------------------------------------------------------.
  TG(_LAYER3),   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,  KC_INS,  KC_DEL,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,     _______, _______, KC_PSCR, KC_SCRL,KC_PAUSE,   KC_UP, _______, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,     _______, _______, KC_HOME, KC_PGUP, KC_LEFT,KC_RIGHT, _______,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,     _______, _______,  KC_END, KC_PGDN, KC_DOWN, _______, _______,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
               _______, _______, _______, _______,              _______, _______,          KC_STOP, _______
          //`---------------------------------------------|   |--------------------------------------------'
  ),

  [_LAYER3] = LAYOUT( /* Base */
  //,-----------------------------------------------------|   |--------------------------------------------------------------------------------.
  TG(_LAYER3), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   QK_BOOT,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     RGB_RST, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     UG_TOGG, UG_NEXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     UG_VALD, UG_VALU, UG_HUED, UG_HUEU, UG_SATD, UG_SATU, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,              XXXXXXX, XXXXXXX,          KC_STOP, XXXXXXX
          //`---------------------------------------------|   |--------------------------------------------'
  )
};

//A description for expressing the layer position in LED mode.
layer_state_t layer_state_set_user(layer_state_t state) {
#ifdef RGBLIGHT_ENABLE
    // 現在の最高位レイヤーを取得
    uint8_t layer = get_highest_layer(state);

    switch (layer) {
    case _LAYER1:
      rgblight_sethsv_noeeprom(HSV_BLUE);   // FNレイヤーは青
      break;
    case _LAYER2:
      rgblight_sethsv_noeeprom(HSV_PURPLE); // ADJUSTレイヤーは紫
      break;
    case _LAYER3:
      rgblight_sethsv_noeeprom(HSV_GREEN);  // FKEYレイヤーは緑
      break;
    case _LAYER4:
      rgblight_sethsv_noeeprom(HSV_YELLOW); // LAYER4は黄色
      break;
    default: // QWERTYレイヤー（デフォルト）
      rgblight_sethsv_noeeprom(0, 0, 0);    // 消灯
      break;
    }
#endif
return state;
}

#include "print.h"

#define MAX_ACTIVE_TH 4
#define SVM_BUF_SIZE 16

typedef struct {
    uint16_t tap_kc;
    uint16_t hold_kc;
    int32_t w_x;
    int32_t w_y;
    int32_t b;
    uint16_t guard;
} svm_config_t;

static const svm_config_t svm_table[] = {
    [0] = {KC_SPC,  KC_RSFT, 1710, -338, -217148, 250},
    [1] = {KC_EQL,  KC_LSFT, 1710, -338, -217148, 250},
    [2] = {KC_BSPC, MO(1),   1710, -338, -217148, 250},
};

typedef struct {
    uint16_t keycode;
    uint8_t  idx;
    uint16_t timer;
    uint16_t interval;
    bool     active;
    bool     combined;
    enum { ST_WAITING, ST_HOLDING, ST_RELEASING } state;
} th_instance_t;

static th_instance_t th_instances[MAX_ACTIVE_TH];
static keyrecord_t event_buffer[SVM_BUF_SIZE];
static uint8_t buf_count = 0;
static bool is_replaying = false;

bool is_any_th_active(void) {
    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
        if (th_instances[i].active && th_instances[i].state != ST_RELEASING) return true;
    }
    return false;
}

bool is_any_th_waiting(void) {
    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
        if (th_instances[i].active && th_instances[i].state == ST_WAITING) return true;
    }
    return false;
}

void replay_buffer(void) {
    if (is_replaying || is_any_th_waiting()) return;
    is_replaying = true;
    if (buf_count > 0) uprintf("SVM: Replaying %d buffered events\n", buf_count);
    for (uint8_t i = 0; i < buf_count; i++) {
        process_record(&event_buffer[i]);
    }
    buf_count = 0;
    is_replaying = false;
}

void execute_hold_action(uint16_t hold_kc, bool pressed) {
    if (hold_kc >= 0x5100 && hold_kc <= 0x51FF) {
        uint8_t layer = hold_kc & 0xFF;
        if (pressed) layer_on(layer); else layer_off(layer);
    } else {
        if (pressed) register_code16(hold_kc); else unregister_code16(hold_kc);
    }
}

void settle_instance(uint8_t inst_idx, bool as_hold) {
    th_instance_t *inst = &th_instances[inst_idx];
    const svm_config_t *conf = &svm_table[inst->idx];

    if (as_hold) {
        inst->state = ST_HOLDING;
        execute_hold_action(conf->hold_kc, true);
        uprintf("SVM [%d]: Settled as HOLD (Hold action executed)\n", inst->idx);
    } else {
        tap_code16(conf->tap_kc);
        inst->active = false;
        uprintf("SVM [%d]: Settled as TAP\n", inst->idx);
    }
    replay_buffer();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (is_replaying) return true;

    // --- 1. T&Hキー自体の処理 ---
    if (keycode >= 0x7E00 && keycode <= 0x7E0F) {
        uint8_t table_idx = keycode - 0x7E00;
        if (table_idx >= sizeof(svm_table)/sizeof(svm_table[0])) return true;

        if (record->event.pressed) {
            for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
                if (!th_instances[i].active) {
                    th_instances[i] = (th_instance_t){
                        .keycode = keycode, .idx = table_idx, .timer = timer_read(),
                        .interval = 0, .active = true, .combined = false, .state = ST_WAITING
                    };
                    uprintf("SVM [%d]: START WAITING\n", table_idx);
                    return false;
                }
            }
        } else {
            for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
                if (th_instances[i].active && th_instances[i].keycode == keycode) {
                    th_instance_t *inst = &th_instances[i];
                    uprintf("SVM [%d]: RELEASED (combined=%d, state=%d)\n", inst->idx, inst->combined, inst->state);

                    if (inst->state == ST_WAITING) {
                        uint16_t x = timer_elapsed(inst->timer);
                        uint16_t y = (inst->interval > 0) ? inst->interval : x;
                        int32_t score = svm_table[inst->idx].w_x * x + svm_table[inst->idx].w_y * y + svm_table[inst->idx].b;

                        uprintf("SVM [%d]: Calc Score=%ld (X:%u, Y:%u)\n", inst->idx, (long)score, x, y);

                        if (score > 0) {
                            if (!inst->combined) {
                                uprintf("SVM [%d]: Score>0 but NO combo. Retro Tapping!\n", inst->idx);
                                tap_code16(svm_table[inst->idx].tap_kc);
                                inst->active = false;
                            } else {
                                settle_instance(i, true);
                                inst->state = ST_RELEASING;
                                inst->timer = timer_read();
                                uprintf("SVM [%d]: -> RELEASING phase\n", inst->idx);
                            }
                        } else {
                            settle_instance(i, false);
                        }
                    } else if (inst->state == ST_HOLDING) {
                        if (!inst->combined) {
                            uprintf("SVM [%d]: Timeout Hold but NO combo. Retro Tapping!\n", inst->idx);
                            execute_hold_action(svm_table[inst->idx].hold_kc, false);
                            tap_code16(svm_table[inst->idx].tap_kc);
                            inst->active = false;
                        } else {
                            inst->state = ST_RELEASING;
                            inst->timer = timer_read();
                            uprintf("SVM [%d]: -> RELEASING phase\n", inst->idx);
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    // --- 2. 他のキーの処理 ---
    if (is_any_th_active()) {
        if (record->event.pressed) {
            for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
                if (th_instances[i].active && (th_instances[i].state == ST_WAITING || th_instances[i].state == ST_HOLDING)) {
                    th_instances[i].combined = true;
                    if (th_instances[i].state == ST_WAITING && th_instances[i].interval == 0) {
                        th_instances[i].interval = timer_elapsed(th_instances[i].timer);
                        uprintf("SVM [%d]: Interval(Y) locked at %u ms\n", th_instances[i].idx, th_instances[i].interval);
                    } else {
                        uprintf("SVM [%d]: Combined flag set to TRUE\n", th_instances[i].idx);
                    }
                }
            }
        }

        if (is_any_th_waiting()) {
            if (buf_count < SVM_BUF_SIZE) {
                event_buffer[buf_count++] = *record;
                uprintf("SVM: Buffered key 0x%04X (count=%d)\n", keycode, buf_count);
                return false;
            }
        }
    }

    return true;
}

void matrix_scan_user(void) {
    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
        if (!th_instances[i].active) continue;

        if (th_instances[i].state == ST_WAITING) {
            if (timer_elapsed(th_instances[i].timer) > svm_table[th_instances[i].idx].guard) {
                uprintf("SVM [%d]: Guard Timeout! Forcing HOLD.\n", th_instances[i].idx);
                settle_instance(i, true);
            }
        } else if (th_instances[i].state == ST_RELEASING) {
            if (timer_elapsed(th_instances[i].timer) > 5) {
                execute_hold_action(svm_table[th_instances[i].idx].hold_kc, false);
                th_instances[i].active = false;
                uprintf("SVM [%d]: Hold Released cleanly\n", th_instances[i].idx);
            }
        }
    }
}
