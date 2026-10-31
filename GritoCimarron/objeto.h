#ifndef OBJETO_H
#define OBJETO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QString>

class Objeto : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

protected:
    bool interactuable;

    public:
             explicit Objeto(QGraphicsItem* parent = nullptr);
    virtual ~Objeto();

    // Metodo generico para cargar y escalar el sprite del objeto 🖼️
    void cargarSprite(const QString& ruta, float porcentajeEscala = 100.0f);

    // Metodo virtual puro: cada objeto responders diferente al ser tocado/rescatado 🎭
    virtual void interactuar() = 0;
};

#endif // OBJETO_H