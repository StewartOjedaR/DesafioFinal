#include "arma.h"
#include <algorithm>

Arma::Arma(QString nombreArma, int danoBase, int capacidad, bool lanzable, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent),
      nombre(std::move(nombreArma)),
      dano(danoBase),
      capacidad(capacidad),
      balasRestantes(capacidad),
      armaLanzable(lanzable) {

    // Hoja de sprites de armas: ruta, ancho, alto, spritesPorFila, spritesPorColumna, escala%
    // Columna 0: Pistola, Columna 1: Machete
    hoja.cargar("sprites/arma.png", 40, 28, 2, 1, 100.0f, QColor(180, 150, 60));
    int col = nombre.contains("Machete", Qt::CaseInsensitive) ? 1 : 0;
    setPixmap(hoja.obtenerSprite(0, col));
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

void Arma::actualizarCooldown(qreal dt) {
    if (cooldownActual > 0.0) {
        cooldownActual = std::max(0.0, cooldownActual - dt);
    }
}

bool Arma::puedeDisparar() const {
    return balasRestantes > 0 && cooldownActual <= 0.0;
}

bool Arma::disparar() {
    if (puedeDisparar()) {
        balasRestantes--;
        cooldownActual = cadenciaDisparo;
        // Indica que acaba de destruirse para lanzar escombros parabolicos
        return true;
    }
    return false;
}

void Arma::recargar(int cantidadBalas) {
    balasRestantes = std::min(capacidad, balasRestantes + cantidadBalas);
}

bool Arma::lanzar() {
    // Comprueba una condicion antes de continuar.
    if (armaLanzable && balasRestantes > 0 && cooldownActual <= 0.0) {
        balasRestantes--;
        cooldownActual = cadenciaDisparo * 1.6;
        return true;
    }
    return false;
}

QString Arma::getNombre() const { return nombre; }
int Arma::getDano() const { return dano; }
int Arma::getCapacidad() const { return capacidad; }
int Arma::getBalasRestantes() const { return balasRestantes; }
bool Arma::tieneBalas() const { return balasRestantes > 0; }
bool Arma::esLanzable() const { return armaLanzable; }
