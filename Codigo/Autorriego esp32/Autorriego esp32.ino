#define SENSOR_PIN_1 34    // Sensor de Humedad 1
#define RELAY_PIN_1 25     // Relé 1

#define SENSOR_PIN_2 39    // Sensor de Humedad 2
#define RELAY_PIN_2 26     // Relé 2

int umbral_humedad_sensor_1 = 2500;  
int umbral_humedad_sensor_2 = 2500;  

void setup() {
  Serial.begin(115200);

  // Configurar los pines para el primer sensor y relé
  pinMode(RELAY_PIN_1, OUTPUT);  
  digitalWrite(RELAY_PIN_1, HIGH);  // Relé apagado inicialmente

  // Configurar los pines para el segundo sensor y relé
  pinMode(RELAY_PIN_2, OUTPUT);  
  digitalWrite(RELAY_PIN_2, HIGH);  // Relé apagado inicialmente
}

void loop() {
  // Leer y procesar el primer sensor
  int sensorValue1 = analogRead(SENSOR_PIN_1);  
  Serial.print("Humedad del suelo (Sensor 1): ");
  Serial.println(sensorValue1);

  if (sensorValue1 > umbral_humedad_sensor_1) {  
    digitalWrite(RELAY_PIN_1, HIGH);  
    Serial.println("Bomba 1 encendida porque el suelo está seco.");
  } else {                        
    digitalWrite(RELAY_PIN_1, LOW);  
    Serial.println("Bomba 1 apagada porque el suelo está suficientemente húmedo.");
  }

  // Leer y procesar el segundo sensor
  int sensorValue2 = analogRead(SENSOR_PIN_2);  
  Serial.print("Humedad del suelo (Sensor 2): ");
  Serial.println(sensorValue2);

  if (sensorValue2 > umbral_humedad_sensor_2) {  
    digitalWrite(RELAY_PIN_2, HIGH);  
    Serial.println("Bomba 2 encendida porque el suelo está seco.");
  } else {                        
    digitalWrite(RELAY_PIN_2, LOW);  
    Serial.println("Bomba 2 apagada porque el suelo está suficientemente húmedo.");
  }

  delay(3000);  
}
