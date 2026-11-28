// Incluye la cabecera QApplication.
#include <QApplication>
// Incluye la cabecera juego.h.
#include "juego.h"

// Declara o define la funcion main.
int main(int argc, char *argv[]) {
    // Declara o define la funcion a.
    QApplication a(argc, argv);
    // Creo la instancia de la ventana Juego
    Juego juego;
    // Prepara el menu y el nivel inicial del juego
    juego.iniciarJuego();
    // Muestra la ventana en pantalla
    juego.show();
    // Entra en el bucle de eventos y devuelve su resultado al terminar
    return a.exec();
// Cierra el bloque anterior.
}
