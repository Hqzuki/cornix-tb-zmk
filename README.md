# Cornix TB ZMK

Cornix右手をCentral、Cornix左手とXIAO BLE（PAW3222トラックボール）をPeripheralにするZMKファームウェアです。ボールの動きはZMKのinput splitで右手へ転送され、ZMK StudioとKeymap Editorからキー配列を編集できます。

## 主な機能

- Windows／Mac配列の自動OS判定
- NUMFN、PAN、GESTURE、MOUSE、BLUETOOTHレイヤー
- PAW3222のカーソル・スクロール・デスクトップ操作
- Cornix左右のレイヤー／Bluetooth接続先LED表示、Bluetoothレイヤーの電池表示
- 5分後のDeep Sleepと、ボール操作によるXIAOの復帰
- ZMK Studioによるキー配列編集

## ドキュメント

機能ごとの説明は[ドキュメント一覧](docs/README.md)から参照できます。

## ファームウェア

GitHub Actionsの成功した[ビルド](https://github.com/Hqzuki/cornix-tb-zmk/actions)から`firmware`成果物を取得します。

- `Crorb_right`：Cornix右手（Central、ホストには「Crorb_right」と表示）
- `Crorb_left`：Cornix左手（Peripheral）
- `Crorb_tb`：XIAOトラックボール（Peripheral）
- `*_reset`：settings_reset用。先にresetを書き込んでから通常のUF2を書き込みます

配線は[トラックボール構成ガイド](docs/setup/trackball-central.md)、各機能の挙動は[ドキュメント一覧](docs/README.md)を参照してください。

## 開発

キーマップは`config/Cornix_tb.keymap`、Keymap Editorの定義は`config/Cornix_tb.json`です。PAW3222ドライバーは`drivers/trackball/`、右Central・左Peripheral・XIAOの設定は`boards/shields/cornix_tb_*`にあります。
