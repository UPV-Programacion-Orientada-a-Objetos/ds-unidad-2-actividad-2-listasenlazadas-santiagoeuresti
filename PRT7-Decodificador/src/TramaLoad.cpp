/**
 * @file TramaLoad.cpp
 * @brief Implementacion de la clase TramaLoad
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#include "../include/TramaLoad.h"
#include "../include/ListaDeCarga.h"
#include "../include/RotorDeMapeo.h"
#include <iostream>

TramaLoad::TramaLoad(char c) : caracter(c) {
}

TramaLoad::~TramaLoad() {
}

void TramaLoad::procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) {
    // Decodificar el caracter usando el estado actual del rotor
    char caracterDecodificado = rotor->getMapeo(caracter);
    
    // Insertar el caracter decodificado al final de la lista de carga
    carga->insertarAlFinal(caracterDecodificado);
    
    // Mostrar informacion de procesamiento
    std::cout << "Fragmento '" << caracter << "' decodificado como '" 
              << caracterDecodificado << "'." << std::endl;
}

char TramaLoad::getCaracter() const {
    return caracter;
}