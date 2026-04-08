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

// --- デバッグログのON/OFFスイッチ (1:ON, 0:OFF) ---
#define DEBUG_SVM 0

#define MAX_ACTIVE_TH 8
#define SVM_BUF_SIZE 32
#define MAX_COMBO_COUNT 2 // n文字以上打たれたらHold確定

typedef struct {
    int32_t w_x;
    int32_t w_y;
    int32_t b;
    uint16_t guard;
} svm_config_t;

svm_config_t get_svm_params(uint16_t tap_kc) {
    switch (tap_kc) {
        case KC_SPC: case KC_BSPC: case KC_ENT:
        case KC_SEMICOLON: case KC_EQUAL: case KC_MINUS:
            return (svm_config_t){1710, -338, -217148, 250};
        case KC_G: case KC_H:
            return (svm_config_t){1000, 0, -180000, 180};
        case KC_A: case KC_S: case KC_D: case KC_F:
        case KC_J: case KC_K: case KC_L:
            return (svm_config_t){1000, 0, -250000, 250};
        default:
            return (svm_config_t){1000, 0, -200000, 200};
    }
}

typedef struct {
    uint16_t keycode;
    uint16_t timer;
    uint16_t interval;
    bool     active;
    bool     combined;
    uint8_t  combo_count;
    enum { ST_WAITING, ST_HOLDING, ST_RELEASING } state;
} th_instance_t;

static th_instance_t th_instances[MAX_ACTIVE_TH];
static keyrecord_t event_buffer[SVM_BUF_SIZE];
static uint8_t buf_count = 0;
static bool is_replaying = false;

// 【新規追加】指定したキーの「押下（Press）」がバッファ内に存在するかチェックする
bool is_press_buffered(uint8_t col, uint8_t row) {
    for (uint8_t i = 0; i < buf_count; i++) {
        if (event_buffer[i].event.key.col == col &&
            event_buffer[i].event.key.row == row &&
            event_buffer[i].event.pressed) {
            return true;
        }
    }
    return false;
}

bool is_any_th_waiting(void) {
    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++)
        if (th_instances[i].active && th_instances[i].state == ST_WAITING) return true;
    return false;
}

void replay_buffer(void) {
    if (is_replaying || is_any_th_waiting()) return;
    is_replaying = true;
    if (buf_count > 0 && DEBUG_SVM) {
        uprintf("SVM: Replaying %d events\n", buf_count);
    }
    for (uint8_t i = 0; i < buf_count; i++) {
        if (DEBUG_SVM) uprintf("SVM:   Replay Event [Col:%d Row:%d], Pressed:%d\n",
                               event_buffer[i].event.key.col,
                               event_buffer[i].event.key.row,
                               event_buffer[i].event.pressed);
        process_record(&event_buffer[i]);
    }
    buf_count = 0;
    is_replaying = false;
}

void execute_dynamic_hold(uint16_t keycode, bool pressed) {
    if (IS_QK_MOD_TAP(keycode)) {
        uint8_t mod_code = (keycode >> 8) & 0x1F;
        bool is_right = mod_code & 0x10;

        if (mod_code & 0x01) { if (pressed) register_code16(is_right ? KC_RCTL : KC_LCTL); else unregister_code16(is_right ? KC_RCTL : KC_LCTL); }
        if (mod_code & 0x02) { if (pressed) register_code16(is_right ? KC_RSFT : KC_LSFT); else unregister_code16(is_right ? KC_RSFT : KC_LSFT); }
        if (mod_code & 0x04) { if (pressed) register_code16(is_right ? KC_RALT : KC_LALT); else unregister_code16(is_right ? KC_RALT : KC_LALT); }
        if (mod_code & 0x08) { if (pressed) register_code16(is_right ? KC_RGUI : KC_LGUI); else unregister_code16(is_right ? KC_RGUI : KC_LGUI); }
    } else if (IS_QK_LAYER_TAP(keycode)) {
        uint8_t layer = (keycode >> 8) & 0x0F;
        if (pressed) layer_on(layer); else layer_off(layer);
    }
}

