#ifndef JUEGO_H
#define JUEGO_H

#include <QGraphicsView>
#include <QTimer>
#include "jugador.h"
#include "nivel.h"
#include "Puntuacion.h"

class Juego : public QGraphicsView {
    Q_OBJECT

private:
    Nivel* nivel;
    Jugador* jugador;
    Puntuacion* puntuacion;
    QTimer* gameTimer;

public:
    explicit Juego(QWidget* parent = nullptr);
    virtual ~Juego();
    void probarEscena();
    void iniciarJuego();
    void cargarNivel();

public slots:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void gameLoop();
};

#endif // JUEGO_H