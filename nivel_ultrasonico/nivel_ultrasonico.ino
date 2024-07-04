#include <NewPing.h>

// Definimos los pines
#define TRIGGER_PIN 33
#define ECHO_PIN 32
#define MAX_DISTANCE 400 // Distancia máxima a medir (en cm)

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

}
