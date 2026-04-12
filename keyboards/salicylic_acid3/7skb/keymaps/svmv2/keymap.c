#include <stdint.h>
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
#define TRAINING_LOG 0

#define MAX_ACTIVE_TH 8
#define SVM_BUF_SIZE 32
#define TAPPING_TOGGLE_TERM 200 // 何ms以内の再タップでリピート判定にするか
#define HOLD_RELEASE_DELAY 30 // Hold→Releaseに移行するまでの猶予時間（ms）
#define MIN_WAIT_TIME 50 // 待機状態とみなす最低時間（ms）
#define TAP_NON_OVERLAPPED false // 重なりがない連続したキー入力を即時にタップとみなす

typedef struct {
    int32_t w_x;
    int32_t w_y;
    int32_t b;
    uint16_t guard;
} svm_config_t;

svm_config_t get_svm_params(uint16_t tap_kc) {
    switch (tap_kc) {
        case KC_SPC: case KC_BSPC: case KC_ENT: case KC_MINUS:
            return (svm_config_t){.w_x=1000, .w_y=-290, .b=-108677, .guard=250};
        case KC_EQUAL:
            return (svm_config_t){.w_x=1000, .w_y=-559, .b=-63593, .guard=250};
        case KC_SEMICOLON:
            return (svm_config_t){.w_x=1000, .w_y=-175, .b=-127712, .guard=250};
        case KC_G: case KC_H:
            return (svm_config_t){.w_x=1000, .w_y=0, .b=-180000, .guard=180};
        case KC_A: case KC_S: case KC_D: case KC_F:
        case KC_J: case KC_K: case KC_L:
            return (svm_config_t){.w_x=1000, .w_y=0, .b=-250000, .guard=250};
        default:
            return (svm_config_t){.w_x=1000, .w_y=0, .b=-200000, .guard=200};
    }
}

typedef struct {
    uint16_t keycode;
    uint16_t timer;

    uint16_t x;
    uint16_t y;
    uint16_t timeout;
    bool     is_hold;

    bool     active;
    bool     combined;
    enum { ST_WAITING, ST_HOLDING, ST_RELEASING } state;
} th_instance_t;

static th_instance_t th_instances[MAX_ACTIVE_TH];
static keyrecord_t event_buffer[SVM_BUF_SIZE];
static uint8_t buf_count = 0;
static bool is_replaying = false;

#if TAPPING_TOGGLE_TERM > 0
typedef struct {
    uint16_t last_tap_keycode;    // 最後にTapされたキー
    uint16_t last_tap_time;       // 最後にTapされた時間
    uint16_t repeating_source_kc; // 現在リピート押しっぱなしにしている元のキー
    bool     is_repeating;        // リピート状態のフラグ
} tap_repeat_t;

static tap_repeat_t tt_state = {0}; // ゼロ初期化
#endif

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

#if TAPPING_TOGGLE_TERM > 0
        tt_state.last_tap_keycode = inst->keycode;
        tt_state.last_tap_time = timer_read();
#endif

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

#if TAPPING_TOGGLE_TERM > 0
    // 1. リピート中のキーが離された時の処理
    if (!record->event.pressed && tt_state.is_repeating && keycode == tt_state.repeating_source_kc) {
        unregister_code16(keycode & 0xFF);
        // 連続リピート可能にするため、フラグを倒して時間を更新
        tt_state.is_repeating = false;
        tt_state.last_tap_keycode = keycode;
        tt_state.last_tap_time = timer_read();
        return false;
    }
    // 2. 違うキーが押されたら状態を全リセット
    if (record->event.pressed && keycode != tt_state.last_tap_keycode) {
        tt_state = (tap_repeat_t){0}; // 構造体を一発で安全に初期化
    }
