#ifndef ESCOMBRO_H
#define ESCOMBRO_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QRectF>

// Escombros proyectados en movimiento parabolico al destruir jaulas (Nivel 1)
class Escombro : public QGraphicsPixmapItem {
private:
    // Posicion horizontal [px]
    qreal rx = 0.0;                 
    // Posicion vertical [px]
    qreal ry = 0.0;                 
    // Velocidad horizontal [px/s]
    qreal vx = 0.0;                 
    // Velocidad vertical [px/s]
    qreal vy = 0.0;                 
    // Radio de colision [px]
    qreal radio_px = 11.0;          
    // Coeficiente de restitucion inelastico con el suelo
    qreal e_restitucion = 0.42;     
    // Factor de perdida tangencial por friccion al impactar
    qreal mu_friccion = 0.72;       
    // Angulo de rotacion visual [grados]
    qreal theta_deg = 0.0;          
    // Velocidad angular visual [grados/s]
    qreal omega_deg_s = 0.0;        
    qreal tiempoVida_s = 4.5;
    bool peligroso = true;
    bool activo = true;

public:
    Escombro(qreal xOrigen, qreal yOrigen, qreal v0_px_s,
             qreal theta0_grados, int variante, QGraphicsItem* parent = nullptr);

    void actualizar(qreal dt, const QList<QRectF>& suelos, qreal altoMundo);
    bool isActivo() const { return activo; }
    bool isPeligroso() const { return peligroso; }
    void marcarInofensivo() { peligroso = false; }
};

// ESCOMBRO_H
#endif 
