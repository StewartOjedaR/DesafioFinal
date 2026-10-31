#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Personaje : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT // Macro obligatorio de Qt para clases que heredan de QObject

protected:
    // Atributos accesibles por las clases hijas (Jugador y Enemigo)
    int salud;
        float velocidad;
        int frameActual;
        QTimer* timerAnimacion;
        QPixmap hojaSprites;    //  Imagen completa que contiene todos los frames
        int anchoSprite;        //  Ancho de un solo frame
        int altoSprite;         //  Alto de un solo frame
        float escalaPorcentaje;
        int columnasSprites; //  Cantidad de frames por fila
         int totalFrames; //  Guarda el total de sprites que tiene la animacion
    public:
             // Constructor
             Personaje(int saludInicial, QGraphicsItem* parent = nullptr);

    // Destructor virtual (indispensable para evitar fugas de memoria con herencia)
    virtual ~Personaje();

    // Metodos comunes
    void recibirDano(int cantidad);
    int getSalud() const;
    void cargarHojaSprites(const QString& ruta, int anchoFrame, int altoFrame, int columnas, float porcentajeEscala);
    // Metodos virtuales puros (= 0): Obligan a Jugador y Enemigo a implementar su propia logica
    virtual void mover() = 0;
        virtual void atacar() = 0;

        public slots:
                       // Slot para actualizar el fotograma de la animacion con el QTimer
                       virtual void actualizarAnimacion();
};

#endif // PERSONAJE_H