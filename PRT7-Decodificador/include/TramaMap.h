/**
 * @file TramaMap.h
 * @brief Clase que representa una trama de mapeo (MAP) del protocolo PRT-7
 * @author Sistema de Decodificacion PRT-7
 * @date 2025-11-06
 */

#ifndef TRAMAMAP_H
#define TRAMAMAP_H

#include "TramaBase.h"

/**
 * @class TramaMap
 * @brief Representa una trama de tipo MAP que contiene instrucciones de rotacion
 * 
 * Las tramas MAP contienen un valor entero que indica cuantas posiciones
 * debe rotar el rotor de mapeo (positivo o negativo).
 */
class TramaMap : public TramaBase {
private:
    int rotacion; ///< El valor de rotacion para el rotor (positivo o negativo)
    
public:
    /**
     * @brief Constructor que inicializa la trama con un valor de rotacion
     * @param rot El valor de rotacion que contiene esta trama MAP
     */
    explicit TramaMap(int rot);
    
    /**
     * @brief Destructor de la clase TramaMap
     */
    ~TramaMap();
    
    /**
     * @brief Procesa la trama MAP rotando el rotor de mapeo
     * @param carga Puntero a la lista de carga (no utilizado en MAP)
     * @param rotor Puntero al rotor que sera rotado
     * 
     * Este metodo simplemente aplica la rotacion especificada al rotor,
     * cambiando el estado del "disco de cifrado".
     */
    void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) override;
    
    /**
     * @brief Obtiene el valor de rotacion almacenado en esta trama
     * @return El valor de rotacion contenido en la trama
     */
    int getRotacion() const;
};

#endif // TRAMAMAP_H