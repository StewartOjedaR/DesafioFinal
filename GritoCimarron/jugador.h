// Inicia la guarda JUGADOR_H contra inclusiones duplicadas.
#ifndef JUGADOR_H
// Define la guarda JUGADOR_H.
#define JUGADOR_H

// Incluye la cabecera personaje.h.
#include "personaje.h"
// Incluye la cabecera arma.h.
#include "arma.h"
// Incluye la cabecera QKeyEvent.
#include <QKeyEvent>
// Incluye la cabecera QPointF.
#include <QPointF>

// Declara la clase Objeto.
class Objeto;
// Declara la clase Rehen.
class Rehen;

// Declara la clase Jugador.
class Jugador : public Personaje {
    // Activa las senales y ranuras de Qt en esta clase.
    Q_OBJECT

// Inicia la seccion private de la clase.
private:
    // Declara o inicializa una variable.
    int vidas = 3;
    // Declara o inicializa una variable.
    Arma* armaEquipada = nullptr;
    // Declara o inicializa una variable.
    qreal cooldownMelee = 0.0;
    // Declara o inicializa una variable.
    qreal ultimoAimX = 1.0;
    // Declara o inicializa una variable.
    qreal ultimoAimY = 0.0;
    // Declara o inicializa una variable.
    bool usandoMouseAim = false;
    // Declara o inicializa una variable.
    QPointF objetivoMouseEscena;

// Inicia la seccion public de la clase.
public:
    // Declara o define la funcion Jugador.
    explicit Jugador(int saludInicial = 100, QGraphicsItem* parent = nullptr);
    // Declara o define la funcion Jugador.
    virtual ~Jugador();

    // Declara o define la funcion atacar.
    void atacar() override;
    // Declara o define la funcion dispararArma.
    void dispararArma(bool comoLanzable = false);
    // Declara o define la funcion actualizarTimers.
    void actualizarTimers(qreal dt);
    // Declara o define la funcion verificarColisiones.
    void verificarColisiones();

    // Declara o define la funcion equiparArma.
    void equiparArma(Arma* nuevaArma);
    // Declara o define la funcion getArmaEquipada.
    Arma* getArmaEquipada() const { return armaEquipada; }

    // Declara o define la funcion rescatar.
    void rescatar(Rehen* rehen);
    // Declara o define la funcion interactuar.
    void interactuar(Objeto* objeto);

    // Declara o define la funcion getVidas.
    int getVidas() const { return vidas; }
    // Declara o define la funcion setVidas.
    void setVidas(int v) { vidas = v; }
    // Declara o define la funcion consumirVidaYReiniciarSalud.
    bool consumirVidaYReiniciarSalud();
    // Declara o define la funcion limpiarTeclas.
    void limpiarTeclas();

    // Declara o define la funcion setObjetivoMouse.
    void setObjetivoMouse(const QPointF& posEscena);
    // Declara o define la funcion direccionApuntado.
    void direccionApuntado(qreal& outDx, qreal& outDy) const;

    // Declara o define la funcion keyPressEvent.
    void keyPressEvent(QKeyEvent* event) override;
    // Declara o define la funcion keyReleaseEvent.
    void keyReleaseEvent(QKeyEvent* event) override;

// Inicia la seccion signals de la clase.
signals:
    // Declara o define la funcion disparoSolicitado.
    void disparoSolicitado(QPointF origen, qreal dirX, qreal dirY, int dano, bool lanzable);
    // Declara o define la funcion ataqueMeleeSolicitado.
    void ataqueMeleeSolicitado(QRectF areaGolpe, int dano);
    // Declara o define la funcion interaccionSolicitada.
    void interaccionSolicitada();
// Cierra el bloque anterior.
};

// JUGADOR_H
#endif 
