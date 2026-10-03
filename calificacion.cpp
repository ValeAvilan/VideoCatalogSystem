#include "calificacion.h"

using namespace std;

bool Calificacion::agregar(int valor) {
    if (valor < 1 || valor > 5) return false;
    valores.push_back(valor);
    return true;
}
double Calificacion::getPromedio() const {
    if (valores.empty()) return 0.0;
    double suma = 0;
    for (int valor : valores) suma += valor;
    return suma / valores.size();
}
int Calificacion::getCantidad() const { return static_cast<int>(valores.size()); }
