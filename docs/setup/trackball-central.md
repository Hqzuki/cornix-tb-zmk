# Cornix TB

Cornix右手をCentral、Cornix左手とXIAO BLE nRF52840（トラックボール）をPeripheralにするTB専用リポジトリです。キーマップは`config/Cornix_tb.keymap`、Keymap Editorの定義は`config/Cornix_tb.json`です。

## 配線

右Centralの設定は`boards/shields/cornix_tb_right_central/`、左Peripheralは`boards/shields/cornix_tb_peripheral/`、XIAOは`boards/shields/cornix_tb_trackball_peripheral/`、PAW3222ドライバーは`drivers/trackball/`にあります。

| PAW3222信号 | XIAOピン |
| --- | --- |
| 自動判別対象（SCK／SDIO／MOTION／CS） | D7・D8・D9・D10 |

起動時に4本の信号ピンの24通りの役割割り当てを毎回調べ、標準配線（D10=SCK、D9=SDIO、D8=CS、D7=MOTION）から試します。検出結果はRAMだけに保持し、フラッシュには保存しません。3.3VとGNDは自動判別できないため、センサー基板の配列と電源電圧を確認して接続してください。自動判定の対象は信号線のみで、電源ピンの誤配線を防ぐ機能ではありません。

センサー解像度は1596 CPIです（PAW3222の38 CPI刻みに合わせた1600 CPIに最も近い値）。

探索時のGPIO出力はオープンドレインにして競合を抑え、センサーを識別した後は通常出力に切り替えます。IDレジスタを2回ずつ読み、検出IDもログに出します。実機での電気的な安全性・信号タイミングを確認する場合は、センサーとXIAOの電圧・ピン配置を事前に照合してください。

## 操作案

- レイヤー4（PAN）でボールを動かすとスクロールします。
- レイヤー5（GESTURE）でボールを動かすとウィンドウ／デスクトップ操作ジェスチャーを送ります。ESDFカーソル移動は使いません。
- 自動マウスレイヤー（レイヤー6）は、LANG1またはLANG2を押すと解除します。
- BLUETOOTHレイヤー（3）ではCornix内側LEDに左右それぞれの電池残量を表示します。緑＝51〜100%、黄＝21〜50%、赤＝0〜20%。XIAO内蔵LEDはUSB給電・充電表示に使います。
- XIAOは、約5分でDeep Sleepに入ります。Deep Sleep中はボールのMOTION信号で起床し、右手へ再接続します。キー入力では起床しません。

## 確認範囲

自動ピン判別やキー・ジェスチャー処理はGitHub Actionsでファームウェアをビルドします。実機の配線、センサー取付方向、無線接続、スリープ復帰は各機材で確認してください。成功したUF2は[GitHub Actions](https://github.com/Hqzuki/cornix-tb-zmk/actions)から取得できます。

詳しい挙動は[ドキュメント一覧](../README.md)を参照してください。
