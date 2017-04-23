[![Build Status](https://secure.travis-ci.org/papabricole/weatherstation.png)](http://travis-ci.org/papabricole/weatherstation)

# Solar Weather Station

 - Wemos D1 mini
 - SHT21 Temperature and Humidity sensor
 - TP4056 - 5V 1A Lithium Battery Charger with Protection
 - LIPO single cell battery 1250mAh
 - Solar panel

The station wakeup every hours, measure the temperature and humidity, push to adafruit.io via MQTT then go to sleep.
The solar panel is directly connected to the lipo charger and the Wemos to the 3.7v TP4056 under voltage protection circuit.
This setup seems to work, at least it's working fine since Nov 2016 till now. The feed is available here:

  https://io.adafruit.com/morganleborgne/feeds/temperature