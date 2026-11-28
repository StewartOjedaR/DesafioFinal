// Incluye la cabecera jugador.h.
#include "jugador.h"
// Incluye la cabecera objeto.h.
#include "objeto.h"
// Incluye la cabecera rehen.h.
#include "rehen.h"
// Incluye la cabecera fisica.h.
#include "fisica.h"
// Incluye la cabecera QKeyEvent.
#include <QKeyEvent>
// Incluye la cabecera algorithm.
#include <algorithm>
// Incluye la cabecera cmath.
#include <cmath>

// Continua la definicion de esta instruccion.
Jugador::Jugador(int saludInicial, QGraphicsItem* parent)
    // Declara o define la funcion Personaje.
    : Personaje(saludInicial, parent), vidas(3) {

    // Declara o define la funcion setFlag.
    setFlag(QGraphicsItem::ItemIsFocusable);
    // Declara o define la funcion setZValue.
    setZValue(20);
    // Asigna un valor a la variable indicada.
    velocidad = 310.0;
    // Asigna un valor a la variable indicada.
    dano = 30;

    // Hoja de sprites del jugador (Benkos Bioho):
    // ":/imagenes/PERSONAJE.png" → 4 columnas × 1 fila, sprite de 300×400 px, escalado al 20%
    // Declara o define la funcion cargarHojaSprites.
    cargarHojaSprites(":/imagenes/PERSONAJE.png", 300, 400, 4, 1, 20.0f);

    // Arma inicial de Benkos (Pistola colonial con municion recargable)
    // Declara o define la funcion Arma.
    armaEquipada = new Arma("Pistola Cimarrona", 25, 14, true, this);
    // Continua la definicion de esta instruccion.
    armaEquipada->setVisible(false);
// Cierra el bloque anterior.
}

// Declara o define la funcion Jugador.
Jugador::~Jugador() {}

