/*
 * PRT7_Emisor_Simple.ino
 * Emisor simple de protocolo PRT-7 para Arduino
 * Compatible con Arduino Uno, Nano, ESP32
 */

#if defined(__has_include)
#  if __has_include(<Arduino.h>)
#    include <Arduino.h>
#  endif
#endif

#ifndef ARDUINO
#include <stdint.h>
struct SerialMock {
  SerialMock& begin(long) { return *this; }
  SerialMock& println() { return *this; }
  template<typename T> SerialMock& println(const T&) { return *this; }
  template<typename T> SerialMock& print(const T&) { return *this; }
};
static SerialMock Serial;
static inline void delay(unsigned long) {}
static inline unsigned long millis() { return 0; }
#endif

// Configuracion
int delayEntreTramas = 3000;  // 3 segundos entre tramas
int baudRate = 115200;        // Velocidad serial

// Secuencia de tramas a enviar
char tramas[][6] = {
  "L,H",    // -> H
  "L,O",    // -> O  
  "L,L",    // -> L
  "M,2",    // Rotar +2
  "L,A",    // -> C (A rotado +2)
  "L, ",    // -> espacio
  "L,W",    // -> Y (W rotado +2)
  "M,-2",   // Rotar -2 (volver al original)
  "L,O",    // -> O
  "L,R",    // -> R
  "L,L",    // -> L
  "L,D"     // -> D
};

int totalTramas = 12;             // Numero total de tramas
int indice = 0;                   // Indice actual
unsigned long ultimoEnvio = 0;   // Tiempo del ultimo envio
int secuenciaCompletada = 0;      // Bandera de completado (0=false, 1=true)

void setup() {
  // Inicializar comunicacion serial
  Serial.begin(baudRate);
  
  // Dar tiempo para que se establezca la conexion
  delay(2000);
  
  // Mensaje de bienvenida
  Serial.println("=====================================");
  Serial.println("    EMISOR PRT-7 - VERSION SIMPLE");
  Serial.println("=====================================");
  Serial.println();
  Serial.println("Iniciando transmision en 3 segundos...");
  Serial.println();
  
  delay(3000);
  ultimoEnvio = millis();
}

void loop() {
  unsigned long tiempoActual = millis();
  
  // Verificar si es tiempo de enviar la siguiente trama
  if (secuenciaCompletada == 0 && 
      (tiempoActual - ultimoEnvio >= delayEntreTramas)) {
    
    if (indice < totalTramas) {
      // Enviar la trama actual
      Serial.println(tramas[indice]);
      
      indice++;
      ultimoEnvio = tiempoActual;
      
    } else {
      // Secuencia completada
      secuenciaCompletada = 1;
      Serial.println("=== SECUENCIA COMPLETADA ===");
      Serial.println("Todas las tramas han sido enviadas.");
      Serial.println("Reinicie el Arduino para repetir.");
      Serial.println("=============================");
    }
  }
  
  delay(100); // Pequena pausa
}