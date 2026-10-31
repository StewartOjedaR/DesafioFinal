#include <QApplication>
#include "juego.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Juego juego;
    juego.probarEscena();  // Prepara los elementos de la escena
    juego.iniciarJuego();  // Arranca el timer del game loop
    juego.show();          // Muestra la ventana principal

    return a.exec();
}