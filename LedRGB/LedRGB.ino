// Definir el pin donde está conectado el LED
const int ledPin = 27;
const int ledPin2 = 26;
// Configurar el tiempo de encendido y apagado en milisegundos
const int delayTime = 1000; // 1000 ms = 1 segundo

void setup() {
  // Configurar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  // Encender el LED
  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin2, HIGH);
  // Esperar durante el tiempo definido
  delay(delayTime);
  
  // Apagar el LED
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
  // Esperar durante el tiempo definido
  delay(delayTime);
}
