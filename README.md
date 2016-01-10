#LICENSE

This software is based in part on the work of the Independent JPEG Group.  
And, this software that coded by prprhyt is released under the MIT License, see LICENSE.  
このソフトウェアはthe Independent JPEG Groupのlibjpegを使用しています。  
また、prprhytが作成したこのクラスにはMIT LICENSEが適用されます。MIT LICENSEについてはLICENSEファイルを参照してください。

# color_detection
指定したJPEGファイルに対してHSV値で指定した範囲の色を検出しマーキングし割合を算出するクラス
forIntelEdisonブランチでは競技会当日に使用したソースファイルを載せています。  
(一部ライセンス関する部分が追加してあります。)

#その他
2016/01/09に東海大学湘南キャンパスで行われたIntel Edison Competition 2015 においてペットボトルロケットでのミッションのために作成したものです。  
本番ではペットボトルロケットにIntel EdisonとWebカメラを載せ、撮影した画像から緑地の割合を算出するミッションを無事遂行することができました。  
チーム一丸となって取り組んだ甲斐がありました。  
日付と競技会名の西暦が異なりますが、誤りでは**ない**  
<sub>最優秀賞貰ったよ!</sub>

#Intel Edisonでの環境構築
本ソフトウェアは9V電源(本番時は006P型9V電池)使用時のIntel Edison Kit for Arduino(とYocto Linux ver2.1)で動作確認済みです。  
最新版のYocto Linux(2016/01/09時点ではver2.1)であれば標準でUSBタイプのWebカメラを認識します。  
`lsusb`で確認し認識しない場合はまずOSバージョンを確認し、それでも解決に至らない場合、電圧不足を疑ってみてください。  

以下に本ソフトウェアを利用するにあたって必要なlibjpegとfswebcamのインストール手順をまとめておきます。  
まず、以下のコマンドをsshかシリアルコンソールでアクセスしたEdisonで実行します。  
`vi /etc/opkg/myfeeds.conf`  
そして以下を記入します。
>src intel-iotdk http://iotdk.intel.com/repos/2.0/intelgalactic  
>src all http://iotdk.intel.com/repos/2.0/iotdk/all  
>src i586 http://iotdk.intel.com/repos/2.0/iotdk/i586  
>src x86 http://iotdk.intel.com/repos/2.0/iotdk/x86  
>src/gz all http://repo.opkg.net/edison/repo/all  
>src/gz edison http://repo.opkg.net/edison/repo/edison  
>src/gz core2-32 http://repo.opkg.net/edison/repo/core2-32  

viエディタで保存して終了するにはescキーを押して`:wq`を実行してください。  
次に以下のコマンドを実行してopkgの設定を更新します。通信環境によっては時間がかかるかもしれません。  
お茶でも淹れて待ちましょう。  
`opkg update`  
最後に以下のコマンドを順番に実行します。  
`opkg install libjpeg-dev libv4l-dev`  
`opkg install fswebcam`  
`opkg list | grep fswebcam`  
環境構築は以上になります。お疲れ様でした。
