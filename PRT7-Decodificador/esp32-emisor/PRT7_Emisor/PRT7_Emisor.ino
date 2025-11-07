/**
 * @file PRT7_Emisor.ino
 * @brief Emisor de protocolo PRT-7 para Arduino/ESP32
 * @autor Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 * 
 * Este sketch implementa el emisor del protocolo PRT-7 que envia tramas
 * LOAD y MAP a traves del puerto serial para ser decodificadas por el
 * sistema receptor en la computadora.
 */

#if defined(__has_include)
#  if __has_include(<Arduino.h>)
#    include <Arduino.h>
#    define ARDUINO_ENV 1
#  endif
#endif
#include <ctype.h>

#ifndef ARDUINO_ENV
#include <stdint.h>
#include <cstring>
#include <cctype>

// Mock muy basico para que el linter de VS Code no marque errores
struct SerialMock {
  SerialMock& begin(long) { return *this; }
  SerialMock& println() { return *this; }
  template<typename T> SerialMock& println(const T&) { return *this; }
  template<typename T> SerialMock& print(const T&) { return *this; }
  int available() { return 0; }
  int read() { return -1; }
};
static SerialMock Serial;
static inline void delay(unsigned long) {}
static inline unsigned long millis() { return 0; }

// Stub minimo de String compatible con los usos de este archivo
class String {
public:
  String() { buf[0] = '\0'; }
  String(const char* s) { copyFrom(s); }
  String(const String& o) { copyFrom(o.c_str()); }
  String& operator=(const char* s) { copyFrom(s); return *this; }
  String& operator=(const String& o) { copyFrom(o.c_str()); return *this; }
  String operator+(const char* s) const { String r(buf); r.append(s); return r; }
  bool operator==(const char* s) const { return std::strcmp(buf, s) == 0; }
  void trim() {
    // quitar espacios y CRLF en extremos
    size_t start = 0; while (buf[start] && std::isspace((unsigned char)buf[start])) start++;
    size_t end = std::strlen(buf); while (end>start && std::isspace((unsigned char)buf[end-1])) end--;
    size_t n = end - start; if (start>0) std::memmove(buf, buf+start, n);
    buf[n] = '\0';
  }
  void toUpperCase() {
    for (size_t i=0;i<std::strlen(buf);++i) buf[i] = (char)std::toupper((unsigned char)buf[i]);
  }
  int length() const { return (int)std::strlen(buf); }
  char charAt(int i) const { return (i>=0 && (size_t)i<std::strlen(buf)) ? buf[i] : '\0'; }
  String substring(int start) const { if ((size_t)start>=std::strlen(buf)) return String(""); return String(buf+start); }
  void reserve(size_t) {}
  String& operator+=(char c) { size_t n=std::strlen(buf); if (n+1<sizeof(buf)) { buf[n]=c; buf[n+1]='\0'; } return *this; }
  const char* c_str() const { return buf; }
private:
  char buf[128];
  void copyFrom(const char* s) {
    if (!s) { buf[0]='\0'; return; }
    std::strncpy(buf, s, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';
  }
  void append(const char* s) {
    if (!s) return; size_t n=std::strlen(buf); size_t m=std::strlen(s); size_t cap=sizeof(buf)-1; size_t copy = (m>cap-n)? (cap-n): m; std::memcpy(buf+n, s, copy); buf[n+copy]='\0';
  }
};
#endif

// Configuracion del sistema
const unsigned long DELAY_ENTRE_TRAMAS = 2000; // 2 segundos entre tramas
const unsigned long BAUD_RATE = 9600;          // Velocidad del puerto serial

// Estados del emisor
enum EstadoEmisor {
  ESPERANDO_INICIO,
  ENVIANDO_SECUENCIA,
  ENVIANDO_MANUAL,
  PAUSADO
};

EstadoEmisor estadoActual = ESPERANDO_INICIO;

// Secuencia predefinida de ejemplo del protocolo PRT-7
const char* secuenciaPredefinida[] = {
  "L,H",    // Carga 'H'
  "L,O",    // Carga 'O' 
  "L,L",    // Carga 'L'
  "M,2",    // Rota +2 posiciones
  "L,A",    // Carga 'A' (se decodifica como 'C')
  "L, ",    // Carga espacio
  "L,W",    // Carga 'W' (se decodifica como 'Y')
  "M,-2",   // Rota -2 posiciones (vuelve al estado original)
  "L,O",    // Carga 'O'
  "L,R",    // Carga 'R'
  "L,L",    // Carga 'L'
  "L,D"     // Carga 'D'
};

const int TOTAL_TRAMAS = sizeof(secuenciaPredefinida) / sizeof(secuenciaPredefinida[0]);
int indiceActual = 0;
unsigned long tiempoUltimaTransmision = 0;

// Variables para modo manual
String bufferEntrada = "";
bool entradaCompleta = false;

/**
 * @brief Configuracion inicial del Arduino
 */
void setup() {
  // Inicializar comunicacion serial
  Serial.begin(BAUD_RATE);
  
  // Dar tiempo para establecer la conexion (compatible con placas sin USB nativo)
  delay(1000);
  
  // Mensaje de bienvenida
  mostrarBienvenida();
  mostrarMenu();
  
  // Inicializar variables
  indiceActual = 0;
  tiempoUltimaTransmision = millis();
  bufferEntrada.reserve(50); // Reservar memoria para el buffer
}

/**
 * @brief Bucle principal del Arduino
 */
void loop() {
  // Procesar entrada serial del usuario
  procesarEntradaSerial();
  
  // Ejecutar acción según el estado actual
  switch (estadoActual) {
    case ESPERANDO_INICIO:
      // No hacer nada, esperar comandos del usuario
      break;
      
    case ENVIANDO_SECUENCIA:
      ejecutarSecuenciaAutomatica();
      break;
      
    case ENVIANDO_MANUAL:
      // El modo manual se maneja en procesarEntradaSerial()
      break;
      
    case PAUSADO:
      // Sistema pausado, esperar comandos
      break;
  }
  
  delay(10); // Pequeña pausa para no saturar el procesador
}

/**
 * @brief Muestra el mensaje de bienvenida
 */
void mostrarBienvenida() {
  Serial.println("==============================================");
  Serial.println("       EMISOR DE PROTOCOLO PRT-7");
  Serial.println("           Arduino/ESP32 v1.0");
  Serial.println("==============================================");
  Serial.println();
  Serial.println("Sistema iniciado correctamente.");
  Serial.print("Puerto serial: ");
  Serial.print(BAUD_RATE);
  Serial.println(" bps");
  Serial.println();
}

/**
 * @brief Muestra el menu de opciones
 */
void mostrarMenu() {
  Serial.println("COMANDOS DISPONIBLES:");
  Serial.println("  AUTO    - Enviar secuencia automatica");
  Serial.println("  MANUAL  - Modo manual (enviar tramas individuales)");
  Serial.println("  PAUSE   - Pausar envio");
  Serial.println("  RESUME  - Reanudar envio");
  Serial.println("  RESET   - Reiniciar secuencia");
  Serial.println("  STATUS  - Mostrar estado actual");
  Serial.println("  HELP    - Mostrar esta ayuda");
  Serial.println();
  Serial.println("En modo manual, envie tramas en formato:");
  Serial.println("  L,X  (donde X es un caracter)");
  Serial.println("  M,N  (donde N es un numero entero)");
  Serial.println();
  Serial.print("> ");
}

/**
 * @brief Procesa la entrada del puerto serial
 */
void procesarEntradaSerial() {
  while (Serial.available()) {
    char caracterRecibido = (char)Serial.read();
    
    if (caracterRecibido == '\n') {
      entradaCompleta = true;
    } else if (caracterRecibido != '\r') {
      bufferEntrada += caracterRecibido;
    }
  }
  
  if (entradaCompleta) {
    procesarComando(bufferEntrada);
    bufferEntrada = "";
    entradaCompleta = false;
  }
}

/**
 * @brief Procesa un comando recibido
 * @param comando El comando a procesar
 */
void procesarComando(String comando) {
  comando.trim();
  comando.toUpperCase();
  
  if (comando == "AUTO") {
    estadoActual = ENVIANDO_SECUENCIA;
    indiceActual = 0;
    tiempoUltimaTransmision = millis();
    Serial.println("Iniciando secuencia automatica...");
    Serial.println();
    
  } else if (comando == "MANUAL") {
    estadoActual = ENVIANDO_MANUAL;
    Serial.println("Modo manual activado. Envie tramas L,X o M,N:");
    
  } else if (comando == "PAUSE") {
    estadoActual = PAUSADO;
    Serial.println("Sistema pausado.");
    
  } else if (comando == "RESUME") {
    if (indiceActual < TOTAL_TRAMAS) {
      estadoActual = ENVIANDO_SECUENCIA;
      Serial.println("Reanudando secuencia automatica...");
    } else {
      Serial.println("Secuencia completada. Use RESET para reiniciar.");
    }
    
  } else if (comando == "RESET") {
    indiceActual = 0;
    tiempoUltimaTransmision = millis();
    estadoActual = ESPERANDO_INICIO;
    Serial.println("Secuencia reiniciada.");
    
  } else if (comando == "STATUS") {
    mostrarEstado();
    
  } else if (comando == "HELP") {
    mostrarMenu();
    
  } else if (estadoActual == ENVIANDO_MANUAL) {
    // Procesar trama manual
    if (validarTrama(comando)) {
      enviarTrama(comando);
    } else {
      Serial.println("Error: Formato de trama invalido. Use L,X o M,N");
    }
    
  } else {
    Serial.println("Comando no reconocido. Use HELP para ver opciones.");
  }
  
  Serial.print("> ");
}

/**
 * @brief Ejecuta la secuencia automatica de tramas
 */
void ejecutarSecuenciaAutomatica() {
  unsigned long tiempoActual = millis();
  
  if (tiempoActual - tiempoUltimaTransmision >= DELAY_ENTRE_TRAMAS) {
    if (indiceActual < TOTAL_TRAMAS) {
      enviarTrama(String(secuenciaPredefinida[indiceActual]));
      indiceActual++;
      tiempoUltimaTransmision = tiempoActual;
    } else {
      // Secuencia completada
      Serial.println();
      Serial.println("=== SECUENCIA AUTOMATICA COMPLETADA ===");
      Serial.println("Use RESET para reiniciar o MANUAL para modo manual.");
      estadoActual = ESPERANDO_INICIO;
      Serial.print("> ");
    }
  }
}

/**
 * @brief Envia una trama por el puerto serial
 * @param trama La trama a enviar
 */
void enviarTrama(String trama) {
  Serial.print("TX: ");
  Serial.println(trama);
  
  // Aqui se enviaria la trama al decodificador
  // En este caso, se muestra en el mismo puerto serial
  // En una implementacion real, se enviaria por un puerto diferente
  delay(100); // Simular tiempo de transmision
}

/**
 * @brief Valida el formato de una trama
 * @param trama La trama a validar
 * @return true si la trama es valida
 */
bool validarTrama(String trama) {
  trama.trim();
  
  if (trama.length() < 3) return false;
  
  char tipo = trama.charAt(0);
  if (tipo != 'L' && tipo != 'M') return false;
  
  if (trama.charAt(1) != ',') return false;
  
  if (tipo == 'L') {
    // Trama LOAD: debe tener al menos un caracter después de la coma
    return trama.length() >= 3;
  } else if (tipo == 'M') {
    // Trama MAP: debe tener un numero válido después de la coma
    String numeroStr = trama.substring(2);
    return esNumeroValido(numeroStr);
  }
  
  return false;
}

/**
 * @brief Verifica si una cadena representa un numero valido
 * @param str La cadena a verificar
 * @return true si es un numero valido
 */
bool esNumeroValido(String str) {
  str.trim();
  if (str.length() == 0) return false;
  
  int inicio = 0;
  if (str.charAt(0) == '-' || str.charAt(0) == '+') {
    inicio = 1;
    if (str.length() == 1) return false;
  }
  
  for (int i = inicio; i < str.length(); i++) {
    if (!isdigit(str.charAt(i))) {
      return false;
    }
  }
  
  return true;
}

/**
 * @brief Muestra el estado actual del sistema
 */
void mostrarEstado() {
  Serial.println();
  Serial.println("=== ESTADO DEL SISTEMA ===");
  Serial.print("Estado: ");
  
  switch (estadoActual) {
    case ESPERANDO_INICIO:
      Serial.println("Esperando comandos");
      break;
    case ENVIANDO_SECUENCIA:
      Serial.println("Enviando secuencia automatica");
  Serial.print("Progreso: ");
  Serial.print(indiceActual);
  Serial.print("/");
  Serial.println(TOTAL_TRAMAS);
      break;
    case ENVIANDO_MANUAL:
      Serial.println("Modo manual activo");
      break;
    case PAUSADO:
      Serial.println("Pausado");
      break;
  }
  
  Serial.print("Puerto serial: ");
  Serial.print(BAUD_RATE);
  Serial.println(" bps");
  Serial.print("Tiempo activo: ");
  Serial.print(millis() / 1000);
  Serial.println(" segundos");
  Serial.println("========================");
  Serial.println();
}