void settle_instance(uint8_t inst_idx, bool as_hold) {
    th_instance_t *inst = &th_instances[inst_idx];
    uint16_t tap_kc = inst->keycode & 0xFF;

    if (as_hold) {
        inst->state = ST_HOLDING;
        execute_dynamic_hold(inst->keycode, true);
        if (DEBUG_SVM) uprintf("SVM: Fixed as HOLD [Key:0x%04X, TapKC:0x%02X]\n", inst->keycode, tap_kc);
        replay_buffer();
    } else {
        register_code16(tap_kc);
        inst->active = false;
        if (DEBUG_SVM) uprintf("SVM: Fixed as TAP [Key:0x%04X, TapKC:0x%02X]\n", inst->keycode, tap_kc);
        replay_buffer();
        unregister_code16(tap_kc);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (is_replaying) {
        if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
            uint16_t tap_kc = keycode & 0xFF;
            if (record->event.pressed) register_code16(tap_kc);
            else unregister_code16(tap_kc);
            return false;
        }
        return true;
    }

    // -- 1. フラグ更新 --
    if (record->event.pressed) {
        for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
            if (th_instances[i].active && th_instances[i].keycode != keycode) {
                if (th_instances[i].state == ST_WAITING || th_instances[i].state == ST_HOLDING) {
                    th_instances[i].combined = true;
                    th_instances[i].combo_count++;

                    if (th_instances[i].state == ST_WAITING && th_instances[i].interval == 0) {
                        uint16_t y = timer_elapsed(th_instances[i].timer);
                        // early settlement
                        th_instances[i].interval = y;
                        if (DEBUG_SVM) uprintf("SVM: Y locked at %u ms for Key:0x%04X\n", y, th_instances[i].keycode);

                        svm_config_t params = get_svm_params(th_instances[i].keycode & 0xFF);
                        int32_t early_score = params.w_x * y + params.w_y * y + params.b;
                        if (early_score > 0) {
                            if (DEBUG_SVM) uprintf("SVM: Early Settle! Y=%u is long enough for HOLD.\n", y);
                            settle_instance(i, true);
                            continue;
                        }

                        // hold with n combo: n文字以上打たれたら問答無用でHold確定
                        if (th_instances[i].combo_count >= MAX_COMBO_COUNT) {
                            settle_instance(i, true);
                        }
                    }
                }
            }
        }
    }

    // -- 2. リリース処理 --
    if (!record->event.pressed) {
        for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
            if (th_instances[i].active && th_instances[i].keycode == keycode) {
                th_instance_t *inst = &th_instances[i];
                uint16_t tap_kc = keycode & 0xFF;
                svm_config_t params = get_svm_params(tap_kc);

                if (inst->state == ST_WAITING) {
                    uint16_t x = timer_elapsed(inst->timer);
                    uint16_t y = (inst->interval > 0) ? inst->interval : x;
                    int32_t score = params.w_x * x + params.w_y * y + params.b;

                    if (score > 0) {
                        if (!inst->combined) {
                            register_code16(tap_kc);
                            inst->active = false;
                            replay_buffer();
                            unregister_code16(tap_kc);
                        } else {
                            settle_instance(i, true);
                            inst->state = ST_RELEASING;
                            inst->timer = timer_read();
                        }
                    } else {
                        settle_instance(i, false);
                    }
                } else if (inst->state == ST_HOLDING) {
                    if (!inst->combined) {
                        execute_dynamic_hold(keycode, false);
                        register_code16(tap_kc);
                        inst->active = false;
                        replay_buffer();
                        unregister_code16(tap_kc);
                    } else {
                        inst->state = ST_RELEASING;
                        inst->timer = timer_read();
                    }
                }
                return false;
            }
        }
    }

    // --- 3. バッファリング ---
    if (is_any_th_waiting()) {
        // 【究極の修正】Pressイベント、またはバッファ内にPressが存在するReleaseイベントのみバッファリングする
        if (record->event.pressed || is_press_buffered(record->event.key.col, record->event.key.row)) {
            if (buf_count < SVM_BUF_SIZE) {
                event_buffer[buf_count++] = *record;
                if (DEBUG_SVM) uprintf("SVM: Buffered Col:%d Row:%d Pressed:%d (count=%d)\n", record->event.key.col, record->event.key.row, record->event.pressed, buf_count);
                return false;
            } else {
                if (DEBUG_SVM) uprintf("SVM: Buffer overflow! Bypassing Key\n");
            }
        } else {
            // バッファに関係ないキーのリリースは即座に通してOSのキーリピート暴発を防ぐ
            if (DEBUG_SVM) uprintf("SVM: Bypassed buffer for Release Col:%d Row:%d\n", record->event.key.col, record->event.key.row);
        }
    }

    // -- 4. 新規T&Hフック --
    if (record->event.pressed && (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode))) {
        for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
            if (!th_instances[i].active) {
                th_instances[i] = (th_instance_t){
                    .keycode = keycode, .timer = timer_read(),
                    .interval = 0, .active = true, .combined = false, .state = ST_WAITING,
                    .combo_count = 0
                };
                if (DEBUG_SVM) uprintf("SVM: START WAITING Key:0x%04X\n", keycode);
                return false;
            }
        }
    }

    // -- 5. T&Hキーのフォールバック・リリース処理 --
    if (!record->event.pressed && (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode))) {
        uint16_t tap_kc = keycode & 0xFF;
        unregister_code16(tap_kc);
        return false;
    }

    return true;
}

void matrix_scan_user(void) {
    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
        if (!th_instances[i].active) continue;

        th_instance_t *inst = &th_instances[i];
        if (inst->state == ST_WAITING) {
            uint16_t x = timer_elapsed(inst->timer);
            uint16_t y = (inst->interval > 0) ? inst->interval : 0; // 初期値はx切片を閾値としてhold時間のみで縮退して判別
            svm_config_t params = get_svm_params(inst->keycode & 0xFF);
            int32_t score = params.w_x * x + params.w_y * y + params.b;

            // スコアが0を超えた（250ms経過した）瞬間にHold確定
            if (score > 0) {
                if (DEBUG_SVM) uprintf("SVM: Threshold reached! HOLD [Key:0x%04X]\n", inst->keycode);
                settle_instance(i, true);
            }
            // 2. 【最大待ち時間のフェイルセーフ】SVMスコアが満たされなくても、guardを超えたら強制Hold
            else if (x > params.guard && !inst->combined) {
                if (DEBUG_SVM) uprintf("SVM: Guard timeout! Force HOLD [Key:0x%04X]\n", inst->keycode);
                settle_instance(i, true);
            }
        }
        // (RELEASINGフェーズの処理はそのまま)
        else if (inst->state == ST_RELEASING) {
            if (timer_elapsed(inst->timer) > 30) {
                execute_dynamic_hold(inst->keycode, false);
                inst->active = false;
            }
        }
    }
}
