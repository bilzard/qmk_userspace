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

#define DEBUG_SVM 1
#define MAX_ACTIVE_TH 8
#define SVM_BUF_SIZE 32

typedef struct {
    int32_t w_x;
    int32_t w_y;
    int32_t b;
    uint16_t guard;
} svm_config_t;

svm_config_t get_svm_params(uint16_t tap_kc) {
    switch (tap_kc) {
        case KC_SPC: case KC_BSPC: case KC_ENT:
            return (svm_config_t){1710, -338, -217148, 250};
        // ホームポジション用の緩い設定（ロールオーバー対策）
        case KC_A: case KC_S: case KC_D: case KC_F:
        case KC_J: case KC_K: case KC_L: case KC_SCLN:
            return (svm_config_t){1710, -338, -350000, 250};
        default:
            return (svm_config_t){1710, -338, -250000, 250};
    }
}

typedef struct {
    uint16_t keycode;
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

bool is_any_th_waiting(void) {
    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++)
        if (th_instances[i].active && th_instances[i].state == ST_WAITING) return true;
    return false;
}

void replay_buffer(void) {
    if (is_replaying || is_any_th_waiting()) return;
    is_replaying = true;
    if (buf_count > 0 && DEBUG_SVM) uprintf("SVM: Replaying %d events\n", buf_count);
    for (uint8_t i = 0; i < buf_count; i++) process_record(&event_buffer[i]);
    buf_count = 0;
    is_replaying = false;
}

// 【修正1】修飾キーの送信をQMK標準の register_code16 に変更（OSとの通信を確実にする）
void execute_dynamic_hold(uint16_t keycode, bool pressed) {
    if (IS_QK_MOD_TAP(keycode)) {
        uint8_t mod_code = (keycode >> 8) & 0x1F;
        bool is_right = mod_code & 0x10; // 右側の修飾キーか判定

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
        if (DEBUG_SVM) uprintf("SVM: Fixed as HOLD [Key:0x%04X]\n", inst->keycode);
    } else {
        tap_code16(tap_kc);
        inst->active = false;
        if (DEBUG_SVM) uprintf("SVM: Fixed as TAP [Key:0x%04X]\n", inst->keycode);
    }
    replay_buffer();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (is_replaying) return true;

    if (record->event.pressed) {
        for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
            if (th_instances[i].active && th_instances[i].keycode != keycode) {
                if (th_instances[i].state == ST_WAITING || th_instances[i].state == ST_HOLDING) {
                    th_instances[i].combined = true;
                    if (th_instances[i].state == ST_WAITING && th_instances[i].interval == 0) {
                        uint16_t y = timer_elapsed(th_instances[i].timer);
                        th_instances[i].interval = y;
                        if (DEBUG_SVM) uprintf("SVM: Y locked at %u ms for Key:0x%04X\n", y, th_instances[i].keycode);

                        // 【修正2】アーリーセトル（早期確定）：間隔(Y)が十分に長いなら、即座にHold確定！
                        svm_config_t params = get_svm_params(th_instances[i].keycode & 0xFF);
                        int32_t early_score = params.w_x * y + params.w_y * y + params.b;
                        if (early_score > 0) {
                            if (DEBUG_SVM) uprintf("SVM: Early Settle! Y=%u is long enough for HOLD.\n", y);
                            settle_instance(i, true);
                        }
                    }
                }
            }
        }
    }

    if (!record->event.pressed) {
        for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
            if (th_instances[i].active && th_instances[i].keycode == keycode) {
                th_instance_t *inst = &th_instances[i];
                uint16_t tap_kc = keycode & 0xFF;

                if (inst->state == ST_WAITING) {
                    uint16_t x = timer_elapsed(inst->timer);
                    uint16_t y = (inst->interval > 0) ? inst->interval : x;
                    svm_config_t params = get_svm_params(tap_kc);
                    int32_t score = params.w_x * x + params.w_y * y + params.b;

                    if (DEBUG_SVM) uprintf("SVM: Released. Score=%ld, X:%u, Y:%u, Combined:%d\n", (long)score, x, y, inst->combined);

                    if (score > 0) {
                        if (!inst->combined) {
                            tap_code16(tap_kc);
                            inst->active = false;
                            replay_buffer();
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
                        tap_code16(tap_kc);
                        inst->active = false;
                    } else {
                        inst->state = ST_RELEASING;
                        inst->timer = timer_read();
                    }
                }
                return false;
            }
        }
    }

    if (is_any_th_waiting()) {
        if (buf_count < SVM_BUF_SIZE) {
            event_buffer[buf_count++] = *record;
            return false;
        }
    }

    if (record->event.pressed && (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode))) {
        for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
            if (!th_instances[i].active) {
                th_instances[i] = (th_instance_t){
                    .keycode = keycode, .timer = timer_read(),
                    .interval = 0, .active = true, .combined = false, .state = ST_WAITING
                };
                if (DEBUG_SVM) uprintf("SVM: START WAITING Key:0x%04X\n", keycode);
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
            svm_config_t params = get_svm_params(th_instances[i].keycode & 0xFF);
            if (timer_elapsed(th_instances[i].timer) > params.guard) {
                if (DEBUG_SVM) uprintf("SVM: Guard Timeout! Forcing HOLD [Key:0x%04X]\n", th_instances[i].keycode);
                settle_instance(i, true);
            }
        } else if (th_instances[i].state == ST_RELEASING) {
            // 【修正3】OSが確実に取りこぼさないよう、リリースまでの猶予を30msに延長
            if (timer_elapsed(th_instances[i].timer) > 30) {
                execute_dynamic_hold(th_instances[i].keycode, false);
                th_instances[i].active = false;
                if (DEBUG_SVM) uprintf("SVM: Hold Released cleanly [Key:0x%04X]\n", th_instances[i].keycode);
            }
        }
    }
}
