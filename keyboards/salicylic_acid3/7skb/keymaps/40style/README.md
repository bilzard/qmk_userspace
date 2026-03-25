# Keymaps

## 40style (35-key Optimized)

7skbを35キーで快適に使用するための設定。
**Home Row Mods (HRM)** での誤爆問題に対応。

### 🛠 本家（default）からの変更点

* **Tapping Term:** 長めに **300 ms**に設定。タイピングが遅い時でもホールド認定されにくくする。
* **Permissive Hold:** タップ順序に応じてホールド認定する（例: push s + push a + release a）。

### 経緯：HRMの誤爆対策

ホームポジションに修飾キーを割り当てている(home row mods)が、タイプする速度が遅い場合に"sa"が"Ctrl+A"と解釈されることが稀にあった。修飾キーの誤爆はターミナル操作時などは特に危険なので、ホールドと判定されにくく設計している。

### 🚀 build手順
```bash
qmk compile -kb salicylic_acid3/7skb -km 40style
```