/**
 * @file ListaDeCarga.cpp
 * @brief Implementacion de la clase ListaDeCarga
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#include "../include/ListaDeCarga.h"
#include <iostream>

ListaDeCarga::ListaDeCarga() : cabeza(nullptr), cola(nullptr), tamanio(0) {
}

ListaDeCarga::~ListaDeCarga() {
    limpiar();
}

void ListaDeCarga::insertarAlFinal(char caracter) {
    NodoCarga* nuevo = new NodoCarga(caracter);
    
    if (estaVacia()) {
        // Primer elemento
        cabeza = nuevo;
        cola = nuevo;
    } else {
        // Agregar al final
        cola->siguiente = nuevo;
        nuevo->anterior = cola;
        cola = nuevo;
    }
    
    tamanio++;
}

void ListaDeCarga::imprimirMensaje() {
    std::cout << std::endl << "MENSAJE OCULTO ENSAMBLADO:" << std::endl;
    
    if (estaVacia()) {
        std::cout << "(vacio)" << std::endl;
        return;
    }
    
    NodoCarga* actual = cabeza;
    while (actual != nullptr) {
        std::cout << actual->dato;
        actual = actual->siguiente;
    }
    std::cout << std::endl;
}

void ListaDeCarga::mostrarEstado() {
    std::cout << "Mensaje: ";
    
    if (estaVacia()) {
        std::cout << "(vacio)";
    } else {
        NodoCarga* actual = cabeza;
        while (actual != nullptr) {
            std::cout << "[" << actual->dato << "]";
            actual = actual->siguiente;
        }
    }
    std::cout << std::endl;
}

int ListaDeCarga::getTamanio() const {
    return tamanio;
}

bool ListaDeCarga::estaVacia() const {
    return cabeza == nullptr;
}

void ListaDeCarga::limpiar() {
    NodoCarga* actual = cabeza;
    while (actual != nullptr) {
        NodoCarga* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    
    cabeza = nullptr;
    cola = nullptr;
    tamanio = 0;
}