#ifndef OBJETO_H
#define OBJETO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QString>

class Objeto : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_DISABLE_COPY(Objeto)

protected:
    bool interactuable;

public:
    explicit Objeto(QGraphicsItem* parent = nullptr);
    virtual ~Objeto();

    void cargarSprite(const QString& ruta, float porcentajeEscala = 100.0f);
    virtual void interactuar() = 0;
};

#endif // OBJETO_H