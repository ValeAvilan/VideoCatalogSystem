#include "video.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

using namespace std;

Video::Video(int id, const string& nom, Tiempo dur, const string& gen,
             const string& ruta)
    : ID(id), nombre(nom), genero(gen), portada(ruta), duracion(dur) {
    if (id <= 0 || nom.empty() || gen.empty())
        throw invalid_argument("El video necesita ID positivo, nombre y genero.");
}
int Video::getID() const { return ID; }
string Video::getNombre() const { return nombre; }
string Video::getGenero() const { return genero; }
string Video::getPortada() const { return portada; }
Tiempo Video::getDuracion() const { return duracion; }
double Video::getPuntuacion() const { return calificaciones.getPromedio(); }
int Video::getCantidadCalificaciones() const { return calificaciones.getCantidad(); }
bool Video::calificar(int valor) { return calificaciones.agregar(valor); }

void Video::imprimirDatos() const {
    cout << "[" << getTipo() << "] ID: " << ID << " | " << nombre
              << " | " << genero << " | Duracion: " << getDuracion() << " | ";
    if (getPuntuacion() == 0) cout << "Sin calificaciones";
    else cout << "Promedio: " << fixed << setprecision(2)
                   << round(getPuntuacion() * 2) / 2 << "/5";
}
