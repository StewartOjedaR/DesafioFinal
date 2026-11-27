#ifndef NIVEL_H
#define NIVEL_H

#include <QGraphicsScene>
#include "enemigo.h"
#include "jugador.h"
#include "arma.h"
#include "rehen.h"

class Nivel : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit Nivel(QWidget *parent = nullptr);
    ~Nivel();
    void actualizarEscena();
    QGraphicsScene* getEscena();
    Jugador* getJugador() const { return jugador; }

private:
    QGraphicsScene *escena;
    Jugador *jugador;
    QList<Enemigo*> enemigos;
    QList<Arma*> armas;
    QList<Rehen*> rehenes;
};

#endif