// Continua la definicion de esta instruccion.
void Jugador::verificarColisiones() {
    // La resolucion fisica y de colisiones se centraliza en Nivel::actualizarEscena()
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::equiparArma(Arma* nuevaArma) {
    // Comprueba una condicion antes de continuar.
    if (nuevaArma && nuevaArma != armaEquipada) {
        // Comprueba una condicion antes de continuar.
        if (armaEquipada && armaEquipada->parent() == this) {
            // Declara o inicializa una variable.
            delete armaEquipada;
        // Cierra el bloque anterior.
        }
        // Asigna un valor a la variable indicada.
        armaEquipada = nuevaArma;
        // Continua la definicion de esta instruccion.
        armaEquipada->setParent(this);
        // Continua la definicion de esta instruccion.
        armaEquipada->setVisible(false);
    // Cierra el bloque anterior.
    }
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::rescatar(Rehen* rehen) {
    // Comprueba una condicion antes de continuar.
    if (rehen && !rehen->isLiberado()) {
        // Continua la definicion de esta instruccion.
        rehen->liberar();
    // Cierra el bloque anterior.
    }
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::interactuar(Objeto* objeto) {
    // Comprueba una condicion antes de continuar.
    if (objeto) {
        // Continua la definicion de esta instruccion.
        objeto->interactuar();
    // Cierra el bloque anterior.
    }
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
bool Jugador::consumirVidaYReiniciarSalud() {
    // Comprueba una condicion antes de continuar.
    if (vidas > 1) {
        // Continua la definicion de esta instruccion.
        vidas--;
        // Asigna un valor a la variable indicada.
        salud = saludMaxima;
        // Asigna un valor a la variable indicada.
        timerInvulnerable = 1.6;
        // Sigue vivo con otra vida
        return true; 
    // Cierra el bloque anterior.
    }
    // Asigna un valor a la variable indicada.
    vidas = 0;
    // Asigna un valor a la variable indicada.
    salud = 0;
    // Game Over
    return false; 
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::limpiarTeclas() {
    // Asigna un valor a la variable indicada.
    movIzquierda = false;
    // Asigna un valor a la variable indicada.
    movDerecha = false;
    // Asigna un valor a la variable indicada.
    movArriba = false;
    // Asigna un valor a la variable indicada.
    movAbajo = false;
    // Asigna un valor a la variable indicada.
    saltar = false;
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::setObjetivoMouse(const QPointF& posEscena) {
    // Asigna un valor a la variable indicada.
    objetivoMouseEscena = posEscena;
    // Asigna un valor a la variable indicada.
    usandoMouseAim = true;
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::direccionApuntado(qreal& outDx, qreal& outDy) const {
    // Comprueba una condicion antes de continuar.
    if (modoCenital && usandoMouseAim) {
        // Declara o define la funcion centroGlobal.
        QPointF c = centroGlobal();
        // Continua la definicion de esta instruccion.
        Vec2D d{objetivoMouseEscena.x() - c.x(), objetivoMouseEscena.y() - c.y()};
        // Comprueba una condicion antes de continuar.
        if (d.lengthSq() > 4.0) {
            // Declara o inicializa una variable.
            Vec2D n = d.normalized();
            // Asigna un valor a la variable indicada.
            outDx = n.x;
            // Asigna un valor a la variable indicada.
            outDy = n.y;
            // Devuelve el resultado de esta funcion.
            return;
        // Cierra el bloque anterior.
        }
    // Cierra el bloque anterior.
    }

    // Comprueba una condicion antes de continuar.
    if (modoCenital) {
        // Declara o inicializa una variable.
        qreal dx = (movDerecha ? 1.0 : 0.0) - (movIzquierda ? 1.0 : 0.0);
        // Declara o inicializa una variable.
        qreal dy = (movAbajo ? 1.0 : 0.0) - (movArriba ? 1.0 : 0.0);
        // Comprueba una condicion antes de continuar.
        if (dx != 0.0 || dy != 0.0) {
            // Declara o inicializa una variable.
            Vec2D n = Vec2D{dx, dy}.normalized();
            // Asigna un valor a la variable indicada.
            outDx = n.x;
            // Asigna un valor a la variable indicada.
            outDy = n.y;
            // Devuelve el resultado de esta funcion.
            return;
        // Cierra el bloque anterior.
        }
    // Cierra el bloque anterior.
    }

    // Asigna un valor a la variable indicada.
    outDx = mirandoDerecha ? 1.0 : -1.0;
    // Asigna un valor a la variable indicada.
    outDy = 0.0;
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::actualizarTimers(qreal dt) {
    // Comprueba una condicion antes de continuar.
    if (cooldownMelee > 0.0) {
        // Asigna un valor a la variable indicada.
        cooldownMelee = std::max(0.0, cooldownMelee - dt);
    // Cierra el bloque anterior.
    }
    // Comprueba una condicion antes de continuar.
    if (armaEquipada) {
        // Continua la definicion de esta instruccion.
        armaEquipada->actualizarCooldown(dt);
    // Cierra el bloque anterior.
    }

    // Comprueba una condicion antes de continuar.
    if (modoCenital) {
        // Declara o inicializa una variable.
        qreal dx = (movDerecha ? 1.0 : 0.0) - (movIzquierda ? 1.0 : 0.0);
        // Declara o inicializa una variable.
        qreal dy = (movAbajo ? 1.0 : 0.0) - (movArriba ? 1.0 : 0.0);
        // Comprueba una condicion antes de continuar.
        if (dx != 0.0 || dy != 0.0) {
            // Declara o inicializa una variable.
            Vec2D n = Vec2D{dx, dy}.normalized();
            // Asigna un valor a la variable indicada.
            ultimoAimX = n.x;
            // Asigna un valor a la variable indicada.
            ultimoAimY = n.y;
        // Cierra el bloque anterior.
        }
    // Cierra el bloque anterior.
    } else {
        // Asigna un valor a la variable indicada.
        ultimoAimX = mirandoDerecha ? 1.0 : -1.0;
        // Asigna un valor a la variable indicada.
        ultimoAimY = 0.0;
    // Cierra el bloque anterior.
    }
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::atacar() {
    // Ataque de machete/grito cuerpo a cuerpo en frente de Benkos
    // Comprueba una condicion antes de continuar.
    if (cooldownMelee <= 0.0) {
        // Asigna un valor a la variable indicada.
        cooldownMelee = 0.34;
        // Declara o define la funcion hitboxGlobal.
        QRectF hb = hitboxGlobal();
        // Declara o inicializa una variable.
        QRectF areaGolpe;
        // Comprueba una condicion antes de continuar.
        if (modoCenital) {
            // Declara o inicializa una variable.
            qreal dx = ultimoAimX;
            // Declara o inicializa una variable.
            qreal dy = ultimoAimY;
            // Declara o define la funcion direccionApuntado.
            direccionApuntado(dx, dy);
            // Declara o define la funcion QPointF.
            QPointF centro = hb.center() + QPointF(dx * 34.0, dy * 34.0);
            // Declara o define la funcion QRectF.
            areaGolpe = QRectF(centro.x() - 42.0, centro.y() - 42.0, 84.0, 84.0);
        // Cierra el bloque anterior.
        } else {
            // Declara o inicializa una variable.
            qreal alcanceX = 62.0;
            // Declara o inicializa una variable.
            qreal inicioX = mirandoDerecha ? hb.center().x() : (hb.center().x() - alcanceX);
            // Declara o define la funcion QRectF.
            areaGolpe = QRectF(inicioX, hb.top() - 10.0, alcanceX, hb.height() + 20.0);
        // Cierra el bloque anterior.
        }
        // Emite una senal de Qt.
        emit ataqueMeleeSolicitado(areaGolpe, dano);
    // Cierra el bloque anterior.
    }
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::dispararArma(bool comoLanzable) {
    // Comprueba una condicion antes de continuar.
    if (!armaEquipada) return;

    // Declara o inicializa una variable.
    qreal dx = ultimoAimX;
    // Declara o inicializa una variable.
    qreal dy = ultimoAimY;
    // Declara o define la funcion direccionApuntado.
    direccionApuntado(dx, dy);
    // Comprueba una condicion antes de continuar.
    if (dx > 0.1) mirandoDerecha = true;
    // Ejecuta la rama alternativa.
    else if (dx < -0.1) mirandoDerecha = false;

    // Comprueba una condicion antes de continuar.
    if (comoLanzable) {
        // Comprueba una condicion antes de continuar.
        if (armaEquipada->lanzar()) {
            // Declara o define la funcion centroGlobal.
            QPointF origen = centroGlobal() + QPointF(dx * 20.0, -10.0);
            // Emite una senal de Qt.
            emit disparoSolicitado(origen, dx, dy, armaEquipada->getDano() + 15, true);
        // Cierra el bloque anterior.
        }
        // Devuelve el resultado de esta funcion.
        return;
    // Cierra el bloque anterior.
    }

    // Comprueba una condicion antes de continuar.
    if (armaEquipada->disparar()) {
        // Declara o define la funcion centroGlobal.
        QPointF origen = centroGlobal() + QPointF(dx * 24.0, 0.0);
        // Emite una senal de Qt.
        emit disparoSolicitado(origen, dx, dy, armaEquipada->getDano(), false);
    // Cierra el bloque anterior.
    } else {
        // Sin balas: usa machete cuerpo a cuerpo automaticamente
        // Declara o define la funcion atacar.
        atacar();
    // Cierra el bloque anterior.
    }
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::keyPressEvent(QKeyEvent* event) {
    // Comprueba una condicion antes de continuar.
    if (event->isAutoRepeat()) return;

    // Declara o define la funcion switch.
    switch (event->key()) {
    // Continua la definicion de esta instruccion.
    case Qt::Key_Left:
    // Continua la definicion de esta instruccion.
    case Qt::Key_A:
        // Declara o define la funcion setMoverIzquierda.
        setMoverIzquierda(true);
        // Asigna un valor a la variable indicada.
        usandoMouseAim = false;
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case Qt::Key_Right:
    // Continua la definicion de esta instruccion.
    case Qt::Key_D:
        // Declara o define la funcion setMoverDerecha.
        setMoverDerecha(true);
        // Asigna un valor a la variable indicada.
        usandoMouseAim = false;
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case Qt::Key_Up:
    // Continua la definicion de esta instruccion.
    case Qt::Key_W:
        // Comprueba una condicion antes de continuar.
        if (!modoCenital) {
            // Declara o define la funcion solicitarSalto.
            solicitarSalto();
        // Cierra el bloque anterior.
        } else {
            // Declara o define la funcion setMoverArriba.
            setMoverArriba(true);
            // Asigna un valor a la variable indicada.
            usandoMouseAim = false;
        // Cierra el bloque anterior.
        }
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case Qt::Key_Down:
    // Continua la definicion de esta instruccion.
    case Qt::Key_S:
        // Comprueba una condicion antes de continuar.
        if (modoCenital) {
            // Declara o define la funcion setMoverAbajo.
            setMoverAbajo(true);
            // Asigna un valor a la variable indicada.
            usandoMouseAim = false;
        // Cierra el bloque anterior.
        }
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case Qt::Key_Space:
        // Comprueba una condicion antes de continuar.
        if (!modoCenital) {
            // Declara o define la funcion solicitarSalto.
            solicitarSalto();
        // Cierra el bloque anterior.
        } else {
            // Declara o define la funcion dispararArma.
            dispararArma(false);
        // Cierra el bloque anterior.
        }
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case Qt::Key_J:
    // Continua la definicion de esta instruccion.
    case Qt::Key_F:
        // Declara o define la funcion atacar.
        atacar();
        // Declara o define la funcion dispararArma.
        dispararArma(false);
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case Qt::Key_K:
        // Lanzamiento parabolico
        dispararArma(true); 
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case Qt::Key_E:
        // Emite una senal de Qt.
        emit interaccionSolicitada();
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    default:
        // Continua la definicion de esta instruccion.
        break;
    // Cierra el bloque anterior.
    }
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
void Jugador::keyReleaseEvent(QKeyEvent* event) {
    // Comprueba una condicion antes de continuar.
    if (event->isAutoRepeat()) return;

    // Declara o define la funcion switch.
    switch (event->key()) {
    // Continua la definicion de esta instruccion.
    case Qt::Key_Left:
    // Continua la definicion de esta instruccion.
    case Qt::Key_A:
        // Declara o define la funcion setMoverIzquierda.
        setMoverIzquierda(false);
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case Qt::Key_Right:
    // Continua la definicion de esta instruccion.
    case Qt::Key_D:
        // Declara o define la funcion setMoverDerecha.
        setMoverDerecha(false);
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case Qt::Key_Up:
    // Continua la definicion de esta instruccion.
    case Qt::Key_W:
        // Declara o define la funcion setMoverArriba.
        setMoverArriba(false);
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    case Qt::Key_Down:
    // Continua la definicion de esta instruccion.
    case Qt::Key_S:
        // Declara o define la funcion setMoverAbajo.
        setMoverAbajo(false);
        // Continua la definicion de esta instruccion.
        break;
    // Continua la definicion de esta instruccion.
    default:
        // Continua la definicion de esta instruccion.
        break;
    // Cierra el bloque anterior.
    }
// Cierra el bloque anterior.
}
