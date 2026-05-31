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

Flashing:

```bash
qmk flash -kb salicylic_acid3/7skb -km svmv2
```

Default bootloader is `atmel-dfu` (Pro Micro with ATm32U4DFU). Override with `-bl avrdude` for Caterina devices.

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
