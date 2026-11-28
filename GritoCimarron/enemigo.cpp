#include "enemigo.h"
#include "jugador.h"
#include "fisica.h"
#include <algorithm>
#include <cmath>

Enemigo::Enemigo(int saludInicial, float xMin, float xMax, int tipo, QGraphicsItem* parent)
    : Personaje(saludInicial, parent),
      tipoIA(tipo),
      limiteIzquierda(xMin),
      limiteDerecha(xMax) {
    setZValue(18);
    // Comprueba una condicion antes de continuar.
    if (tipoIA == 3) {
        // Soldado Rastreador inteligente (IA adaptativa):
        // ":/imagenes/IA.png" → 4 columnas × 1 fila, sprite de 300×400 px, escalado al 22%
        cargarHojaSprites(":/imagenes/IA.png", 300, 400, 4, 1, 22.0f);
        velocidad = 205.0;
        rangoDeteccion = 900.0;
        dano = 20;
        puntosRecompensa = 450;
        cooldownDisparo = 1.6;
    } else if (tipoIA == 2) {
        // Guardia colonial de persecucion:
        // ":/imagenes/IA.png" → 4 columnas × 1 fila, sprite de 300×400 px, escalado al 20%
        cargarHojaSprites(":/imagenes/IA.png", 300, 400, 4, 1, 20.0f);
        velocidad = 155.0;
        rangoDeteccion = 420.0;
        dano = 15;
        puntosRecompensa = 200;
        cooldownDisparo = 1.8;
    } else {
        // Guardia menor en patrulla fija:
        // ":/imagenes/IA.png" → 4 columnas × 1 fila, sprite de 300×400 px, escalado al 20%
        cargarHojaSprites(":/imagenes/IA.png", 300, 400, 4, 1, 20.0f);
        velocidad = 125.0;
        rangoDeteccion = 320.0;
        dano = 15;
        puntosRecompensa = 150;
        cooldownDisparo = 2.0;
    }
}

Enemigo::~Enemigo() {}

void Enemigo::atacar() {
    if (!objetivoActual || !estaVivo()) return;
    if (cooldownDisparo > 0.0) return;

    QPointF miCentro = centroGlobal();
    QPointF objCentro = objetivoActual->centroGlobal();
    Vec2D delta{objCentro.x() - miCentro.x(), objCentro.y() - miCentro.y()};
    qreal dist = delta.length();

    if (dist <= rangoDeteccion && dist > 1.0) {
        // El rastreador (tipoIA == 3) dispara con prediccion cinematica
        if (tipoIA == 3) {
            qreal velProyectil = 390.0;
            qreal tVuelo = dist / velProyectil;
            Vec2D futuro{
                objCentro.x() + objetivoActual->getVx() * tVuelo * (0.4 + 0.5 * nivelAprendizaje),
                objCentro.y() + objetivoActual->getVy() * tVuelo * (0.4 + 0.5 * nivelAprendizaje)
            };
            delta = Vec2D{futuro.x - miCentro.x(), futuro.y - miCentro.y()};
            cooldownDisparo = 1.45;
        } else {
            cooldownDisparo = (tipoIA == 2) ? 1.75 : 2.1;
        }

        Vec2D dir = delta.normalized();
        if (!modoCenital) {
            // En vista lateral dispara horizontalmente hacia el jugador
            dir = Vec2D{(delta.x >= 0.0) ? 1.0 : -1.0, 0.0};
        // Cierra el bloque anterior.
        }
        mirandoDerecha = (dir.x >= 0.0);
        // Emite una senal de Qt.
        emit disparoEnemigo(miCentro + QPointF(dir.x * 22.0, dir.y * 22.0),
                            // Continua la definicion de esta instruccion.
                            dir.x, dir.y, dano);
    }
}

void Enemigo::actualizarIA(qreal dt, Jugador* jugador, const QList<QRectF>& muros,
                           const QPointF& metaEscape) {
    if (!estaVivo()) return;

    objetivoActual = jugador;
    puntoMetaEscape = metaEscape;
    if (cooldownDisparo > 0.0) {
        cooldownDisparo = std::max(0.0, cooldownDisparo - dt);
    }

    movIzquierda = false;
    movDerecha = false;
    movArriba = false;
    movAbajo = false;

    if (tipoIA == 1) {
        onPatrullaFija();
    } else if (tipoIA == 2) {
        onPersecucion();
    // Muestra la alerta cuando existe texto
    } else {
        onRastreadorInteligente(dt, muros);
    }

    atacar();
}

void Enemigo::onPatrullaFija() {
    if (moviendoDerechaPatrulla) {
        setMoverDerecha(true);
        if (x() >= limiteDerecha) {
            moviendoDerechaPatrulla = false;
        }
    // Inicia la rama de victoria final
    } else {
        setMoverIzquierda(true);
        if (x() <= limiteIzquierda) {
            moviendoDerechaPatrulla = true;
        }
    }

    // Si en Nivel 1 el jugador esta muy cerca horizontalmente, orientarse hacia el para disparar
    if (objetivoActual) {
        qreal dx = objetivoActual->centroGlobal().x() - centroGlobal().x();
        qreal dy = std::abs(objetivoActual->centroGlobal().y() - centroGlobal().y());
        if (std::abs(dx) < rangoDeteccion && dy < 95.0) {
            mirandoDerecha = (dx >= 0.0);
        }
    }
}

