#pragma once
#include "video.h"
#include "serie.h"
#include <memory>
#include <vector>

using namespace std;

class Catalogo {
private:
    vector<unique_ptr<Video>> videos;
public:
    bool cargar(const string& archivo, string& error);
    Video* buscarVideo(int id);
    Video* buscarVideo(const string& titulo);
    Serie* buscarSerie(int id);
    Serie* buscarSerie(const string& titulo);
    void mostrar(double minimo = 0, const string& genero = "", bool soloPeliculas = false,
                 bool soloSeries = false) const;
    bool calificar(int id, int valor);
};
