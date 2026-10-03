#pragma once
#include "video.h"
#include "episodio.h"
#include <vector>

using namespace std;

class Serie : public Video {
private:
    vector<Episodio> episodios; // Composicion: la serie es dueña de los episodios.
public:
    Serie(int id, const string& nombre, const string& genero,
          const string& portada = "");
    void agregarEpisodio(const Episodio& episodio);
    Episodio* buscarEpisodio(int id);
    Episodio* buscarEpisodio(const string& titulo);
    Tiempo getDuracion() const override;
    double getPuntuacion() const override;
    int getCantidadCalificaciones() const override;
    string getTipo() const override;
    void imprimir() const override;
    void mostrarEpisodios(double minimo = 0) const;
};
