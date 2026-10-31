#include "Juego.h"

Juego::Juego(QWidget* parent) : QGraphicsView(parent) {
    // 1. Creamos la escena con las dimensiones del mapa
    escena = new QGraphicsScene(0, 0, 1280, 720, this);
    setScene(escena);

    // 2. Configuramos la ventana (camara)
   // setFixedSize(1280, 720);
    resize(1280, 720);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 3. Creamos el Game Loop principal (60 FPS ≈ 16 ms)
    timerGameLoop = new QTimer(this);
    connect(timerGameLoop, &QTimer::timeout, this, &Juego::gameLoop);
}

Juego::~Juego() {
    delete escena;
}
void Juego::probarEscena() {
    // 1. Fondo del nivel
  /*  QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(QPixmap(":/imagenes/fondo_nivel1.png"));
    fondo->setZValue(-10); // Detrás de los personajes y objetos
    escena->addItem(fondo);
*/
    // 2. Jugador (Ejemplo: Sprite de 48x48 px, hoja de 4 columnas y 12 frames en total)
    jugador = new Jugador();
    jugador->cargarHojaSprites("/benkos.png", 100, 190, 6, 150.0f);
    jugador->setPos(100, 400);
    jugador->setZValue(10);
    escena->addItem(jugador);
/*
    // 3. Enemigo (Ejemplo: Sprite de 32x32 px, hoja de 6 columnas y 6 frames)
    Enemigo* enemigoTest = new Enemigo();
    enemigoTest->cargarHojaSprites(":/sprites/enemigo_sheet.png", 32, 32, 6, 6, 150.0f);
    enemigoTest->setPos(500, 400);
    enemigoTest->setZValue(10);
    escena->addItem(enemigoTest);

    // 4. Rehen (Ejemplo: Sprite de 32x32 px, hoja de 3 columnas y 3 frames)
    Rehen* rehenTest = new Rehen(500);
    rehenTest->cargarHojaSprites(":/sprites/rehen_sheet.png", 32, 32, 3, 3, 150.0f);
    rehenTest->setPos(700, 400);
    rehenTest->setZValue(10);
    escena->addItem(rehenTest);

    // 5. Objeto / Armas en el suelo (Ejemplo: Cuchillo o Municion estatica)
    Objeto* cuchillo = new Objeto();
    cuchillo->cargarSprite(":/sprites/cuchillo.png", 100.0f);
    cuchillo->setPos(300, 430);
    cuchillo->setZValue(5);
    escena->addItem(cuchillo);
}*/
}
void Juego::iniciarJuego() {
   // cargarNivel();

    timerGameLoop->start(16); // Inicia la actualizacion a 60 FPS
}

void Juego::gameLoop() {
    // 1. Hacemos que la camara siga al jugador (Scroll lateral)
    if (jugador != nullptr) {
        centerOn(jugador);
    }

    // 2. Aqui comprobaremos las colisiones globales en cada frame
}