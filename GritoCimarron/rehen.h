#ifndef REHEN_H
#define REHEN_H

#include "Objeto.h"
#include <QTimer>

class Rehen : public Objeto {
    Q_OBJECT

private:
    bool liberado;
    int puntosRecompensa;

    QPixmap hojaSprites;
    int anchoSprite;
    int altoSprite;
    int frameActual;
    int totalFrames;
    float escalaPorcentaje;
    QTimer* timerAnimacion;

public:
    explicit Rehen(int puntos = 500, QGraphicsItem* parent = nullptr);
    virtual ~Rehen();

    void cargarHojaSprites(const QString& ruta, int anchoFrame, int altoFrame, float porcentajeEscala = 100.0f);
    void interactuar() override;

    bool isLiberado() const;
    int getPuntosRecompensa() const;

public slots:
    void actualizarAnimacion();
};

#endif // REHEN_H