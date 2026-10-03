#!/bin/bash
set -e
cd "$(dirname "$0")"
mkdir -p build
CXX="${CXX:-clang++}"
fuentes=(main.cpp interfaz.cpp tiempo.cpp calificacion.cpp video.cpp pelicula.cpp episodio.cpp serie.cpp catalogo.cpp multimedia.cpp)
modo="${1:-grafico}"
if [ "$modo" = "consola" ]; then
    "$CXX" -std=c++17 -Wall -Wextra -Wpedantic "${fuentes[@]}" -o build/catalogo_consola
    echo "Listo. Ejecuta: ./build/catalogo_consola"
elif [ "$modo" = "grafico" ]; then
    PKG="${PKG_CONFIG:-pkg-config}"
    if ! command -v "$PKG" >/dev/null && [ -x /opt/homebrew/bin/pkg-config ]; then
        PKG=/opt/homebrew/bin/pkg-config
    fi
    if "$PKG" --exists opencv4 2>/dev/null; then
        paquete=opencv4
    elif "$PKG" --exists opencv5 2>/dev/null; then
        paquete=opencv5
    else
        echo "Falta OpenCV. En macOS: brew install opencv pkg-config"
        echo "Puedes estudiar la consola con: ./compilar.sh consola"
        exit 1
    fi
    # pkg-config entrega opciones del compilador y del enlazador.
    # Los encabezados de la biblioteca externa se tratan como del sistema.
    opciones=(-isystem "$("$PKG" --variable=includedir "$paquete")")
    # Solo enlazamos los tres modulos que usamos, no todo OpenCV.
    read -r -a bibliotecas <<< "$("$PKG" --libs-only-L "$paquete")"
    "$CXX" -std=c++17 -Wall -Wextra -Wpedantic -DCON_OPENCV "${opciones[@]}" \
        "${fuentes[@]}" "${bibliotecas[@]}" \
        -lopencv_core -lopencv_imgcodecs -lopencv_highgui -o build/catalogo
    echo "Listo. Ejecuta: ./build/catalogo"
else
    echo "Uso: ./compilar.sh [grafico|consola]"
    exit 1
fi
