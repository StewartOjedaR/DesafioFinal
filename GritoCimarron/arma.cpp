#include "Arma.h"
#include <QDebug>

Arma::Arma(QString nombreArma, int danoBase, int capacidad, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent),
    nombre(nombreArma), dano(danoBase), capacidadCargador(capacidad) {
    balasRestantes = capacidadCargador;
}

Arma::~Arma() {}

bool Arma::disparar() {
    if (balasRestantes > 0) {
        balasRestantes--;
        qDebug() << nombre << "disparo. Balas restantes:" << balasRestantes;
        return true; // Disparo exitoso
    }
    qDebug() << "Sin municion en" << nombre << "!";
    return false; // No hay balas
}

void Arma::recargar(int cantidadBalas) {
    balasRestantes += cantidadBalas;
    if (balasRestantes > capacidadCargador) {
        balasRestantes = capacidadCargador;
    }
}

int Arma::getDano() const { return dano; }
int Arma::getBalasRestantes() const { return balasRestantes; }
bool Arma::tieneBalas() const { return balasRestantes > 0; }

void Arma::cargarSprite(const QString& ruta, float porcentajeEscala) {
    QPixmap pixmap(ruta);
    if (!pixmap.isNull()) {
        int nuevoAncho = static_cast<int>(pixmap.width() * (porcentajeEscala / 100.0f));
        int nuevoAlto = static_cast<int>(pixmap.height() * (porcentajeEscala / 100.0f));
        setPixmap(pixmap.scaled(nuevoAncho, nuevoAlto, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}