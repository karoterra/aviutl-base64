# aviutl-base64

[AviUtl](http://spring-fragrance.mints.ne.jp/aviutl/)
拡張編集スクリプトでオブジェクトを Base64 エンコードするための DLL です。

## 使い方

```lua
local Base64 = require("KaroterraBase64")
local data, w, h = obj.getpixeldata()

-- Base64.encodeBMP(data, w, h, prefix, suffix)
-- dataをBMPとしてBase64エンコード
-- 引数:
--      data: 画像データ (obj.getpixeldata()の第1戻り値)
--      w: 幅 (obj.getpixeldata()の第2戻り値)
--      h: 高さ (obj.getpixeldata()の第3戻り値)
--      prefix: データの前に付ける文字列
--      suffix: データの後に付ける文字列
-- 戻り値:
--      文字列
local bmp = Base64.encodeBMP(data, w, h, "url(\"data:image/bmp;base64,", "\")")

-- Base64.encodePNG(data, w, h, prefix, suffix)
-- dataをPNGとしてBase64エンコード
-- 引数:
--      data: 画像データ (obj.getpixeldata()の第1戻り値)
--      w: 幅 (obj.getpixeldata()の第2戻り値)
--      h: 高さ (obj.getpixeldata()の第3戻り値)
--      prefix: データの前に付ける文字列
--      suffix: データの後に付ける文字列
-- 戻り値:
--      文字列
local png = Base64.encodePNG(data, w, h, "url(\"data:image/png;base64,", "\")")

-- Base64.hashMD5(data, w, h)
-- dataのMD5ハッシュを計算
-- 引数:
--      data: 画像データ (obj.getpixeldata()の第1戻り値)
--      w: 幅 (obj.getpixeldata()の第2戻り値)
--      h: 高さ (obj.getpixeldata()の第3戻り値)
-- 戻り値:
--      文字列
local digest = Base64.hashMD5(data, w, h)
```

## 更新履歴

更新履歴は [CHANGELOG](CHANGELOG.md) を参照してください。

## ライセンス

このソフトウェアは MIT ライセンスのもとで公開されます。
詳細は [LICENSE](LICENSE) を参照してください。

使用したライブラリ等については [CREDITS](CREDITS.md) を参照してください。
