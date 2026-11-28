#ifndef FISICA_H
#define FISICA_H

#include <QtGlobal>
#include <QRectF>
#include <cmath>

// Vector 2D en el plano: r = (x, y) [m o px], v = (vx, vy) [m/s o px/s]
// Declara la estructura Vec2D.
struct Vec2D {
    qreal x = 0.0;
    qreal y = 0.0;
    Vec2D operator+(const Vec2D& o) const { return {x + o.x, y + o.y}; }
    Vec2D operator-(const Vec2D& o) const { return {x - o.x, y - o.y}; }
    Vec2D operator*(qreal s) const { return {x * s, y * s}; }

    // Producto escalar: a · b = ax*bx + ay*by
    qreal dot(const Vec2D& o) const { return x * o.x + y * o.y; }
    // Producto cruz 2D (componente Z): a × b = ax*by - ay*bx
    qreal cross(const Vec2D& o) const { return x * o.y - y * o.x; }
    // Norma al cuadrado ||v||^2 y norma ||v||
    qreal lengthSq() const { return x * x + y * y; }
    qreal length() const { return std::sqrt(lengthSq()); }
    // Vector unitario v_hat = v / ||v||
    Vec2D normalized() const {
        qreal norm = length();
        // Devuelve el vector resultante
        return (norm > 1e-6) ? Vec2D{x / norm, y / norm} : Vec2D{0.0, 0.0};
    }
};

// Abre el espacio de nombres Fisica.
namespace Fisica {
    // Escala metrica del mundo 2D: 1 metro (m) = 100 pixeles (px)
    constexpr qreal ESCALA_PX_POR_METRO = 100.0;
    constexpr qreal metrosAPixeles(qreal metros) { return metros * ESCALA_PX_POR_METRO; }
    constexpr qreal pixelesAMetros(qreal px)     { return px / ESCALA_PX_POR_METRO; }

    // Constantes fisicas en unidades del Sistema Internacional (SI) y su equivalente en px
    // Aceleracion gravitacional terrestre [m/s^2]
    constexpr qreal g_SI = 9.81;                                    
    // g = 981.0 [px/s^2]
    constexpr qreal g = metrosAPixeles(g_SI);                       

    // Velocidad inicial vertical de salto [m/s]
    constexpr qreal v0_salto_SI = -5.90;                            
    // v0_y = -590.0 [px/s] (h_max = v0^2 / 2g ≈ 1.77 m)
    constexpr qreal v0_salto = metrosAPixeles(v0_salto_SI);         

    // Velocidad terminal de caida libre [m/s]
    constexpr qreal v_terminal_SI = 9.50;                           
    // v_max = 950.0 [px/s]
    constexpr qreal v_terminal = metrosAPixeles(v_terminal_SI);     

    // 1. Cinematica MRUA (Movimiento Rectilineo Uniformemente Acelerado)
    //    v(t + dt) = v(t) + a * dt
    //    r(t + dt) = r(t) + v(t + dt) * dt
   void integrarMRUA(qreal& rx, qreal& ry, qreal& vx, qreal& vy,
                      qreal ax, qreal ay, qreal dt);

    // 2. Tiro Parabolico (Composicion de MRU horizontal + MRUA vertical con g)
    //    vx0 = v0 * cos(theta0),  vy0 = -v0 * sin(theta0)
    void descomponerTiroParabolico(qreal v0, qreal theta0_grados,
                                             qreal& vx0, qreal& vy0);

    // 3. Movimiento Oscilatorio — Pendulo Simple
    //    alpha = d²theta/dt² = -(g / L) * sin(theta) - gamma * omega
    //    Integrado con Velocity-Verlet para conservar la energia mecanica E = K + U.
     void actualizarPenduloSimple(qreal& theta_rad, qreal& omega_rad_s, qreal L_px,
                                 qreal gravedad_px_s2, qreal gamma, qreal dt);

    // Posicion cartesiana de la masa pendular: x = x0 + L*sin(theta), y = y0 + L*cos(theta)
    void posicionMasaPendular(qreal xPivote, qreal yPivote, qreal L_px,
                              qreal theta_rad, qreal& outX, qreal& outY);

    // Velocidad tangencial instantanea: vx = L*omega*cos(theta), vy = -L*omega*sin(theta)
    void velocidadTangencialPendular(qreal L_px, qreal theta_rad, qreal omega_rad_s,
                                     qreal& outVx, qreal& outVy);

    // 4. Colisiones Elasticas 2D (Coeficiente de restitucion e = 1.0 -> choque perfectamente elastico)
    // 4a. Esfera contra muro rigido (m_muro -> ∞): v' = v - (1 + e)*(v · n_hat)*n_hat
    bool colisionElasticaEsferaMuro(qreal& cx, qreal& cy, qreal R,
                                    qreal& vx, qreal& vy, const QRectF& muro,
                                    qreal e = 1.0);

    // 4b. Choque elastico entre dos esferas de masas m1 y m2 (conserva p = m1*v1 + m2*v2 y Ek):
    //     j = -(1 + e)*(v_rel · n_hat) / (1/m1 + 1/m2)
    // Define la funcion colisionElasticaEsferas.
    bool colisionElasticaEsferas(qreal& x1, qreal& y1, qreal R1, qreal m1_kg,
                                 qreal& vx1, qreal& vy1,
                                 qreal& x2, qreal& y2, qreal R2, qreal m2_kg,
                                 qreal& vx2, qreal& vy2,
                                 qreal e = 1.0);

    // 5. Friccion / Amortiguamiento viscoso: F_f = -b * v  =>  v(t + dt) = v(t) * e^(-(b/m)*dt)
    //la funcion aplicarAmortiguamiento.
    void aplicarAmortiguamiento(qreal& vx, qreal& vy, qreal b_sobre_m, qreal dt);
}

// FISICA_H
#endif 
