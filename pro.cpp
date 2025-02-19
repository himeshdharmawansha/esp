#include <WiFi.h>
#include <ESP32WebServer.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "QCom-2025";
const char* password = "ucsc@123";

// DHT sensor setup
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Initialize the server
ESP32WebServer server(80);

// HTML webpage
String htmlPage = "String htmlPage = "<html>\
<head>\
  <title>ESP32 Sensor Dashboard</title>\
  <style>\
    /* Add your CSS here as before... */\
  </style>\
</head>\
<body>\
    <div class=\"main\">\
        <section class=\"leftside\">\
            <h1 style=\"font-family: 'Arial', sans-serif; color:#007BFF; font-size: 3em; border-radius: 10px;\">\
                Binfinity\
              </h1>\
            <img class=\"container\" src=\"https://img.freepik.com/free-vector/recycle-bin-ecological-sustainability-icon-isolated_24640-134036.jpg?t=st=1739950412~exp=1739954012~hmac=c61e5cb2cfe6f218d929cc0538b69c214270f7e5cf76c76240e04982a08e97a9&w=360\" alt=\"Recycle Bin\">\
        </section>\
        <section class=\"rightside\">\
            <div class=\"container\">\
                <h2>Bin Filling Status</h2>\
                <p>Bin Filling: <span class=\"value\" id=\"distance\">-- %</span></p>\
                <p id=\"status\" class=\"status\">Status: --</p>\
            </div>\
            <div class=\"container\">\
                <h2>Environmental Data</h2>\
                <p>Temperature: <span class=\"value\" id=\"temp\">%TEMP%</span></p>\
                <p>Humidity: <span class=\"value\" id=\"hum\">%HUMIDITY%</span></p>\
            </div>\
            <div class=\"container message\" id=\"fullMessage\">\
                <p>The trash bin is full (90%+).</p>\
            </div>\
        </section>\
    </div>\
  <script>\
    const maxDistance = 100;  // Max distance (empty bin)\
    const minDistance = 10;   // Min distance (full bin)\
    let temperature = %TEMP%;\
    let humidity = %HUMIDITY%;\
    let distance = 80; // Example distance (adjust this value for testing)\
\
    function calculateBinPercentage(distance) {\
        return 100 - distance;\
    }\
\
    function fetchData() {\
        document.getElementById('temp').textContent = temperature + ' °C';\
        document.getElementById('hum').textContent = humidity + ' %';\
\
        // Get the bin filling percentage\
        const fillingPercentage = calculateBinPercentage(distance);\
        document.getElementById('distance').textContent = fillingPercentage + ' %';\
\
        const statusElement = document.getElementById('status');\
        if (humidity > 65) {\
            statusElement.textContent = 'Status: trash is rotten!';\
            statusElement.className = 'status danger';\
            const fullMessage = document.getElementById('fullMessage');\
            fullMessage.style.display = 'block';\
        } else {\
            if (fillingPercentage < 25) {\
                statusElement.textContent = 'Status: Safe';\
                statusElement.className = 'status safe';\
            } else if (fillingPercentage < 50) {\
                statusElement.textContent = 'Status: Caution';\
                statusElement.className = 'status warning';\
            } else if (fillingPercentage < 75) {\
                statusElement.textContent = 'Status: Danger';\
                statusElement.className = 'status danger';\
            } else {\
                statusElement.textContent = 'Status: Full';\
                statusElement.className = 'status danger';\
                const fullMessage = document.getElementById('fullMessage');\
                fullMessage.style.display = 'block';\
            }\
        }\
    }\
    setInterval(fetchData, 5000); // Update every 5 seconds\
    window.onload = fetchData;    // Load initial data\
  </script>\
</body>\
</html>";
";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  dht.begin();

  // Serve the webpage
  server.on("/", HTTP_GET, []() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    if (isnan(temp) || isnan(humidity)) {
      server.send(500, "text/plain", "Failed to read sensor data");
      return;
    }
    String page = htmlPage;
    page.replace("%TEMP%", String(temp));
    page.replace("%HUMIDITY%", String(humidity));
    server.send(200, "text/html", page);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
