/**
 * @file DecodificadorPRT7.h
 * @brief Clase principal que maneja el decodificador del protocolo PRT-7
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#ifndef DECODIFICADORPRT7_H
#define DECODIFICADORPRT7_H

#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"
#include "TramaBase.h"
class SerialPort; // forward

/**
 * @class DecodificadorPRT7
 * @brief Clase principal que orquesta el proceso de decodificacion
 * 
 * Esta clase maneja la comunicacion serial, el parseo de tramas,
 * la instanciacion de objetos polimorficos y la coordinacion entre
 * las estructuras de datos para decodificar el mensaje oculto.
 */
class DecodificadorPRT7 {
private:
    ListaDeCarga* listaCarga;  ///< Lista que almacena los caracteres decodificados
    RotorDeMapeo* rotor;       ///< Rotor que realiza el mapeo de caracteres
    bool activo;               ///< Estado del decodificador
    
    /**
     * @brief Parsea una linea de entrada y crea la trama correspondiente
     * @param linea La linea de texto recibida (ej. "L,A" o "M,5")
     * @return Puntero a la trama creada, nullptr si hay error
     * 
     * Analiza el formato de la linea y crea el objeto TramaLoad o TramaMap
     * correspondiente usando polimorfismo.
     */
    TramaBase* parsearTrama(const char* linea);
    
    /**
     * @brief Procesa una sola trama usando polimorfismo
     * @param trama Puntero a la trama a procesar
     * 
     * Llama al metodo procesar de la trama, que se ejecutara segun
     * el tipo real del objeto (TramaLoad o TramaMap).
     */
    void procesarTrama(TramaBase* trama);
    
    /**
     * @brief Convierte una cadena a entero (reemplazo de atoi sin STL)
     * @param str La cadena a convertir
     * @return El valor entero correspondiente
     */
    int stringAEntero(const char* str);
    
    /**
     * @brief Copia una cadena (reemplazo de strcpy sin STL)
     * @param destino Buffer donde copiar
     * @param origen Cadena a copiar
     */
    void copiarCadena(char* destino, const char* origen);
    
    /**
     * @brief Busca un caracter en una cadena (reemplazo de strchr sin STL)
     * @param str Cadena donde buscar
     * @param ch Caracter a buscar
     * @return Puntero al caracter encontrado, nullptr si no existe
     */
    char* buscarCaracter(char* str, char ch);
    
public:
    /**
     * @brief Constructor que inicializa las estructuras de datos
     */
    DecodificadorPRT7();
    
    /**
     * @brief Destructor que libera la memoria
     */
    ~DecodificadorPRT7();
    
    /**
     * @brief Inicializa el decodificador y sus componentes
     * @return true si la inicializacion fue exitosa
     */
    bool inicializar();
    
    /**
     * @brief Ejecuta el bucle principal de decodificacion
     * 
     * Este metodo simula la lectura del puerto serial leyendo desde
     * la entrada estandar. En una implementacion real, leeria del COM.
     */
    void ejecutar();

    /**
     * @brief Ejecuta leyendo lineas desde un puerto serial real (Windows)
     * @param puerto Nombre del puerto, ej. "COM3"
     * @param baud   Velocidad (ej. 9600)
     */
    void ejecutarSerial(const char* puerto, unsigned long baud);
    
    /**
     * @brief Simula el procesamiento de datos de un Arduino
     * 
     * Procesa una secuencia predefinida de tramas para demostrar
     * el funcionamiento del sistema sin necesidad de hardware real.
     */
    void simularArduino();
    
    /**
     * @brief Finaliza el decodificador mostrando el resultado
     */
    void finalizar();
    
    /**
     * @brief Obtiene el estado actual del decodificador
     * @return true si el decodificador esta activo
     */
    bool estaActivo() const;
    
    /**
     * @brief Detiene el decodificador
     */
    void detener();
};

#endif // DECODIFICADORPRT7_H