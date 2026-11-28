#include "jaula.h"
#include <algorithm>

Jaula::Jaula(Rehen* rehen, int saludInicial, QGraphicsItem* parent)
    : Objeto(parent),
      salud(saludInicial),
      saludMaxima(saludInicial),
      rehenContenido(rehen) {

    // Hoja de sprites de la jaula:
    // "sprites/jaula.png" → 3 columnas × 1 fila, sprite de 84×92 px, escala 100%
    // Columna 0: intacta, Columna 1: dañada, Columna 2: casi destruida
    hoja.cargar("sprites/jaula.png", 84, 92, 3, 1, 100.0f);
    setPixmap(hoja.obtenerSprite(0, 0));
    setZValue(9);
}

Jaula::~Jaula() {
}

void Jaula::interactuar() {
    recibirGolpe(20);
}

bool Jaula::recibirGolpe(int dano) {
    if (destruida) return false;

    salud = std::max(0, salud - dano);
    int pct = (salud * 100) / std::max(1, saludMaxima);
    int columna = (pct > 66) ? 0 : (pct > 33 ? 1 : 2);
    setPixmap(hoja.obtenerSprite(0, columna));

    if (salud <= 0) {
        destruida = true;
        interactuable = false;
        if (rehenContenido) {
            rehenContenido->liberar();
        }
        // Indica que acaba de destruirse para lanzar escombros parabolicos
        return true; 
    }
    return false;
}
