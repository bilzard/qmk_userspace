# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repo is

QMK userspace overlay for the **salicylic_acid3/7skb** split keyboard. QMK firmware lives at `external/qmk_firmware/` (git submodule) and is never edited directly. This repo's `keyboards/` directory is merged into QMK at build time via the overlay mechanism.

**Users only edit files under `keyboards/`.**

## Setup

```bash
git submodule update --init --recursive
qmk config user.qmk_home=$(realpath external/qmk_firmware)
qmk config user.overlay_dir=$(realpath .)
qmk setup
```

## Build & Flash

```bash
qmk compile -kb salicylic_acid3/7skb -km svmv2
```

Flashing. The two halves have **different bootloaders**, so flash each half separately with the USB cable plugged into the half being flashed (TRRS does not carry flashing):

```bash
qmk flash -kb salicylic_acid3/7skb -km svmv2                # left  (ATm32U4DFU, atmel-dfu)
qmk flash -kb salicylic_acid3/7skb -km svmv2 -bl avrdude    # right (Caterina)
```

`svmv2/rules.mk` sets `BOOTLOADER = atmel-dfu` to match the left half. Since one firmware image can only target one bootloader, `QK_BOOT` works **only when the left half is the USB-connected one** — flash the right half using its reset switch. See [issue #2](https://github.com/bilzard/qmk_userspace/issues/2) for the plan to unify both halves via ISP.

If `qmk doctor` reports ARM GCC errors, ignore them: 7skb is AVR-based and builds fine as long as `Successfully compiled using avr-gcc` is shown.

## Repository layout

```
keyboards/salicylic_acid3/7skb/keymaps/
  svmv2/      ← main keymap; custom SVM Tap/Hold engine
  40style/    ← simpler keymap; uses QMK native Tap/Hold + Retro Tap
external/qmk_firmware/   ← QMK submodule (read-only)
analysis/                ← Jupyter notebooks for SVM training data analysis
preprocessed/            ← labelled stroke data used for SVM training
logs/                    ← raw key event logs captured from the keyboard
```

## Keymap documentation

- [svmv2](keyboards/salicylic_acid3/7skb/keymaps/svmv2/README.md) — custom SVM Tap/Hold engine
- [40style](keyboards/salicylic_acid3/7skb/keymaps/40style/README.md) — QMK native Tap/Hold + Retro Tap

## CI

GitHub Actions builds on every push using the official `qmk/.github` reusable workflow and publishes firmware artifacts. Build targets are declared in `qmk.json`.
