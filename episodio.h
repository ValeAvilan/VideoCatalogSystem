#pragma once
#include "video.h"

using namespace std;

class Episodio : public Video {
private:
    int temporada, numero;
public:
    Episodio(int id, const string& nombre, Tiempo duracion, const string& genero,
             int temporada, int numero);
    string getTipo() const override;
    void imprimir() const override;
};
