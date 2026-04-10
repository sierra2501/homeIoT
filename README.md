# homeIoT

## 概要

このリポジトリは、sierraが書いた同人誌「おウチIoT おウチの環境を計測して監視する。」で使用したプログラムやスクリプト、設定ファイルを格納しています。

## フォルダ構成

<pre>
.
├── Arduino：M5StackやM5StickCのソースコードを格納しています。
├── sh：Raspberry PiでCronで定周期実行しているスクリプトを格納しています。
└── docker
    └── grafana：Grafanaのdocker-compose.yamlと設定ファイルを格納しています。
</pre>

## 注意事項

- [Arduino] ソースコード内のUUIDは適宜変更してください。
- [sh] スクリプトのMACアドレスはBLEを送信している端末のMACアドレスを指定します。
- [sh] CSVファイルの保存先は適宜変更してください。
