# robosys2020_task1

## 概要

自宅にある扇風機を赤外線でON、OFFします。

## 回路



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
