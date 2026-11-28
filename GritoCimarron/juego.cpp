#include "juego.h"
#include "rehen.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QHBoxLayout>// Incluye el layout horizontal usado en el HUD
#include <QVBoxLayout>// Incluye el layout vertical usado en el panel.
#include <QFont>// Incluye la configuracion de fuentes.
#include <algorithm>

Juego::Juego(QWidget* parent)
    : QGraphicsView(parent) {

    // Establece el titulo de la ventana
    setWindowTitle("El Grito de Benkos - Fantasías Animadas de la Historia de Colombia");
 //   setFixedSize(1280, 720);
    // Fija un tamaño minimo para la ventana
    setMinimumSize(800, 450);
    // Permite que la ventana crezca y se encoja
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Oculta la barra de desplazamiento horizontal
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Oculta la barra de desplazamiento vertical
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Hace que la vista reciba el foco de teclado.
    setFocusPolicy(Qt::StrongFocus);
    // Activa el seguimiento del raton aunque no haya boton pulsado.
    setMouseTracking(true);
    // Suaviza el dibujo con antialiasing para que al agrandar la ventana no quede borde feo
    setRenderHint(QPainter::Antialiasing);

    // Para que al agrandar la ventana no quede borde feo
    // Quita el marco alrededor de la vista.
    setFrameStyle(QFrame::NoFrame);
    // Pinta el fondo de la escena de color negro.
    setBackgroundBrush(QBrush(Qt::black));
    // Actualiza toda el area visible en cada repintado.
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    // Alinea la escena con la esquina superior izquierda
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Crea el nivel y lo asocia a esta ventana como padre.
    nivel = new Nivel(this);
    // Asigna el nivel creado como escena de la vista
    setScene(nivel);
 //   nivel->setSceneRect(0, 0, anchoMundo, altoMundo);// Asegura que la escena tenga el tamaño del mundo panoramico

    // Crea el objeto que administra la puntuacion
    puntuacion = new Puntuacion();

    // Conecta los puntos ganados del nivel con su ranura.
    connect(nivel, &Nivel::puntosGanados, this, &Juego::onPuntosGanados);
    // Conecta la finalizacion de nivel con su ranura.
    connect(nivel, &Nivel::nivelCompletado, this, &Juego::onNivelCompletado);
    // Conecta la peticion de fin de juego con su ranura
    connect(nivel, &Nivel::gameOverSolicitado, this, &Juego::onGameOverSolicitado);

    // Construye el HUD fijo.
    crearHUD();
    // Construye el panel superpuesto
    crearOverlay();

    // Crea el temporizador que impulsa el bucle de juego
    gameTimer = new QTimer(this);
    // Conecta cada vencimiento del temporizador con el bucle.
    connect(gameTimer, &QTimer::timeout, this, &Juego::gameLoop);
    // Inicia el cronometro de fotogramas
    relojFrame.start();
}

Juego::~Juego() {
    // Comprueba si existe el temporizador antes de usarlo.
    if (gameTimer) {
        // Detiene el bucle de juego.
        gameTimer->stop();
    }
    // Libera la memoria de la puntuacion
    delete puntuacion;
    // Deja el puntero de puntuacion a nulo
    puntuacion = nullptr;
}

