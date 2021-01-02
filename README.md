# Solar Weather Station

 - Wemos D1 mini
 - SHT21 Temperature and Humidity sensor
 - TP4056 - 5V 1A Lithium Battery Charger with Protection
 - LIPO single cell battery 1250mAh
 - Solar panel

The station wakeup every hours, measure the temperature and humidity, push to adafruit.io via MQTT then go to sleep.
The solar panel is directly connected to the lipo charger and the Wemos to the 3.7v TP4056 under voltage protection circuit.
