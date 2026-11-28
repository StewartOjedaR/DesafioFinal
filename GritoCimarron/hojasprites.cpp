// Incluye la cabecera hojasprites.h.
#include "hojasprites.h"
// Incluye la cabecera QTransform.
#include <QTransform>
// Incluye la cabecera QDebug.
#include <QDebug>
// Incluye la cabecera algorithm.
#include <algorithm>

// Continua la definicion de esta instruccion.
HojaSprites::HojaSprites(const QString& ruta, int ancho, int alto,
                         // Continua la definicion de esta instruccion.
                         int numColumnas, int numFilas, float escalaPorcentaje) {
    // Declara o define la funcion cargar.
    cargar(ruta, ancho, alto, numColumnas, numFilas, escalaPorcentaje);
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
bool HojaSprites::cargar(const QString& ruta, int ancho, int alto,
                         // Continua la definicion de esta instruccion.
                         int numColumnas, int numFilas, float escalaPorcentaje,
                         // Continua la definicion de esta instruccion.
                         const QColor& colorTemporal) {
    // Continua la definicion de esta instruccion.
    spritesNormales.clear();
    // Continua la definicion de esta instruccion.
    spritesEspejo.clear();

    // Asigna un valor a la variable indicada.
    anchoSprite = std::max(1, ancho);
    // Asigna un valor a la variable indicada.
    altoSprite = std::max(1, alto);
    // Asigna un valor a la variable indicada.
    columnas = std::max(1, numColumnas);
    // Asigna un valor a la variable indicada.
    filas = std::max(1, numFilas);

    // Declara o inicializa una variable.
    float factor = std::max(1.0f, escalaPorcentaje) / 100.0f;
    // Declara o inicializa una variable.
    int anchoFinal = std::max(1, static_cast<int>(anchoSprite * factor));
    // Declara o inicializa una variable.
    int altoFinal = std::max(1, static_cast<int>(altoSprite * factor));

    // Declara o define la funcion imagen.
    QPixmap imagen(ruta);
    // Comprueba una condicion antes de continuar.
    if (imagen.isNull()) {
        // Si aun no existe el archivo de sprites, crea un cuadro temporal del tamaño esperado
        // Declara o define la funcion qDebug.
        qDebug() << "Aviso: falta la hoja de sprites" << ruta;
        // Asigna un valor a la variable indicada.
        cargada = false;
        // Declara o define la funcion cuadro.
        QPixmap cuadro(anchoFinal, altoFinal);
        // Continua la definicion de esta instruccion.
        cuadro.fill(colorTemporal);
        // Continua la definicion de esta instruccion.
        spritesNormales.fill(cuadro, filas * columnas);
        // Continua la definicion de esta instruccion.
        spritesEspejo.fill(cuadro, filas * columnas);
        // Devuelve el resultado de esta funcion.
        return false;
    // Cierra el bloque anterior.
    }

    // Declara o inicializa una variable.
    int colsReales = std::max(1, imagen.width() / anchoSprite);
    // Declara o inicializa una variable.
    int filasReales = std::max(1, imagen.height() / altoSprite);
    // Asigna un valor a la variable indicada.
    columnas = std::min(columnas, colsReales);
    // Asigna un valor a la variable indicada.
    filas = std::min(filas, filasReales);

    // Declara o inicializa una variable.
    QTransform espejo;
    // Continua la definicion de esta instruccion.
    espejo.scale(-1.0, 1.0);

    // Continua la definicion de esta instruccion.
    spritesNormales.reserve(filas * columnas);
    // Continua la definicion de esta instruccion.
    spritesEspejo.reserve(filas * columnas);

    // Recortar cada sprite (fila, columna) una sola vez al cargar
    // Recorre los elementos indicados.
    for (int f = 0; f < filas; ++f) {
        // Recorre los elementos indicados.
        for (int c = 0; c < columnas; ++c) {
            // Declara o inicializa una variable.
            QPixmap recorte = imagen.copy(c * anchoSprite, f * altoSprite,
                                          // Continua la definicion de esta instruccion.
                                          anchoSprite, altoSprite);
            // Declara o inicializa una variable.
            QPixmap escalado = recorte.scaled(anchoFinal, altoFinal,
                                              // Continua la definicion de esta instruccion.
                                              Qt::IgnoreAspectRatio,
                                              // Continua la definicion de esta instruccion.
                                              Qt::SmoothTransformation);
            // Continua la definicion de esta instruccion.
            spritesNormales.append(escalado);
            // Continua la definicion de esta instruccion.
            spritesEspejo.append(escalado.transformed(espejo));
        // Cierra el bloque anterior.
        }
    // Cierra el bloque anterior.
    }

    // Asigna un valor a la variable indicada.
    cargada = true;
    // Devuelve el resultado de esta funcion.
    return true;
// Cierra el bloque anterior.
}

// Continua la definicion de esta instruccion.
QPixmap HojaSprites::obtenerSprite(int fila, int columna, bool voltearHorizontal) const {
    // Comprueba una condicion antes de continuar.
    if (spritesNormales.isEmpty()) return QPixmap();

    // Declara o inicializa una variable.
    int f = std::clamp(fila, 0, filas - 1);
    // Declara o inicializa una variable.
    int c = std::clamp(columna, 0, columnas - 1);
    // Declara o inicializa una variable.
    int indice = f * columnas + c;

    // Devuelve el resultado de esta funcion.
    return voltearHorizontal ? spritesEspejo[indice] : spritesNormales[indice];
// Cierra el bloque anterior.
}