void Juego::crearHUD() {
    // Crea el contenedor del HUD sobre la ventana
    hudWidget = new QWidget(this);
    // Coloca el HUD en la esquina superior con 62 pixeles de alto
    hudWidget->setGeometry(0, 0, 1280, 62);
    // Aplica estilo al contenedor y a sus etiquetas
    hudWidget->setStyleSheet(
        "QWidget { background-color: rgba(18, 18, 26, 200); color: white; }"
        "QLabel { background: transparent; font-family: Arial; font-weight: bold; }");

    // Crea un layout horizontal dentro del HUD
    auto* layout = new QHBoxLayout(hudWidget);
    // Ajusta los margenes del layout
    layout->setContentsMargins(16, 8, 16, 8);
    // Define la separacion entre widgets
    layout->setSpacing(18);

    // Crea la etiqueta de texto de la salud
    auto* lblHpTxt = new QLabel("SALUD:", hudWidget);
    // Crea la barra de progreso de la salud
    barraSalud = new QProgressBar(hudWidget);
    // Establece el rango de la barra entre 0 y 100
    barraSalud->setRange(0, 100);
    // Inicializa la barra al valor maximo
    barraSalud->setValue(100);
    // Fija el ancho de la barra
    barraSalud->setFixedWidth(155);
    // Fija el alto de la barra
    barraSalud->setFixedHeight(20);
    barraSalud->setStyleSheet(
        "QProgressBar { border: 2px solid #111; border-radius: 5px; background: #333; text-align: center; color: white; font-weight: bold; }"
        // Define el color rojo del sector que representa la salud.
        "QProgressBar::chunk { background-color: #d93838; border-radius: 3px; }");

    // Crea la etiqueta de vidas.
    lblVidas = new QLabel("VIDAS: 3", hudWidget);
    // Aplica el estilo rojo a las vidas.
    lblVidas->setStyleSheet("color: #ff8080; font-size: 14px;");

    // Crea la etiqueta del arma y sus balas.
    lblArma = new QLabel("BALAS: 14/14", hudWidget);
    // Aplica el estilo amarillo al arma
    lblArma->setStyleSheet("color: #ffd452; font-size: 14px;");

    // Crea la etiqueta de jaulas liberadas
    lblObjetivo = new QLabel("JAULAS: 0/4", hudWidget);
    // Aplica el estilo verde al objetivo
    lblObjetivo->setStyleSheet("color: #7ee88b; font-size: 14px;");

    // Crea la etiqueta del tiempo restante
    lblTiempo = new QLabel("TIEMPO: 95s", hudWidget);
    // Aplica el estilo azul al tiempo
    lblTiempo->setStyleSheet("color: #7fd4ff; font-size: 14px;");

    // Crea la etiqueta de puntos y record.
    lblPuntuacion = new QLabel("PUNTOS: 0 | RÉCORD: 0", hudWidget);
    // Aplica el estilo dorado a la puntacion
    lblPuntuacion->setStyleSheet("color: #ffe66d; font-size: 14px;");

    // Añade la etiqueta "SALUD:" al layout
    layout->addWidget(lblHpTxt);
    // Añade la barra de salud al layout
    layout->addWidget(barraSalud);
    // Añade las vidas al layout
    layout->addWidget(lblVidas);
    // Añade el arma al layout
    layout->addWidget(lblArma);
    // Añade el objetivo al layout
    layout->addWidget(lblObjetivo);
    // Añade el tiempo al layout
    layout->addWidget(lblTiempo);
    // Deja un espacio elastico antes de la puntuacion.
    layout->addStretch();
    // Añade la puntuacion al final del layout
    layout->addWidget(lblPuntuacion);

    // Banner inferior de alertas fisicas y eventos
    // Crea la etiqueta de alerta sobre la ventana
    lblAlerta = new QLabel(this);
    // Situa la alerta debajo del HUD
    lblAlerta->setGeometry(140, 72, 1000, 34);
    // Centra el texto de la alerta
    lblAlerta->setAlignment(Qt::AlignCenter);
    // Continua la definicion de esta instruccion.
    lblAlerta->setStyleSheet(
        // Define fondo oscuro semitransparente y color amarillo del texto
        "background-color: rgba(0, 0, 0, 175); color: #ffe066; "
        // Define fuente, tamaño, negrita y esquinas redondeadas
        "font-family: Arial; font-size: 14px; font-weight: bold; "
        // Define el radio de las esquinas y el relleno interior
        "border-radius: 8px; padding: 4px 12px;");
    // Oculta la alerta hasta que se necesite
    lblAlerta->hide();
    // Oculta el HUD mientras este el menu
    hudWidget->hide();
}

