#ifndef CONFIG_H
#define CONFIG_H

#define VREF 3.3  // ADC voltage reference

/* WiFi Config */
#define WIFI_SSID     ""  // Put here your Wi-Fi SSID
#define WIFI_PASSWORD ""   // Put here your Wi-Fi password

/* MQTT config */
#define MQTT_PUBLISH_FREQUENCY   500              // MQTT publish rate in milliseconds
#define MQTT_CLIENT_NAME        "mqttx_d85bc32b"  // MQTT client name
#define MQTT_VARIABLE_LABEL     "db"              // MQTT variable label

/* Debug config */
#define DEBUG_MQTT      // MQTT debug enable
#define DEBUG_MIC       // Microphone debug enable

#endif /* CONFIG_H */
