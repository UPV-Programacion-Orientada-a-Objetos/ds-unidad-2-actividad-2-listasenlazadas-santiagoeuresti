/**
 * @file RotorDeMapeo.h
 * @brief Lista circular doblemente enlazada que implementa el "disco de cifrado"
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#ifndef ROTORDEMAPEO_H
#define ROTORDEMAPEO_H

/**
 * @struct NodoRotor
 * @brief Nodo para la lista circular doblemente enlazada del rotor
 */
struct NodoRotor {
    char dato;           ///< El caracter almacenado en este nodo
    NodoRotor* siguiente; ///< Puntero al siguiente nodo
    NodoRotor* anterior;  ///< Puntero al nodo anterior
    
    /**
     * @brief Constructor del nodo
     * @param c El caracter a almacenar en el nodo
     */
    NodoRotor(char c) : dato(c), siguiente(nullptr), anterior(nullptr) {}
};

/**
 * @class RotorDeMapeo
 * @brief Lista circular doblemente enlazada que simula un disco de cifrado
 * 
 * Esta clase implementa una lista circular que contiene el alfabeto A-Z y
 * permite rotaciones para cambiar el mapeo de caracteres. Actua como un
 * "disco de cifrado" similar a las maquinas Enigma.
 */
class RotorDeMapeo {
private:
    NodoRotor* cabeza; ///< Puntero a la posicion "cero" actual del rotor
    int tamanio;       ///< Numero de elementos en el rotor (26 para A-Z)
    
    /**
     * @brief Busca un nodo que contenga el caracter especificado
     * @param c El caracter a buscar
     * @return Puntero al nodo que contiene el caracter, nullptr si no se encuentra
     */
    NodoRotor* buscarNodo(char c);
    
public:
    /**
     * @brief Constructor que inicializa el rotor con el alfabeto A-Z
     */
    RotorDeMapeo();
    
    /**
     * @brief Destructor que libera toda la memoria del rotor
     */
    ~RotorDeMapeo();
    
    /**
     * @brief Rota el rotor un numero especificado de posiciones
     * @param posiciones Numero de posiciones a rotar (positivo o negativo)
     * 
     * Mueve el puntero cabeza las posiciones indicadas. Un valor positivo
     * rota hacia adelante, un valor negativo rota hacia atras.
     */
    void rotar(int posiciones);
    
    /**
     * @brief Obtiene el caracter mapeado para un caracter de entrada
     * @param caracterEntrada El caracter a mapear
     * @return El caracter mapeado segun la rotacion actual
     * 
     * Busca el caracter de entrada en el rotor, calcula su posicion relativa
     * a la cabeza actual, y devuelve el caracter que estaria en la posicion
     * cero si el caracter de entrada estuviera alli.
     */
    char getMapeo(char caracterEntrada);
    
    /**
     * @brief Muestra el estado actual del rotor (para depuracion)
     */
    void mostrarEstado();
    
    /**
     * @brief Obtiene el caracter en la posicion cero actual
     * @return El caracter en la posicion de la cabeza
     */
    char getCabeza();
};

#endif // ROTORDEMAPEO_H