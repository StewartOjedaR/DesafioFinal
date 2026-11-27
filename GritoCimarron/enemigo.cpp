#include "Enemigo.h"
#include <QDebug>

Enemigo::Enemigo(int saludInicial, float xMin, float xMax, QGraphicsItem* parent)
    : Personaje(saludInicial, parent),
    tipoIA(1),
    limiteIzquierda(xMin),
    limiteDerecha(xMax) {

    // Cargar hoja de sprites del enemigo
    // Parametros: ruta, anchoFrame, altoFrame, columnas, escala%
    cargarHojaSprites("sprites/enemigo.png", 64, 64, 4, 100.0f);
}

Enemigo::~Enemigo() {}

void Enemigo::atacar() {
    qDebug() << "El enemigo ataca o dispara!";
}

void Enemigo::onPatrullaFija() {
    qDebug() << "Patrulla Fija activada";
}

void Enemigo::onPersecucion() {
    qDebug() << "Persecución activada";
}