void Juego::crearOverlay() {
    // Crea el contenedor del panel sobre la ventana.
    panelOverlay = new QWidget(this);
    // Le da tamaño y posicion centrada.
    panelOverlay->setGeometry(190, 95, 900, 530);
    panelOverlay->setStyleSheet(
        // Define fondo oscuro, borde dorado y esquinas redondeadas
        "QWidget#panelOverlay { background-color: rgba(20, 22, 32, 238); "
        // Cierra la regla de estilo del panel
        "border: 4px solid #e0a82e; border-radius: 16px; }");
    // Asigna el nombre usado por la regla de estilpo
    panelOverlay->setObjectName("panelOverlay");

    // Crea un layout vertical dentro del panel
    auto* vbox = new QVBoxLayout(panelOverlay);
    // Ajusta los margenes del panel
    vbox->setContentsMargins(36, 28, 36, 28);
    // Define la separacion entre elementos del panel
    vbox->setSpacing(16);

    // Crea la etiqueta del titulo del panel
    lblTituloOverlay = new QLabel(panelOverlay);
    // Centra el titulo
    lblTituloOverlay->setAlignment(Qt::AlignCenter);
    // Permite que el titulo ocupe varias lineas
    lblTituloOverlay->setWordWrap(true);
    // Aplica estilo dorado al titulo
    lblTituloOverlay->setStyleSheet(
        // Define color, fuente, tamaño y fondo transparente del titulo
        "color: #ffd24c; font-family: Arial; font-size: 30px; font-weight: bold; background: transparent;");

    // Crea la etiqueta del texto detallado del panel.
    lblDetalleOverlay = new QLabel(panelOverlay);
    // Centra el detalle
    lblDetalleOverlay->setAlignment(Qt::AlignCenter);
    // Permite que el detalle ocupe varias lineas
    lblDetalleOverlay->setWordWrap(true);
    lblDetalleOverlay->setStyleSheet(
        // Define color, fuente, tamaño, interlineado y fondo del detalle
        "color: #f0f0f5; font-family: Arial; font-size: 15px; line-height: 1.4; background: transparent;");

    QString estiloBtn =
        // Define el aspecto normal del boton principal
        "QPushButton { background-color: #c93b2b; color: white; font-family: Arial; "
        // Define tamaño, relleno, esquinas y borde del boton principal
        "font-size: 16px; font-weight: bold; padding: 11px 22px; border-radius: 8px; border: 2px solid #ffd24c; }"
        // Define el color cuando el raton pasa sobre el boton
        "QPushButton:hover { background-color: #e04d3c; }";

    // Crea el boton principal del panel
    btnPrincipal = new QPushButton(panelOverlay);
    // Aplica al boton principal el estilo preparado
    btnPrincipal->setStyleSheet(estiloBtn);
    // Fija el ancho minimo del boton principal
    btnPrincipal->setMinimumWidth(280);

    // Crea el boton secundario del panel
    btnSecundario = new QPushButton(panelOverlay);
    // Continua la definicion de esta instruccion.
    btnSecundario->setStyleSheet(
        // Define el aspecto normal del boton secundario
        "QPushButton { background-color: #2f6e49; color: white; font-family: Arial; "
        // Define tamaño, relleno, esquinas y borde del boton secundario
        "font-size: 15px; font-weight: bold; padding: 10px 20px; border-radius: 8px; border: 2px solid #90e0a8; }"
        // Define el color al pasar el raton sobre el boton secundario
        "QPushButton:hover { background-color: #3a8559; }");
    // Fija el ancho minimo del boton secundario
    btnSecundario->setMinimumWidth(280);

    // Crea el boton de volver al menu con su texto
    btnMenu = new QPushButton("Volver al Menú Principal", panelOverlay);
    // Continua la definicion de esta instruccion.
    btnMenu->setStyleSheet(
        // Define el aspecto normal del boton de menu.
        "QPushButton { background-color: #424858; color: white; font-family: Arial; "
        // Define tamaño, relleno, esquinas y borde del boton de menu
        "font-size: 14px; font-weight: bold; padding: 9px 18px; border-radius: 8px; border: 2px solid #99a; }"
        // Define el color al pasar el raton sobre el boton de menu
        "QPushButton:hover { background-color: #555c70; }");
    // Fija el ancho minimo del boton de menu
    btnMenu->setMinimumWidth(240);

    // Añade el titulo al panel
    vbox->addWidget(lblTituloOverlay);
    // Añade el detalle con factor de estiramiento 1
    vbox->addWidget(lblDetalleOverlay, 1);
    // Añade el boton principal centrado
    vbox->addWidget(btnPrincipal, 0, Qt::AlignCenter);
    // Añade el boton secundario centrado
    vbox->addWidget(btnSecundario, 0, Qt::AlignCenter);
    // Añade el boton de menu centrado
    vbox->addWidget(btnMenu, 0, Qt::AlignCenter);

    // Conecta el boton principal con su ranura
    connect(btnPrincipal, &QPushButton::clicked, this, &Juego::onBotonPrincipal);
    // Conecta el boton secundario con su ranura
    connect(btnSecundario, &QPushButton::clicked, this, &Juego::onBotonSecundario);
    // Conecta el boton de menu con su ranura
    connect(btnMenu, &QPushButton::clicked, this, &Juego::onBotonMenu);
}

void Juego::mostrarOverlay(const QString& titulo, const QString& detalle, const QString& textoBtn1, const QString& textoBtn2,                           bool mostrarBtnMenu) {
    // Escribe el titulo recibido en la etiqueta correspondiente
    lblTituloOverlay->setText(titulo);
    // Escribe el detalle recibido en la etiqueta correspondiente
    lblDetalleOverlay->setText(detalle);
    // Escribe el texto del boton principal
    btnPrincipal->setText(textoBtn1);

    // Comprueba si no se facilito texto para el boton secundario
    if (textoBtn2.isEmpty()) {
        // Oculta el boton secundario
        btnSecundario->hide();
    } else {
        // Asigna el texto recibido al boton secundario
        btnSecundario->setText(textoBtn2);
        // Muestra el boton secundario
        btnSecundario->show();
   }

    // Muestra u oculta el boton de menu segun el parametro
    btnMenu->setVisible(mostrarBtnMenu);
    // Hace visible el panel
    panelOverlay->show();
    // Trae el panel por delante de los demas widgets
    panelOverlay->raise();
}

