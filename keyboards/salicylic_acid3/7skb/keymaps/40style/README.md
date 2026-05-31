# 40style keymap

## Overview

QMK native Tap/Hold (MT/LT) based keymap with a custom Retro Tap implementation and per-key tapping term adjustment. Unlike `svmv2`, this keymap does not disable `NO_ACTION_TAPPING` and relies on QMK's built-in tapping engine.

## Layers

| Layer | Name | Activation |
|---|---|---|
| 0 | `_QWERTY` | Default |
| 1 | `_LAYER1` | `MO(_LAYER1)` — right pinky key |
| 3 | `_LAYER3` | `TG(_LAYER3)` — top-left key on LAYER1 |

- **LAYER1**: F1–F12, navigation (arrows, Home/End, PgUp/PgDn), PrintScreen, ScrollLock, Pause
- **LAYER3**: RGB controls (`UG_*`), bootloader (`QK_BOOT`), single-hand operation only

LAYER2 and LAYER4 are defined in the enum but have no keymap entries.

## Retro Tap

Custom implementation via `process_record_user` / `post_process_record_user`. When an MT/LT key is released without any other key being pressed during hold (`retro_interrupted == false`) and QMK resolved it as Hold (`tap.count == 0`), the base keycode is injected as a tap via `tap_code16(keycode & 0xFF)`.

This differs from QMK's built-in `RETRO_TAPPING` in that it is triggered only when there is no interrupt, giving cleaner hold-only behaviour.

## Tapping term

Default: `TAPPING_TERM` = 250 ms. `get_tapping_term()` applies a faster threshold in specific cases:

- **Condition**: MT with Shift mods, or LT — AND the tap keycode is not a letter/number key
- **Result**: `TAPPING_TERM_FAST` = 120 ms

This makes layer-switch and modifier keys on non-typing positions feel snappier while preserving accuracy on home-row assignments.

`QUICK_TAP_TERM` = 120 ms.

## RGB layer indicator

`layer_state_set_user()` sets the underglow colour per layer:

| Layer | Colour |
|---|---|
| LAYER1 | Blue |
| LAYER2 | Purple |
| LAYER3 | Green |
| LAYER4 | Yellow |
| Default | Off |

## Build flags

| Flag | Value |
|---|---|
| `VIA_ENABLE` | yes |
| `DYNAMIC_TAPPING_TERM_ENABLE` | no |
| `RGBLIGHT_ENABLE` | no |
| `MOUSEKEY_ENABLE` | yes |
| `LTO_ENABLE` | yes |

## Debug / analysis modes

- `MY_DEBUG`: enables QMK console debug output for matrix and keyboard events
- `MY_ANALYSIS`: logs raw physical key events (`[timestamp] Rr,Cc: Push/Release`) for data collection
