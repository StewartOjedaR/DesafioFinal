#ifndef PUNTUACION_H
#define PUNTUACION_H

#include <QGraphicsTextItem>
#include <QFont>

class Puntuacion : public QGraphicsTextItem {
private:
    int puntosActuales; //  Puntos en la partida actual
    int recordMaximo;   //  Maxima puntuación registrada

public:
    explicit Puntuacion(QGraphicsItem* parent = nullptr);
    virtual ~Puntuacion();

    // Metodos para modificar y consultar el puntaje
    void agregarPuntos(int cantidad);
    void reiniciarPuntos();

    int getPuntosActuales() const;
    int getRecordMaximo() const;

    // Actualiza el texto en la pantalla 🖼️
    void actualizarTexto();
};

#endif // PUNTUACION_H