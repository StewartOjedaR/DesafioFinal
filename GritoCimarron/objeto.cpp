#include "objeto.h"

Objeto::Objeto(QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), interactuable(true) {
}

Objeto::~Objeto() {}

void Objeto::cargarSprite(const QString& ruta, float porcentajeEscala) {
    QPixmap pixmap(ruta);
    if (!pixmap.isNull()) {
        int nuevoAncho = static_cast<int>(pixmap.width() * (porcentajeEscala / 100.0f));
        int nuevoAlto = static_cast<int>(pixmap.height() * (porcentajeEscala / 100.0f));
        setPixmap(pixmap.scaled(nuevoAncho, nuevoAlto, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}