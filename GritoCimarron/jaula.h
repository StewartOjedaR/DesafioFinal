#ifndef JAULA_H
#define JAULA_H

#include "objeto.h"
#include "rehen.h"
#include "hojasprites.h"

// Infraestructura de la hacienda que encierra a un hermano cimarron (Nivel 1)
class Jaula : public Objeto {
    // Activa las senales y ranuras de Qt en esta clase.
    Q_OBJECT

private:
    int salud = 60;
    int saludMaxima = 60;
    bool destruida = false;
    Rehen* rehenContenido = nullptr;
    HojaSprites hoja;

public:
    explicit Jaula(Rehen* rehen, int saludInicial = 60, QGraphicsItem* parent = nullptr);
    virtual ~Jaula();

    void interactuar() override;
    bool recibirGolpe(int dano);
    bool isDestruida() const { return destruida; }
    Rehen* getRehen() const { return rehenContenido; }
};

// JAULA_H
#endif 
