#include <WiFi.h>
#include <AsyncMqttClient.h>

#include <Conf.h>
#include <Var.h>

// #define WIFI_SSID "Dialog 4G"
// #define WIFI_PASSWORD "212FFAL1LAN"
// #define MQTT_BROKER_IP "192.168.8.100" // IP address of the Docker host
// #define MQTT_PORT 1883

// // MQTT topics
// const char* intTopic = "esp32/data/int"; // Topic for integer data

// AsyncMqttClient mqttClient;
// TimerHandle_t mqttReconnectTimer;
// TimerHandle_t wifiReconnectTimer;

// int count = 1; // Initialize count to 1

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("WiFi connected");
      connectToMqtt();
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0);
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void setup() {
  Serial.begin(115200);

  // Set up WiFi and MQTT reconnect timers
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);
  
  mqttClient.onConnect([](bool sessionPresent) {
    // Connected to MQTT, no additional messages
  });
  
  mqttClient.setServer(MQTT_BROKER_IP, MQTT_PORT);

  connectToWifi(); // Initiate Wi-Fi connection
}

void loop() {
  if (mqttClient.connected() && count <= 50) {
    // Send integer data
    String intData = String(count);
    mqttClient.publish(intTopic, 2, true, intData.c_str());
    Serial.println(intData); // Only printing the number

    count++;
    delay(1000);
  }
}