void Juego::ocultarOverlay() {
    // Esconde el panel.
    panelOverlay->hide();
    // Devuelve el foco a la vista del juego
    setFocus();
}

void Juego::iniciarJuego() {
    // Coloca el juego en estado de menu
    estado = MENU;
    // Carga el nivel 1
    nivel->cargarNivel(1);
    // Obtiene el jugador del nivel cargado
    jugador = nivel->getJugador();
    // Mantiene el HUD oculto en el menu
    hudWidget->hide();
    // Mantiene la alerta oculta en el menu
    lblAlerta->hide();

    mostrarOverlay(
        "EL GRITO DE BENKOS",
        // Texto de bienvenida y descripcion de los dos niveles.
        "Fantasías animadas de la historia de Colombia\n\n"
        // Describe el objetivo y las fisicas del primer nivel.
        "NIVEL 1 (Vista Lateral - La Hacienda): Destruye las 4 jaulas para liberar a tus hermanos y escapa antes que el reloj llegue a cero.\n"
        // Detalla gravedad, salto y obstaculos del primer nivel
        "  • Físicas: Gravedad y salto (MRUA), tiro parabólico de escombros y cadenas que ralentizan al impactar.\n\n"
        // Describe el objetivo del segundo nivel
        "NIVEL 2 (Vista Cenital - Ruta a Palenque): Sobrevive a la guardia española y abre paso hacia Palenque.\n"
        // Detalla pendulos, cañones y enemigo inteligente del segundo nivel
        "  • Físicas: Péndulos con púas (movimiento oscilatorio), cañones con balas de rebote elástico y Soldado Rastreador IA.\n\n"
        // Indica los controles de movimiento, salto y ataque
        "CONTROLES: [A/D o Flechas] Mover | [W/↑/Espacio] Saltar (N1) | [J / F / Click Izq] Atacar y Disparar\n"
        // Indica los controles de lanzamiento, interaccion y pausa
        "[K / Click Der] Lanzar arma parabólica | [E] Interactuar | [P / Esc] Pausa",
        // Texto del boton para empezar en el nivel 1
        "Jugar desde Nivel 1",
        // Texto del boton para probar directamente el nivel 2
        "Probar Nivel 2 (Cenital)",
        // No muestra el boton de menu en el menu inicial
        false);
}

void Juego::cambiarNivel(int siguienteNivel) {
    // Pide al nivel que cargue el numero recibido.
    nivel->cargarNivel(siguienteNivel);
    // Actualiza el puntero al jugador del nuevo nivel
    jugador = nivel->getJugador();
    // Comprueba que exista un jugador antes de limpiar teclas.
    if (jugador) {
        // Borra el estado de las teclas pulsadas.
        jugador->limpiarTeclas();
    }
    // Coloca el juego en estado de partida
    estado = JUGANDO;
    // Muestra el HUD
    hudWidget->show();
    // Oculta cualquier panel superpuesto.
    ocultarOverlay();
    // Refresca los valores del HUD
    actualizarHUD();
    // Coloca la camara sobre el jugador
    actualizarCamara();

    // Reinicia el cronometro de fotogramas
    relojFrame.restart();
    // Comprueba si el temporizador existe y no esta activo
    if (gameTimer && !gameTimer->isActive()) {
        // Arranca el bucle con intervalos de 16 ms.
        gameTimer->start(16);
    }
}

void Juego::pausarJuego() {
    // Comprueba si el juego estaba en marcha
    if (estado == JUGANDO) {
        // Cambia el estado a pausa
        estado = PAUSA;
        // Limpia las teclas si hay jugador.
        if (jugador) jugador->limpiarTeclas();
        mostrarOverlay(
            // Titulo del panel de pausa.
            "JUEGO EN PAUSA",
            // Listado de controles rapidos.
            "Controles rápidos:\n"
            // Indica el movimiento en ambos niveles
            "• A/D o Flechas: Moverse (W/A/S/D en Nivel 2)\n"
            // Indica el salto en el primer nivel
            "• W / ↑ / Espacio: Saltar en Nivel 1\n"
            // Indica machete y pistola
            "• J / F / Click Izquierdo: Machete y Pistola\n"
            // Indica el lanzamiento parabolico.
            "• K / Click Derecho: Lanzamiento parabólico\n"
            // Indica como reanudar la partida
            "• P / Esc: Reanudar",
            // Texto para reanudar
            "Reanudar",
            // Texto para reiniciar el nivel.
            "Reiniciar Nivel",
            // Muestra tambien el boton de menu
            true);
    // Comprueba si el juego ya estaba en pausa.
    } else if (estado == PAUSA) {
        // Vuelve al estado de partida
        estado = JUGANDO;
        // Quita el panel de pausa
        ocultarOverlay();
        // Reinicia el cronometro para no contar la pausa
        relojFrame.restart();
    }
}

