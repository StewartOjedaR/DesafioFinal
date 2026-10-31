#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.h"
#include <QKeyEvent>

class Jugador : public Personaje {
    Q_OBJECT

private:
    int vidas;
protected:
    float velocidadY;      //  Velocidad en el eje Y
    float gravedad;        //  Fuerza de gravedad (ej. 0.8f)
    float fuerzaSalto;     //  Impulso inicial del salto (ej. -12.0f)
    bool enElSuelo;        //  Indica si el personaje está apoyado en una plataforma
    public:
             explicit Jugador(int saludInicial = 100, QGraphicsItem* parent = nullptr);
    virtual ~Jugador();

    // Sobrescribimos los metodos virtuales puros de la clase base Personaje
    void mover() override;
    void atacar() override;
    void verificarColisiones();
    // Metodo de Qt para detectar la pulsacion de teclas
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // JUGADOR_H