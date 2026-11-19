# 電源とスリープ

電源まわりは、センサー内部の省電力、ZMKのアイドルタイマー、XIAO CentralのDeep Sleepの3つを分けて考えます。

## タイマー

| 時間 | 動作 |
| --- | --- |
| ボール操作が止まったとき | PAW3222がSleep1／2／3へ自動ダウンシフトし、センサー消費電力を下げる |
| 5分間操作がない | ZMKがアイドル状態になり、Deep Sleepへ移行する |
| 5分間操作がない | XIAO CentralがSystem OFF（Deep Sleep）へ入る |

ZMKのアイドル状態は内部の電源管理トリガーです。アイドル専用のLEDモードはありません。`CONFIG_ZMK_IDLE_TIMEOUT`と`CONFIG_ZMK_IDLE_SLEEP_TIMEOUT`はいずれも300000 msに設定されています。

## Deep Sleepからの復帰

- センサーのMOTION信号を起床入力として保持します。Deep Sleep中にボールを動かすとXIAOが再起動し、左右PeripheralとBluetoothホストへ再接続します。
- キー入力だけでは、電源OFF中のXIAOを起こせません。左右CornixはPeripheralであり、Centralが寝ている間はキーイベントを転送できないためです。
- 復帰には通常の起動・再接続時間がかかります。すぐ使える待機状態ではありません。
- センサー検出が終わっていない、またはMOTION信号が不安定な場合、Centralがスリープを拒否して動作を続けることがあります。

## 無線設定

XIAO Centralは0 dBm、左右Peripheralは+2 dBmです。左右PeripheralはLE 1M PHYに固定し、2M PHYを無効にしています。Central側の分割接続レイテンシは0です。
