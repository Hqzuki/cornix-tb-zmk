# Cornix TB ドキュメント

機能ごとに説明を分けています。まず知りたい内容のページを開いてください。

## 挙動

- [レイヤーの挙動](behavior/layers.md)：レイヤー番号、用途、マウス入力による切替
- [トラックボールの挙動](behavior/trackball.md)：自動配線判定、感度、カーソル加速、微振動カット、PAN／GESTURE
- [LEDの挙動](behavior/leds.md)：左右のLED、色、Bluetoothレイヤーの電池表示、XIAOのLED
- [電源とスリープ](behavior/power.md)：センサーの省電力モード、5分後のDeep Sleep、復帰方法
- [BluetoothとOS自動判定](auto-os.md)：接続先ごとの判定方法、制限、確認手順

## セットアップ

- [トラックボールCentralの配線と構成](setup/trackball-central.md)

## 設定を変更するとき

| 変更する内容 | 主なファイル |
| --- | --- |
| キー配列・レイヤー名 | `config/Cornix_tb.keymap` |
| トラックボール入力処理・各レイヤーのセンサー動作 | `boards/shields/cornix_tb_central/cornix_tb_central.keymap` |
| センサーのピン候補・CPI・デッドゾーン | `boards/shields/cornix_tb_central/cornix_tb_central.overlay` |
| レイヤー／接続先のLED表示 | `drivers/status/patterns.h` |
| LED更新・左右への状態同期 | `drivers/status/controller_tb.c` |
| PAW3222通信・自動判定・省電力処理 | `drivers/trackball/` |
| OS自動判定 | `drivers/keymap/auto_os.c`、`docs/auto-os.md` |
