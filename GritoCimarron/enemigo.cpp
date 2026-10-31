#include "Enemigo.h"
#include <QDebug>

Enemigo::Enemigo(int saludInicial, float xMin, float xMax, QGraphicsItem* parent)
    : Personaje(saludInicial, parent),
    tipoIA(1),
    limiteIzquierda(xMin),
    limiteDerecha(xMax),
    moviendoDerecha(true) {
}

Enemigo::~Enemigo() {}

void Enemigo::mover() {
    if (moviendoDerecha) {
        setPos(x() + velocidad, y()); // Mover a la derecha
        setTransform(QTransform().scale(1, 1));

        // Si llega al limite derecho, cambia de sentido
        if (x() >= limiteDerecha) {
            moviendoDerecha = false;
        }
    } else {
        setPos(x() - velocidad, y()); // Mover a la izquierda ⬅
        setTransform(QTransform().scale(-1, 1)); // Voltear sprite

        // Si llega al limite izquierdo, cambia de sentido
        if (x() <= limiteIzquierda) {
            moviendoDerecha = true;
        }
    }
}

void Enemigo::atacar() {
    qDebug() << "El enemigo ataca o dispara!";
}