void Enemigo::onPersecucion() {
    if (!objetivoActual) return;

    QPointF miCentro = centroGlobal();
    QPointF jCentro = objetivoActual->centroGlobal();
    Vec2D delta{jCentro.x() - miCentro.x(), jCentro.y() - miCentro.y()};
    qreal dist = delta.length();

    if (dist > rangoDeteccion) {
        onPatrullaFija();
        // Sale del ciclo sin actualizar nada
        return;
    }

    if (dist > 45.0) {
        if (delta.x < -12.0) setMoverIzquierda(true);
        // Ejecuta la rama alternativa.
        else if (delta.x > 12.0) setMoverDerecha(true);

        if (modoCenital) {
            if (delta.y < -12.0) setMoverArriba(true);
            // Ejecuta la rama alternativa.
            else if (delta.y > 12.0) setMoverAbajo(true);
        }
    }
}

void Enemigo::onRastreadorInteligente(qreal dt, const QList<QRectF>& muros) {
    // Comprueba una condicion antes de continuar.
    if (!objetivoActual) return;

    QPointF miCentro = centroGlobal();
    QPointF jCentro = objetivoActual->centroGlobal();
    Vec2D haciaJugador{jCentro.x() - miCentro.x(), jCentro.y() - miCentro.y()};
    qreal dist = haciaJugador.length();

    // 1. Aprendizaje continuo del patron de evasion del jugador:
    // Observa el producto cruzado entre el vector enemigo->jugador y la velocidad del jugador.
    Vec2D velJ{objetivoActual->getVx(), objetivoActual->getVy()};
    if (velJ.lengthSq() > 100.0 && dist > 1.0) {
        Vec2D dirRel = haciaJugador.normalized();
        Vec2D velNorm = velJ.normalized();
        qreal cruz = dirRel.x * velNorm.y - dirRel.y * velNorm.x; 
        qreal tasaAprendizaje = 1.6 * dt;
        sesgoEvasionLateral = sesgoEvasionLateral * (1.0 - tasaAprendizaje) + cruz * tasaAprendizaje;
        nivelAprendizaje = std::min(1.0, nivelAprendizaje + 0.09 * dt);
    }

    // 2. Prediccion de intercepcion + flanqueo para acorralar entre muros/salida
    qreal tiempoLookahead = std::clamp(dist / 360.0, 0.15, 0.95) * (0.4 + 0.6 * nivelAprendizaje);
    Vec2D posPredicha{
        jCentro.x() + velJ.x * tiempoLookahead,
        jCentro.y() + velJ.y * tiempoLookahead
    };

    // Vector perpendicular para cortar la linea de escape lateral aprendida
    Vec2D dirHaciaPred = Vec2D{posPredicha.x - miCentro.x(), posPredicha.y - miCentro.y()}.normalized();
    Vec2D perp{-dirHaciaPred.y, dirHaciaPred.x};
    qreal pesoFlanqueo = std::clamp(sesgoEvasionLateral * 1.8, -0.85, 0.85) * nivelAprendizaje;

    // Tambien intenta colocarse entre el jugador y la puerta de escape a Palenque cuando esta cerca
    Vec2D corteEscape{puntoMetaEscape.x() - jCentro.x(), puntoMetaEscape.y() - jCentro.y()};
    Vec2D dirCorte = corteEscape.normalized();
    qreal pesoCorte = (dist < 380.0) ? 0.35 * nivelAprendizaje : 0.0;

    Vec2D direccionDeseada = dirHaciaPred + perp * pesoFlanqueo + dirCorte * pesoCorte;

    // 3. Evasion de muros de piedra para no quedar atascado al acorralar
    // Recorre los elementos indicados.
    for (const QRectF& m : muros) {
        QPointF mCentro = m.center();
        Vec2D desdeMuro{miCentro.x() - mCentro.x(), miCentro.y() - mCentro.y()};
        qreal dMuro = desdeMuro.length();
        qreal radioRepulsion = std::max(m.width(), m.height()) * 0.65 + 42.0;
        if (dMuro < radioRepulsion && dMuro > 1.0) {
            qreal fuerza = (radioRepulsion - dMuro) / radioRepulsion;
            direccionDeseada = direccionDeseada + desdeMuro.normalized() * (fuerza * 1.4);
        }
    }

    Vec2D dirFinal = direccionDeseada.normalized();
    // Comprueba una condicion antes de continuar.
    if (dirFinal.x < -0.25) setMoverIzquierda(true);
    // Ejecuta la rama alternativa.
    else if (dirFinal.x > 0.25) setMoverDerecha(true);

    // Comprueba una condicion antes de continuar.
    if (dirFinal.y < -0.25) setMoverArriba(true);
    // Ejecuta la rama alternativa.
    else if (dirFinal.y > 0.25) setMoverAbajo(true);

    // Aceleracion progresiva conforme aprende el patron de Benkos
    // Asigna un valor a la variable indicada.
    velocidad = 195.0 + 48.0 * nivelAprendizaje;
}