bool Juego::verificarGameOver() const {
    // Devuelve si el estado actual es derrota
    return estado == GAME_OVER;
}

void Juego::rescatarRehen(Rehen* rehen) {
    // Delega el rescate en la funcion de procesamiento
    procesarRescate(rehen);
}

void Juego::procesarRescate(Rehen* rehen) {
    // Comprueba que el rehen exista y aun no este liberado
    if (rehen && !rehen->isLiberado()) {
        // Marca al rehen como liberado
        rehen->liberar();
        // Comprueba que exista el gestor de puntuacion
        if (puntuacion) {
            // Suma la recompensa correspondiente al rehen.
            puntuacion->agregarPuntosRehen(rehen->getPuntosRecompensa());
        }
    }
}

Puntuacion* Juego::obtenerPuntuacion() const {
    // Devuelve el puntero al gestor de puntuacion
    return puntuacion;
}

void Juego::cargarNivel() {
    // Pide al nivel que vuelva a cargarse
    nivel->cargarNivel();
    // Actualiza el puntero al jugador
    jugador = nivel->getJugador();
}

void Juego::actualizarHUD() {
    // Sale si no hay jugador o nivel disponibles
    if (!jugador || !nivel) return;

    // Ajusta el maximo de la barra a la salud maxima.
    barraSalud->setMaximum(jugador->getSaludMaxima());
    // Muestra la salud actual del jugador
    barraSalud->setValue(jugador->getSalud());
    // Muestra el numero de vidas
    lblVidas->setText(QString("VIDAS: %1").arg(jugador->getVidas()));

    // Comprueba si el jugador tiene un arma equipada
    if (jugador->getArmaEquipada()) {
        // Guarda el arma equipada en una variable local
        Arma* a = jugador->getArmaEquipada();
        // Muestra balas restantes y capacidad del arma
        lblArma->setText(QString("BALAS: %1/%2").arg(a->getBalasRestantes()).arg(a->getCapacidad()));
    }

    // Comprueba si el nivel actual es el primero
    if (nivel->getNumeroNivel() == 1) {
        // Prepara el texto de jaulas liberadas sobre el total
        lblObjetivo->setText(QString("JAULAS: %1/%2")
                                 // Añade el numero de rehenes liberados al texto
                                 .arg(nivel->getRehenesLiberados())
                                 // Añade el total de rehenes al texto.
                                 .arg(nivel->getTotalRehenes()));
        // Redondea el tiempo restante a segundos enteros.
        int seg = static_cast<int>(std::ceil(nivel->getTiempoRestante()));
        // Muestra el tiempo restante formateado
        lblTiempo->setText(QString("TIEMPO: %1s").arg(seg));
        // Aplica estilo rojo o azul segun el tiempo restante
        lblTiempo->setStyleSheet(seg <= 15
                                     // Estilo rojo cuando quedan 15 segundos o menos
                                     ? "color: #ff5555; font-size: 15px;"
                                     // Estilo azul cuando todavia queda mas tiempo
                                     : "color: #7fd4ff; font-size: 14px;");
    } else {
        // Muestra los guardias que quedan.
        lblObjetivo->setText(QString("GUARDIAS: %1").arg(nivel->getEnemigosRestantes()));
        // Indica que el modo es cenital hacia Palenque.
        lblTiempo->setText("MODO: CENITAL (PALENQUE)");
        // Aplica el estilo morado del modo cenital.
        lblTiempo->setStyleSheet("color: #c59cff; font-size: 14px;");
    }

    // Comprueba si hay puntuacion que mostrar.
    if (puntuacion) {
        // Prepara el texto de puntos y record
        lblPuntuacion->setText(QString("PUNTOS: %1 | RÉCORD: %2")
                                   // Añade los puntos actuales al texto
                                   .arg(puntuacion->getPuntosActuales())
                                   // Añade el record maximo al texto
                                   .arg(puntuacion->getRecordMaximo()));
    }

    // Toma el mensaje de estado del nivel como alerta
    QString alerta = nivel->getMensajeEstado();
    // Detecta una reduccion de velocidad sin otra alerta
    if (jugador->getFactorVelocidad() < 0.99 && alerta.isEmpty()) {
        // Avisa de la ralentizacion por escombros
        alerta = "¡Velocidad reducida por impacto de escombros!";
    // Detecta un impulso de velocidad sin otra alerta
    } else if (jugador->getFactorVelocidad() > 1.05 && alerta.isEmpty()) {
        // Avisa de que el impulso cimarron esta activo
        alerta = "¡Impulso de velocidad cimarrón activo!";
    }

    // Comprueba si no hay ninguna alerta que mostrar
    if (alerta.isEmpty()) {
        // Oculta la etiqueta de alerta
        lblAlerta->hide();
    // Cierra el bloque anterior.
    } else {
        // Escribe el texto de la alerta
        lblAlerta->setText(alerta);
        // Hace visible la etiqueta de alerta
        lblAlerta->show();
    }
}

