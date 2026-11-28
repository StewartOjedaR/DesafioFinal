#include "escombro.h"
#include "fisica.h"
#include "hojasprites.h"
#include <cmath>

Escombro::Escombro(qreal xOrigen, qreal yOrigen, qreal v0_px_s,
                   qreal theta0_grados, int variante, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), rx(xOrigen), ry(yOrigen) {

    // Descomposicion del tiro parabolico: vx0 = v0*cos(theta0), vy0 = -v0*sin(theta0)
    Fisica::descomponerTiroParabolico(v0_px_s, theta0_grados, vx, vy);
    omega_deg_s = (vx >= 0.0 ? 1.0 : -1.0) * (240.0 + (variante * 90.0));

    // Hoja de sprites de escombros: ruta, ancho, alto, spritesPorFila, spritesPorColumna, escala%
    HojaSprites hoja;
    hoja.cargar("imagenes/escombros.png", 26, 26, 3, 1, 100.0f,
                QColor(125, 95, 65));
    QPixmap pix = hoja.obtenerSprite(0, variante % 3);
    setPixmap(pix);
    setTransformOriginPoint(pix.width() / 2.0, pix.height() / 2.0);
    setPos(rx - pix.width() / 2.0, ry - pix.height() / 2.0);
    setZValue(14);
}

void Escombro::actualizar(qreal dt, const QList<QRectF>& suelos, qreal altoMundo) {
    if (!activo) return;

    tiempoVida_s -= dt;
    if (tiempoVida_s <= 0.0) {
        activo = false;
        return;
    }

    if (tiempoVida_s < 1.0) {
        setOpacity(tiempoVida_s);
    }

    // Movimiento parabolico bajo gravedad constante ay = g
    Fisica::integrarMRUA(rx, ry, vx, vy, 0.0, Fisica::g, dt);
    theta_deg += omega_deg_s * dt;

    // Rebote inelastico contra plataformas y suelo (coeficiente de restitucion e = 0.42)
    // Recorre los elementos indicados.
    for (const QRectF& s : suelos) {
        if (Fisica::colisionElasticaEsferaMuro(rx, ry, radio_px, vx, vy, s, e_restitucion)) {
            vx *= mu_friccion;
            omega_deg_s *= 0.55;
            if (std::abs(vy) < 60.0) {
                vy = 0.0;
                peligroso = false;
            }
        }
    }

    if (ry + radio_px > altoMundo) {
        ry = altoMundo - radio_px;
        vy = -std::abs(vy) * e_restitucion;
        vx *= mu_friccion;
        if (std::abs(vy) < 60.0) {
            vy = 0.0;
            peligroso = false;
        }
    }

    QPixmap p = pixmap();
    setPos(rx - p.width() / 2.0, ry - p.height() / 2.0);
    setRotation(theta_deg);
}
