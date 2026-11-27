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
    int capacidad;
    int balasRestantes;
    bool armaLanzable;

public:
    explicit Arma(QString nombreArma, int danoBase, int capacidad, bool lanzable, QGraphicsItem* parent = nullptr);
    virtual ~Arma();

    bool disparar();
    void recargar(int cantidadBalas);
    void lanzar();

    int getDano() const;
    int getBalasRestantes() const;
    bool tieneBalas() const;

    void cargarSprite(const QString& ruta, float porcentajeEscala = 100.0f);
};

#endif // ARMA_H