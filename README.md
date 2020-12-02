# robosys2020_task1

## 概要

自宅にある扇風機を赤外線でON、OFFします。

## 回路


## 信号

赤外線リモコンの信号は、赤外線LEDのHIGHの時間、LOWの時間の組み合わせで0, 1を表現している。
例えば、今回操作する扇風機のリモコンの信号は、以下のような時間の組み合わせで0, 1を表現している。

![0](https://github.com/shimotoriharuki/robosys2020_task1/blob/master/imgs/irremocon_0.png)
![1](https://github.com/shimotoriharuki/robosys2020_task1/blob/master/imgs/irremocon_1.png)

また、HIGHの信号は以下の図のように、38[kHz]の周波数で高速にON, OFFを繰り返している。

![hz](https://github.com/shimotoriharuki/robosys2020_task1/blob/master/imgs/irremocon_hz.png)


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
