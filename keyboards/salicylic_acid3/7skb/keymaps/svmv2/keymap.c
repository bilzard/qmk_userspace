#include <stdint.h>
#include QMK_KEYBOARD_H

// ============================================================================
// Keymap Layers
// ============================================================================

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

#include "print.h"

// ============================================================================
// Configuration
// ============================================================================

// --- デバッグログのON/OFFスイッチ (1:ON, 0:OFF) ---
#define DEBUG_SVM 0
#define TRAINING_LOG 0
#define CROSS_HAND_CONSTRAINT 1 // クロスハンド制約の有効化フラグ

#define MAX_ACTIVE_TH 8
#define SVM_BUF_SIZE 32
#define TAPPING_TOGGLE_TERM 200 // 何ms以内の再タップでリピート判定にするか
#define HOLD_RELEASE_DELAY 30 // Hold→Releaseに移行するまでの猶予時間（ms）
#define MIN_WAIT_TIME 50 // 待機状態とみなす最低時間（ms）
#define TAP_NON_OVERLAPPED false // 重なりがない連続したキー入力を即時にタップとみなす

// ============================================================================
// SVM Parameters
// ============================================================================

typedef struct {
    int32_t w_x;
    int32_t w_y;
    int32_t b;
    uint16_t guard;
} svm_config_t;

svm_config_t get_svm_params(uint16_t tap_kc) {
    switch (tap_kc) {
        case KC_TAB: case KC_BSPC: case KC_SPC: case KC_ENT:
            return (svm_config_t){.w_x=1000, .w_y=-290, .b=-108677, .guard=250};
        case KC_SEMICOLON: case KC_Z:
            return (svm_config_t){.w_x=1000, .w_y=-175, .b=-127712, .guard=250};
        case KC_A: case KC_S: case KC_D: case KC_F: case KC_G:
        case KC_H: case KC_J: case KC_K: case KC_L:
        case KC_W: case KC_O:
            return (svm_config_t){.w_x=1000, .w_y=0, .b=-250000, .guard=250};
        default:
            return (svm_config_t){.w_x=1000, .w_y=0, .b=-200000, .guard=200};
    }
}
// ============================================================================
// T&H State
// ============================================================================

typedef struct {
    uint16_t keycode;
    uint16_t timer;

    uint16_t x;
    uint16_t y;
    uint16_t timeout;
    bool     is_hold;

    // for cross-hand constraint
    uint8_t col;
    uint8_t row;
    bool force_tap;

    bool     active;
    bool     combined;
    enum { ST_WAITING, ST_HOLDING, ST_RELEASING } state;
} th_instance_t;

static th_instance_t th_instances[MAX_ACTIVE_TH];
typedef struct {
    keyrecord_t record;
    uint16_t keycode;
} svm_event_t;
static svm_event_t event_buffer[SVM_BUF_SIZE];

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

// ============================================================================
// Helpers: Key Classification
// ============================================================================

// --- クロスハンド制約用ヘルパー ---
bool is_left_hand(uint8_t row) {
    return row <= 4; // 左手はRow 0〜4
}

// 指定したインスタンスと、現在のキーイベントが「物理的に同じキー」かを判定する
bool is_same_physical_key(th_instance_t *inst, keyrecord_t *record) {
    return (inst->col == record->event.key.col && inst->row == record->event.key.row);
}

bool is_cross_hand_target(uint16_t keycode) {
    // Layer-Tapの場合、移動先のレイヤー番号によって判定を変える
    if (IS_QK_LAYER_TAP(keycode)) {
        uint8_t target_layer = (keycode >> 8) & 0x0F;

        // カーソルやマウス操作など、片手で完結させたいレイヤーは除外（Holdを許容）
        if (target_layer == _LAYER3 || target_layer == _LAYER4) {
            return false;
        }
        // それ以外のレイヤー（記号レイヤーなど）はタイピング中に誤爆しやすいなら true にする
        return true;
    }

    return false; // 上記以外は対象外
}

// 新しく押されたキーが「純粋な文字入力キー」かどうかを判定する
bool is_pure_typing_key(uint16_t keycode) {
    // 修飾キー単体、Mod-Tap、Layer-Tap はタイピングではない（修飾の意図）
    if (IS_QK_MODS(keycode) || IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
        return false;
    }
    // 基本的な文字キー（A〜Z、数字、記号など）
    if (IS_QK_BASIC(keycode)) {
        return true;
    }
    // その他のキー（マクロや特殊キーなど）はとりあえずタイピング扱いとする
    return true;
 }

