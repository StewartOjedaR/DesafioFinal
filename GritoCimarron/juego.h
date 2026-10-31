#ifndef JUEGO_H
#define JUEGO_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "jugador.h"
#include "enemigo.h"
#include "rehen.h"
#include "Puntuacion.h"

class Juego : public QGraphicsView {
    Q_OBJECT

private:
    QGraphicsScene* escena;       // El mundo donde viven los objetos
    Jugador* jugador;             //  El personaje principal
    Puntuacion* puntuacion;       // Contador de puntos
    QTimer* timerGameLoop;        // Bucle principal del juego (Game Loop)

public:
    explicit Juego(QWidget* parent = nullptr);
    virtual ~Juego();
    void probarEscena();
    void iniciarJuego();
    void cargarNivel();

public slots:
    void gameLoop(); // Se ejecuta frame a frame para actualizar la escena
};

#endif // JUEGO_H