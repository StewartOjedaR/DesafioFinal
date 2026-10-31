#include "jugador.h"
#include "objeto.h"
#include "rehen.h"
#include <QKeyEvent> // <-- Agregar esta línea
#include <QDebug>

Jugador::Jugador(int saludInicial, QGraphicsItem* parent)
    : Personaje(saludInicial, parent), vidas(3) {

    // Habilitamos que este elemento pueda recibir el foco del teclado
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

Jugador::~Jugador() {}

void Jugador::mover() {
    // 1. Aplicamos la gravedad si el jugador está en el aire
    if (!enElSuelo) {
        velocidadY += gravedad;
    } else {
        velocidadY = 0; // Si esta en el suelo, no cae mas
    }

    // 2. Actualizamos la posicion vertical
    setPos(x(), y() + velocidadY);

    // 3. Verificamos si cayo sobre el suelo o una plataforma 🧱
    // (Aqui usaremos las colisiones mas adelante)
}

void Jugador::atacar() {
    qDebug() << "El jugador dispara su arma! 🔫";
}

// Deteccion de teclas ⌨️
void Jugador::keyPressEvent(QKeyEvent* event) {
    // 1. Movimiento en el plano del suelo (ejes X e Y) 🚶‍♂️
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
        setPos(x() - velocidad, y());
        setTransform(QTransform().scale(-1, 1));
    }
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
        setPos(x() + velocidad, y());
        setTransform(QTransform().scale(1, 1));
    }
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
        setPos(x(), y() - velocidad); // Mueve al personaje "hacia el fondo" del escenario
    }
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S) {
        setPos(x(), y() + velocidad); // Mueve al personaje "hacia el frente"
    }

    // 2. Tecla dedicada exclusivamente al SALTO
    else if (event->key() == Qt::Key_Space && enElSuelo) {
        velocidadY = fuerzaSalto; // Aplicamos el impulso hacia arriba
        enElSuelo = false;
    }
}
void Jugador::verificarColisiones() {
    // 1. Obtenemos la lista de items con los que colisiona el jugador
    QList<QGraphicsItem*> colisiones = collidingItems();

    // Asumimos inicialmente que esta en el aire
    bool tocandoPlataforma = false;

    // 2. Recorremos cada objeto detectado
    for (QGraphicsItem* item : colisiones) {

        // ¿Es un Objeto / Plataforma?
        Objeto* plataforma = dynamic_cast<Objeto*>(item);
        if (plataforma != nullptr) {
            // Verificamos si el jugador está cayendo sobre la parte superior de la plataforma
            if (velocidadY > 0 && y() + pixmap().height() <= plataforma->y() + 10) {
                setPos(x(), plataforma->y() - pixmap().height()); // Ajustamos la posición sobre el suelo 📍
                velocidadY = 0;
                enElSuelo = true;
                tocandoPlataforma = true;
            }
        }

        // ¿Es un Rehen?
        Rehen* rehen = dynamic_cast<Rehen*>(item);
        if (rehen != nullptr && !rehen->isLiberado()) {
            rehen->interactuar(); // Activa el rescate y otorga los puntos 🏆
        }
    }

    // Si no toca ninguna plataforma, sigue cayendo por gravedad 🪂
    if (!tocandoPlataforma && velocidadY >= 0) {
        enElSuelo = false;
    }
}