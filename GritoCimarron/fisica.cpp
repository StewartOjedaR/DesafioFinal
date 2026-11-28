#include "fisica.h"
#include <algorithm>

// Abre el espacio de nombres Fisica.
namespace Fisica {

// Se define la funcion integrarMRUA.
void integrarMRUA(qreal& rx, qreal& ry, qreal& vx, qreal& vy,
                  // Continua la definicion de esta instruccion.
                  qreal ax, qreal ay, qreal dt) {
    // v(t + dt) = v(t) + a * dt
    vx += ax * dt;
    vy += ay * dt;
    // Comprueba una condicion antes de continuar.
    if (vy > v_terminal) {
        // Asigna un valor a la variable indicada.
        vy = v_terminal;
    }
    // r(t + dt) = r(t) + v(t + dt) * dt (Euler simplectico)
    rx += vx * dt;
    ry += vy * dt;
}

// Se define la funcion descomponerTiroParabolico.
void descomponerTiroParabolico(qreal v0, qreal theta0_grados,
                               qreal& vx0, qreal& vy0) {
    constexpr qreal PI = 3.14159265358979323846;
    qreal theta0_rad = theta0_grados * PI / 180.0;
    // Asigna un valor a la variable indicada.
    vx0 = v0 * std::cos(theta0_rad);
    // En coordenadas de pantalla +Y apunta hacia abajo
    vy0 = -v0 * std::sin(theta0_rad);
}

//la funcion actualizarPenduloSimple.
void actualizarPenduloSimple(qreal& theta_rad, qreal& omega_rad_s, qreal L_px,

                             qreal gravedad_px_s2, qreal gamma, qreal dt) {
    // Comprueba una condicion antes de continuar.
    if (L_px <= 1.0) return;

    // Ecuacion diferencial del pendulo: alpha = -(g / L) * sin(theta) - gamma * omega
     qreal alpha1 = -(gravedad_px_s2 / L_px) * std::sin(theta_rad) - gamma * omega_rad_s;
     qreal thetaNuevo = theta_rad + omega_rad_s * dt + 0.5 * alpha1 * dt * dt;
     qreal omegaPred = omega_rad_s + alpha1 * dt;
    qreal alpha2 = -(gravedad_px_s2 / L_px) * std::sin(thetaNuevo) - gamma * omegaPred;

    theta_rad = thetaNuevo;
     omega_rad_s = omega_rad_s + 0.5 * (alpha1 + alpha2) * dt;

}

//la funcion posicionMasaPendular.
void posicionMasaPendular(qreal xPivote, qreal yPivote, qreal L_px,

                          qreal theta_rad, qreal& outX, qreal& outY) {
    outX = xPivote + L_px * std::sin(theta_rad);
    outY = yPivote + L_px * std::cos(theta_rad);
}

//la funcion velocidadTangencialPendular.
void velocidadTangencialPendular(qreal L_px, qreal theta_rad, qreal omega_rad_s,
                                 qreal& outVx, qreal& outVy) {
    // v = d/dt [L*sin(theta), L*cos(theta)] = [L*omega*cos(theta), -L*omega*sin(theta)]
      outVx = L_px * omega_rad_s * std::cos(theta_rad);
     outVy = -L_px * omega_rad_s * std::sin(theta_rad);
}

//la funcion colisionElasticaEsferaMuro.
bool colisionElasticaEsferaMuro(qreal& cx, qreal& cy, qreal R,
                                qreal& vx, qreal& vy, const QRectF& muro,
                                qreal e) {
    qreal cercanoX = std::clamp(cx, muro.left(), muro.right());
    qreal cercanoY = std::clamp(cy, muro.top(), muro.bottom());

    qreal dx = cx - cercanoX;
    qreal dy = cy - cercanoY;
    qreal distSq = dx * dx + dy * dy;

    // Comprueba una condicion antes de continuar.
    if (distSq >= R * R) {
        // Devuelve el resultado de esta funcion.
        return false;
    }

    // Normal de contacto n_hat = (nx, ny) y profundidad de penetracion
     qreal nx = 0.0;
     qreal ny = 0.0;
     qreal penetracion = 0.0;

    // Comprueba una condicion antes de continuar.
    if (distSq > 1e-8) {
        qreal dist = std::sqrt(distSq);
        nx = dx / dist;
        ny = dy / dist;
        penetracion = R - dist;
    } else {
        qreal dIzq = std::abs(cx - muro.left());
        qreal dDer = std::abs(muro.right() - cx);
        qreal dArr = std::abs(cy - muro.top());
        qreal dAba = std::abs(muro.bottom() - cy);
        qreal minD = std::min({dIzq, dDer, dArr, dAba});
        // Comprueba una condicion antes de continuar.
        if (minD == dIzq) { nx = -1.0; ny = 0.0; penetracion = dIzq + R; }
        // Ejecuta la rama alternativa.
        else if (minD == dDer) { nx = 1.0; ny = 0.0; penetracion = dDer + R; }
        // Ejecuta la rama alternativa.
        else if (minD == dArr) { nx = 0.0; ny = -1.0; penetracion = dArr + R; }
        // Ejecuta la rama alternativa.
        else { nx = 0.0; ny = 1.0; penetracion = dAba + R; }
    }

    cx += nx * penetracion;
    cy += ny * penetracion;

    // Componente normal de la velocidad: v_n = v · n_hat
    qreal vn = vx * nx + vy * ny;
    // Comprueba una condicion antes de continuar.
    if (vn < 0.0) {
        // v' = v - (1 + e) * (v · n_hat) * n_hat
        // Continua la definicion de esta instruccion.
        vx -= (1.0 + e) * vn * nx;
        // Continua la definicion de esta instruccion.
        vy -= (1.0 + e) * vn * ny;
        // Devuelve el resultado de esta funcion.
        return true;
     }
    // Devuelve el resultado de esta funcion.
    return false;
}

//la funcion colisionElasticaEsferas.
bool colisionElasticaEsferas(qreal& x1, qreal& y1, qreal R1, qreal m1_kg,
                             qreal& vx1, qreal& vy1,
                             qreal& x2, qreal& y2, qreal R2, qreal m2_kg,
                             qreal& vx2, qreal& vy2,
                             qreal e) {
    qreal dx = x2 - x1;
    qreal dy = y2 - y1;
    qreal distMin = R1 + R2;
    qreal distSq = dx * dx + dy * dy;

    // Comprueba una condicion antes de continuar.
    if (distSq >= distMin * distMin || distSq < 1e-9) {
        // Devuelve el resultado de esta funcion.
        return false;
    }

    qreal dist = std::sqrt(distSq);
    qreal nx = dx / dist;
    qreal ny = dy / dist;

    // Separacion de solape inversamente proporcional a las masas (m1, m2)
    qreal solape = distMin - dist;
    qreal invM1 = (m1_kg > 0.0) ? 1.0 / m1_kg : 0.0;
    qreal invM2 = (m2_kg > 0.0) ? 1.0 / m2_kg : 0.0;
    qreal invMTot = invM1 + invM2;
    // Comprueba una condicion antes de continuar.
    if (invMTot > 0.0) {
        x1 -= nx * solape * (invM1 / invMTot);
        y1 -= ny * solape * (invM1 / invMTot);
        x2 += nx * solape * (invM2 / invMTot);
        y2 += ny * solape * (invM2 / invMTot);
    }

    // Velocidad relativa proyectada sobre la normal: v_rel · n_hat
    qreal vRelN = (vx2 - vx1) * nx + (vy2 - vy1) * ny;
    // Comprueba una condicion antes de continuar.
    if (vRelN > 0.0) {
        // Devuelve el resultado de esta funcion.
        return false;
    // Cierra el bloque anterior.
    }

    // Magnitud del impulso escalar: j = -(1 + e) * (v_rel · n_hat) / (1/m1 + 1/m2)
    qreal j = -(1.0 + e) * vRelN / invMTot;
    qreal Jx = j * nx;
    qreal Jy = j * ny;

    // Cambio de cantidad de movimiento: dv = J / m
    vx1 -= Jx * invM1;
    vy1 -= Jy * invM1;
    vx2 += Jx * invM2;
    vy2 += Jy * invM2;
    return true;
}

//La funcion aplicarAmortiguamiento.
void aplicarAmortiguamiento(qreal& vx, qreal& vy, qreal b_sobre_m, qreal dt) {
    // Solucion analitica del amortiguamiento lineal dv/dt = -(b/m)*v
    qreal factor = std::exp(-b_sobre_m * dt);
    vx *= factor;
    vy *= factor;
    // Comprueba una condicion antes de continuar.
    if (std::abs(vx) < 1.0) vx = 0.0;
    // Comprueba una condicion antes de continuar.
    if (std::abs(vy) < 1.0) vy = 0.0;
}
} 
