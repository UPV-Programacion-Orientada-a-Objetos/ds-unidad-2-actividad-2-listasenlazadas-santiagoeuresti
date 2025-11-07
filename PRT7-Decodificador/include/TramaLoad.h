/**
 * @file TramaLoad.h
 * @brief Clase que representa una trama de carga (LOAD) del protocolo PRT-7
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#ifndef TRAMALOAD_H
#define TRAMALOAD_H

#include "TramaBase.h"

/**
 * @class TramaLoad
 * @brief Representa una trama de tipo LOAD que contiene un fragmento de datos
 * 
 * Las tramas LOAD contienen un caracter que debe ser decodificado usando
 * el estado actual del rotor de mapeo y luego almacenado en la lista de carga.
 */
class TramaLoad : public TramaBase {
private:
    char caracter; ///< El caracter contenido en esta trama LOAD
    
public:
    /**
     * @brief Constructor que inicializa la trama con un caracter
     * @param c El caracter que contiene esta trama LOAD
     */
    explicit TramaLoad(char c);
    
    /**
     * @brief Destructor de la clase TramaLoad
     */
    ~TramaLoad();
    
    /**
     * @brief Procesa la trama LOAD decodificando el caracter y almacenandolo
     * @param carga Puntero a la lista donde se almacenan los datos decodificados
     * @param rotor Puntero al rotor que realiza el mapeo de caracteres
     * 
     * Este metodo toma el caracter de la trama, lo decodifica usando el rotor
     * y lo inserta al final de la lista de carga.
     */
    void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) override;
    
    /**
     * @brief Obtiene el caracter almacenado en esta trama
     * @return El caracter contenido en la trama
     */
    char getCaracter() const;
};

#endif // TRAMALOAD_H