// 指定したキーの「押下（Press）」がバッファ内に存在するかチェックする
bool is_press_buffered(uint8_t col, uint8_t row) {
    for (uint8_t i = 0; i < buf_count; i++) {
        if (event_buffer[i].record.event.key.col == col &&
            event_buffer[i].record.event.key.row == row &&
            event_buffer[i].record.event.pressed) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// Event Buffer: Operations
// ============================================================================

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

    uint8_t i = 0;
    while (i < buf_count) {
        svm_event_t current_event = event_buffer[i];
        if (DEBUG_SVM) uprintf("SVM:   Replay Event [Col:%d Row:%d], Pressed:%d\n",
                            current_event.record.event.key.col,
                            current_event.record.event.key.row,
                            current_event.record.event.pressed);

        process_record(&(current_event.record));
        i++;

        // 【追加】ポーズ条件の厳格化
        if (current_event.record.event.pressed && is_any_th_waiting()) {
            bool release_found_in_buffer = false;
            for (uint8_t j = i; j < buf_count; j++) {
                if (event_buffer[j].record.event.key.col == current_event.record.event.key.col &&
                    event_buffer[j].record.event.key.row == current_event.record.event.key.row &&
                    !event_buffer[j].record.event.pressed) {
                    release_found_in_buffer = true;
                    break;
                }
            }
            if (!release_found_in_buffer) {
                if (DEBUG_SVM) uprintf("SVM: Replay paused at event %d\n", i);
                break;
            }
        }
    }

    uint8_t remaining = buf_count - i;
    for (uint8_t j = 0; j < remaining; j++) {
        event_buffer[j] = event_buffer[i + j];
    }
    buf_count = remaining;
    is_replaying = false;
}

// ============================================================================
// T&H Action Execution
// ============================================================================

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
    } else {
        tap_code16(tap_kc);
        inst->active = false;
        if (DEBUG_SVM) uprintf("SVM: Fixed as TAP [Key:0x%04X, TapKC:0x%02X]\n", inst->keycode, tap_kc);

#if TAPPING_TOGGLE_TERM > 0
        tt_state.last_tap_keycode = inst->keycode;
        tt_state.last_tap_time = timer_read();
#endif
    }
}

// ============================================================================
// Event Handlers (process_record_user helpers)
// ============================================================================

static void handle_other_key_press(uint16_t keycode, keyrecord_t *record) {
    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
        th_instance_t *inst = &th_instances[i];
        if (!inst->active) continue;
        if (is_same_physical_key(inst, record)) continue;
        if (inst->state != ST_WAITING && inst->state != ST_HOLDING) continue;

        inst->combined = true;

        if (inst->state != ST_WAITING) continue;

        inst->y = (uint16_t)(record->event.time - inst->timer);

#if CROSS_HAND_CONSTRAINT
        if (!is_cross_hand_target(inst->keycode)) continue;
        if (!is_pure_typing_key(keycode)) continue;
        bool th_is_left = is_left_hand(inst->row);
        bool other_is_left = is_left_hand(record->event.key.row);
        if (DEBUG_SVM) {
            uprintf("SVM-CROSS: TH_Row:%d(Left:%d) vs New_Row:%d(Left:%d)\n",
                    inst->row, th_is_left, record->event.key.row, other_is_left);
        }
        if (th_is_left == other_is_left) {
            inst->force_tap = true;
            if (DEBUG_SVM) uprintf("SVM: Cross-hand violated! Forced TAP. [Key:0x%04X]\n", inst->keycode);
        }
#endif
    }
}

static bool handle_th_release(uint16_t keycode, keyrecord_t *record) {
    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
        th_instance_t *inst = &th_instances[i];
        if (!inst->active) continue;
        if (!is_same_physical_key(inst, record)) continue;

        uint16_t tap_kc = keycode & 0xFF;

        if (inst->state == ST_WAITING) {
            inst->x = (uint16_t)(record->event.time - inst->timer);
        } else if (inst->state == ST_HOLDING) {
            if (!inst->combined) {
                // 【Retro Tap】単独長押しなのでここで即座に離す
                execute_dynamic_hold(keycode, false);
                tap_code16(tap_kc);
                inst->active = false;
#if TAPPING_TOGGLE_TERM > 0
                tt_state.last_tap_keycode = inst->keycode;
                uint16_t actual_time = inst->timer;
                if (inst->x != 0xFFFF) {
                    actual_time += inst->x;
                }
                tt_state.last_tap_time = actual_time;
#endif
            } else {
                // 【コンボ後】ここでは離さず、余韻フェーズへ送る！
                inst->state = ST_RELEASING;
                inst->timer = timer_read();
            }
        }
        return true;
    }
    return false;
}

