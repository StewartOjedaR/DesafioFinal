#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"

class Enemigo : public Personaje {
    Q_OBJECT

private:
    int tipoIA;              // 1: Patrulla fija, 2: Persecución
    float limiteIzquierda;   //  Limite izquierdo de patrulla
    float limiteDerecha;     // Limite derecho de patrulla
    bool moviendoDerecha;    //  Direccion actual del movimiento

public:
    explicit Enemigo(int saludInicial = 50, float xMin = 0, float xMax = 200, QGraphicsItem* parent = nullptr);
    virtual ~Enemigo();

    // Sobrescribimos los métodos virtuales puros de la clase base Personaje
    void mover() override;
    void atacar() override;
};

#endif // ENEMIGO_H