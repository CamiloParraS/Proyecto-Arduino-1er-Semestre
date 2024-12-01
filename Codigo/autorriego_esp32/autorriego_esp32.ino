#include <WiFi.h>
#include <WebServer.h>

// Credenciales WiFi - Deben ser remplasadas
const char* ssid = "Jhenny";
const char* password = "camilosanchez11385";

// configuracion de servidor web
WebServer server(80);

// Definir pines
#define SENSOR_PIN_1 34    // Sensor de Humedad 1
#define RELAY_PIN_1 25     // Relé 1

#define SENSOR_PIN_2 39    // Sensor de Humedad 2
#define RELAY_PIN_2 26     // Relé 2

// umbrales de humedad
int umbralHumedadSensor1 = 2500;  
int umbralHumedadSensor2 = 2500;  

// Variables para guardar el estado de los componentes
int sensorValue1 = 0;
int sensorValue2 = 0;
bool bomba1Estado = false;
bool bomba2Estado = false;

void setup() {
  Serial.begin(115200);

  // Configura pines de el rele
  pinMode(RELAY_PIN_1, OUTPUT);  
  digitalWrite(RELAY_PIN_1, HIGH);  // Rele empieza apagado

  pinMode(RELAY_PIN_2, OUTPUT);  
  digitalWrite(RELAY_PIN_2, HIGH);  // Rele empieza apagad

  // Conectarse al WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());

  // Definir la ruta de el servidor
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  
  // Arranca el servidor
  server.begin();
}

void loop() {
  // Gestionar solicitudes de el cliente
  server.handleClient();

  // Lee y procesa el primer sensor
  sensorValue1 = analogRead(SENSOR_PIN_1);  
  
  if (sensorValue1 > umbralHumedadSensor1) {  
    digitalWrite(RELAY_PIN_1, HIGH);  
    bomba1Estado = true;
  } else {                        
    digitalWrite(RELAY_PIN_1, LOW);  
    bomba1Estado = false;
  }

  // Lee y procesa el segundo sensor
  sensorValue2 = analogRead(SENSOR_PIN_2);  

  if (sensorValue2 > umbralHumedadSensor2) {  
    digitalWrite(RELAY_PIN_2, HIGH);  
    bomba2Estado = true;
  } else {                        
    digitalWrite(RELAY_PIN_2, LOW);  
    bomba2Estado = false;
  }

  delay(3000);  
}

// Disenio pagina web
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
                "<h2>Planta 1</h2>"
                "<p>Valor de Humedad: <span id='humedad1'>" + String(sensorValue1) + "</span></p>"
                "<p>Estado de Bomba: <span id='bomba1' class='status-" + (bomba1Estado ? "on'>Encendida" : "off'>Apagada") + "</span></p>"
                "</div>"
                "<div class='datos-sensor'>"
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