/*void Juego::actualizarCamara() {
    // Comprueba una condicion antes de continuar.
    if (!jugador || !nivel) return;
    // Declara o inicializa una variable.
    QPointF centro = jugador->centroGlobal();
    // En Nivel 1 el centro vertical queda fijo en 360 para vista lateral de 720px
    // Declara o inicializa una variable.
    qreal camY = (nivel->getNumeroNivel() == 1) ? 360.0 : centro.y();
    // Declara o define la funcion centerOn.
    centerOn(centro.x(), camY);
// Cierra el bloque anterior.
}*/

void Juego::gameLoop() {
    // Comprueba si no se esta jugando o no hay nivel
    if (estado != JUGANDO || !nivel) {
        // Reinicia el cronometro mientras no se juega
        relojFrame.restart();
        // Termina el procesamiento de la tecla
        return;
    }

    // Calcula el tiempo transcurrido en segundos
    qreal dt = relojFrame.restart() / 1000.0;
    // Limita el paso de tiempo entre 1 y 35 milisegundos
    dt = std::clamp(dt, 0.001, 0.035);

    // Avanza la simulacion de la escena.
    nivel->actualizarEscena(dt);
    // Ajusta la camara a la nueva posicion
    actualizarCamara();
    // Refresca los valores del HUD
    actualizarHUD();
}

void Juego::onPuntosGanados(int puntos, QString) {
    // Comprueba que exista el gestor de puntuacion.
    if (puntuacion) {
        // Suma los puntos recibidos.
        puntuacion->agregarPuntos(puntos);
    }
}

void Juego::onNivelCompletado(int nivelTerminado, int tiempoRestanteSeg) {
    // Limpia el estado de teclas del jugador.
    if (jugador) jugador->limpiarTeclas();

    // Toma las vidas del jugador, o cero si no existe.
    int vidas = jugador ? jugador->getVidas() : 0;
    // Calcula el bonus final con tiempo y vidas, o cero si no hay puntuacion
    int bonus = puntuacion ? puntuacion->calcularBonusFinalNivel(tiempoRestanteSeg, vidas) : 0;

    // Comprueba si se termino el primer nivel
    if (nivelTerminado == 1) {
        // Marca el estado de transicion
        estado = TRANSICION;
        // Recuerda que el siguiente nivel es el 2
        nivelPendiente = 2;
        mostrarOverlay(
            // Titulo del nivel 1 completado
            "¡HACIENDA LIBERADA! (NIVEL 1 COMPLETADO)",
            // Detalle con bonus, puntuacion y adelanto del nivel 2
            QString("¡Liberaste a tus hermanos y escapaste a tiempo!\n\n"
                    // Indica el bonus obtenido por tiempo y vida
                    "Bonus por tiempo y vidas: +%1 puntos\n"
                    // Indica la puntuacion acumulada
                    "Puntuación acumulada: %2\n\n"
                    // Anuncia el siguiente desafio
                    "Siguiente desafío — NIVEL 2 (Vista Cenital):\n"
                    // Advierte sobre la guardia española
                    "Enfréntate a la guardia española camino a Palenque.\n"
                    // Describe pendulos y cañones del nivel 2
                    "Esquiva péndulos con púas (movimiento oscilatorio), cañones con balas de rebote elástico\n"
                    // Describe al soldado rastreador inteligente
                    "y al Soldado Rastreador inteligente que aprende tu patrón de evasión.")
                // Sustituye %1 por el bonus calculado
                .arg(bonus)
                // Sustituye %2 por la puntuacion actual, o cero
                .arg(puntuacion ? puntuacion->getPuntosActuales() : 0),
            // Texto del boton para continuar al nivel 2.
            "Continuar al Nivel 2",
            // No se usa texto para un segundo boton
            QString(),
            // Muestra el boton de menu
            true);
    } else {
        // Marca el estado de victoria
        estado = VICTORIA;
        mostrarOverlay(
            // Titulo de la victoria en Palenque
            "¡VICTORIA! ¡PALENQUE ES LIBRE!",
            // Detalle con la liberacion, bonus y puntuaciones finales
            QString("¡Benkos Biohó rompió las cadenas y guió a su pueblo hacia la libertad!\n\n"
                    // Indica el bonus final del nivel
                    "Bonus final de nivel: +%1 puntos\n"
                    // Indica la puntuacion final
                    "PUNTUACIÓN FINAL: %2\n"
                    // Indica el record maximo
                    "RÉCORD MÁXIMO: %3")
                // Sustituye %1 por el bonus calculad
                .arg(bonus)
                // Sustituye %2 por la puntuacion actual, o cero
                .arg(puntuacion ? puntuacion->getPuntosActuales() : 0)
                // Sustituye %3 por el record maximo, o cero
                .arg(puntuacion ? puntuacion->getRecordMaximo() : 0),
            // Texto para empezar de nuevo desde el nivel 1
            "Jugar de Nuevo (Nivel 1)",
            // Texto para repetir el nivel 2
            "Repetir Nivel 2",
            // Muestra el boton de menu
            true);
    }
}