static bool buffer_event_if_needed(uint16_t keycode, keyrecord_t *record) {
    if (is_replaying || !is_any_th_waiting()) return false;

    bool should_buffer = record->event.pressed || is_press_buffered(record->event.key.col, record->event.key.row);
    if (!should_buffer) {
        // バッファに関係ないキーのリリースは即座に通してOSのキーリピート暴発を防ぐ
        if (DEBUG_SVM) uprintf("SVM: Bypassed buffer for Release Col:%d Row:%d\n", record->event.key.col, record->event.key.row);
        return false;
    }
    if (buf_count < SVM_BUF_SIZE) {
        event_buffer[buf_count].record = *record;
        event_buffer[buf_count].keycode = keycode;
        buf_count++;
        if (DEBUG_SVM) uprintf("SVM: Buffered Col:%d Row:%d Pressed:%d (count=%d)\n", record->event.key.col, record->event.key.row, record->event.pressed, buf_count);
        return true;
    }
    if (DEBUG_SVM) uprintf("SVM: Buffer overflow! Bypassing Key\n");
    return false;
}

static bool hook_new_th(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return false;
    if (!IS_QK_MOD_TAP(keycode) && !IS_QK_LAYER_TAP(keycode)) return false;

#if TAPPING_TOGGLE_TERM > 0
    uint16_t tap_kc = keycode & 0xFF;
    if (tap_kc != KC_SPC) {
        if (keycode == tt_state.last_tap_keycode && timer_elapsed(tt_state.last_tap_time) < TAPPING_TOGGLE_TERM) {
            if (DEBUG_SVM) uprintf("SVM: Repeat Tap detected! [Key:0x%04X]\n", keycode);
            tt_state.is_repeating = true;
            tt_state.repeating_source_kc = keycode;
            register_code16(tap_kc);
            return true;
        }
    }
#endif

    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
        if (!th_instances[i].active) {
            svm_config_t params = get_svm_params(keycode & 0xFF);
            th_instances[i] = (th_instance_t){
                .keycode = keycode,
                .timer = record->event.time,
                .x = 0xFFFF,
                .y = 0xFFFF,
                .timeout = params.guard,
                .is_hold = true,
                .col = record->event.key.col,
                .row = record->event.key.row,
                .force_tap = false,
                .active = true,
                .combined = false,
                .state = ST_WAITING,
            };
            if (DEBUG_SVM) uprintf("SVM: START WAITING Key:0x%04X\n", keycode);
            return true;
        }
    }
    if (DEBUG_SVM) uprintf("SVM-ERROR: MAX_ACTIVE_TH is FULL! Dropped Key:0x%04X\n", keycode);
    return false;
}

// ============================================================================
// Entry: process_record_user
// ============================================================================

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (DEBUG_SVM) {
        uprintf("SVM-HOOK: [C:%d R:%d] %s KC:0x%04X\n",
                record->event.key.col, record->event.key.row, record->event.pressed ? "Press" : "Release", keycode);
    }

#if TAPPING_TOGGLE_TERM > 0
    if (!record->event.pressed && tt_state.is_repeating && keycode == tt_state.repeating_source_kc) {
        unregister_code16(keycode & 0xFF);
        tt_state.is_repeating = false;
        tt_state.last_tap_keycode = keycode;
        tt_state.last_tap_time = timer_read();
        return false;
    }
    if (record->event.pressed && keycode != tt_state.last_tap_keycode) {
        tt_state = (tap_repeat_t){0};
    }
#endif

    if (record->event.pressed) {
        handle_other_key_press(keycode, record);
    } else if (handle_th_release(keycode, record)) {
        return false;
    }
    if (buffer_event_if_needed(keycode, record)) return false;
    if (hook_new_th(keycode, record)) return false;

    return true;
}

// ============================================================================
// SVM Scoring
// ============================================================================

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

// ============================================================================
// Training Log (Debug)
// ============================================================================

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

// ============================================================================
// T&H State Handlers (matrix_scan_user helpers)
// ============================================================================

