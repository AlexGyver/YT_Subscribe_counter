# Счётчик подписок YouTube

## Папки

**Библиотеки** - библиотека для модуля и дисплея, установить в C:\Program Files\Arduino\libraries
  
**silver_button_count** - первая версия системы

## Схема
![СХЕМА](https://github.com/AlexGyver/YT_Subscribe_counter/blob/master/scheme.jpg)

##  Материалы и компоненты
Всё указанное ниже можно найти здесь
http://alexgyver.ru/arduino_shop/

* ESP8266 https://ali.ski/brmEy0
* USB программатор https://ali.ski/Vz4n2W
* LCD 1602 I2C https://ali.ski/6evCYY
* Понижайка https://ali.ski/XcLwNm
* RGB LED https://ali.ski/LbjAn
* Фоторезистор https://ali.ski/rM7y5

## Вам также пригодится 
* Всё для пайки http://alexgyver.ru/all-for-soldering/
* Электронные компоненты http://alexgyver.ru/electronics/

## HOW TO

`const char* ssid = "WIFI login";`  
вместо **WIFI login** вписать логин вайфая для роутера  
`const char* password = "WIFI pass";`  
вместо **WIFI pass** вписать пароль вайфая для роутера  

`String host = "https://www.googleapis.com/youtube/v3/channels?part=statistics&key=(API KEY)&id=(CHANNEL ID)";`  
вместо **API KEY** вписать полученный в гугле ключ API  
вместо **CHANNEL** ID вписать ID канала  

`String httpsFingerprint = "20:47:22:DB:D3:C9:1E:8F:6F:C1:C2:B2:3C:8C:8A:0F:C8:88:3E:45";`  
с этим проблема, ключ меняется раз в два дня, решение пока не нашёл  

Для работы с ESP нужно установить ядро в Arduino IDE, вся инфа тут https://esp8266.ru/arduino-ide-esp8266/  