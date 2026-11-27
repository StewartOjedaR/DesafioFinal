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

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Rehen::actualizarAnimacion);
    timerAnimacion->start(150);

    // Cargar hoja de sprites del rehen
    // Parametros: ruta, anchoFrame, altoFrame, escala%
    cargarHojaSprites("sprites/rehen.png", 32, 32, 100.0f);
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
        totalFrames = hojaSprites.width() / anchoSprite;
    }
}

void Rehen::interactuar() {
    if (!liberado) {
        liberado = true;
        interactuable = false;
        qDebug() << "Rehen rescatado!" << puntosRecompensa << "puntos.";
    }
}

bool Rehen::isLiberado() const {
    return liberado;
}

int Rehen::getPuntosRecompensa() const {
    return puntosRecompensa;
}

void Rehen::actualizarAnimacion() {
    if (hojaSprites.isNull() || anchoSprite == 0) return;

    int posX = frameActual * anchoSprite;
    QPixmap frameRecortado = hojaSprites.copy(posX, 0, anchoSprite, altoSprite);

    int nuevoAncho = static_cast<int>(anchoSprite * (escalaPorcentaje / 100.0f));
    int nuevoAlto = static_cast<int>(altoSprite * (escalaPorcentaje / 100.0f));
    QPixmap frameEscalado = frameRecortado.scaled(nuevoAncho, nuevoAlto, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setPixmap(frameEscalado);

    frameActual = (frameActual + 1) % totalFrames;
}