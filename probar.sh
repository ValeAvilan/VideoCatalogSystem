#!/bin/bash
set -e
cd "$(dirname "$0")"
mkdir -p build
"${CXX:-clang++}" -std=c++17 -Wall -Wextra -Wpedantic \
    -fsanitize=address,undefined -g -I. tests/pruebas.cpp \
    tiempo.cpp calificacion.cpp video.cpp pelicula.cpp episodio.cpp serie.cpp catalogo.cpp \
    -o build/pruebas
./build/pruebas
