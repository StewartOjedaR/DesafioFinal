#ifndef ARMA_H
#define ARMA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QString>

class Arma : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

private:
    QString nombre;
    int dano;
    int balasRestantes;
    int capacidadCargador;
    float cadenciaDisparo; // Tiempo entre disparos en segundos

public:
    explicit Arma(QString nombreArma = "Pistola", int danoBase = 25, int capacidad = 12, QGraphicsItem* parent = nullptr);
    virtual ~Arma();

    bool disparar(); // Resta una bala si hay disponibles
    void recargar(int cantidadBalas);

    // Getters y Setters
    int getDano() const;
    int getBalasRestantes() const;
    bool tieneBalas() const;

    // Carga de sprite del arma 🖼
    void cargarSprite(const QString& ruta, float porcentajeEscala = 100.0f);
};

#endif // ARMA_H