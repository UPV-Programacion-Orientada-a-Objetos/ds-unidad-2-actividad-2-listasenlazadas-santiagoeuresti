# CMakeLists.txt para el Decodificador PRT-7
# Proyecto de Estructuras de Datos - Listas Doblemente Enlazadas
# Autor: Sistema de Decodificacion PRT-7
# Fecha: 2025-11-06

cmake_minimum_required(VERSION 3.16)

# Configuracion del proyecto
project(PRT7-Decodificador 
    VERSION 1.0.0
    DESCRIPTION "Decodificador de Protocolo Industrial PRT-7"
    LANGUAGES CXX
)

# Configuracion del estandar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuracion de directorios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Configuracion de flags de compilacion
if(MSVC)
    # Flags para Visual Studio
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi /RTC1")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
else()
    # Flags para GCC/Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

# Definir archivos fuente
set(HEADER_FILES
    include/TramaBase.h
    include/TramaLoad.h
    include/TramaMap.h
    include/ListaDeCarga.h
    include/RotorDeMapeo.h
    include/DecodificadorPRT7.h
    include/SerialPort.h
)

set(SOURCE_FILES
    src/TramaLoad.cpp
    src/TramaMap.cpp
    src/ListaDeCarga.cpp
    src/RotorDeMapeo.cpp
    src/DecodificadorPRT7.cpp
    src/SerialPort.cpp
    main.cpp
)

# Crear el ejecutable principal
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Configurar directorios de include
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Propiedades del target
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "prt7_decodificador"
    DEBUG_POSTFIX "_debug"
)

# Definir macros de compilacion
target_compile_definitions(${PROJECT_NAME} 
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG_MODE>
        $<$<CONFIG:Release>:RELEASE_MODE>
        PROJECT_VERSION="${PROJECT_VERSION}"
)

# Configuracion para diferentes sistemas operativos
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WINDOWS_PLATFORM)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LINUX_PLATFORM)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE MACOS_PLATFORM)
endif()

# Configuracion de instalacion
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    COMPONENT Runtime
)

install(FILES ${HEADER_FILES}
    DESTINATION include/${PROJECT_NAME}
    COMPONENT Development
)

# Crear grupos de archivos para IDEs (Visual Studio, etc.)
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})

# Informacion de construccion
message(STATUS "Configurando proyecto: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Compilador: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Sistema: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Arquitectura: ${CMAKE_SYSTEM_PROCESSOR}")

# Target personalizado para limpiar completamente
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/lib
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMENT "Limpiando todos los archivos generados"
)

# Target para mostrar informacion del proyecto
add_custom_target(info
    COMMAND ${CMAKE_COMMAND} -E echo "Proyecto: ${PROJECT_NAME}"
    COMMAND ${CMAKE_COMMAND} -E echo "Version: ${PROJECT_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "Descripcion: ${PROJECT_DESCRIPTION}"
    COMMAND ${CMAKE_COMMAND} -E echo "Directorio fuente: ${CMAKE_SOURCE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Directorio binario: ${CMAKE_BINARY_DIR}"
    COMMENT "Mostrando informacion del proyecto"
)