#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <NewPing.h>


#define WIFI_SSID "SmartWifi2"
#define WIFI_PASSWORD "00420265426"
// Definimos los pines
#define TRIGGER_PIN 33
#define ECHO_PIN 32
#define MAX_DISTANCE 400 // Distancia máxima a medir (en cm)

//Bilbiotecas para influxDB
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

//Configuracion de Influx 
#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "bNiV9Lb74wxwcsg0G5nR98lcbBULK2OKZCwI5-mu2YTQ-_iqM6P-hcp-dGmZFtAAUYmWZJ1NPoYuVbuu8z0x9g=="
#define INFLUXDB_ORG "a7d277f0afe45106"
#define INFLUXDB_BUCKET "ESP32"

#define TZ_INFO "UTC-4"

Point sensor("wifi_status");


//Inicializacion del cliente influx, este maneja la comunicacion
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

float convertir(float dist){
  return ((-10.0)*(dist-13.0));
}

float getNivel(float dist){
  
  if((dist>=3.0)&&(dist<=13.0)){
    return convertir(dist);
  }else{
    if(dist<3.0){
      return convertir(3.0);
    }else{
      return convertir(13.0);
    }
  }


}


void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando sensor ultrasonico HC-SR04...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ");
  Serial.print(WIFI_SSID);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  //Sincronizacion (necesario)
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
   if (client.validateConnection()) {
      Serial.print("CConectado a InfluxDB: ");
      Serial.println(client.getServerUrl());
    } else {
      Serial.print("InfluxDB conexion fallida: ");
      Serial.println(client.getLastErrorMessage());
    }

}

void loop() {
  delay(500); // Pequeña pausa para no saturar el sensor
  unsigned int uS = sonar.ping(); // Hacemos una medición
  float distance = uS / US_ROUNDTRIP_CM; // Convertimos el tiempo a distancia
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Nivel: ");
  Serial.print(getNivel(distance));
  Serial.println("%");

  //Creacion punto de datos
  Point sensorData("ultrasonic_sensor");
  sensorData.addTag("device", "ESP32");
  sensorData.addField("distance", distance);
  sensorData.addField("level", getNivel(distance));

  // Enviar datos a InfluxDB
  if (!client.writePoint(sensorData)) {
    Serial.print("Error sending data to InfluxDB: ");
    Serial.println(client.getLastErrorMessage());
  } else {
    Serial.println("Data sent to InfluxDB successfully.");
  }
  

}