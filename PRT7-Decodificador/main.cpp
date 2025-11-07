/**
 * @file main.cpp
 * @brief Punto de entrada principal del Decodificador PRT-7
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#include "include/DecodificadorPRT7.h"
#include <iostream>
#include <cstdlib>

/**
 * @brief Muestra el menu principal del programa
 */
void mostrarMenu() {
    std::cout << "===============================================" << std::endl;
    std::cout << "      DECODIFICADOR DE PROTOCOLO PRT-7       " << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "1. Ejecutar simulacion Arduino (automatico)" << std::endl;
    std::cout << "2. Modo manual (ingreso de tramas)" << std::endl;
    std::cout << "3. Leer desde puerto COM (Windows)" << std::endl;
    std::cout << "4. Salir" << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

/**
 * @brief Muestra informacion sobre el protocolo PRT-7
 */
void mostrarAyuda() {
    std::cout << std::endl << "=== INFORMACION DEL PROTOCOLO PRT-7 ===" << std::endl;
    std::cout << "El protocolo PRT-7 utiliza dos tipos de tramas:" << std::endl;
    std::cout << "- LOAD (L,X): Contiene un fragmento de datos (caracter X)" << std::endl;
    std::cout << "- MAP (M,N): Instrucciones para rotar el disco N posiciones" << std::endl;
    std::cout << std::endl;
    std::cout << "Ejemplos de tramas validas:" << std::endl;
    std::cout << "L,A    -> Carga el caracter 'A'" << std::endl;
    std::cout << "L,     -> Carga un espacio" << std::endl;
    std::cout << "M,5    -> Rota el disco +5 posiciones" << std::endl;
    std::cout << "M,-3   -> Rota el disco -3 posiciones" << std::endl;
    std::cout << std::endl;
}

/**
 * @brief Funcion principal del programa
 * @return Codigo de salida del programa
 */
int main() {
    std::cout << "Iniciando sistema..." << std::endl << std::endl;
    
    // Crear instancia del decodificador
    DecodificadorPRT7 decodificador;
    
    // Inicializar el sistema
    if (!decodificador.inicializar()) {
        std::cout << "Error critico: No se pudo inicializar el decodificador." << std::endl;
        return 1;
    }
    
    int opcion = 0;
    bool continuar = true;
    
    while (continuar) {
        mostrarMenu();
        std::cin >> opcion;
        std::cin.ignore(); // Limpiar el buffer de entrada
        
        std::cout << std::endl;
        
        switch (opcion) {
            case 1:
                std::cout << "Ejecutando simulacion de Arduino..." << std::endl;
                decodificador.simularArduino();
                decodificador.finalizar();
                break;
                
            case 2:
                mostrarAyuda();
                decodificador.ejecutar();
                decodificador.finalizar();
                break;
                
            case 3: {
                char puerto[32];
                char baudStr[32];
                std::cout << "Ingrese puerto (ej. COM3): ";
                std::cin.getline(puerto, sizeof(puerto));
                if (puerto[0] == '\0') {
                    std::cout << "Puerto invalido." << std::endl;
                    break;
                }
                std::cout << "Ingrese baud (enter=9600, tipico ESP32=115200): ";
                std::cin.getline(baudStr, sizeof(baudStr));
                int baud = std::atoi(baudStr);
                if (baud <= 0) baud = 9600;
                decodificador.ejecutarSerial(puerto, (unsigned long)baud);
                decodificador.finalizar();
                break; }

            case 4:
                std::cout << "Cerrando el decodificador..." << std::endl;
                continuar = false;
                break;
                
            default:
                std::cout << "Opcion invalida. Por favor, seleccione 1, 2 o 3." << std::endl;
                break;
        }
        
    if (continuar && (opcion == 1 || opcion == 2 || opcion == 3)) {
            char respuesta;
            std::cout << std::endl << "¿Desea realizar otra operacion? (s/n): ";
            std::cin >> respuesta;
            std::cin.ignore();
            
            if (respuesta != 's' && respuesta != 'S') {
                continuar = false;
            } else {
                // Reinicializar para nueva operacion
                decodificador.~DecodificadorPRT7();
                new (&decodificador) DecodificadorPRT7();
                decodificador.inicializar();
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << std::endl << "Programa terminado exitosamente." << std::endl;
    return 0;
}