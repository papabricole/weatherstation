#include <Adafruit_BMP280.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>

// don't forget to ignore all changes to the following file using:
// git update-index --skip-worktree src/credential.h
#include "credential.h"

// Time to sleep (in seconds):
const unsigned int sleepTimeS = 15 * 60;

// Adafruit IO
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883

#define GOTO_SLEEP

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);

Adafruit_BMP280 bmp; // I2C 0x76

// Functions
void connect();
void publish(float temperature, float pressure);
void disconnect();

void setup()
{
    Serial.begin(9600);
    Serial.printf("setup...");
#ifdef GOTO_SLEEP
    // Connect D0 to RST to wake up
    pinMode(D0, WAKEUP_PULLUP);
#endif

    // BMP280
    if (!bmp.begin(0x76)) {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
        while (1)
            ;
    }

    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, /* Operating Mode. */
        Adafruit_BMP280::SAMPLING_X2, /* Temp. oversampling */
        Adafruit_BMP280::SAMPLING_X16, /* Pressure oversampling */
        Adafruit_BMP280::FILTER_X16, /* Filtering. */
        Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    const float temperature = bmp.readTemperature();
    const float pressure = bmp.readPressure() / 100.f;

    Serial.printf("Temperature = ");
    Serial.print(temperature);
    Serial.println(" *C");

    Serial.printf("Pressure = ");
    Serial.print(pressure);
    // Serial.println(" %RH");

    // connect to adafruit io
    connect();
    publish(temperature, pressure);
    disconnect();

#ifdef GOTO_SLEEP
    // delay(20*1000);
    Serial.println("Put ESP8266 in sleep mode");
    ESP.deepSleep(sleepTimeS * 1000000);
#endif
}

void loop()
{
}

// connect to adafruit io via MQTT
void connect()
{
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(F("."));
    }
    Serial.println();

    Serial.println(F("WiFi connected"));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());

    Serial.print(F("Connecting to Adafruit IO... "));

    int8_t ret;
    while ((ret = mqtt.connect()) != 0) {
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println(F("Retrying MQTT connection in 5 seconds..."));
        mqtt.disconnect();
        delay(5000); // wait 5 seconds
    }

    Serial.println(F("Adafruit IO Connected!"));
}

void disconnect()
{
    mqtt.disconnect();
    WiFi.disconnect();
}

void publish(float temperature_data, float pressure_data)
{
    // Setup feeds for temperature & humidity
    Adafruit_MQTT_Publish tfeed = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/Temperature");
    Adafruit_MQTT_Publish pfeed = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/Pressure");

    // Publish data
    if (!tfeed.publish(temperature_data))
        Serial.println(F("Failed to publish temperature"));
    else
        Serial.println(F("Temperature published!"));

    if (!pfeed.publish(pressure_data))
        Serial.println(F("Failed to publish pressure"));
    else
        Serial.println(F("Pressure published!"));

    Serial.println();
}
