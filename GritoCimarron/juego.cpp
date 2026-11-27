#include "juego.h"
#include <QKeyEvent>

Juego::Juego(QWidget* parent) : QGraphicsView(parent), nivel(nullptr), jugador(nullptr), gameTimer(nullptr) {
    nivel = new Nivel();
    setFixedSize(1280, 720);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::StrongFocus);
    setScene(nivel);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Juego::gameLoop);
    gameTimer->start(16);
}

Juego::~Juego() {
    if (gameTimer) {
        gameTimer->stop();
    }
}

void Juego::iniciarJuego() {
    if (nivel) {
        jugador = nivel->getJugador();
    }
    if (gameTimer && !gameTimer->isActive()) {
        gameTimer->start(16);
    }
}

void Juego::gameLoop() {
    if (!nivel) {
        return;
    }

    if (jugador) {
        if (jugador->getMoverIzquierda()) {
            jugador->setVx(-jugador->getVelocidad());
        } else if (jugador->getMoverDerecha()) {
            jugador->setVx(jugador->getVelocidad());
        } else {
            jugador->setVx(0);
        }

        if (jugador->getMoverArriba()) {
            jugador->setVy(-jugador->getVelocidad());
        } else if (jugador->getMoverAbajo()) {
            jugador->setVy(jugador->getVelocidad());
        } else {
            jugador->setVy(0);
        }
    }

    nivel->actualizarEscena();

    if (puntuacion) {
        puntuacion->actualizarTexto();
    }

    update();
}

void Juego::keyPressEvent(QKeyEvent* event) {
    if (jugador) {
        QGraphicsView::keyPressEvent(event);
        jugador->keyPressEvent(event);
    }
}

void Juego::keyReleaseEvent(QKeyEvent* event) {
    QGraphicsView::keyReleaseEvent(event);
}

void Juego::cargarNivel() {
    nivel->getEscena();
}