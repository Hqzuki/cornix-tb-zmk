# レイヤーの挙動

Cornix TBのレイヤーは`config/Cornix_tb.keymap`の定義順で並びます。現在は7レイヤーです。

| ID | 名前 | 用途 |
| ---: | --- | --- |
| 0 | MAC | macOS／iOS／iPadOS用の基本配列。OS自動判定で有効になる |
| 1 | WIN | Windows／Linux系および判定前の基本配列 |
| 2 | NUMFN | 数字・Fキー、記号、括弧入力 |
| 3 | BLUETOOTH | Bluetooth接続先の選択と削除 |
| 4 | PAN | トラックボールでスクロール |
| 5 | GESTURE | トラックボールの方向ジェスチャーでウィンドウ／デスクトップ操作 |
| 6 | MOUSE | カーソル移動とマウスボタン。自動マウスレイヤー（AML）からも使われる |

## 基本配列と自動OS判定

起動時・未接続時はWIN（ID 1）です。Bluetoothホストの読み取り情報を観察し、macOS系と判定するとMAC（ID 0）を有効にします。WINとMACの手動切替や保存はありません。接続相手ごとの判定条件と限界は[OS自動判定の説明](../auto-os.md)を参照してください。

## 補助レイヤー

- NUMFN、BLUETOOTH、PANなどはキー操作で一時的に有効にするレイヤーです。対応キーの割当はKeymap Editorで確認・変更できます。
- MOUSEはトラックボールの動きで一時的に有効になります。動作中に通常キーの大半を押しても維持し、LANG1またはLANG2で解除します。
- GESTURE（ID 5）はボールの方向をショートカットに変換します。カーソル移動は送りません。
- 各レイヤーのセンサー回転入力は`&enc_off`です。

Keymap Editorでレイヤーの順番を変えるとLED色・OS判定の参照番号も関係します。特にMAC=0、WIN=1、BLUETOOTH=3、PAN=4、GESTURE=5、MOUSE=6は、関連コードと一緒に確認してください。
