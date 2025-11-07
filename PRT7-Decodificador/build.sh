#!/bin/bash
# Script de construccion para el Decodificador PRT-7
# Archivo: build.sh
# Autor: Sistema de Decodificacion PRT-7
# Fecha: 2025-11-06

echo "================================================"
echo "       CONSTRUCTOR DEL DECODIFICADOR PRT-7"
echo "================================================"
echo

# Verificar si CMake esta disponible
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake no encontrado en el sistema."
    echo "Por favor, instale CMake."
    exit 1
fi

# Crear directorio de construccion si no existe
if [ ! -d "build" ]; then
    echo "Creando directorio de construccion..."
    mkdir build
fi

# Navegar al directorio de construccion
cd build

echo "Configurando proyecto con CMake..."
cmake ..
if [ $? -ne 0 ]; then
    echo "ERROR: Fallo la configuracion de CMake."
    exit 1
fi

echo
echo "Compilando proyecto..."
make -j$(nproc)
if [ $? -ne 0 ]; then
    echo "ERROR: Fallo la compilacion."
    exit 1
fi

echo
echo "================================================"
echo "       COMPILACION EXITOSA"
echo "================================================"
echo
echo "El ejecutable se encuentra en: build/bin/prt7_decodificador"
echo
echo "Para ejecutar el programa:"
echo "  cd build/bin"
echo "  ./prt7_decodificador"
echo

# Preguntar si desea ejecutar el programa
read -p "¿Desea ejecutar el programa ahora? (s/n): " ejecutar
if [[ $ejecutar == "s" || $ejecutar == "S" ]]; then
    echo
    echo "Ejecutando el decodificador..."
    echo
    ./bin/prt7_decodificador
fi