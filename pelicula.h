#pragma once
#include "video.h"

using namespace std;

class Pelicula : public Video {
private:
    int anio;
    string clasificacion;
public:
    Pelicula(int id, const string& nombre, Tiempo duracion, const string& genero,
             int anio, const string& clasificacion, const string& portada);
    string getTipo() const override;
    void imprimir() const override;
};
