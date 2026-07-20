# QMK Userspace (7skb カスタム構成)

このリポジトリは、QMK本体をサブモジュールとして保持し、独自のキーマップを管理するための環境です。

## 1. 開発環境のセットアップ

### CLIのインストール (macOS)
```bash
brew install qmk/qmk/qmk
```

### リポジトリの準備と初期化

このリポジトリをクローンした後、以下のコマンドでQMK本体を展開し、パスを設定します。

```bash
# サブモジュールの展開
git submodule update --init --recursive

# QMK CLIの設定（ディレクトリの紐付け）
qmk config user.qmk_home=$(realpath external/qmk_firmware)
qmk config user.overlay_dir=$(realpath .)

# ビルド環境の依存関係をインストール
qmk setup
```

## 2. ビルド方法

### ローカルでビルドする

```bash
qmk compile -kb salicylic_acid3/7skb -km svmv2
```

### 書き込み (Flash)

**左右でbootloaderが異なります。** 書き込む側にUSBケーブルを挿し替えて、左右それぞれに書き込んでください（TRRS経由では書き込めません）。

| | USB ID | bootloader | 書き込みツール |
|---|---|---|---|
| 左 | `03EB:2FF4` (ATMEL ATm32U4DFU) | atmel-dfu | dfu-programmer |
| 右 | `2341:0037` (Arduino Micro) | Caterina | avrdude |

```bash
qmk flash -kb salicylic_acid3/7skb -km svmv2                # 左
qmk flash -kb salicylic_acid3/7skb -km svmv2 -bl avrdude    # 右
```

`svmv2/rules.mk` の `BOOTLOADER = atmel-dfu` は左に合わせた指定です。ファームは1つで`QK_BOOT`のジャンプ先はビルド時に固定されるため、**`QK_BOOT`はUSBを挿している側が左のときだけ動きます**。右に書き込むときはリセットスイッチを押してください。

左右を揃える案は [issue #2](https://github.com/bilzard/qmk_userspace/issues/2) を参照。

QMK Toolboxでも書き込めます（bootloaderを自動判別するため`-bl`の指定は不要）。ただしIntelバイナリなのでApple SiliconではRosettaが必要です。

## 3. トラブルシューティング

### `qmk doctor` で ARM GCC のエラーが出る場合
`arm-none-eabi-gcc` のテストコンパイルに失敗し、`Major problems detected` と表示されることがあります。

- **7skb (AVR系) の場合:**
  `Successfully compiled using avr-gcc` と出ていれば、ARMのエラーが出ていても無視して 7skb のビルドは可能です。

- **RP2040等のARMボードを使う場合:**
  HomebrewでインストールされたGCCのバージョンが新しすぎることが原因です。以下のコマンドで、安定版のツールチェーン（Cask版）に入れ替えることで解消します。

```bash
  brew uninstall arm-none-eabi-gcc
  brew install --cask gcc-arm-embedded
```

### `⚠ QMK home does not appear to be a Git repository!`

`external/qmk_firmware` が Git サブモジュールであるために出る警告ですが、動作に影響はありません。無視して進めて問題ありません。
