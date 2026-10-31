#include "Rehen.h"
#include <QDebug>

Rehen::Rehen(int puntos, QGraphicsItem* parent)
    : Objeto(parent),
    liberado(false),
    puntosRecompensa(puntos),
    anchoSprite(0),
    altoSprite(0),
    frameActual(0),
    totalFrames(1),
    escalaPorcentaje(100.0f) {

    // 1. Inicializamos y conectamos el temporizador de animacion
    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Rehen::actualizarAnimacion);
    timerAnimacion->start(150); // Cambia de frame cada 150 ms
}

Rehen::~Rehen() {
    delete timerAnimacion;
}

void Rehen::cargarHojaSprites(const QString& ruta, int anchoFrame, int altoFrame, float porcentajeEscala) {
    hojaSprites.load(ruta);
    anchoSprite = anchoFrame;
    altoSprite = altoFrame;
    escalaPorcentaje = porcentajeEscala;

    if (!hojaSprites.isNull() && anchoSprite > 0) {
        totalFrames = hojaSprites.width() / anchoSprite; // Calculamos cuantos sprites hay en la fila 🧮
    }
}

// Logica que se activa cuando el Jugador toca/rescata al rehen 🤝
void Rehen::interactuar() {
    if (!liberado) {
        liberado = true;
        interactuable = false; // Ya no se puede volver a rescatar
        qDebug() << "¡Rehén rescatado! Otorga" << puntosRecompensa << "puntos. 🎖️";

        // Aquí podrias cambiar la hoja de sprites a la animacion de huida/agradecimiento 🏃‍♂️
    }
}

bool Rehen::isLiberado() const {
    return liberado;
}

int Rehen::getPuntosRecompensa() const {
    return puntosRecompensa;
}

// Ciclo de animacion del rehen 🎞️
void Rehen::actualizarAnimacion() {
    if (hojaSprites.isNull() || anchoSprite == 0) return;

    // Recortamos el frame correspondiente en la fila
    int posX = frameActual * anchoSprite;
    QPixmap frameRecortado = hojaSprites.copy(posX, 0, anchoSprite, altoSprite);

    // Escalamos la imagen según el porcentaje
    int nuevoAncho = static_cast<int>(anchoSprite * (escalaPorcentaje / 100.0f));
    int nuevoAlto = static_cast<int>(altoSprite * (escalaPorcentaje / 100.0f));
    QPixmap frameEscalado = frameRecortado.scaled(nuevoAncho, nuevoAlto, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setPixmap(frameEscalado);

    // Avanzamos al siguiente frame usando el operador modulo (%) 🔄
    frameActual = (frameActual + 1) % totalFrames;
}