void Juego::onGameOverSolicitado(QString razon) {
    // Marca el estado de derrota
    estado = GAME_OVER;
    // Limpia el estado de teclas del jugador
    if (jugador) jugador->limpiarTeclas();
    // Toma el numero de nivel actual, o 1 si no hay nivel
    int nivelActual = nivel ? nivel->getNumeroNivel() : 1;
    // Recuerda el nivel actual para reintentarlo
    nivelPendiente = nivelActual;

    mostrarOverlay(
        // Titulo del panel de derrota
        "FIN DEL JUEGO",
        // Detalle con la razon y las puntuaciones obtenidas
        QString("%1\n\n"
                // Indica la puntuacion conseguida
                "Puntuación obtenida: %2\n"
                // Indica el record maximo
                "Récord máximo: %3")
            // Sustituye %1 por la razon de la derrota
            .arg(razon)
            // Sustituye %2 por la puntuacion actual, o cero
            .arg(puntuacion ? puntuacion->getPuntosActuales() : 0)
            // Sustituye %3 por el record maximo, o cero
            .arg(puntuacion ? puntuacion->getRecordMaximo() : 0),
        // Texto del boton para reintentar el nivel actual
        QString("Reintentar Nivel %1").arg(nivelActual),
        // Texto del boton para empezar desde el nivel 1
        "Reiniciar desde Nivel 1",
        // Muestra el boton de menu.
        true);
}

void Juego::onBotonPrincipal() {
    // Comprueba si el panel visible es el menu
    if (estado == MENU) {
        // Reinicia la puntuacion antes de empezar
        if (puntuacion) puntuacion->reiniciarPuntos();
        // Empieza la partida en el nivel 1
        cambiarNivel(1);
    // Comprueba si el panel visible es la pausa
    } else if (estado == PAUSA) {
        // Reanuda la partida
        pausarJuego();
    // Comprueba si el panel visible es la transicion
    } else if (estado == TRANSICION) {
        // Carga el nivel pendiente
        cambiarNivel(nivelPendiente);
    // Comprueba si el panel visible es la victoria
    } else if (estado == VICTORIA) {
        // Reinicia la puntuacion para una partida nueva
        if (puntuacion) puntuacion->reiniciarPuntos();
        // Empieza de nuevo en el nivel 1
        cambiarNivel(1);
    // Comprueba si el panel visible es la derrota
    } else if (estado == GAME_OVER) {
        // Reintenta el nivel pendiente
        cambiarNivel(nivelPendiente);
    }
}

void Juego::onBotonSecundario() {
    // Comprueba si el panel visible es el menu
    if (estado == MENU) {
        // Reinicia la puntuacion antes de empezar
        if (puntuacion) puntuacion->reiniciarPuntos();
        // Empieza directamente en el nivel 2
        cambiarNivel(2);
    // Comprueba si el panel visible es la pausa
    } else if (estado == PAUSA) {
        // Reinicia el nivel que estaba en curso
        cambiarNivel(nivel ? nivel->getNumeroNivel() : 1);
    // Comprueba si el panel visible es la victoria
    } else if (estado == VICTORIA) {
        cambiarNivel(2);
    // Comprueba si el panel visible es la derrota
    } else if (estado == GAME_OVER) {
        // Reinicia la puntuacion para empezar de cero
        if (puntuacion) puntuacion->reiniciarPuntos();
        // Empieza de nuevo en el nivel 1
        cambiarNivel(1);
    }
}

void Juego::onBotonMenu() {
    // Detiene el temporizador si existe
    if (gameTimer) gameTimer->stop();
    // Vuelve al menu inicial
    iniciarJuego();
}

