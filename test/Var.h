// MQTT topics
const char* intTopic = "esp32/data/int"; // Topic for integer data

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

int count = 1; // Initialize count to 1