static void peek_buffer_for_y(th_instance_t *inst) {
    if (inst->combined || buf_count == 0) return;
    for (uint8_t j = 0; j < buf_count; j++) {
        if (!event_buffer[j].record.event.pressed) continue;
        if (event_buffer[j].record.event.time < inst->timer) continue;

        inst->combined = true;
        inst->y = (uint16_t)(event_buffer[j].record.event.time - inst->timer);

#if CROSS_HAND_CONSTRAINT
        if (is_cross_hand_target(inst->keycode) && is_pure_typing_key(event_buffer[j].keycode)) {
            bool th_is_left = is_left_hand(inst->row);
            bool other_is_left = is_left_hand(event_buffer[j].record.event.key.row);
            if (th_is_left == other_is_left) {
                inst->force_tap = true;
                if (DEBUG_SVM) uprintf("SVM: Peeked Cross-hand violated! Forced TAP. [Key:0x%04X]\n", inst->keycode);
            }
        }
#endif
        if (DEBUG_SVM) uprintf("SVM: Peeked buffer for Y! [Y:%dms]\n", inst->y);
        break;
    }
}

static uint16_t compute_dynamic_timeout(int32_t w_num, uint16_t val, int32_t w_den, int32_t b, uint16_t guard) {
    if (w_den == 0) return MIN_WAIT_TIME;
    int32_t t = -(w_num * val + b) / w_den;
    if (t < MIN_WAIT_TIME) return MIN_WAIT_TIME;
    if (t > guard)         return guard;
    return (uint16_t)t;
}

static void process_waiting(uint8_t i) {
    th_instance_t *inst = &th_instances[i];

    peek_buffer_for_y(inst);

    uint16_t t = timer_elapsed(inst->timer);
    uint16_t tap_kc = inst->keycode & 0xFF;
    svm_config_t params = get_svm_params(tap_kc);
    bool settle_now = false;

#if CROSS_HAND_CONSTRAINT
    if (inst->force_tap) { inst->is_hold = false; settle_now = true; } else
#endif
    if (inst->x != 0xFFFF && inst->y != 0xFFFF) {
        int32_t score = get_svm_score(tap_kc, inst->x, inst->y);
        inst->is_hold = (score > 0);
        settle_now = true;
        if (DEBUG_SVM) uprintf("SVM-SCORE: X:%dms, Y:%dms -> Score:%d (Hold:%d)\n", inst->x, inst->y, score, inst->is_hold);
    } else if (inst->x != 0xFFFF) {
#if TAP_NON_OVERLAPPED
        inst->timeout = MIN_WAIT_TIME;
#else
        inst->timeout = compute_dynamic_timeout(params.w_x, inst->x, params.w_y, params.b, params.guard);
#endif
        inst->is_hold = false;
        if (t >= inst->timeout) settle_now = true;
    } else if (inst->y != 0xFFFF) {
        inst->timeout = compute_dynamic_timeout(params.w_y, inst->y, params.w_x, params.b, params.guard);
        inst->is_hold = true;
        if (t >= inst->timeout) settle_now = true;
    } else {
        if (t >= inst->timeout) settle_now = true;
    }

    if (settle_now) {
        bool was_hold = inst->is_hold;
        settle_instance(i, was_hold);
        if (DEBUG_SVM) uprintf("SVM: Timeout! Settled as %s [Key:0x%04X]\n", was_hold ? "HOLD" : "TAP", inst->keycode);
        if (was_hold && inst->x != 0xFFFF) {
            inst->state = ST_RELEASING;
            inst->timer = timer_read();
        }
    }
}

static void process_releasing(uint8_t i) {
    th_instance_t *inst = &th_instances[i];
    if (is_any_th_waiting()) {
        inst->timer = timer_read();
        return;
    }
    if (timer_elapsed(inst->timer) > HOLD_RELEASE_DELAY) {
        execute_dynamic_hold(inst->keycode, false);
        inst->active = false;
        if (DEBUG_SVM) uprintf("SVM: HOLD safely released [Key:0x%04X]\n", inst->keycode);
    }
}

// ============================================================================
// Entry: matrix_scan_user
// ============================================================================

void matrix_scan_user(void) {
#if TRAINING_LOG > 0
    training_log();
#endif

    for (uint8_t i = 0; i < MAX_ACTIVE_TH; i++) {
        if (!th_instances[i].active) continue;
        if (th_instances[i].state == ST_WAITING)   process_waiting(i);
        else if (th_instances[i].state == ST_RELEASING) process_releasing(i);
    }

    if (buf_count > 0 && !is_any_th_waiting() && !is_replaying) {
        replay_buffer();
    }
}
