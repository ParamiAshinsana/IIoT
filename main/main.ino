#include <WiFi.h>                // Include the ESP32 WiFi library
#include <AsyncMqttClient.h>      // Include the AsyncMqttClient library
#include <Conf.h>                 // Your configuration file

// const char* ssid = "Dialog 4G";
// const char* password = "212FFAL1LAN";
// const char* mqtt_server = "192.168.8.101";

// const char* birthTopic = "device/birthMessage";

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting setup...");

  // Initialize Wi-Fi
  WiFi.onEvent(WiFiEvent);
  connectToWifi();

  // Initialize MQTT client
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.setServer(mqtt_server, 1883);

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  Serial.println("Setup complete");
}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:  // Corrected to the updated event name
      Serial.println("Wi-Fi connected!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
      
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:  // Corrected to the updated event name
      Serial.println("Wi-Fi disconnected, retrying...");
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  mqttClient.publish(birthTopic, 1, true, "Device is now online!");  // Publish birth message with retain flag set to true
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void loop() {
  // No need to handle MQTT loop as AsyncMqttClient handles it
}
