/**
 * @file RotorDeMapeo.cpp
 * @brief Implementacion de la clase RotorDeMapeo
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#include "../include/RotorDeMapeo.h"
#include <iostream>

RotorDeMapeo::RotorDeMapeo() : cabeza(nullptr), tamanio(26) {
    // Crear los nodos para el alfabeto A-Z
    NodoRotor* primero = nullptr;
    NodoRotor* anterior = nullptr;
    
    for (int i = 0; i < 26; i++) {
        char caracter = 'A' + i;
        NodoRotor* nuevo = new NodoRotor(caracter);
        
        if (primero == nullptr) {
            // Primer nodo
            primero = nuevo;
            cabeza = nuevo;  // La cabeza empieza en 'A'
        } else {
            // Enlazar con el nodo anterior
            anterior->siguiente = nuevo;
            nuevo->anterior = anterior;
        }
        
        anterior = nuevo;
    }
    
    // Cerrar el circulo
    if (anterior != nullptr && primero != nullptr) {
        anterior->siguiente = primero;
        primero->anterior = anterior;
    }
}

RotorDeMapeo::~RotorDeMapeo() {
    if (cabeza == nullptr) return;
    
    // Romper el circulo temporalmente
    NodoRotor* ultimo = cabeza->anterior;
    ultimo->siguiente = nullptr;
    
    // Eliminar todos los nodos
    NodoRotor* actual = cabeza;
    while (actual != nullptr) {
        NodoRotor* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}

void RotorDeMapeo::rotar(int posiciones) {
    if (cabeza == nullptr || posiciones == 0) return;
    
    // Normalizar las posiciones al rango [0, 25]
    posiciones = posiciones % tamanio;
    if (posiciones < 0) {
        posiciones += tamanio;
    }
    
    // Mover la cabeza las posiciones indicadas
    for (int i = 0; i < posiciones; i++) {
        cabeza = cabeza->siguiente;
    }
}

NodoRotor* RotorDeMapeo::buscarNodo(char c) {
    if (cabeza == nullptr) return nullptr;
    
    NodoRotor* actual = cabeza;
    do {
        if (actual->dato == c) {
            return actual;
        }
        actual = actual->siguiente;
    } while (actual != cabeza);
    
    return nullptr; // No encontrado
}

char RotorDeMapeo::getMapeo(char caracterEntrada) {
    // Manejar espacios y caracteres especiales sin mapeo
    if (caracterEntrada == ' ' || caracterEntrada < 'A' || caracterEntrada > 'Z') {
        return caracterEntrada;
    }
    
    // Para un mapeo simple: calcular la posicion del caracter en el alfabeto original
    // y devolver el caracter que esta en esa posicion en el rotor rotado
    int posicionOriginal = caracterEntrada - 'A';
    
    // Encontrar el caracter en la posicion correspondiente en el rotor rotado
    NodoRotor* actual = cabeza;
    for (int i = 0; i < posicionOriginal && actual != nullptr; i++) {
        actual = actual->siguiente;
    }
    
    return (actual != nullptr) ? actual->dato : caracterEntrada;
}

void RotorDeMapeo::mostrarEstado() {
    if (cabeza == nullptr) {
        std::cout << "Rotor vacio" << std::endl;
        return;
    }
    
    std::cout << "Estado del rotor (cabeza en '" << cabeza->dato << "'): ";
    NodoRotor* actual = cabeza;
    do {
        std::cout << actual->dato;
        if (actual == cabeza) std::cout << "*";
        std::cout << " ";
        actual = actual->siguiente;
    } while (actual != cabeza);
    std::cout << std::endl;
}

char RotorDeMapeo::getCabeza() {
    return (cabeza != nullptr) ? cabeza->dato : '\0';
}