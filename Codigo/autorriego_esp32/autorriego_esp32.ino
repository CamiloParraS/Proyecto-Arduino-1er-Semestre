#include <WiFi.h>
#include <WebServer.h>

// WiFi Credentials - REPLACE WITH YOUR OWN
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Web Server Configuration
WebServer server(80);

// Pin Definitions
#define SENSOR_PIN_1 34    // Sensor de Humedad 1
#define RELAY_PIN_1 25     // Relé 1

#define SENSOR_PIN_2 39    // Sensor de Humedad 2
#define RELAY_PIN_2 26     // Relé 2

// Humidity Thresholds
int umbral_humedad_sensor_1 = 2500;  
int umbral_humedad_sensor_2 = 2500;  

// Variables to store current sensor states
int sensorValue1 = 0;
int sensorValue2 = 0;
bool bomba1Estado = false;
bool bomba2Estado = false;

void setup() {
  Serial.begin(115200);

  // Configure relay pins
  pinMode(RELAY_PIN_1, OUTPUT);  
  digitalWrite(RELAY_PIN_1, HIGH);  // Relay initially off

  pinMode(RELAY_PIN_2, OUTPUT);  
  digitalWrite(RELAY_PIN_2, HIGH);  // Relay initially off

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Define web server routes
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  
  // Start server
  server.begin();
}

void loop() {
  // Handle client requests
  server.handleClient();

  // Read and process the first sensor
  sensorValue1 = analogRead(SENSOR_PIN_1);  
  
  if (sensorValue1 > umbral_humedad_sensor_1) {  
    digitalWrite(RELAY_PIN_1, HIGH);  
    bomba1Estado = true;
  } else {                        
    digitalWrite(RELAY_PIN_1, LOW);  
    bomba1Estado = false;
  }

  // Read and process the second sensor
  sensorValue2 = analogRead(SENSOR_PIN_2);  

  if (sensorValue2 > umbral_humedad_sensor_2) {  
    digitalWrite(RELAY_PIN_2, HIGH);  
    bomba2Estado = true;
  } else {                        
    digitalWrite(RELAY_PIN_2, LOW);  
    bomba2Estado = false;
  }

  delay(3000);  
}

// Web page handler for root URL
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>"
                "<meta charset='UTF-8'>"
                "<title>Sistema de Autorriego con ESP32</title>"
                "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                "<style>"
                "body{font-family:Arial;text-align:center;max-width:600px;margin:0 auto;padding:20px;}"
                "h1{color:#2c3e50;}"
                ".sensor-data{background:#f1f1f1;padding:15px;margin:10px 0;border-radius:5px;}"
                ".status-on{color:green;}"
                ".status-off{color:red;}"
                "</style>"
                "</head><body>"
                "<h1>Sistema de Autorriego con ESP32</h1>"
                "<div class='sensor-data'>"
                "<h2>Planta 1 1</h2>"
                "<p>Valor de Humedad: <span id='humedad1'>" + String(sensorValue1) + "</span></p>"
                "<p>Estado de Bomba: <span id='bomba1' class='status-" + (bomba1Estado ? "on'>Encendida" : "off'>Apagada") + "</span></p>"
                "</div>"
                "<div class='sensor-data'>"
                "<h2>Planta 2</h2>"
                "<p>Valor de Humedad: <span id='humedad2'>" + String(sensorValue2) + "</span></p>"
                "<p>Estado de Bomba: <span id='bomba2' class='status-" + (bomba2Estado ? "on'>Encendida" : "off'>Apagada") + "</span></p>"
                "</div>"
                "<script>"
                "function actualizarDatos() {"
                "  fetch('/status')"
                "    .then(response => response.json())"
                "    .then(data => {"
                "      document.getElementById('humedad1').textContent = data.humedad1;"
                "      document.getElementById('humedad2').textContent = data.humedad2;"
                "      document.getElementById('bomba1').textContent = data.bomba1Estado ? 'Encendida' : 'Apagada';"
                "      document.getElementById('bomba1').className = 'status-' + (data.bomba1Estado ? 'on' : 'off');"
                "      document.getElementById('bomba2').textContent = data.bomba2Estado ? 'Encendida' : 'Apagada';"
                "      document.getElementById('bomba2').className = 'status-' + (data.bomba2Estado ? 'on' : 'off');"
                "    });"
                "}"
                "setInterval(actualizarDatos, 3000);"
                "</script>"
                "</body></html>";
  
  server.send(200, "text/html", html);
}

// JSON status endpoint
void handleStatus() {
  String jsonResponse = "{\"humedad1\":" + String(sensorValue1) + 
                        ",\"humedad2\":" + String(sensorValue2) + 
                        ",\"bomba1Estado\":" + (bomba1Estado ? "true" : "false") + 
                        ",\"bomba2Estado\":" + (bomba2Estado ? "true" : "false") + "}";
  
  server.send(200, "application/json", jsonResponse);
}