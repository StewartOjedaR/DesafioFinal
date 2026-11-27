#include "jugador.h"
#include "objeto.h"
#include "rehen.h"
#include <QKeyEvent>
#include <QDebug>

Jugador::Jugador(int saludInicial, QGraphicsItem* parent)
    : Personaje(saludInicial, parent), vidas(3) {

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    // Cargar hoja de sprites del jugador
    // Parametros: ruta, anchoFrame, altoFrame, columnas, escala%
    cargarHojaSprites("sprites/jugador.png", 64, 64, 4, 100.0f);
}

Jugador::~Jugador() {}

void Jugador::verificarColisiones() {
}

void Jugador::atacar() {
    qDebug() << "Jugador ataca!";
}

void Jugador::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
        setMoverIzquierda(true);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        setMoverDerecha(true);
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        setMoverArriba(true);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        setMoverAbajo(true);
        break;
    }
}

void Jugador::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
        setMoverIzquierda(false);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        setMoverDerecha(false);
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        setMoverArriba(false);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        setMoverAbajo(false);
        break;
    }
}