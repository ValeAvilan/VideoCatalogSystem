#include "../catalogo.h"
#include "../pelicula.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>

using namespace std;

bool cerca(double a, double b) { return abs(a - b) < 0.00001; }

int main() {
    Tiempo a(1, 80), b(0, 40);
    assert(a.getMinutosTotales() == 140);
    assert((a + b) == Tiempo(3, 0));
    assert(b < a);
    ostringstream salida;
    salida << a;
    assert(salida.str() == "2 h 20 min (140 min)");
    bool rechazo = false;
    try { Tiempo negativo(-1, 20); } catch (const invalid_argument&) { rechazo = true; }
    assert(rechazo);
    rechazo = false;
    try { Tiempo grande(0, numeric_limits<int>::max()); auto total = grande + b; (void)total; }
    catch (const overflow_error&) { rechazo = true; }
    assert(rechazo);

    Calificacion calificacion;
    assert(calificacion.getPromedio() == 0);
    assert(!calificacion.agregar(0));
    assert(!calificacion.agregar(6));
    assert(calificacion.agregar(4) && calificacion.agregar(5));
    assert(calificacion.getCantidad() == 2);
    assert(cerca(calificacion.getPromedio(), 4.5));

    Catalogo catalogo;
    string error;
    assert(catalogo.cargar("datos.csv", error));
    assert(catalogo.buscarVideo(999) == nullptr);
    assert(catalogo.buscarSerie(101) == nullptr);
    assert(catalogo.buscarSerie(201)->getDuracion() == Tiempo(1, 6));
    assert(cerca(catalogo.buscarSerie(201)->getPuntuacion(), 34.0 / 11));
    assert(cerca(catalogo.buscarSerie(202)->getPuntuacion(), 33.0 / 11));
    assert(catalogo.calificar(201, 5));
    assert(cerca(catalogo.buscarSerie(201)->getPuntuacion(), 39.0 / 12));
    assert(!catalogo.calificar(101, 6));
    assert(!catalogo.calificar(999, 4));
    assert(catalogo.calificar(101, 1));
    assert(cerca(catalogo.buscarVideo(101)->getPuntuacion(), 19.0 / 6));
    assert(catalogo.calificar(301, 1));
    assert(cerca(catalogo.buscarSerie(201)->getPuntuacion(), 40.0 / 13));
    assert(catalogo.calificar(304, 5));
    assert(cerca(catalogo.buscarSerie(202)->getPuntuacion(), 38.0 / 12));

    // Despacho virtual: una referencia a Video utiliza la version de Serie.
    Video& base = *catalogo.buscarSerie(201);
    assert(base.getTipo() == "Serie");
    assert(base.getDuracion() == Tiempo(1, 6));
    assert(base.calificar(4));
    assert(cerca(base.getPuntuacion(), 44.0 / 14));

    // Una carga fallida no debe borrar ni cambiar las notas de la sesion.
    const string valido = "P;888;Ejemplo;Drama;60;0;0;0;2020;A;;4\n";
    const string invalidos[] = {
        "P;888;Duplicada;Drama;60;0;0;0;2020;A;;4\n",
        "E;900;Huerfano;Drama;60;999;1;1;0;;;4\n",
        "P;900;Mala nota;Drama;60;0;0;0;2020;A;;6\n",
        "P;900;Minutos invalidos;Drama;60abc;0;0;0;2020;A;;4\n",
        "P;900;Duracion negativa;Drama;-1;0;0;0;2020;A;;4\n",
        "P;900;Faltan columnas\n",
        "Z;900;Tipo desconocido;Drama;60;0;0;0;2020;A;;4\n"
    };
    for (const string& invalido : invalidos) {
        ofstream archivo("build/datos_prueba.csv");
        archivo << valido << invalido;
        archivo.close();
        assert(!catalogo.cargar("build/datos_prueba.csv", error));
        assert(!error.empty());
        assert(catalogo.buscarVideo(888) == nullptr);
        assert(cerca(catalogo.buscarVideo(101)->getPuntuacion(), 19.0 / 6));
    }
    assert(!catalogo.cargar("build/no_existe.csv", error));
    assert(catalogo.cargar("datos.csv", error));
    assert(cerca(catalogo.buscarVideo(101)->getPuntuacion(), 18.0 / 5));
    // Las notas vacias exportadas por Excel no son votos ni causan errores.
    ofstream excel("build/datos_excel.csv");
    excel << "S;800;Serie ejemplo;Drama;0;0;0;0;0;;;3,4,,,\n"
          << "E;801;Capitulo ejemplo;Drama;20;800;1;1;0;;;,,,\n";
    excel.close();
    assert(catalogo.cargar("build/datos_excel.csv", error));
    assert(catalogo.buscarSerie(800)->getCantidadCalificaciones() == 2);
    assert(cerca(catalogo.buscarSerie(800)->getPuntuacion(), 3.5));
    assert(catalogo.buscarVideo(801)->getCantidadCalificaciones() == 0);
    cout << "Pruebas de duraciones, promedios, polimorfismo y carga: OK\n";
}
