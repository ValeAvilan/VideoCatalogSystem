#include "catalogo.h"
#include "pelicula.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <stdexcept>

using namespace std;

namespace {
// CSV sencillo separado por ;. No admite campos con ; ni comillas especiales.
vector<string> separar(const string& linea, char separador) {
    vector<string> campos;
    string campo;
    istringstream entrada(linea);
    while (getline(entrada, campo, separador)) campos.push_back(campo);
    if (!linea.empty() && linea.back() == separador) campos.push_back("");
    return campos;
}
int entero(const string& texto) {
    size_t usados;
    int valor = stoi(texto, &usados);
    if (usados != texto.size()) throw invalid_argument("Numero invalido: " + texto);
    return valor;
}
string minusculas(string texto) {
    for (char& letra : texto) letra = static_cast<char>(tolower(static_cast<unsigned char>(letra)));
    return texto;
}
void agregarNotas(Video& video, const string& texto) {
    if (texto.empty()) return;
    for (const string& nota : separar(texto, ',')) {
        // Excel puede guardar celdas de notas vacias como comas adicionales.
        if (nota.empty()) continue;
        if (!video.calificar(entero(nota))) throw invalid_argument("Calificacion fuera de 1 a 5.");
    }
}
}

bool Catalogo::cargar(const string& archivo, string& error) {
    error.clear();
    ifstream entrada(archivo);
    if (!entrada) { error = "No se pudo abrir " + archivo; return false; }
    Catalogo nuevo; //solo se puede remplazar si el formato del archivo es válido, si no se conserva el anterior
    string linea;
    int numeroLinea = 0;
    try {
        while (getline(entrada, linea)) {
            numeroLinea++;
            if (!linea.empty() && linea.back() == '\r') linea.pop_back();

            if (linea.empty() || linea[0] == '#') continue;

            auto c = separar(linea, ';');
            if (c.size() != 12) throw invalid_argument("Se esperan 12 columnas.");

            int id = entero(c[1]);
            if (nuevo.buscarVideo(id)) throw invalid_argument("ID repetido.");

            if (c[0] == "P") {
                nuevo.videos.push_back(make_unique<Pelicula>(id, c[2], Tiempo(0, entero(c[4])),
                                                            c[3], entero(c[8]), c[9], c[10]));
                agregarNotas(*nuevo.videos.back(), c[11]);
            } else if (c[0] == "S") {
                nuevo.videos.push_back(make_unique<Serie>(id, c[2], c[3], c[10]));
                agregarNotas(*nuevo.videos.back(), c[11]);
            } else if (c[0] == "E") {
                Serie* serie = nuevo.buscarSerie(entero(c[5]));
                if (!serie) throw invalid_argument("La serie debe aparecer antes de sus episodios.");
                if (c[3] != serie->getGenero()) throw invalid_argument("Genero distinto al de la serie.");
                Episodio episodio(id, c[2], Tiempo(0, entero(c[4])), c[3], entero(c[6]), entero(c[7]));
                agregarNotas(episodio, c[11]);
                serie->agregarEpisodio(episodio);
            } else throw invalid_argument("Tipo desconocido: usa P, S o E.");
        }
        if (entrada.bad()) throw runtime_error("Error al leer el archivo.");
        if (nuevo.videos.empty()) throw invalid_argument("El archivo no contiene videos.");
        videos.swap(nuevo.videos); // El catalogo anterior se destruye automaticamente.
        return true;
    } catch (const exception& e) {
        error = "Linea " + to_string(numeroLinea) + ": " + e.what();
        return false;
    }
}

Video* Catalogo::buscarVideo(int id) {
    for (const auto& video : videos) {
        if (video->getID() == id) return video.get();
        Serie* serie = dynamic_cast<Serie*>(video.get());
        if (serie) {
            Episodio* episodio = serie->buscarEpisodio(id);
            if (episodio) return episodio;
        }
    }
    return nullptr;
}
Serie* Catalogo::buscarSerie(int id) { return dynamic_cast<Serie*>(buscarVideo(id)); }
Video* Catalogo::buscarVideo(const string& titulo) {
    for (const auto& video : videos) {
        if (minusculas(video->getNombre()) == minusculas(titulo)) return video.get();
        Serie* serie = dynamic_cast<Serie*>(video.get());
        if (serie) {
            Episodio* episodio = serie->buscarEpisodio(titulo);
            if (episodio) return episodio;
        }
    }
    return nullptr;
}
Serie* Catalogo::buscarSerie(const string& titulo) {
    return dynamic_cast<Serie*>(buscarVideo(titulo));
}

void Catalogo::mostrar(double minimo, const string& genero, bool soloPeliculas, bool soloSeries) const {
    bool encontrado = false;
    for (const auto& video : videos) {
        if (soloPeliculas && video->getTipo() != "Pelicula") continue;
        if (soloSeries && video->getTipo() != "Serie") continue;
        if (!genero.empty() && minusculas(video->getGenero()) != minusculas(genero)) continue;
        if (video->getPuntuacion() < minimo) continue;
        video->imprimir(); // POLIMORFISMO: la misma llamada, diferentes clases reales.
        encontrado = true;
    }
    if (!encontrado) cout << "No hay videos los requisitos.\n";
}
bool Catalogo::calificar(int id, int valor) {
    Video* video = buscarVideo(id);
    return video && video->calificar(valor);
}
