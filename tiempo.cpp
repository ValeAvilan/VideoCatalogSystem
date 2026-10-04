#include "tiempo.h"
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

Tiempo::Tiempo() : hora(0), min(0) {}

Tiempo::Tiempo(int hr, int m) {
    if (hr < 0 || m < 0 || hr > (numeric_limits<int>::max() - m) / 60)
        throw invalid_argument("Duracion fuera de rango.");
    int total = hr * 60 + m;
    hora = total / 60;
    min = total % 60;
}

int Tiempo::getMinutosTotales() const { return hora * 60 + min; }
void Tiempo::imprimir() const { cout << *this; }

Tiempo Tiempo::operator+(const Tiempo& otro) const {
    int a = getMinutosTotales(), b = otro.getMinutosTotales();
    if (a > numeric_limits<int>::max() - b)
        throw overflow_error("La suma de duraciones es demasiado grande.");
    return Tiempo(0, a + b); // El constructor convierte los minutos a horas.
}

ostream& operator<<(ostream& salida, const Tiempo& tiempo) {
    return salida << tiempo.hora << " h " << tiempo.min << " min ("
                  << tiempo.getMinutosTotales() << " min)";
}
