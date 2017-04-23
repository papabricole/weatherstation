#include "SHT21.h"
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>

// don't forget to ignore all changes to the following file using:
// git update-index --skip-worktree credential.h 
#include "credential.h"

// Time to sleep (in seconds):
const unsigned int sleepTimeS = 60 * 60;

// WiFi parameters
#define WLAN_SSID MY_WLAN_SSID
#define WLAN_PASS MY_WLAN_PASS

// Adafruit IO
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME MY_AIO_USERNAME
#define AIO_KEY MY_AIO_KEY

// SHT21 sensor
SHT21 SHT21;

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Functions
void connect();
void publish(float temperature, float humidity);
void disconnect();

void setup()
{
    // Connect D0 to RST to wake up
    pinMode(D0, WAKEUP_PULLUP);

    SHT21.begin();
    const float temperature = SHT21.getTemperature();
    const float humidity = SHT21.getHumidity();
    SHT21.reset();

    Serial.begin(115200);
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" *C");

    Serial.print("Humidity = ");
    Serial.print(humidity);
    Serial.println(" %RH");

    // connect to adafruit io
    connect();
    publish(temperature, humidity);
    disconnect();

    // delay(20*1000);
    Serial.println("Put ESP8266 in sleep mode");
    ESP.deepSleep(sleepTimeS * 1000000);
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
        switch (ret) {
        case 1:
            Serial.println(F("Wrong protocol"));
            break;
        case 2:
            Serial.println(F("ID rejected"));
            break;
        case 3:
            Serial.println(F("Server unavail"));
            break;
        case 4:
            Serial.println(F("Bad user/pass"));
            break;
        case 5:
            Serial.println(F("Not authed"));
            break;
        case 6:
            Serial.println(F("Failed to subscribe"));
            break;
        default:
            Serial.println(F("Connection failed"));
            break;
        }
        Serial.println(F("Retrying connection..."));
        if (ret >= 0)
            mqtt.disconnect();

        delay(5000);
    }

    Serial.println(F("Adafruit IO Connected!"));
}

void disconnect()
{
    mqtt.disconnect();
    WiFi.disconnect();
}

void publish(float temperature_data, float humidity_data)
{
    // Setup feeds for temperature & humidity
    Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
    Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");

    // Publish data
    if (!temperature.publish(temperature_data))
        Serial.println(F("Failed to publish temperature"));
    else
        Serial.println(F("Temperature published!"));

    if (!humidity.publish(humidity_data))
        Serial.println(F("Failed to publish humidity"));
    else
        Serial.println(F("Humidity published!"));

    Serial.println();
}
