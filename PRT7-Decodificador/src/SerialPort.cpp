/**
 * @file SerialPort.cpp
 */

#include "../include/SerialPort.h"

SerialPort::SerialPort()
#ifdef _WIN32
    : handle(INVALID_HANDLE_VALUE)
#endif
    , abierto(false) {}

SerialPort::~SerialPort() { cerrar(); }

bool SerialPort::abrir(const char* puerto, unsigned long baud) {
#ifdef _WIN32
    if (abierto) cerrar();

    // Construir ruta estilo \\.\COM3
    char ruta[64];
    int i = 0;
    ruta[i++] = '\\'; ruta[i++] = '\\'; ruta[i++] = '.'; ruta[i++] = '\\';
    for (int j = 0; puerto[j] != '\0' && i < 60; ++j) { ruta[i++] = puerto[j]; }
    ruta[i] = '\0';

    handle = CreateFileA(
        ruta,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        0,
        0);

    if (handle == INVALID_HANDLE_VALUE) {
        abierto = false;
        return false;
    }

    // Configurar buffers
    SetupComm(handle, 1024, 1024);

    // Configurar timeouts
    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = 50;        // ms
    timeouts.ReadTotalTimeoutMultiplier = 10; // ms por byte
    timeouts.ReadTotalTimeoutConstant = 100;  // ms
    timeouts.WriteTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 100;
    SetCommTimeouts(handle, &timeouts);

    // Configurar DCB
    DCB dcb; ZeroMemory(&dcb, sizeof(dcb));
    dcb.DCBlength = sizeof(dcb);
    if (!GetCommState(handle, &dcb)) { cerrar(); return false; }

    dcb.BaudRate = baud;
    dcb.ByteSize = 8;
    dcb.Parity   = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fParity = FALSE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;

    if (!SetCommState(handle, &dcb)) { cerrar(); return false; }

    // Purga inicial
    PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);

    abierto = true;
    return true;
#else
    (void)puerto; (void)baud;
    return false;
#endif
}

int SerialPort::leerLinea(char* buffer, int maxLen) {
#ifdef _WIN32
    if (!abierto || buffer == 0 || maxLen <= 1) return -1;

    int count = 0;
    char ch;
    DWORD bytes = 0;

    while (count < maxLen - 1) {
        if (!ReadFile(handle, &ch, 1, &bytes, 0)) {
            return -1; // error
        }
        if (bytes == 0) {
            // timeout sin datos
            return 0;
        }
        if (ch == '\n') {
            break;
        }
        if (ch != '\r') {
            buffer[count++] = ch;
        }
    }
    buffer[count] = '\0';
    return count;
#else
    (void)buffer; (void)maxLen;
    return -1;
#endif
}

void SerialPort::cerrar() {
#ifdef _WIN32
    if (abierto) {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
        abierto = false;
    }
#endif
}

bool SerialPort::estaAbierto() const { return abierto; }

bool SerialPort::escribir(const char* data, int len) {
#ifdef _WIN32
    if (!abierto || data == 0 || len <= 0) return false;
    DWORD escritos = 0;
    if (!WriteFile(handle, data, (DWORD)len, &escritos, 0)) {
        return false;
    }
    return escritos == (DWORD)len;
#else
    (void)data; (void)len; return false;
#endif
}

bool SerialPort::escribirLinea(const char* str) {
#ifdef _WIN32
    if (!abierto || str == 0) return false;
    // Enviar la cadena
    int len = 0; while (str[len] != '\0') len++;
    if (!escribir(str, len)) return false;
    // Enviar salto de linea \n
    const char nl = '\n';
    return escribir(&nl, 1);
#else
    (void)str; return false;
#endif
}
