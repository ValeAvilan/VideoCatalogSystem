#include "pelicula.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Pelicula::Pelicula(int id, const string& nom, Tiempo dur, const string& gen,
                 int year, const string& clas, const string& ruta)
    : Video(id, nom, dur, gen, ruta), anio(year), clasificacion(clas) {
    if (year <= 0 || clas.empty()) throw invalid_argument("Datos de pelicula invalidos.");
}
string Pelicula::getTipo() const { return "Pelicula"; }
void Pelicula::imprimir() const {
    imprimirDatos();
    cout << " | Anio: " << anio << " | Clasificacion: " << clasificacion << '\n';
}
