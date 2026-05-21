# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with this keymap.

## Overview

Custom Tap/Hold decision engine that replaces QMK's built-in tapping logic with an SVM (Support Vector Machine) classifier. `#define NO_ACTION_TAPPING` in `config.h` disables QMK's mechanism entirely; all T&H logic is self-contained in `keymap.c`.

## SVM decision model

Two timing features are extracted per T&H key event:

- **x**: duration from T&H press to T&H release (ms)
- **y**: duration from T&H press to the next key press (ms)

```
score = w_x * x + w_y * y + b
score > 0  →  Hold
score ≤ 0  →  Tap
```

Per-key parameters are returned by `get_svm_params()`. The weights were fit from labelled stroke data in `preprocessed/` using the analysis notebooks in `analysis/`.

## State machine

Each active T&H key is tracked as a `th_instance_t`. States:

```
[key pressed]
      │
      ▼
 ST_WAITING ──(settled as Hold)──► ST_HOLDING ──(released with combo)──► ST_RELEASING
      │
      └──(settled as Tap) → tap_code sent, instance freed
```

`process_waiting()` is called every scan cycle for each waiting instance and applies the following priority:

1. `force_tap` set (cross-hand constraint violated) → Tap immediately
2. Both x and y known → SVM score decides
3. Only x known (released, no next key yet) → dynamic timeout back-calculated from SVM boundary → settles as Tap
4. Only y known (next key arrived, not yet released) → dynamic timeout → settles as Hold
5. Neither known → `guard` timeout (falls back to Hold)

## Event buffering

While any T&H is `ST_WAITING`, subsequent key events are stored in `event_buffer[]` instead of being processed. Once all waiting instances settle, `replay_buffer()` reprocesses them in order. This preserves correct key ordering regardless of typing speed.

A release event is only buffered if its corresponding press is already in the buffer; orphan releases are passed through immediately to prevent OS key-repeat misfires.

## Cross-hand constraint

Controlled by `CROSS_HAND_CONSTRAINT` (default 1). When a Layer-Tap key and a same-hand typing key are pressed together, the T&H is forced to Tap. Determined by row number: rows 0–4 = left hand, rows 5+ = right hand.

`_LAYER3` and `_LAYER4` are exempt because they are used for single-hand operations (RGB, bootloader).

## Retro Tap

If a key has settled as Hold but is released without having been combined with any other key (`combined == false`), the Hold is cancelled and the tap keycode is sent. Implemented in `handle_th_release()`.

## Repeat Tap

Re-pressing the same T&H key within `TAPPING_TOGGLE_TERM` (200 ms) triggers OS key repeat via `register_code16`. `KC_SPC` is excluded because it is commonly held immediately after a tap.

## Tuning parameters

All defined near the top of `keymap.c`:

| Macro | Default | Description |
|---|---|---|
| `MAX_ACTIVE_TH` | 8 | Max simultaneously tracked T&H instances |
| `SVM_BUF_SIZE` | 32 | Event buffer capacity |
| `TAPPING_TOGGLE_TERM` | 200 | Repeat Tap detection window (ms) |
| `HOLD_RELEASE_DELAY` | 30 | Grace period before Hold is released in ST_RELEASING (ms) |
| `MIN_WAIT_TIME` | 50 | Floor on dynamic timeouts (ms) |
| `TAP_NON_OVERLAPPED` | false | Treat non-overlapping consecutive keys as immediate Tap |
| `CROSS_HAND_CONSTRAINT` | 1 | Enable cross-hand forced-Tap (0 to disable) |
| `DEBUG_SVM` | 0 | Serial debug output (set to 1, enable `CONSOLE_ENABLE` in rules.mk) |
| `TRAINING_LOG` | 0 | Raw physical key event logging for data collection |
