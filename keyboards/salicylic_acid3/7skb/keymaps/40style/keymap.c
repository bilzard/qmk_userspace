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
  RGB_RST = SAFE_RANGE
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

// 状態を保持するためのグローバル変数
static uint16_t pending_retro_key = KC_NO;
static bool retro_interrupted = false;

// 【ステップ1】押された瞬間の監視（割り込みの検知）
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // 押されたキーが MT (Mod-Tap) または LT (Layer-Tap) の範囲か判定
        bool is_dual_role = (keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
                            (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX);

        if (is_dual_role) {
            // MT/LT が押されたらターゲットとして記憶し、割り込みフラグをリセット
            pending_retro_key = keycode;
            retro_interrupted = false;
        } else {
            // それ以外の普通のキーが押されたら「割り込みあり」と判定
            retro_interrupted = true;
        }
    }

    // QMK本家の通常処理（レイヤー切り替えやShiftの適用など）はそのまま生かす
    return true;
}

// 【ステップ2】離された瞬間の処理（タップの強制注入）
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    // キーが離された時
    if (!record->event.pressed) {
        if (keycode == pending_retro_key) {
            // QMKの標準仕様：tap.count == 0 なら「TAPPING_TERMを超過した（長押し判定された）」という意味
            if (!retro_interrupted && record->tap.count == 0) {

                // 本家処理が終わっているので、Layer/Mod はすでにお行儀よく解除されている。
                // ここで、キーコードの「下位8ビット（ベースとなるキーコード）」だけを抽出する。
                // 例: LT(1, KC_MINS) -> KC_MINS に変換される
                uint16_t base_kc = keycode & 0xFF;

                // 抽出したベースキーをタップ！（親指Shiftが生きていれば、自動で反映される）
                tap_code16(base_kc);
            }
            // 処理が終わったらターゲットをクリア
            pending_retro_key = KC_NO;
        }
    }
}

#ifdef DYNAMIC_TAPPING_TERM_ENABLE
extern uint16_t g_tapping_term;
#endif

/*
    ホールド判定の閾値を文脈に合わせて調整する
    * 条件A: レイヤー切り替えまたはシフト修飾
    * 条件B: 文字や記号に割り当てられていない場合
    条件Aかつ条件Bの場合は小さめの閾値を適用、それ以外は通常の閾値を適用
*/
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // 条件A: レイヤー切り替えまたはシフト修飾
    bool is_shift_mt_or_lt = (IS_QK_MOD_TAP(keycode) && (QK_MOD_TAP_GET_MODS(keycode) & MOD_MASK_SHIFT)) ||
                             IS_QK_LAYER_TAP(keycode);

    if (is_shift_mt_or_lt) {
        // タップ側のキーコードを取得
        uint16_t tap_keycode = IS_QK_MOD_TAP(keycode) ?
                               QK_MOD_TAP_GET_TAP_KEYCODE(keycode) :
                               QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);

        // 条件B: 文字や数字に割り当てられていない場合
        bool is_typing_key = (tap_keycode >= KC_A && tap_keycode <= KC_0);
        if (!is_typing_key) {
            return TAPPING_TERM_FAST;
        }
    }

    // それ以外（文字・記号への割り当て、非ShiftのMod-Tap、通常の単打キーなど）は通常判定
#ifdef DYNAMIC_TAPPING_TERM_ENABLE
    return g_tapping_term;
#else
    return TAPPING_TERM;
#endif
}

#ifdef MY_DEBUG
void keyboard_post_init_user(void) {
    debug_enable = true;
    debug_matrix = true;   // 物理的なスイッチの反応を見る
    debug_keyboard = true; // QMKの論理的なイベントを見る
}
#endif

#ifdef MY_ANALYSIS
#include "print.h"

// 前回のマトリックス状態を保持する配列
static matrix_row_t previous_matrix[MATRIX_ROWS] = {0};

void matrix_scan_user(void) {
    // --- 既存の matrix_scan_user の処理があればここに残す ---

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
                    uprintf("[%08lu] R%d,C%d: %s\n",
                            now, r, c, is_pressed ? "Push" : "Release");
                }
            }
            // 状態を更新
            previous_matrix[r] = current_row;
        }
    }
}
#endif
