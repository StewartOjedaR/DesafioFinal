#include "Personaje.h"

// Constructor de la clase Personaje
Personaje::Personaje(int saludInicial, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent) {

    // 1. Inicializamos los atributos protegidos
    salud = saludInicial;
    velocidad = 5.0f;
    frameActual = 0;

    // 2. Creamos la instancia del temporizador
    timerAnimacion = new QTimer(this);

    // 3. Conectamos la señal del timer con el slot de la animacion
    connect(timerAnimacion, &QTimer::timeout, this, &Personaje::actualizarAnimacion);

    // 4. Iniciamos el temporizador para que emita una señal cada 100 milisegundos
    timerAnimacion->start(100);
}

// Destructor para liberar la memoria
Personaje::~Personaje() {
    delete timerAnimacion;
}

// Ejemplo de implementacion de recibirDano
void Personaje::recibirDano(int cantidad) {
    salud -= cantidad;
    if (salud < 0) salud = 0;
}

int Personaje::getSalud() const {
    return salud;
}


void Personaje::cargarHojaSprites(const QString& ruta, int anchoFrame, int altoFrame, int columnas, float porcentajeEscala) {
    hojaSprites.load(ruta);
    anchoSprite = anchoFrame;
    altoSprite = altoFrame;
    columnasSprites = columnas;
    escalaPorcentaje = porcentajeEscala;

    if (!hojaSprites.isNull() && anchoSprite > 0 && altoSprite > 0) {
        int filas = hojaSprites.height() / altoSprite;
        int cols = hojaSprites.width() / anchoSprite;
        totalFrames = filas * cols; // Ejemplo: 3 filas * 4 columnas = 12 totalFrames
    }
}
void Personaje::actualizarAnimacion() {
    if (hojaSprites.isNull() || columnasSprites == 0) return;

    // Calculamos fila y columna exacta del frame actual
    int columnaActual = frameActual % columnasSprites;
    int filaActual = frameActual / columnasSprites;

    int posX = columnaActual * anchoSprite;
    int posY = filaActual * altoSprite;

    // Recortamos el cuadro exacto
    QPixmap frameRecortado = hojaSprites.copy(posX, posY, anchoSprite, altoSprite);

    // Escalamos y asignamos
    int nuevoAncho = static_cast<int>(anchoSprite * (escalaPorcentaje / 100.0f));
    int nuevoAlto = static_cast<int>(altoSprite * (escalaPorcentaje / 100.0f));

    QPixmap frameEscalado = frameRecortado.scaled(nuevoAncho, nuevoAlto, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(frameEscalado);

    // Avanzamos al siguiente frame usando el total real
    frameActual = (frameActual + 1) % totalFrames;
}
