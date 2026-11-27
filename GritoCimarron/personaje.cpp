#include "Personaje.h"

Personaje::Personaje(int saludInicial, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent) {

    salud = saludInicial;
    vy = 0;
    vx = 0;
    movIzquierda = false;
    movDerecha = false;
    movArriba = false;
    movAbajo = false;
    saltar = false;
    enElSuelo = true;
    modoCenital = false;
    mirandoDerecha = true;
}

Personaje::~Personaje() {
}

void Personaje::setMoverIzquierda(bool activo) {
    movIzquierda = activo;
}
void Personaje::setMoverDerecha(bool activo) {
    movDerecha = activo;
}
void Personaje::setMoverArriba(bool activo) {
    movArriba = activo;
}
void Personaje::setMoverAbajo(bool activo) {
    movAbajo = activo;
}

bool Personaje::getMoverIzquierda() const {
    return movIzquierda;
}

bool Personaje::getMoverDerecha() const {
    return movDerecha;
}

bool Personaje::getMoverArriba() const {
    return movArriba;
}

bool Personaje::getMoverAbajo() const {
    return movAbajo;
}

qreal Personaje::getVx() const {
    return vx;
}

qreal Personaje::getVy() const {
    return vy;
}

qreal Personaje::getVelocidad() const {
    return velocidad;
}

void Personaje::setVx(qreal valor) {
    vx = valor;
}

void Personaje::setVy(qreal valor) {
    vy = valor;
}

void Personaje::actualizarPosicion() {
    qreal nuevoX = x() + vx;
    qreal nuevoY = y() + vy;
    setPos(nuevoX, nuevoY);
}

void Personaje::recibirDano(int cantidad) {
    salud -= cantidad;
    if (salud < 0) salud = 0;
}

int Personaje::getSalud() const {
    return salud;
}

void Personaje::cargarHojaSprites(const QString& ruta, int anchoFrame, int altoFrame, int columnas, float porcentajeEscala) {
    hojaSprites.load(ruta);
    anchoSprite = anchoFrame;
    altoSprite = altoFrame;
    columnasSprites = columnas;
    escalaPorcentaje = porcentajeEscala;

    if (!hojaSprites.isNull() && anchoSprite > 0 && altoSprite > 0) {
        int filas = hojaSprites.height() / altoSprite;
        int cols = hojaSprites.width() / anchoSprite;
        totalFrames = filas * cols;
    }
}

void Personaje::actualizarAnimacion() {
    if (hojaSprites.isNull() || columnasSprites == 0 || totalFrames == 0) { return;}
    contadorFrames++;

    int columnaActual = frameActual % columnasSprites;
    int filaActual = frameActual / columnasSprites;

    int posX = columnaActual * anchoSprite;
    int posY = filaActual * altoSprite;

    QPixmap frameRecortado = hojaSprites.copy(posX, posY, anchoSprite, altoSprite);

    int nuevoAncho = static_cast<int>(anchoSprite * (escalaPorcentaje / 100.0));
    int nuevoAlto = static_cast<int>(altoSprite * (escalaPorcentaje / 100.0));

    QPixmap frameEscalado = frameRecortado.scaled(nuevoAncho, nuevoAlto, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(frameEscalado);

    frameActual = (frameActual + 1) % totalFrames;
}