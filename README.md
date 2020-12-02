# robosys2020_task1

## 概要

Raspberry Pi4 ModelB 4GBを使って、自宅にある扇風機を赤外線LEDでON, OFFするデバイスドライバのリポジトリです。
なお、このデバイスドライバは、千葉工業大学 未来ロボティクス学科 6セメスター 2020年度 ロボットシステム学の講義の課題で作成しました。

## 回路構成

### 部品

- 赤外線LED：G011577
- 抵抗：390[Ω]

### 回路図

Raspberry Pi4のGPIO25に赤外線LEDを接続します。抵抗は手元に390[Ω]しかなかったため、並列に接続して抵抗値を下げました。

<img src="https://github.com/shimotoriharuki/robosys2020_task1/blob/master/imgs/circuit.png" width="250">


## 信号

赤外線リモコンの信号は、赤外線LEDのHIGHの時間、LOWの時間の組み合わせで0, 1を表現しています。
例えば、今回操作する扇風機のリモコンの信号は、以下のような時間の組み合わせで0, 1を表現しています。
この0, 1の並びで信号が形成され、様々な意味を持った信号を作ることができます。

![0](https://github.com/shimotoriharuki/robosys2020_task1/blob/master/imgs/irremocon_0.png)
![1](https://github.com/shimotoriharuki/robosys2020_task1/blob/master/imgs/irremocon_1.png)

また、信号にはスタート信号とエンド信号があり、それぞれ以下の図のように表現します。これらは必ず信号の最初と最後にあり、信号の初めと終わりを受信側に知らせる役割があります。

![start](https://github.com/shimotoriharuki/robosys2020_task1/blob/master/imgs/irremocon_start.png)
![end](https://github.com/shimotoriharuki/robosys2020_task1/blob/master/imgs/irremocon_end.png)

外乱強くするために、HIGHの信号は以下の図のように38[kHz]の周波数で高速にON, OFFを繰り返しています。
このようなパターンの赤外線は自然界にはほぼありませんので、38[kHz]の信号のみを受信するように受信側で設定されていれば外乱に強い通信を行うことができます。

![hz](https://github.com/shimotoriharuki/robosys2020_task1/blob/master/imgs/irremocon_hz.png)

扇風機を操作する具体的な信号は、扇風機の赤外線リモコンから発せられる赤外線を赤外線受光モジュールで受信し、以下の画像のようにオシロスコープを使って読み取りました。
読み取った結果、扇風機をON, OFFする信号は、スタート信号をS、エンド信号をEとすると、
```
S 0110 0000 1100 0101 0000 1110 0000 0111 E
```
となりました。

<img src="https://github.com/shimotoriharuki/robosys2020_task1/blob/master/imgs/scope.jpg" width="250">


## 使用方法

次のコマンドを実行します。
```shell
$ git clone https://github.com/shimotoriharuki/robosys2020_task1
$ cd task1
$ make
$ sudo insmod myled.ko 
$ sudo chmod 666 /dev/myled0 
```

赤外線LEDを扇風機に向け、次のコマンドを実行すると扇風機がON、OFFします。
```shell
$ echo 1 > /dev/myled0 
```

もう一度`sudo insmod myled.ko`する際は、次を実行してから`sudo insmod myled.ko`を実行してください。
```shell
sudo rmmod myled
```