#endif

    // -- 1. event: other key pressed
    if (record->event.pressed) {
        for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
            th_instance_t *inst = &th_instances[i];
            if (inst->active && inst->keycode != keycode) {
                if (inst->state == ST_WAITING || inst->state == ST_HOLDING) {
                    inst->combined = true;

                    if (inst->state == ST_WAITING) {
                        inst->y = timer_elapsed(inst->timer);
                    }
                }
            }
        }
    }

    // -- 2. リリース処理 --
    if (!record->event.pressed) {
        for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
            th_instance_t *inst = &th_instances[i];
            if (inst->active && inst->keycode == keycode) {
                uint16_t tap_kc = keycode & 0xFF;

                if (inst->state == ST_WAITING) {
                    inst->x = timer_elapsed(inst->timer);
                }
                else if (inst->state == ST_HOLDING) {
                    if (!inst->combined) {
                        // 【Retro Tap】単独長押しなのでここで即座に離す
                        execute_dynamic_hold(keycode, false);
                        register_code16(tap_kc);
                        inst->active = false;
#if TAPPING_TOGGLE_TERM > 0
                        tt_state.last_tap_keycode = inst->keycode;
                        uint16_t actual_time = inst->timer;
                        if (inst->x != 0xFFFF) {
                            actual_time += inst->x;
                        }
                        tt_state.last_tap_time = actual_time;
#endif
                        replay_buffer();
                        unregister_code16(tap_kc);
                    } else {
                        // 【コンボ後】ここでは離さず、余韻フェーズへ送る！
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

#if TAPPING_TOGGLE_TERM > 0
    // 【修正】スペースキー等、Tapの直後にHoldとして多用するキーはリピート連打の対象から外す
    uint16_t tap_kc = keycode & 0xFF;
    if (tap_kc != KC_SPC) { // ← KC_SPC は除外
        if (keycode == tt_state.last_tap_keycode && timer_elapsed(tt_state.last_tap_time) < TAPPING_TOGGLE_TERM) {
            if (DEBUG_SVM) uprintf("SVM: Repeat Tap detected! [Key:0x%04X]\n", keycode);
            tt_state.is_repeating = true;
            tt_state.repeating_source_kc = keycode;
            register_code16(tap_kc);
            return false;
        }
    }
#endif

        for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
            if (!th_instances[i].active) {
                svm_config_t params = get_svm_params(keycode & 0xFF);
                th_instances[i] = (th_instance_t){
                    .keycode = keycode,
                    .timer = timer_read(),
                    .x = 0xFFFF,
                    .y = 0xFFFF,
                    .timeout = params.guard,
                    .is_hold = true,
                    .active = true,
                    .combined = false,
                    .state = ST_WAITING,
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

int32_t get_svm_score(uint16_t tap_kc, uint16_t x, uint16_t y) {
    svm_config_t params = get_svm_params(tap_kc);

    // 1. SVMのスコア
    int32_t svm_score = params.w_x * x + params.w_y * y + params.b;

#if TAP_NON_OVERLAPPED
    // 2. 物理的なオーバーラップスコア (x > y ならプラス)
    int32_t overlap_score = (int32_t)x - (int32_t)y;

    // 3. 2つの式の最小値を取る (両方 >0 の場合のみプラスになる)
    return (svm_score < overlap_score) ? svm_score : overlap_score;
#else
    return svm_score;
#endif
}

#if TRAINING_LOG > 0
#include "print.h"

// 前回のマトリックス状態を保持する配列
static matrix_row_t previous_matrix[MATRIX_ROWS] = {0};

void training_log(void) {
    // 全ての行（Row）をスキャン
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        // 現在の行の物理的な状態（ビットマスク）を取得
        matrix_row_t current_row = matrix_get_row(r);

        // 前回との状態の差分（XOR）を計算
        matrix_row_t changed_bits = current_row ^ previous_matrix[r];

        // いずれかのキーに変化があった場合のみ処理
        if (changed_bits) {
            // 現在のマイコンの起動からの経過時間（ミリ秒・32bit）を取得
            uint32_t now = timer_read32();

            // どの列（Col）に変化があったかを特定
            for (uint8_t c = 0; c < MATRIX_COLS; c++) {
                // その列のビットが変化していた場合
                if (changed_bits & (1 << c)) {
                    // 現在のビットが立っていればPush、落ちていればRelease
                    bool is_pressed = (current_row & (1 << c)) != 0;

                    // Python等でパースしやすいフォーマットでコンソール出力
                    // 例: [00123456] R1,C2: Push
                    uprintf("[%08lu] R%d,C%d: %s\n", now, r, c, is_pressed ? "Push" : "Release");
                }
            }
            // 状態を更新
            previous_matrix[r] = current_row;
        }
    }
}
#endif

void matrix_scan_user(void) {
#if TRAINING_LOG > 0
    training_log();
#endif

    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
        if (!th_instances[i].active) continue;

        th_instance_t *inst = &th_instances[i];

        if (inst->state == ST_WAITING) {
            uint16_t t = timer_elapsed(inst->timer);
            uint16_t tap_kc = inst->keycode & 0xFF;
            svm_config_t params = get_svm_params(tap_kc);
            bool settle_now = false;

            // 1. 両方確定している場合 (即時決着)
            if (inst->x != 0xFFFF && inst->y != 0xFFFF) {
                int32_t score = get_svm_score(tap_kc, inst->x, inst->y);
                inst->is_hold = (score > 0);
                settle_now = true;
            }
            // 2. Xのみ確定 (Release後、未コンボ)
            else if (inst->x != 0xFFFF) {
#if TAP_NON_OVERLAPPED
                inst->timeout = MIN_WAIT_TIME;
#else
                if (params.w_y == 0) {
                    inst->timeout = MIN_WAIT_TIME;
                } else {
                    int32_t timeout_svm = -(params.w_x * inst->x + params.b) / params.w_y;
                    inst->timeout = (timeout_svm < 0) ? MIN_WAIT_TIME : (timeout_svm < params.guard ? timeout_svm : params.guard);
                    inst->timeout = (inst->timeout > MIN_WAIT_TIME) ? inst->timeout : MIN_WAIT_TIME;
                }
#endif
                inst->is_hold = false;
                if (t >= inst->timeout) settle_now = true;
            }
            // 3. Yのみ確定 (Press後、未リリース)
            else if (inst->y != 0xFFFF) {
                if (params.w_x == 0) {
                    inst->timeout = MIN_WAIT_TIME;
                } else {
                    int32_t timeout_svm = -(params.w_y * inst->y + params.b) / params.w_x;
                    inst->timeout = (timeout_svm < 0) ? MIN_WAIT_TIME : (timeout_svm < params.guard ? timeout_svm : params.guard);
                    inst->timeout = (inst->timeout > MIN_WAIT_TIME) ? inst->timeout : MIN_WAIT_TIME;
                }
                inst->is_hold = true;
                if (t >= inst->timeout) settle_now = true;
            }
            // 4. 何も起きていない (単なる待機)
            else {
                if (t >= inst->timeout) settle_now = true;
            }

            // --- 確定の執行 ---
            if (settle_now) {
                settle_instance(i, inst->is_hold);
                if (DEBUG_SVM) uprintf("SVM: Timeout! Settled as %s [Key:0x%04X, X:%dms, Y:%dms]\n", inst->is_hold ? "HOLD" : "TAP", inst->keycode, inst->x, inst->y);

                // 【最大のスタック原因の解消】
                // Holdとして確定したが、すでに物理的な指が離されている場合！
                if (inst->is_hold && inst->x != 0xFFFF) {
                    // Releaseイベントはもう来ないので、強制的に余韻フェーズへ送る
                    inst->state = ST_RELEASING;
                    inst->timer = timer_read();
                }
            }
        }
        else if (inst->state == ST_RELEASING) {
            if (timer_elapsed(inst->timer) > HOLD_RELEASE_DELAY) {
                execute_dynamic_hold(inst->keycode, false);
                inst->active = false;
            }
        }
    }
}
