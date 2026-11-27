#include "Puntuacion.h"
#include <QString>

Puntuacion::Puntuacion(QGraphicsItem* parent)
    : QGraphicsTextItem(parent), puntosActuales(0), recordMaximo(0) {

    setDefaultTextColor(Qt::yellow);
    setFont(QFont("Arial", 16, QFont::Bold));
    actualizarTexto();
}

Puntuacion::~Puntuacion() {}

void Puntuacion::agregarPuntos(int cantidad) {
    puntosActuales += cantidad;
    if (puntosActuales > recordMaximo) {
        recordMaximo = puntosActuales;
    }
    actualizarTexto();
}

void Puntuacion::reiniciarPuntos() {
    puntosActuales = 0;
    actualizarTexto();
}

int Puntuacion::getPuntosActuales() const {
    return puntosActuales;
}

int Puntuacion::getRecordMaximo() const {
    return recordMaximo;
}

void Puntuacion::actualizarTexto() {
    setPlainText(QString("Puntos: ") + QString::number(puntosActuales));
}