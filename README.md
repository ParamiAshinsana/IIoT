ESP32 WiFi & 4G Communication with MQTT and OTA Support
============================
This project is focused on implementing robust communication switching between WiFi and 4G on an ESP32 module, with an emphasis on testing MQTT capabilities and Firmware-Over-The-Air (FOTA) updates. The goal is to enable seamless switching between WiFi and 4G based on availability and optimize MQTT functionalities over both connection methods, including security via SSL/TLS.
### directory brief

    .               
    ├── main                            # Main folder include all the files
    │   ├── main.ino     
    │   ├── config.h                    # All the configurations, such as wifi, mqtt, pin def.
    │   └── README.md                   # Doc     
    |── flows.json                      # node-red flows
    |── README.md

### To Do
- [ ] Communication method switch WiFi & 4G based on availability
- [ ] Checking AP & STA mode on ESP32 at the same time
- [ ] MQTT over WiFi/ 4G
    - [ ] QoS 2, will, birth, retain messages
    - [ ] Testing what is the max packet size can be sent by choosen MQTT library
    - [ ] Testing on Sending long string in single json payload
    - [ ] SSL/TLS //server needed, so note down the steps
    - [ ] Test & Verify
- [ ] Firmware-Over-The-Air (FOTA)
- [ ] Soft reset 
- [ ] Hard reset
