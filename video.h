#pragma once
#include "tiempo.h"
#include "calificacion.h"
#include <string>

using namespace std;

class Video {
protected:
    int ID;
    string nombre, genero, portada;
    Tiempo duracion;
    Calificacion calificaciones;
    void imprimirDatos() const;
public:
    Video(int id, const string& nombre, Tiempo duracion, const string& genero,
          const string& portada = "");
    virtual ~Video() = default; //para destruir correctamente las clases hijas.
    int getID() const;
    string getNombre() const;
    string getGenero() const;
    string getPortada() const;
    virtual Tiempo getDuracion() const;
    virtual double getPuntuacion() const;
    virtual int getCantidadCalificaciones() const;
    virtual bool calificar(int valor);
    virtual string getTipo() const = 0;
    virtual void imprimir() const = 0; //Video es una clase abstracta porque tiene al menos una funcion virtual pura
};
