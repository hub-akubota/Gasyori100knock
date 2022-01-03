# 画像処理100本ノックチャレンジ

[yoyoyo-yo様のGasyori100knock](https://github.com/yoyoyo-yo/Gasyori100knock)にC++でチャレンジするだけのレポジトリ。

基本的にpythonの解答は本家のanswerを引用しており、そちらを見本にC++の解答を再実装している。

また余力に応じて発展的な取り組みもする。

## 環境

- macOS Big Sur, 2.4 GHz Quad Core Intel Core i5, 16 GB Memory
- openCV: brew install
```
brew install opencv
```

## 解答の実行スクリプト

```
git clone <this repository>
cd <this repository>
poetry install
./run.sh <#>
# answers/#/main.py, answers/#/main.cppが順番に実行される
# ex) ./run.sh 1
```
