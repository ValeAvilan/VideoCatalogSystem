#include "serie.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Serie::Serie(int id, const string& nom, const string& gen, const string& ruta)
    : Video(id, nom, Tiempo(), gen, ruta) {}

void Serie::agregarEpisodio(const Episodio& episodio) {
    if (buscarEpisodio(episodio.getID())) throw invalid_argument("ID de episodio repetido.");
    // Comprobar la suma antes de modificar la serie.
    Tiempo total = getDuracion() + episodio.getDuracion();
    (void)total;
    episodios.push_back(episodio);
}
Episodio* Serie::buscarEpisodio(const string& titulo) {
    for (Episodio& episodio : episodios)
        if (episodio.getNombre() == titulo) return &episodio;
    return nullptr;
}
Episodio* Serie::buscarEpisodio(int id) {
    for (Episodio& episodio : episodios)
        if (episodio.getID() == id) return &episodio;
    return nullptr;
}
Tiempo Serie::getDuracion() const {
    Tiempo total;
    for (const Episodio& episodio : episodios) total = total + episodio.getDuracion();
    return total;
}
double Serie::getPuntuacion() const {
    // Todas las notas pesan lo mismo: las directas y las de cada episodio.
    double suma = Video::getPuntuacion() * Video::getCantidadCalificaciones();
    int cantidad = getCantidadCalificaciones();
    for (const Episodio& episodio : episodios) {
        suma += episodio.getPuntuacion() * episodio.getCantidadCalificaciones();
    }
    return cantidad == 0 ? 0 : suma / cantidad;
}
int Serie::getCantidadCalificaciones() const {
    int cantidad = Video::getCantidadCalificaciones();
    for (const Episodio& episodio : episodios) cantidad += episodio.getCantidadCalificaciones();
    return cantidad;
}
string Serie::getTipo() const { return "Serie"; }
void Serie::imprimir() const {
    imprimirDatos();
    cout << " | Episodios: " << episodios.size() << '\n';
}
void Serie::mostrarEpisodios(double minimo) const {
    bool encontrado = false;
    for (const Episodio& episodio : episodios) {
        if (episodio.getPuntuacion() >= minimo) {
            episodio.imprimir();
            encontrado = true;
        }
    }
    if (!encontrado) cout << "No hay episodios que cumplan el filtro.\n";
}
