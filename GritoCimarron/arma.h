#ifndef ARMA_H
#define ARMA_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QString>
#include "hojasprites.h"

class Arma : public QObject, public QGraphicsPixmapItem {
    // Activa las señales y ranuras de Qt en esta clase.
    Q_OBJECT

private:
    QString nombre;
    int dano;
    int capacidad;
    int balasRestantes;
    bool armaLanzable;
    // Segundos entre disparos
    qreal cadenciaDisparo = 0.28; 
    qreal cooldownActual = 0.0;
    HojaSprites hoja;

public:
    explicit Arma(QString nombreArma, int danoBase, int capacidad, bool lanzable,
                  QGraphicsItem* parent = nullptr);
    virtual ~Arma();

    void actualizarCooldown(qreal dt);
    bool puedeDisparar() const;
    bool disparar();
    void recargar(int cantidadBalas);
    bool lanzar();
    QString getNombre() const;
    int getDano() const;
    int getCapacidad() const;
    int getBalasRestantes() const;
    bool tieneBalas() const;
    bool esLanzable() const;
    void cargarSprite(const QString& ruta, float porcentajeEscala = 100.0f);
};

// ARMA_H
#endif 
