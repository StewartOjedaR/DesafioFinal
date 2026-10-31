#ifndef REHEN_H
#define REHEN_H

#include "Objeto.h"
#include <QTimer>

class Rehen : public Objeto {
    Q_OBJECT

private:
    bool liberado;            // Estado: false = atado, true = rescatado
    int puntosRecompensa;     //  Puntos que le otorga al jugador (ej. 500)

    // Manejo de animacion
    QPixmap hojaSprites;      //  Hoja de sprites del rehen
    int anchoSprite;
    int altoSprite;
    int frameActual;
    int totalFrames;
    float escalaPorcentaje;
    QTimer* timerAnimacion;   //  Temporizador para animar al rehén en pantalla

public:
    explicit Rehen(int puntos = 500, QGraphicsItem* parent = nullptr);
    virtual ~Rehen();

    // Carga de la hoja de sprites y animacion
    void cargarHojaSprites(const QString& ruta, int anchoFrame, int altoFrame, float porcentajeEscala = 100.0f);

    // Implementacion obligatoria del metodo virtual puro de Objeto
    void interactuar() override;

    // Métodos de consulta (Getters)
    bool isLiberado() const;
    int getPuntosRecompensa() const;

public slots:
    void actualizarAnimacion(); // 🎞️ Cambia los fotogramas del rehen
};

#endif // REHEN_H