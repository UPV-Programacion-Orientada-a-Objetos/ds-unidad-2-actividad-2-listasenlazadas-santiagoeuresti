/**
 * @file DecodificadorPRT7.cpp
 * @brief Implementacion de la clase DecodificadorPRT7
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#include "../include/DecodificadorPRT7.h"
#include "../include/TramaLoad.h"
#include "../include/TramaMap.h"
#include "../include/SerialPort.h"
#include <iostream>

DecodificadorPRT7::DecodificadorPRT7() : listaCarga(nullptr), rotor(nullptr), activo(false) {
}

DecodificadorPRT7::~DecodificadorPRT7() {
    if (listaCarga != nullptr) {
        delete listaCarga;
    }
    if (rotor != nullptr) {
        delete rotor;
    }
}

bool DecodificadorPRT7::inicializar() {
    std::cout << "Iniciando Decodificador PRT-7..." << std::endl;
    
    // Crear las estructuras de datos
    listaCarga = new ListaDeCarga();
    rotor = new RotorDeMapeo();
    
    if (listaCarga == nullptr || rotor == nullptr) {
        std::cout << "Error: No se pudo inicializar las estructuras de datos." << std::endl;
        return false;
    }
    
    activo = true;
    std::cout << "Decodificador inicializado correctamente." << std::endl;
    std::cout << "Rotor configurado con alfabeto A-Z, cabeza en 'A'." << std::endl;
    std::cout << std::endl;
    
    return true;
}

void DecodificadorPRT7::ejecutar() {
    if (!activo) {
        std::cout << "Error: Decodificador no inicializado." << std::endl;
        return;
    }
    
    std::cout << "=== MODO MANUAL ===" << std::endl;
    std::cout << "Ingrese tramas en formato 'L,X' o 'M,N' (escriba 'quit' para salir):" << std::endl;
    
    char buffer[100];
    while (activo) {
        std::cout << "> ";
        std::cin.getline(buffer, sizeof(buffer));
        
        // Verificar comando de salida
        if (buffer[0] == 'q' && buffer[1] == 'u' && buffer[2] == 'i' && buffer[3] == 't') {
            break;
        }
        
        if (buffer[0] != '\0') {
            std::cout << "Trama recibida: [" << buffer << "] -> Procesando... -> ";
            
            TramaBase* trama = parsearTrama(buffer);
            if (trama != nullptr) {
                procesarTrama(trama);
                listaCarga->mostrarEstado();
                delete trama;
            } else {
                std::cout << "Error: Formato de trama invalido." << std::endl;
            }
            
            std::cout << std::endl;
        }
    }
}

void DecodificadorPRT7::simularArduino() {
    if (!activo) {
        std::cout << "Error: Decodificador no inicializado." << std::endl;
        return;
    }
    
    std::cout << "=== SIMULACION ARDUINO ===" << std::endl;
    std::cout << "Procesando secuencia predefinida..." << std::endl << std::endl;
    
    // Secuencia de ejemplo del enunciado
    const char* secuencia[] = {
        "L,H", "L,O", "L,L", "M,2", "L,A", "L, ", "L,W", 
        "M,-2", "L,O", "L,R", "L,L", "L,D"
    };
    
    int totalTramas = sizeof(secuencia) / sizeof(secuencia[0]);
    
    for (int i = 0; i < totalTramas; i++) {
        std::cout << "Trama recibida: [" << secuencia[i] << "] -> Procesando... -> ";
        
        TramaBase* trama = parsearTrama(secuencia[i]);
        if (trama != nullptr) {
            procesarTrama(trama);
            listaCarga->mostrarEstado();
            delete trama;
        } else {
            std::cout << "Error en trama: " << secuencia[i] << std::endl;
        }
        
        std::cout << std::endl;
    }
}

TramaBase* DecodificadorPRT7::parsearTrama(const char* linea) {
    if (linea == nullptr || linea[0] == '\0') {
        return nullptr;
    }
    
    // Copiar la linea para modificarla
    char buffer[100];
    copiarCadena(buffer, linea);
    
    // Recortar espacios al inicio y final
    int start = 0; while (buffer[start] == ' ' || buffer[start] == '\t' || buffer[start] == '[') start++;
    int end = 0; while (buffer[end] != '\0') end++; end--;
    while (end >= start && (buffer[end] == ' ' || buffer[end] == '\t' || buffer[end] == '\r' || buffer[end] == ']')) { buffer[end] = '\0'; end--; }
    
    // Saltar prefijo "TX:" si existe
    if ((buffer[start] == 'T' || buffer[start] == 't') &&
        (buffer[start+1] == 'X' || buffer[start+1] == 'x') &&
        buffer[start+2] == ':') {
        start += 3;
        if (buffer[start] == ' ') start++;
    }
    
    // Mover el contenido al inicio si start > 0
    if (start > 0) {
        int i = 0; while (buffer[start + i] != '\0') { buffer[i] = buffer[start + i]; i++; }
        buffer[i] = '\0';
    }
    
    // Busqueda con criterio estricto: token debe ser 'L' o 'M' seguido inmediatamente de coma (permitiendo espacios)
    int i = 0;
    while (buffer[i] != '\0') {
        char c = buffer[i];
        if (c == 'L' || c == 'l' || c == 'M' || c == 'm') {
            // Evitar falsos positivos (palabras como 'Load' o 'mode')
            if (i > 0) {
                char prev = buffer[i - 1];
                bool prevAlpha = (prev >= 'A' && prev <= 'Z') || (prev >= 'a' && prev <= 'z');
                if (prevAlpha) { i++; continue; }
            }
            // Tras el tipo, solo se permiten espacios antes de la coma
            int j = i + 1;
            while (buffer[j] == ' ' || buffer[j] == '\t') j++;
            if (buffer[j] != ',') { i++; continue; }

            // Determinar tipo y dato
            char tipo = (c == 'l') ? 'L' : (c == 'm' ? 'M' : c);
            int p = j + 1;
            // Saltar espacios despues de la coma
            while (buffer[p] == ' ' || buffer[p] == '\t') p++;

            if (tipo == 'L') {
                // Trama LOAD: si no hay dato, considerar espacio
                char caracter = (buffer[p] == '\0') ? ' ' : buffer[p];
                return new TramaLoad(caracter);
            } else {
                // Trama MAP: convertir a entero desde p
                int rotacion = stringAEntero(&buffer[p]);
                return new TramaMap(rotacion);
            }
        }
        i++;
    }
    // No se encontro un patron valido
    return nullptr;
}

void DecodificadorPRT7::procesarTrama(TramaBase* trama) {
    if (trama != nullptr && listaCarga != nullptr && rotor != nullptr) {
        trama->procesar(listaCarga, rotor);
    }
}

void DecodificadorPRT7::finalizar() {
    std::cout << "---" << std::endl;
    std::cout << "Flujo de datos terminado." << std::endl;
    
    if (listaCarga != nullptr) {
        listaCarga->imprimirMensaje();
    }
    
    std::cout << "---" << std::endl;
    std::cout << "Liberando memoria... Sistema apagado." << std::endl;
    
    activo = false;
}

bool DecodificadorPRT7::estaActivo() const {
    return activo;
}

void DecodificadorPRT7::detener() {
    activo = false;
}

int DecodificadorPRT7::stringAEntero(const char* str) {
    if (str == nullptr) return 0;
    
    int resultado = 0;
    int signo = 1;
    int i = 0;
    
    // Manejar signo negativo
    if (str[0] == '-') {
        signo = -1;
        i = 1;
    } else if (str[0] == '+') {
        i = 1;
    }
    
    // Convertir digitos
    while (str[i] >= '0' && str[i] <= '9') {
        resultado = resultado * 10 + (str[i] - '0');
        i++;
    }
    
    return resultado * signo;
}

void DecodificadorPRT7::copiarCadena(char* destino, const char* origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

char* DecodificadorPRT7::buscarCaracter(char* str, char ch) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ch) {
            return &str[i];
        }
        i++;
    }
    return nullptr;
}

void DecodificadorPRT7::ejecutarSerial(const char* puerto, unsigned long baud) {
    if (!activo) {
        std::cout << "Error: Decodificador no inicializado." << std::endl;
        return;
    }
#ifdef _WIN32
    SerialPort sp;
    std::cout << "Iniciando Decodificador PRT-7. Conectando a puerto COM..." << std::endl;
    std::cout << "Abriendo puerto " << puerto << " a " << baud << " bps..." << std::endl;
    if (!sp.abrir(puerto, baud)) {
        std::cout << "No se pudo abrir el puerto." << std::endl;
        return;
    }
    std::cout << "Conexion establecida. Esperando tramas..." << std::endl;

    // Intentar activar emisor interactivo (si estuviera cargado) enviando AUTO\n
    // No afecta al emisor simple; si no existe, se ignora.
    sp.escribirLinea("AUTO");

    char linea[128];
    int leidos = 0;
    int ciclosVacios = 0;
    while (activo) {
        leidos = sp.leerLinea(linea, sizeof(linea));
        if (leidos > 0) {
            ciclosVacios = 0;

            // Detectar rapidamente si la linea parece PRT-7 (L, o M, con coma inmediata tras espacios)
            bool posible = false; int ii = 0;
            while (linea[ii] != '\0') {
                char ch = linea[ii];
                if (ch == 'L' || ch == 'l' || ch == 'M' || ch == 'm') {
                    if (ii > 0) {
                        char prev = linea[ii - 1];
                        bool prevAlpha = (prev >= 'A' && prev <= 'Z') || (prev >= 'a' && prev <= 'z');
                        if (prevAlpha) { ii++; continue; }
                    }
                    int jj = ii + 1; while (linea[jj] == ' ' || linea[jj] == '\t') jj++;
                    if (linea[jj] == ',') { posible = true; break; }
                }
                ii++;
            }

            if (!posible) {
                // Silenciosamente ignorar ruido que no es PRT-7
                continue;
            }

            std::cout << "Trama recibida: [" << linea << "] -> Procesando... -> ";
            TramaBase* trama = parsearTrama(linea);
            if (trama != nullptr) {
                procesarTrama(trama);
                listaCarga->mostrarEstado();
                delete trama;
                std::cout << std::endl;
            } else {
                std::cout << "Error: Formato de trama invalido." << std::endl;
            }
        } else if (leidos == 0) {
            // timeout sin datos
            ciclosVacios++;
            if (ciclosVacios > 3000) { // ~ varios segundos sin datos
                // seguir esperando sin terminar; o podria romper
                ciclosVacios = 0;
            }
        } else {
            std::cout << "Error de lectura del puerto." << std::endl;
            break;
        }
    }
#else
    std::cout << "Lectura de puerto COM solo disponible en Windows." << std::endl;
    (void)puerto; (void)baud;
#endif
}