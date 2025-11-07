/**
 * @file SerialPort.h
 * @brief Envoltorio minimo para leer lineas desde un puerto COM en Windows (Win32 API)
 */

#ifndef SERIALPORT_H
#define SERIALPORT_H

#ifdef _WIN32
#include <windows.h>
#endif

class SerialPort {
#ifdef _WIN32
    HANDLE handle;
#endif
    bool abierto;

public:
    SerialPort();
    ~SerialPort();

    /**
     * @brief Abre el puerto serial
     * @param puerto Nombre del puerto, por ejemplo "COM3" (se convierte a \\ \\.\\COM3)
     * @param baud   Velocidad en baudios (ej. 9600)
     * @return true si abrio correctamente
     */
    bool abrir(const char* puerto, unsigned long baud);

    /**
     * @brief Lee una linea terminada en \n (o \r\n)
     * @param buffer Buffer de salida
     * @param maxLen Tamano maximo del buffer
     * @return Numero de bytes leidos (>0 si hay linea completa, 0 si timeout, -1 si error)
     */
    int leerLinea(char* buffer, int maxLen);

    /**
     * @brief Escribe datos crudos al puerto
     * @param data puntero a los bytes a enviar
     * @param len numero de bytes a enviar
     * @return true si se envio correctamente
     */
    bool escribir(const char* data, int len);

    /**
     * @brief Escribe una cadena terminada en \n
     */
    bool escribirLinea(const char* str);

    /**
     * @brief Cierra el puerto
     */
    void cerrar();

    /**
     * @brief Verifica si el puerto esta abierto
     */
    bool estaAbierto() const;
};

#endif // SERIALPORT_H
