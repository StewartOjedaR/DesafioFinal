#include "Puntuacion.h"
#include <QString>

Puntuacion::Puntuacion(QGraphicsItem* parent)
    : QGraphicsTextItem(parent), puntosActuales(0), recordMaximo(0) {

    // Configuracion visual del texto en Qt 🎨
    setDefaultTextColor(Qt::yellow); // Color del texto
    setFont(QFont("Arial", 16, QFont::Bold)); // Fuente y tamaño

    actualizarTexto(); // Muestra "Puntos: 0" al inicio
}

Puntuacion::~Puntuacion() {}

void Puntuacion::agregarPuntos(int cantidad) {
    puntosActuales += cantidad;

    // Si superamos el record, lo actualizamos
    if (puntosActuales > recordMaximo) {
        recordMaximo = puntosActuales;
    }

    actualizarTexto(); // Refrescamos el texto en pantalla 🔄
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
    // Cambia el texto dibujado en la escena de Qt
    setPlainText(QString("Puntos: ") + QString::number(puntosActuales));
}