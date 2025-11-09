/*
ESP32 DHT11 Web Portal (Open Access Point, no password, no login)
Files contained here:
 - ESP32_DHT11_Portal.ino  (main sketch)
 - README (below inside this file)
*/

#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// ----- CONFIG -----
const char* apName = "ESP_DHT_PORTAL"; // Open AP (no password)

#define DHTPIN 14    // <-- default used pin. DO NOT USE GPIO34 (input-only). See README.
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

unsigned long lastRead = 0;
float lastTemp = NAN;
float lastHum = NAN;

void handleRoot() {
  String page = R"rawliteral(
<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>ESP DHT11 Portal</title>
  <style>
    body{font-family:Arial;margin:12px;background:#f2f2f2}
    .card{background:#fff;padding:20px;border-radius:10px;box-shadow:0 6px 18px rgba(0,0,0,0.08);}
    .row{display:flex;gap:12px;align-items:center}
  </style>
</head>
<body>
  <div class="card">
    <h2>DHT11 Live Readings</h2>
    <div class="row">
      <div>
        <strong>Temperature:</strong>
        <div id="temp">-- °C</div>
      </div>
      <div>
        <strong>Humidity:</strong>
        <div id="hum">-- %</div>
      </div>
    </div>
    <hr>
    <div>
      <strong>Raw details:</strong>
      <pre id="raw">--</pre>
    </div>
  </div>
  <script>
    async function fetchSensor(){
      try{
        const r = await fetch('/sensor');
        const j = await r.json();
        if(j.ok){
          document.getElementById('temp').innerText = j.temperature.toFixed(1) + ' °C';
          document.getElementById('hum').innerText = j.humidity.toFixed(1) + ' %';
          document.getElementById('raw').innerText = JSON.stringify(j, null, 2);
        } else {
          document.getElementById('raw').innerText = 'Error: ' + j.error;
        }
      } catch(e){
        document.getElementById('raw').innerText = 'Fetch error: ' + e;
      }
    }
    setInterval(fetchSensor, 2000);
    fetchSensor();
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", page);
}

void handleSensor() {
  unsigned long now = millis();
  if (now - lastRead > 1500) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    lastHum = h;
    lastTemp = t;
    lastRead = now;
  }

  String json;
  if (isnan(lastTemp) || isnan(lastHum)) {
    json = "{\"ok\":false, \"error\":\"read_failed\"}";
  } else {
    json = String("{\"ok\":true,\"temperature\":") + String(lastTemp,2) + ",\"humidity\":" + String(lastHum,2) + ",\"since\":" + String(lastRead) + "}";
  }
  server.send(200, "application/json", json);
}

void setup(){
  Serial.begin(115200);
  delay(100);
  dht.begin();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(apName);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP started. Connect to ssid: "); Serial.println(apName);
  Serial.print("AP IP: "); Serial.println(myIP);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/sensor", HTTP_GET, handleSensor);
  server.begin();
}

void loop(){
  server.handleClient();
}
