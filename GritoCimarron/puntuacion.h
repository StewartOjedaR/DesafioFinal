#ifndef PUNTUACION_H
#define PUNTUACION_H

#include <QGraphicsTextItem>
#include <QFont>

class Puntuacion : public QGraphicsTextItem {
private:
    int puntosActuales;
    int recordMaximo;

public:
    explicit Puntuacion(QGraphicsItem* parent = nullptr);
    virtual ~Puntuacion();

    void agregarPuntos(int cantidad);
    void reiniciarPuntos();

    int getPuntosActuales() const;
    int getRecordMaximo() const;

    void actualizarTexto();
};

#endif // PUNTUACION_H