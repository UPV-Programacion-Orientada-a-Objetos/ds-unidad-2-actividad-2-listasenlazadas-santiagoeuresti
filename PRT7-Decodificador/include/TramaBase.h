/**
 * @file TramaBase.h
 * @brief Clase base abstracta para todas las tramas del protocolo PRT-7
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#ifndef TRAMABASE_H
#define TRAMABASE_H

// Forward declarations para evitar dependencias circulares
class ListaDeCarga;
class RotorDeMapeo;

/**
 * @class TramaBase
 * @brief Clase base abstracta que define la interfaz para todas las tramas
 * 
 * Esta clase define el comportamiento comun para todas las tramas del protocolo PRT-7.
 * Utiliza polimorfismo para permitir el procesamiento uniforme de diferentes tipos de tramas.
 */
class TramaBase {
public:
    /**
     * @brief Destructor virtual para permitir la destruccion polimorfica correcta
     * 
     * Es fundamental que sea virtual para garantizar que los destructores
     * de las clases derivadas sean llamados correctamente cuando se elimine
     * un objeto a traves de un puntero a la clase base.
     */
    virtual ~TramaBase() {}
    
    /**
     * @brief Metodo virtual puro para procesar la trama
     * @param carga Puntero a la lista de carga donde se almacenan los datos decodificados
     * @param rotor Puntero al rotor de mapeo que realiza la transformacion de caracteres
     * 
     * Este metodo debe ser implementado por todas las clases derivadas para
     * definir como cada tipo de trama interactua con las estructuras de datos.
     */
    virtual void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) = 0;
};

#endif // TRAMABASE_H