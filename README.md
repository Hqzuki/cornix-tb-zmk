# Cornix TB ZMK

XIAO BLEをCentral、左右のCornixをPeripheralにするトラックボール構成のZMKファームウェアです。PAW3222トラックボールをXIAOに接続し、ZMK StudioとKeymap Editorからキー配列を編集できます。

## 主な機能

- Windows／Mac配列の自動OS判定
- NUMFN、PAN、GESTURE、MOUSE、BLUETOOTHレイヤー
- PAW3222のカーソル・スクロール・デスクトップ操作
- Cornix左右のレイヤー／Bluetooth接続先LED表示、Bluetoothレイヤーの電池表示
- 5分後のDeep Sleepと、ボール操作によるCentralの復帰
- ZMK Studioによるキー配列編集

## ドキュメント

機能ごとの説明は[ドキュメント一覧](docs/README.md)から参照できます。

## ファームウェア

GitHub Actionsの成功した[ビルド](https://github.com/Hqzuki/cornix-tb-zmk/actions)から`firmware`成果物を取得します。

- XIAO Central：TB Central用UF2
- Cornix Peripheral：TB Peripheral用UF2

配線は[トラックボール構成ガイド](docs/setup/trackball-central.md)、各機能の挙動は[ドキュメント一覧](docs/README.md)を参照してください。

## 開発

キーマップは`config/Cornix_tb.keymap`、Keymap Editorの定義は`config/Cornix_tb.json`です。PAW3222ドライバーは`drivers/trackball/`、TB CentralとPeripheralの設定は`boards/shields/cornix_tb_*`にあります。
