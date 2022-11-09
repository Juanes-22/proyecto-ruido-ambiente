/*************************************************************************
   TinyZero Microphone dB SPL meter IoT Project:
   This program uses the TinyZero with a microphone and
   connects to Ubidots via MQTT where messages get published periodically

   The circuit:
   - TinyZero, DF Sound Level Meter
 ************************************************************************/

#include <PubSubClient.h>
#include <WiFi101.h>
#include <Wire.h>
#include <Wireling.h>
#include <stdio.h>
#include <avr/dtostrf.h>

#if defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#else
#define SerialMonitorInterface Serial
#endif

#include "config.h"

/* Pin assignments */
const uint8_t mic_pin = A0;

/* MQTT */
const char mqtt_broker[] = "192.168.10.100";
const char mqtt_username[] = "juan";
const char mqtt_password[] = "2201";
const char mqtt_topic[] = "sala/sensor1";

unsigned long publish_millis = 0;

/* Initializate objects */
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

/****************************************
   Main Functions
 ****************************************/

void setup()
{
  Wire.begin();
  Wireling.begin();

  SerialMonitorInterface.begin(115200);
  WiFi.setPins(8, 2, A3, -1); // very important for Tinyduino
  while(!SerialMonitorInterface);

  connectToWiFi(WIFI_SSID, WIFI_PASSWORD); // WiFi Connection

  mqtt_client.setServer(mqtt_broker, 1883); // Ubidots MQTT broker

  publish_millis = millis();
}

void loop()
{
  static unsigned int last_db_value = 0;
  static unsigned int last_published_value = 0;

  float db_value = getSoundLevelReading();

  // Current value is different to previous value
  if (db_value != last_db_value)
  {
    last_db_value = db_value;
  }

  // Subscribes to get the value of the sound_level variable in the TinyZero device
  if (!mqtt_client.connected())
  {    
    reconnect();
  }

  /* MQTT publish */
  if (abs(millis() - publish_millis) > MQTT_PUBLISH_FREQUENCY)
  {
    static char str_sound_level[6];
    static char mqtt_payload[70];

    // 4 is mininum width, 2 is precision; db value is copied onto str_sound_level
    dtostrf(db_value, 4, 2, str_sound_level);

    sprintf(mqtt_payload, "%s", "");                                // cleans the payload content
    sprintf(mqtt_payload, "{\"%s\": ", MQTT_VARIABLE_LABEL);        // adds the variable label    
    sprintf(mqtt_payload, "%s %s}", mqtt_payload, str_sound_level); // adds the value and closes the dictionary brackets
    SerialMonitorInterface.println(mqtt_payload);

    mqtt_client.publish(mqtt_topic, mqtt_payload);  // publish variable
    publish_millis = millis();
  }
  
  mqtt_client.loop();
}

/****************************************
   Auxiliar Functions
 ****************************************/

int getSoundLevelReading()
{
  float voltage, db_value;

  voltage = analogRead(mic_pin) / 1023.0 * VREF;
  db_value = voltage * 50.0;    // convert voltage to decibel value
  
  return db_value;
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    SerialMonitorInterface.println("Attempting MQTT connection...");

    // Attempt to connect
    if (mqtt_client.connect(MQTT_CLIENT_NAME, mqtt_username, mqtt_password)) {
      SerialMonitorInterface.println("connected");
    } else {
      SerialMonitorInterface.print("failed, rc=");
      SerialMonitorInterface.print(mqtt_client.state());
      SerialMonitorInterface.println(" try again in 2 seconds");
      
      delay(2000);  // wait 2 seconds before retrying
    }
  }
}

void connectToWiFi(char *ssid, char *password)
{
  // Attempt to connect to WiFi network:
  SerialMonitorInterface.print("Connecting WiFi: ");
  SerialMonitorInterface.println(ssid);

  // Connect to WiFi, and loop until connection is secured
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    SerialMonitorInterface.print(".");
    delay(500);
  }

  // Print out the local IP address
  IPAddress ip = WiFi.localIP();
  SerialMonitorInterface.println("");
  SerialMonitorInterface.println("WiFi connected");
  SerialMonitorInterface.println("IP address: ");
  SerialMonitorInterface.println(ip);
}
