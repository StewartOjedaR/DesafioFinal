// Inicia la guarda HOJASPRITES_H contra inclusiones duplicadas.
#ifndef HOJASPRITES_H
// Define la guarda HOJASPRITES_H.
#define HOJASPRITES_H

// Incluye la cabecera QPixmap.
#include <QPixmap>
// Incluye la cabecera QString.
#include <QString>
// Incluye la cabecera QVector.
#include <QVector>
// Incluye la cabecera QColor.
#include <QColor>

// Carga una hoja de sprites conociendo:
// - ruta de la imagen
// - ancho y alto de cada sprite (en pixeles)
// - cantidad de columnas (sprites por fila) y filas (sprites por columna)
// Permite acceder rapido a cualquier sprite con obtenerSprite(fila, columna).
// Declara la clase HojaSprites.
class HojaSprites {
// Inicia la seccion private de la clase.
private:
    // Declara o inicializa una variable.
    QVector<QPixmap> spritesNormales;
    // Declara o inicializa una variable.
    QVector<QPixmap> spritesEspejo;
    // Declara o inicializa una variable.
    int anchoSprite = 0;
    // Declara o inicializa una variable.
    int altoSprite = 0;
    // Declara o inicializa una variable.
    int columnas = 1;
    // Declara o inicializa una variable.
    int filas = 1;
    // Declara o inicializa una variable.
    bool cargada = false;

// Inicia la seccion public de la clase.
public:
    // Declara o define la funcion HojaSprites.
    HojaSprites() = default;
    // Declara o define la funcion HojaSprites.
    HojaSprites(const QString& ruta, int ancho, int alto,
                // Continua la definicion de esta instruccion.
                int numColumnas, int numFilas, float escalaPorcentaje = 100.0f);

    // Declara o define la funcion cargar.
    bool cargar(const QString& ruta, int ancho, int alto,
                // Continua la definicion de esta instruccion.
                int numColumnas, int numFilas, float escalaPorcentaje = 100.0f,
                // Declara o define la funcion QColor.
                const QColor& colorTemporal = QColor(120, 120, 140));

    // Acceso rapido O(1) al sprite en (fila, columna)
    // Declara o define la funcion obtenerSprite.
    QPixmap obtenerSprite(int fila, int columna, bool voltearHorizontal = false) const;

    // Declara o define la funcion estaCargada.
    bool estaCargada() const { return cargada; }
    // Declara o define la funcion tieneSprites.
    bool tieneSprites() const { return !spritesNormales.isEmpty(); }
    // Declara o define la funcion getColumnas.
    int getColumnas() const { return columnas; }
    // Declara o define la funcion getFilas.
    int getFilas() const { return filas; }
    // Declara o define la funcion getTotalSprites.
    int getTotalSprites() const { return spritesNormales.size(); }
// Cierra el bloque anterior.
};

// HOJASPRITES_H
#endif 
