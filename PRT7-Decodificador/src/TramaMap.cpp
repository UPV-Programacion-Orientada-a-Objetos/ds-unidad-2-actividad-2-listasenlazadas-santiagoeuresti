/**
 * @file TramaMap.cpp
 * @brief Implementacion de la clase TramaMap
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#include "../include/TramaMap.h"
#include "../include/RotorDeMapeo.h"
#include <iostream>

TramaMap::TramaMap(int rot) : rotacion(rot) {
}

TramaMap::~TramaMap() {
}

void TramaMap::procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) {
    // Rotar el rotor segun el valor especificado
    rotor->rotar(rotacion);
    
    // Mostrar informacion de procesamiento
    std::cout << "ROTANDO ROTOR ";
    if (rotacion >= 0) {
        std::cout << "+" << rotacion;
    } else {
        std::cout << rotacion;
    }
    std::cout << "." << std::endl;
}

int TramaMap::getRotacion() const {
    return rotacion;
}