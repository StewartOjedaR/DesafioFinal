#include <QApplication>
#include "juego.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Juego juego;
    juego.iniciarJuego();
    juego.show();
    return a.exec();
}