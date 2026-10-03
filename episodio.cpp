#include "episodio.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Episodio::Episodio(int id, const string& nom, Tiempo dur, const string& gen,
                   int temp, int num)
    : Video(id, nom, dur, gen), temporada(temp), numero(num) {
    if (temp <= 0 || num <= 0) throw invalid_argument("Temporada y episodio deben valores positios.");
}
string Episodio::getTipo() const { return "Episodio"; }
void Episodio::imprimir() const {
    imprimirDatos();
    cout << " | Temporada: " << temporada << " | Episodio: " << numero << '\n';
}
