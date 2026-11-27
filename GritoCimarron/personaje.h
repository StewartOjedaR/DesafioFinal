#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Personaje : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

protected:
    bool movIzquierda=false;
    bool movDerecha=false;
    bool movAbajo=false;
    bool movArriba=false;
    bool saltar=false;
    bool enElSuelo=true;
    bool modoCenital=false;
    bool mirandoDerecha=true;
    qreal vy=0;
    qreal vx=0;
    qreal velocidad=5.0;
    int salud;
    int dano;
    int frameActual=0;
    int contadorFrames=0;
    int velocidadAnimacion=5;
    QPixmap hojaSprites;
    int anchoSprite;
    int altoSprite;
    float escalaPorcentaje;
    int columnasSprites;
    int totalFrames;
    public:
    Personaje(int saludInicial, QGraphicsItem* parent = nullptr);
    virtual ~Personaje();
    void recibirDano(int cantidad);
    int getSalud() const;
    void cargarHojaSprites(const QString& ruta, int anchoFrame, int altoFrame, int columnas, float porcentajeEscala);
    void setMoverIzquierda(bool activo);
    void setMoverDerecha(bool activo);
    void setMoverArriba(bool activo);
    void setMoverAbajo(bool activo);

    bool getMoverIzquierda() const;
    bool getMoverDerecha() const;
    bool getMoverArriba() const;
    bool getMoverAbajo() const;

    qreal getVx() const;
    qreal getVy() const;
    qreal getVelocidad() const;
    void setVx(qreal valor);
    void setVy(qreal valor);
    virtual void atacar() = 0;
    virtual void actualizarPosicion();
    virtual void actualizarAnimacion();
};

#endif // PERSONAJE_H