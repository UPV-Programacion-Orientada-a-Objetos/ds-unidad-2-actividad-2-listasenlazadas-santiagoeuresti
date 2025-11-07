/**
 * @file ListaDeCarga.h
 * @brief Lista doblemente enlazada para almacenar los caracteres decodificados
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#ifndef LISTADECARGA_H
#define LISTADECARGA_H

/**
 * @struct NodoCarga
 * @brief Nodo para la lista doblemente enlazada de carga
 */
struct NodoCarga {
    char dato;           ///< El caracter almacenado en este nodo
    NodoCarga* siguiente; ///< Puntero al siguiente nodo
    NodoCarga* anterior;  ///< Puntero al nodo anterior
    
    /**
     * @brief Constructor del nodo
     * @param c El caracter a almacenar en el nodo
     */
    NodoCarga(char c) : dato(c), siguiente(nullptr), anterior(nullptr) {}
};

/**
 * @class ListaDeCarga
 * @brief Lista doblemente enlazada que almacena los caracteres decodificados
 * 
 * Esta clase implementa una lista doblemente enlazada para mantener
 * en orden los caracteres decodificados que forman el mensaje final.
 */
class ListaDeCarga {
private:
    NodoCarga* cabeza; ///< Puntero al primer nodo de la lista
    NodoCarga* cola;   ///< Puntero al ultimo nodo de la lista
    int tamanio;       ///< Numero de elementos en la lista
    
public:
    /**
     * @brief Constructor que inicializa una lista vacia
     */
    ListaDeCarga();
    
    /**
     * @brief Destructor que libera toda la memoria de la lista
     */
    ~ListaDeCarga();
    
    /**
     * @brief Inserta un caracter al final de la lista
     * @param caracter El caracter a insertar
     * 
     * Agrega un nuevo nodo con el caracter especificado al final de la lista,
     * manteniendo el orden de llegada de los datos decodificados.
     */
    void insertarAlFinal(char caracter);
    
    /**
     * @brief Imprime el mensaje completo ensamblado
     * 
     * Recorre la lista desde el inicio hasta el final e imprime
     * todos los caracteres para mostrar el mensaje decodificado.
     */
    void imprimirMensaje();
    
    /**
     * @brief Imprime el estado actual de la lista (para depuracion)
     * 
     * Muestra cada caracter entre corchetes para visualizar
     * el contenido actual de la lista de carga.
     */
    void mostrarEstado();
    
    /**
     * @brief Obtiene el numero de elementos en la lista
     * @return El tamanio actual de la lista
     */
    int getTamanio() const;
    
    /**
     * @brief Verifica si la lista esta vacia
     * @return true si la lista esta vacia, false en caso contrario
     */
    bool estaVacia() const;
    
    /**
     * @brief Limpia toda la lista liberando la memoria
     */
    void limpiar();
};

#endif // LISTADECARGA_H