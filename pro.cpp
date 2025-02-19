// #include <WiFi.h>
// #include <WebServer.h>
// #include "DHT.h"

// // Wi-Fi credentials
// const char* ssid = "YOUR_SSID";
// const char* password = "YOUR_PASSWORD";

// // DHT Sensor settings
// #define DHTPIN 4         // Pin connected to DHT sensor
// #define DHTTYPE DHT22    // Change to DHT11 if you're using that sensor
// DHT dht(DHTPIN, DHTTYPE);

// // Create web server on port 80
// WebServer server(80);

// HTML UI with placeholders for sensor data
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>ESP32 Sensor Dashboard</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; background-color: #f2f2f2; }
    .container { background: #fff; padding: 20px; border-radius: 10px; width: 300px; margin: 50px auto; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }
    h2 { color: #333; }
    .value { font-size: 2em; color: #007BFF; margin: 10px 0; }
  </style>
</head>
<body>
  <div class="container">
    <h2>Sensor Readings</h2>
    <p>Temperature: <span class="value" id="temp">-- °C</span></p>
    <p>Humidity: <span class="value" id="hum">-- %</span></p>
  </div>

  <script>
    function fetchData() {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          document.getElementById('temp').textContent = data.temperature + ' °C';
          document.getElementById('hum').textContent = data.humidity + ' %';
        });
    }
    setInterval(fetchData, 5000); // Fetch data every 5 seconds
    window.onload = fetchData;    // Fetch data on page load
  </script>
</body>
</html>
)rawliteral";

// Handles the root URL and serves the HTML page
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// Handles the /data endpoint and returns sensor data in JSON format
void handleData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    server.send(500, "application/json", "{\"error\":\"Failed to read sensor\"}");
    return;
  }

  String json = "{\"temperature\":" + String(temperature, 2) + ",\"humidity\":" + String(humidity, 2) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}
