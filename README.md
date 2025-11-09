## ESP32 DHT11 Web Portal (Open AP, no login) 🎛️🌐


What this does
--------------
- Creates an open (no password) WiFi access point named: ESP_DHT_PORTAL 🔓
- Serves a simple webpage showing live DHT11 readings (temperature 🌡️ and humidity 💧)
- The page updates every 2 seconds using AJAX fetch from /sensor endpoint ⏱️


Important note about GPIO34
---------------------------
- GPIO34-39 on the ESP32 are INPUT-ONLY pins. DHT11 requires bidirectional signaling,
so it will NOT work on GPIO34. This code uses GPIO14 by default. ⚠️


Wiring
------
DHT11 module (typical 3-pin: VCC, DATA, GND):
- VCC -> 3.3V 🔌
- DATA -> GPIO14 🔗
- GND -> GND 🟫
- Add a 10K pull-up resistor between DATA and VCC if not built-in. 🛠️


Libraries required
------------------
- Install "DHT sensor library" (by Adafruit) 📚


How to use
----------
1. Upload sketch to ESP32. ⬆️
2. Connect your phone/PC to WiFi SSID: ESP_DHT_PORTAL (no password). 📶
3. Open browser to http://192.168.4.1/ 🌍
4. Observe live temperature and humidity readings. 👀

---

[![BuyMeACoffee](https://img.shields.io/badge/Buy%20Me%20a%20Coffee-ffdd00?style=for-the-badge&logo=buy-me-a-coffee&logoColor=black)](https://buymeacoffee.com/rohankini) [![PayPal](https://img.shields.io/badge/PayPal-00457C?style=for-the-badge&logo=paypal&logoColor=white)](https://paypal.me/RohanKinirk) 