void Juego::keyPressEvent(QKeyEvent* event) {
    // Detecta la tecla de pausa P o Escape.
    if (event->key() == Qt::Key_P || event->key() == Qt::Key_Escape) {
        // Solo actua si se ajugando o ya estaba en pausa
        if (estado == JUGANDO || estado == PAUSA) {
            // Alterna entre partida y pausa
            pausarJuego();
            // Sale sin pasar la tecla al jugador
            return;
        }
    }

    // Comprueba si el juego no esta en marcha
    if (estado != JUGANDO) {
        // Detecta Enter para activar el boton principal del panel
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            // Ejecuta la accion del boton principal
            onBotonPrincipal();
            return;
        }
        // Deja que la vista base procese la tecla
        QGraphicsView::keyPressEvent(event);
        // Devuelve el resultado de esta funcion.
        return;
    }

    // Comprueba que exista un jugador
    if (jugador) {
        // Pasa la tecla pulsada al jugador
        jugador->keyPressEvent(event);
    }
}

void Juego::keyReleaseEvent(QKeyEvent* event) {
    // Pasa la tecla al jugador solo si esta jugando y existe
    if (jugador && estado == JUGANDO) {
        // Notifica al jugador que la tecla se libero
        jugador->keyReleaseEvent(event);
    }
    // Deja que la vista base procese tambien la liberacion
    QGraphicsView::keyReleaseEvent(event);
}

void Juego::mouseMoveEvent(QMouseEvent* event) {
    // Actualiza el objetivo solo si esta jugando y hay jugador
    if (jugador && estado == JUGANDO) {
        // Convierte la posicion de pantalla a coordenadas de la escena
        jugador->setObjetivoMouse(mapToScene(event->pos()));
    }
    // Deja que la vista base procese el movimiento
    QGraphicsView::mouseMoveEvent(event);
}

void Juego::mousePressEvent(QMouseEvent* event) {
    // Actua solo si esta jugando y hay jugador.
    if (jugador && estado == JUGANDO) {
        // Actualiza el objetivo del jugador con la posicion del clic.
        jugador->setObjetivoMouse(mapToScene(event->pos()));
        // Detecta el boton izquierdo del raton.
        if (event->button() == Qt::LeftButton) {
            // Realiza un ataque cuerpo a cuerpo
            jugador->atacar();
            // Dispara el arma normal
            jugador->dispararArma(false);
        // Detecta el boton derecho del raton
        } else if (event->button() == Qt::RightButton) {
            // Dispara el arma con lanzamiento parabolico.
            jugador->dispararArma(true);
        }
    }
    // Deja que la vista base procese el clic.
    QGraphicsView::mousePressEvent(event);
}

void Juego::resizeEvent(QResizeEvent* event) {
    // Llama primero al manejo estandar de redimensionado.
    QGraphicsView::resizeEvent(event);
    // Al expandir la ventana, el area visible crece sola.
    // Solo recentramos la camara, NO reescalamos nada.
    // Recalcula la posicion de la camara.
    actualizarCamara();
}

void Juego::actualizarCamara() {
    // Sale si todavia no hay nivel.
    if (!nivel) return;
    auto* jugador = nivel->getJugador(); 
    // Sale si el nivel no tiene jugador.
    if (!jugador) return;

    // Obtiene el rectangulo que ocupa el jugador.
    QRectF r = jugador->boundingRect();
    // Calcula la coordenada horizontal del centro del jugador.
    qreal cx = jugador->x() + r.width() / 2.0;
    // Calcula la coordenada vertical del centro del jugador.
    qreal cy = jugador->y() + r.height() / 2.0;


    // Calcula el rectangulo de la escena que se ve en el viewport.
    QRectF visible = mapToScene(viewport()->rect()).boundingRect();
    // Calcula la mitad del ancho visible.
    qreal mitadAncho = visible.width() / 2.0;
    // Calcula la mitad del alto visible.
    qreal mitadAlto  = visible.height() / 2.0;

    // Continua la definicion de esta instruccion.
    qreal camX, camY;
    // Centra X en el mundo si este cabe por completo en la vista
    if (visible.width() >= anchoMundo) camX = anchoMundo / 2.0;
    // Si no cabe, limita X para no mostrar zonas fuera del mundo
    else camX = std::clamp(cx, mitadAncho, anchoMundo - mitadAncho);

    // Centra Y en el mundo si este cabe por completo en la vista
    if (visible.height() >= altoMundo) camY = altoMundo / 2.0;
    // Si no cabe, limita Y para no mostrar zonas fuera del mundo
    else camY = std::clamp(cy, mitadAlto, altoMundo - mitadAlto);

    // Mueve la camara al punto calculado.
    centerOn(camX, camY);
   if (puntuacion) {
        // Vuelve a calcular visible despues del centerOn
        // Recalcula el area visible tras centrar la camara
        QRectF v2 = mapToScene(viewport()->rect()).boundingRect();
        // Coloca la puntuacion en la esquina superior izquierda visible
        puntuacion->setPos(v2.left() + 12, v2.top() + 10);
        // Si tu overlay de pausa / game over es un QGraphicsRectItem,
        // haz lo mismo: overlay->setRect(v2); overlayTexto->setPos(...)
    }
}