   #include <WiFi.h>
   #include <HTTPClient.h>
   #include <Update.h>

   #include <Conf.h>

  //  // Wi-Fi credentials
  //  const char* ssid = "Dialog 4G";
  //  const char* password = "212FFAL1LAN";

  //  // Link to the firmware file on GitHub
  //  const char* firmwareURL = "https://github.com/ParamiAshinsana/ESP32-OTA/raw/67962647a5c360e063cf43fc7ce56a530ed5ea16/BasicOTA_copy_20241105191832.ino.bin";

   void setup() {
    Serial.begin(115200);
    Serial.println("Connecting to WiFi...");
    
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");

    // Check for an update
    checkForUpdate();
}

void loop() {
    Serial.println("Device running...V1");
    delay(5000);  // Replace with actual device code
}

void checkForUpdate() {
    Serial.println("Checking for firmware update...");

    HTTPClient http;
    http.begin(firmwareURL);
    int httpCode = http.GET();

    // Check the HTTP response code
    if (httpCode == 200) { // HTTP OK
        Serial.println("HTTP Response Code: 200 - OK");
        
        int contentLength = http.getSize();
        Serial.println("Content Length: " + String(contentLength));

        // Check if the update can begin
        if (contentLength > 0) {
            bool canBegin = Update.begin(contentLength);

            if (canBegin) {
                Serial.println("Starting OTA update...");

                WiFiClient* client = http.getStreamPtr();
                size_t written = Update.writeStream(*client);

                if (written == contentLength) {
                    Serial.println("Firmware written successfully");
                } else {
                    Serial.println("Written only: " + String(written) + "/" + String(contentLength));
                }

                // End the update process
                if (Update.end()) {
                    Serial.println("Update completed! Rebooting...");
                    ESP.restart();
                } else {
                    Serial.println("Update failed. Error #: " + String(Update.getError()));
                }
            } else {
                Serial.println("Not enough space for OTA update");
            }
        } else {
            Serial.println("Invalid content length");
        }
    } else {
        Serial.println("Failed to fetch firmware. HTTP code: " + String(httpCode));
    }

    // End HTTP client connection
    http.end();
}
