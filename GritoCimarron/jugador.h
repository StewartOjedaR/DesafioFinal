#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.h"
#include <QKeyEvent>

class Jugador : public Personaje {
    Q_OBJECT

private:
    int vidas;

public:
    explicit Jugador(int saludInicial = 100, QGraphicsItem* parent = nullptr);
    virtual ~Jugador();

    void atacar() override;
    void verificarColisiones();
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
};

#endif // JUGADOR_H