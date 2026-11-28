// Incluye la cabecera mejora.h.
#include "mejora.h"
// Incluye la cabecera jugador.h.
#include "jugador.h"
// Incluye la cabecera arma.h.
#include "arma.h"
// Incluye la cabecera cmath.
#include <cmath>

// Continua la definicion de esta instruccion.
Mejora::Mejora(Tipo tipoMejora, int valorMejora, QGraphicsItem* parent)
    // Declara o define la funcion Objeto.
    : Objeto(parent), tipo(tipoMejora), valor(valorMejora) {

    // Hoja de sprites de mejoras:
    // "sprites/mejoras.png" → 3 columnas × 1 fila, sprite de 34×34 px, escala 100%
    // Columna 0: Salud (+vida), Columna 1: Municion (+balas), Columna 2: Velocidad (+rapidez)
    // Continua la definicion de esta instruccion.
    hoja.cargar("sprites/mejoras.png", 34, 34, 3, 1, 100.0f);
    // Declara o define la funcion setPixmap.
    setPixmap(hoja.obtenerSprite(0, static_cast<int>(tipo)));
    // Declara o define la funcion setZValue.
    setZValue(10);
// Cierra el bloque anterior.
}

// Declara o define la funcion Mejora.
Mejora::~Mejora() {}

// Continua la definicion de esta instruccion.
void Mejora::setPosicionBase(qreal x, qreal y) {
    // Asigna un valor a la variable indicada.
    baseY = y;
    // Declara o define la funcion setPos.
    setPos(x, y);
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Mejora::actualizarAnimacion(qreal dt) {
    // Comprueba una condicion antes de continuar.
    if (recogida) return;
    // Continua la definicion de esta instruccion.
    tiempoAnim += dt;
    // Declara o define la funcion setY.
    setY(baseY + std::sin(tiempoAnim * 4.0) * 4.0);
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Mejora::interactuar() {
    // Asigna un valor a la variable indicada.
    recogida = true;
    // Asigna un valor a la variable indicada.
    interactuable = false;
    // Declara o define la funcion setVisible.
    setVisible(false);
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Mejora::aplicarA(Jugador* jugador) {
    // Comprueba una condicion antes de continuar.
    if (recogida || !jugador) return;

    // Declara o define la funcion switch.
    switch (tipo) {
    // Continua la definicion de esta instruccion.
    case SALUD:
        // Continua la definicion de esta instruccion.
        jugador->curar(valor);
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case MUNICION:
        // Comprueba una condicion antes de continuar.
        if (jugador->getArmaEquipada()) {
            // Continua la definicion de esta instruccion.
            jugador->getArmaEquipada()->recargar(valor);
        // Cierra el bloque anterior.
        }
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case VELOCIDAD:
        // Continua la definicion de esta instruccion.
        jugador->aplicarModificadorVelocidad(1.42, 6.0);
        // Continua la definicion de esta instruccion.
        break;
    // Cierra el bloque anterior.
    }
    // Declara o define la funcion interactuar.
    interactuar();
// Cierra el bloque anterior.
}
