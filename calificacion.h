#pragma once
#include <vector>

using namespace std;

class Calificacion {
private:
    vector<int> valores;
public:
    bool agregar(int valor);
    double getPromedio() const;
    int getCantidad() const;
};
