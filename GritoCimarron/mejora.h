// Inicia la guarda MEJORA_H contra inclusiones duplicadas.
#ifndef MEJORA_H
// Define la guarda MEJORA_H.
#define MEJORA_H

// Incluye la cabecera objeto.h.
#include "objeto.h"
// Incluye la cabecera hojasprites.h.
#include "hojasprites.h"

// Declara la clase Jugador.
class Jugador;

// Declara la clase Mejora.
class Mejora : public Objeto {
    // Activa las senales y ranuras de Qt en esta clase.
    Q_OBJECT

// Inicia la seccion public de la clase.
public:
    // Declara la enumeracion Tipo.
    enum Tipo {
        // Asigna un valor a la variable indicada.
        SALUD = 0,
        // Asigna un valor a la variable indicada.
        MUNICION = 1,
        // Asigna un valor a la variable indicada.
        VELOCIDAD = 2
    // Cierra el bloque anterior.
    };

// Inicia la seccion private de la clase.
private:
    // Declara o inicializa una variable.
    Tipo tipo;
    // Declara o inicializa una variable.
    int valor;
    // Declara o inicializa una variable.
    bool recogida = false;
    // Declara o inicializa una variable.
    qreal baseY = 0.0;
    // Declara o inicializa una variable.
    qreal tiempoAnim = 0.0;
    // Declara o inicializa una variable.
    HojaSprites hoja;

// Inicia la seccion public de la clase.
public:
    // Declara o define la funcion Mejora.
    explicit Mejora(Tipo tipoMejora = SALUD, int valorMejora = 35, QGraphicsItem* parent = nullptr);
    // Declara o define la funcion Mejora.
    virtual ~Mejora();

    // Declara o define la funcion setPosicionBase.
    void setPosicionBase(qreal x, qreal y);
    // Declara o define la funcion actualizarAnimacion.
    void actualizarAnimacion(qreal dt);
    // Declara o define la funcion aplicarA.
    void aplicarA(Jugador* jugador);
    // Declara o define la funcion interactuar.
    void interactuar() override;

    // Declara o define la funcion getTipo.
    Tipo getTipo() const { return tipo; }
    // Declara o define la funcion getValor.
    int getValor() const { return valor; }
    // Declara o define la funcion isRecogida.
    bool isRecogida() const { return recogida; }
// Cierra el bloque anterior.
};

// MEJORA_H
#endif 
