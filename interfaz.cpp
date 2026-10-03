#include "interfaz.h"
#include "catalogo.h"
#include "multimedia.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;

// getline evita problemas al mezclar numeros con titulos que tienen espacios.
string leerTexto(const string& mensaje) {
    cout << mensaje;
    string texto;
    if (!getline(cin, texto)) throw runtime_error("Fin de entrada.");
    return texto;
}
int leerEntero(const string& mensaje, int minimo, int maximo) {
    while (true) {
        istringstream entrada(leerTexto(mensaje));
        int valor;
        char sobrante;
        if (entrada >> valor && !(entrada >> sobrante) && valor >= minimo && valor <= maximo)
            return valor;
        cout << "Escribe un entero entre " << minimo << " y " << maximo << ".\n";
    }
}

// Las ventanas pertenecen a la interfaz, no a las clases de POO.
void consultarMultimedia(Video& video) {
    // AQUI VAN TUS IMAGENES: GUARDALAS EN LA CARPETA imagenes/.
    // LOS NOMBRES ESTAN EN imagenes/LEEME.md Y EN LA COLUMNA portada DE datos.csv.
    // LAS PELICULAS ABREN SU PORTADA DIRECTAMENTE; LAS SERIES TODAVIA PREGUNTAN.
    if (video.getTipo() == "Pelicula" || leerTexto("Mostrar portada (s/n): ") == "s") {
        if (video.getPortada().empty()) cout << "Este contenido no tiene portada configurada.\n";
        else mostrarPortada(video.getPortada());
    }

}

void elegirMultimedia(Catalogo& catalogo, bool soloPeliculas, bool soloSeries = false) {
    string titulo = leerTexto("Titulo para ver (Enter = omitir): ");
    if (titulo.empty()) return;
    Video* video = catalogo.buscarVideo(titulo);
    if (!video || video->getTipo() == "Episodio" ||
        (soloPeliculas && video->getTipo() != "Pelicula") ||
        (soloSeries && video->getTipo() != "Serie")) {
        cout << "No existe ese titulo de pelicula o serie.\n";
        return;
    }
    consultarMultimedia(*video);
}

int ejecutarInterfaz() {
    Catalogo catalogo;
    string error;
    bool cargado = false;
    cout << "\nVIDEO CATALOG SYSTEM\n";
    try {
        while (true) {
            cout << "\n=== MENÚ ===\n"
                         "1. Cargar archivo de datos\n"
                         "2. Mostrar videos (por calificación o género)\n"
                         "3. Mostrar series (por calificación o género)\n"
                         "4. Mostrar capítulos de una serie (por calificación)\n"
                         "5. Mostrar películas (por calificación o género)\n"
                         "6. Calificar un video\n"
                         "0. Salir\n";
            int opcion = leerEntero("Elige una opción: ", 0, 6);
            if (opcion == 0) break;
            if (opcion == 1) {
                if (catalogo.cargar("datos.csv", error)) {
                    cargado = true;
                    cout << "Datos cargados correctamente.\n";
                } else cout << error << "\nSe mantienen los datos del catálogo anterior.\n";
                continue;
            }
            if (!cargado) {
                cout << "Primero carga el archivo, seleccionando la opcion 1.\n";
                continue;
            }
            if (opcion == 2 || opcion == 3 || opcion == 5) {
                int minimo = leerEntero("Calificacion minima (0 = todos, 1-5): ", 0, 5);
                string genero = leerTexto("Genero (Accion, Romance, Comedia, Drama, Misterio; Enter = todos): ");
                catalogo.mostrar(minimo, genero, opcion == 5, opcion == 3);
                elegirMultimedia(catalogo, opcion == 5, opcion == 3);
            } else if (opcion == 4) {
                string titulo = leerTexto("Nombre de la serie: ");
                Serie* serie = catalogo.buscarSerie(titulo);
                if (!serie) { cout << "No existe una serie con ese titulo.\n"; continue; }
                int minimo = leerEntero("Calificacion minima (0 = todos): ", 0, 5);
                serie->imprimir();
                serie->mostrarEpisodios(minimo);
                consultarMultimedia(*serie);
            } else if (opcion == 6) {
                string titulo = leerTexto(">> Título del video a calificar: ");
                Video* video = catalogo.buscarVideo(titulo);
                if (!video) { cout << "No existe un video con ese titulo.\n"; continue; }
                int nota = leerEntero(">> Valor (1-5): ", 1, 5);
                if (catalogo.calificar(video->getID(), nota)) {
                    cout << "Calificación " << fixed << setprecision(2)
                              << static_cast<double>(nota) << " agregada a '"
                              << video->getNombre() << "'\n";
                    video->imprimir();
                }
            }
        }
    } catch (const runtime_error&) {
        cout << "\nEntrada cerrada.\n";
    }
    return 0;
}
