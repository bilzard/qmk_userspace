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

int RGB_current_mode;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  bool result = false;
  switch (keycode) {
    #ifdef RGBLIGHT_ENABLE
      case QK_UNDERGLOW_MODE_NEXT:
          if (record->event.pressed) {
            rgblight_mode(RGB_current_mode);
            rgblight_step();
            RGB_current_mode = rgblight_config.mode;
          }
        break;
      case RGB_RST:
          if (record->event.pressed) {
            eeconfig_update_rgblight_default();
            rgblight_enable();
            RGB_current_mode = rgblight_config.mode;
          }
        break;
    #endif
    default:
      result = true;
      break;
  }

  return result;
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

        // 条件B: 文字や記号に割り当てられていない場合
        bool is_typing_key = (tap_keycode >= KC_A && tap_keycode <= KC_0) ||
                             (tap_keycode >= KC_MINUS && tap_keycode <= KC_SLASH);
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
