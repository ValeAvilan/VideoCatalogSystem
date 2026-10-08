#pragma once
#include <ostream>

using namespace std;

class Tiempo {
private:
    int hora;
    int min;
public:
    Tiempo();
    Tiempo(int hr, int m);
    int getMinutosTotales() const;
    void imprimir() const;
    Tiempo operator+(const Tiempo& otro) const;
    bool operator<(const Tiempo& otro) const;
    friend ostream& operator<<(ostream& salida, const Tiempo& tiempo);
};
