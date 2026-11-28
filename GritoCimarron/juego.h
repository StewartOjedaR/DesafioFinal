#ifndef JUEGO_H
#define JUEGO_H

#include <QGraphicsView>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QWidget>
#include "jugador.h"
#include "nivel.h"
#include "puntuacion.h"

// Declara de forma anticipada la clase Rehen para usar punteros
class Rehen;

// Define la ventana del juego como una vista de QGraphicsScene
class Juego : public QGraphicsView {
    // Activa las senales y ranuras de Qt en esta clase.
    Q_OBJECT

// Inicia la seccion public de la clase.
public:
    // Enumera los estados posibles del juego.
    enum EstadoJuego {
        // Estado del menu principal
        MENU,
        // Estado con la partida en ejecucion
        JUGANDO,
        // Estado con la partida en pausa
        PAUSA,
        // Estado de paso entre niveles
        TRANSICION,
        // Estado de victoria final
        VICTORIA,
        // Estado de derrota
        GAME_OVER
    };

private:
    // puntero a nivel actual; apunta a null hasta iniciar el nivel
    Nivel* nivel = nullptr;
    //puntero de jugador del nivel; empieza sin jugador asignado
    Jugador* jugador = nullptr;
    // puntero de la puntuacion; empieza sin objeto asignado
    Puntuacion* puntuacion = nullptr;
    // Guarda el temporizador del bucle; empieza sin objeto asignado
    QTimer* gameTimer = nullptr;
    // Mide el tiempo transcurrido entre fotogramas.
    QElapsedTimer relojFrame;
    // Conserva el estado actual del juego, inicialmente en el menu.
    EstadoJuego estado = MENU;
    // Recuerda el nivel que se cargara despues de una transicion o reintento.
    int nivelPendiente = 1;

    // HUD fijo en pantalla (sobre la vista, no se desplaza con el scroll)
    // Contenedor del HUD fijo sobre la vista
    QWidget* hudWidget = nullptr;
    // Barra que muestra la salud del jugador
    QProgressBar* barraSalud = nullptr;
    // Etiqueta con el numero de vidas
    QLabel* lblVidas = nullptr;
    // Etiqueta con el estado del arma
    QLabel* lblArma = nullptr;
    // Etiqueta con la puntuacion y el record
    QLabel* lblPuntuacion = nullptr;
    // Etiqueta con el objetivo del nivel
    QLabel* lblObjetivo = nullptr;
    // Etiqueta con el tiempo restante
    QLabel* lblTiempo = nullptr;
    // Etiqueta del aviso inferior
    QLabel* lblAlerta = nullptr;

    // Panel superpuesto para Menu / Pausa / Transicion / Victoria / Game Over
    // Contenedor del panel superpuesto
    QWidget* panelOverlay = nullptr;
    // Etiqueta con el titulo del panel
    QLabel* lblTituloOverlay = nullptr;
    // Etiqueta con el texto detallado del panel
    QLabel* lblDetalleOverlay = nullptr;
    // Boton principal de accion del panel
    QPushButton* btnPrincipal = nullptr;
    // Boton secundario de accion del panel
    QPushButton* btnSecundario = nullptr;
    // Boton para volver al menu principal
    QPushButton* btnMenu = nullptr;

    // Crea los widgets del HUD y los coloca sobre la vista
    void crearHUD();
    // Crea el panel superpuesto y sus botones
    void crearOverlay();
    // Actualiza los valores mostrados en el HUD
    void actualizarHUD();
    // Ajusta el punto de vista para seguir al jugador
    void actualizarCamara();
    // Muestra el panel con titulo, detalle y botones indicados
    void mostrarOverlay(const QString& titulo, const QString& detalle,
                        // Añade el texto del primer boton y, si se da, el del segundo
                        const QString& textoBtn1, const QString& textoBtn2 = QString(),
                        // Indica si ademas debe aparecer el boton de menu
                        bool mostrarBtnMenu = false);
    // Oculta el panel superpuesto y devuelve el foco a la vista
    void ocultarOverlay();

public:
    explicit Juego(QWidget* parent = nullptr);
    virtual ~Juego();

    // Prepara el menu inicial y carga el primer nivel
    void iniciarJuego();
    // Alterna entre pausa y partida en curso
    void pausarJuego();
    // Carga el nivel indicado y reinicia la partida
    void cambiarNivel(int siguienteNivel);
    // Consulta si el juego termino en derrota
    bool verificarGameOver() const;
    // Registra el rescate de un rehen
    void rescatarRehen(Rehen* rehen);
    // Libera al rehen y suma su recompensa
    void procesarRescate(Rehen* rehen);
    // Devuelve el objeto de puntuacion del juego
    Puntuacion* obtenerPuntuacion() const;
    // Vuelve a cargar el nivel actual
    void cargarNivel();
    // Ajusta la camara cuando cambia el tamaño de la ventana
    void resizeEvent(QResizeEvent* event) override;
    // Ancho logico del mundo que puede recorrer la camara
    qreal anchoMundo = 9000.0;
    // Alto logico del mundo que puede recorrer la camara
    qreal altoMundo = 660.0;

// Inicia las ranuras publicas para eventos de entrada
public slots:
    // Procesa las teclas pulsadas durante el juego
    void keyPressEvent(QKeyEvent* event) override;
    // Procesa las teclas liberadas durante el juego
    void keyReleaseEvent(QKeyEvent* event) override;
    // Procesa los clics del raton sobre la vista
    void mousePressEvent(QMouseEvent* event) override;
    // Procesa el movimiento del raton sobre la vista
    void mouseMoveEvent(QMouseEvent* event) override;
    // Ejecuta un ciclo de actualizacion del juego
    void gameLoop();

// Inicia la seccion private slots de la clase.
private slots:
    // Suma puntos recibidos desde el nivel.
    void onPuntosGanados(int puntos, QString motivo);
    // Gestiona la finalizacion de un nivel.
    void onNivelCompletado(int nivelTerminado, int tiempoRestanteSeg);
    // Muestra el fin del juego con la razon indicada.
    void onGameOverSolicitado(QString razon);
    // Atiende la pulsacion del boton principal.
    void onBotonPrincipal();
    // Atiende la pulsacion del boton secundario.
    void onBotonSecundario();
    // Atiende la pulsacion del boton de menu.
    void onBotonMenu();
};

// JUEGO_H
#endif 
