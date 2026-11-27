#include "Arma.h"
#include <QDebug>

Arma::Arma(QString nombreArma, int danoBase, int capacidad, bool lanzable, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent),
    nombre(nombreArma), dano(danoBase), capacidad(capacidad), armaLanzable(lanzable),
    balasRestantes(capacidad) {

    // Cargar sprite del arma (imagen individual)
    // Parametros: ruta, escala%
    cargarSprite("sprites/arma.png", 100.0f);
}


Arma::~Arma() {}

void Arma::cargarSprite(const QString& ruta, float porcentajeEscala) {
    QPixmap pixmap(ruta);
    if (!pixmap.isNull()) {
        int nuevoAncho = static_cast<int>(pixmap.width() * (porcentajeEscala / 100.0f));
        int nuevoAlto = static_cast<int>(pixmap.height() * (porcentajeEscala / 100.0f));
        setPixmap(pixmap.scaled(nuevoAncho, nuevoAlto, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

bool Arma::disparar() {
    if (balasRestantes > 0 && armaLanzable) {
        balasRestantes--;
        qDebug() << nombre << "disparo. Balas restantes:" << balasRestantes;
        return true;
    }
    qDebug() << "Sin municion en" << nombre << "!";
    return false;
}

void Arma::recargar(int cantidadBalas) {
    balasRestantes += cantidadBalas;
    if (balasRestantes > capacidad) {
        balasRestantes = capacidad;
    }
}

int Arma::getDano() const { return dano; }
int Arma::getBalasRestantes() const { return balasRestantes; }
bool Arma::tieneBalas() const { return balasRestantes > 0; }