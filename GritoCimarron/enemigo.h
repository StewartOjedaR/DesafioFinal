#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"

class Enemigo : public Personaje {
    Q_OBJECT

private:
    int tipoIA;              // 1: Patrulla fija, 2: Persecucion
    float limiteIzquierda;   // Limite izquierdo de patrulla
    float limiteDerecha;     // Limite derecho de patrulla

public:
    explicit Enemigo(int saludInicial = 50, float xMin = 0, float xMax = 200, QGraphicsItem* parent = nullptr);
    virtual ~Enemigo();
    void atacar() override;

private slots:
    void onPatrullaFija();
    void onPersecucion();
};

#endif // ENEMIGO_H