#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include <QPointF>
#include <QList>
#include <QRectF>

// Declara antes la clase Jugador para poder usar punteros.
class Jugador;

class Enemigo : public Personaje {
    // Activa las senales y ranuras de Qt en esta clase.
    Q_OBJECT

private:
    // 1: Patrulla fija, 2: Persecucion, 3: Rastreador inteligente
    int tipoIA = 1;              
    float limiteIzquierda = 0.0f;
    float limiteDerecha = 200.0f;
    qreal rangoDeteccion = 360.0;
    qreal cooldownDisparo = 1.2;
    int puntosRecompensa = 150;

    // Estado de aprendizaje adaptativo para tipoIA == 3 (Soldado Rastreador)
    // Aprende hacia que lado esquiva el jugador (-1 o +1)
    qreal sesgoEvasionLateral = 0.0; 
    // Crece de 0.0 a 1.0 mientras observa al jugador
    qreal nivelAprendizaje = 0.0;    
    qreal timerCambioEstrategia = 0.0;

    bool moviendoDerechaPatrulla = true;
    Jugador* objetivoActual = nullptr;
    QPointF puntoMetaEscape;

public:
    explicit Enemigo(int saludInicial = 50, float xMin = 0.0f, float xMax = 200.0f,
                     int tipo = 1, QGraphicsItem* parent = nullptr);
    virtual ~Enemigo();

    void atacar() override;
    void actualizarIA(qreal dt, Jugador* jugador, const QList<QRectF>& muros,
                      const QPointF& metaEscape);

    int getTipoIA() const { return tipoIA; }
    int getPuntosRecompensa() const { return puntosRecompensa; }
    qreal getNivelAprendizaje() const { return nivelAprendizaje; }

signals:
    void disparoEnemigo(QPointF origen, qreal dirX, qreal dirY, int dano);

private slots:
    void onPatrullaFija();
    void onPersecucion();
    void onRastreadorInteligente(qreal dt, const QList<QRectF>& muros);
};

// ENEMIGO_H
#endif 
