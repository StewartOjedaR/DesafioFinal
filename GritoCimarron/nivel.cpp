#include "nivel.h"

Nivel::Nivel(QWidget *parent)
{
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0,0,640,300);
    
    jugador = new Jugador(100, nullptr);
    addItem(jugador);
    
    for(int i=0; i<3; i++) {
        Enemigo* e = new Enemigo(50, i*150, i*150 + 150);
        enemigos.append(e);
        addItem(e);
    }
    
    Arma* a = new Arma("Pistola", 10, 6, true);
    armas.append(a);
    addItem(a);
    
    for(int i=0; i<2; i++) {
        Rehen* r = new Rehen(500, nullptr);
        rehenes.append(r);
        addItem(r);
    }
}

Nivel::~Nivel()
{
    delete jugador;
    qDeleteAll(enemigos);
    qDeleteAll(armas);
    qDeleteAll(rehenes);
}

QGraphicsScene* Nivel::getEscena()
{
    return escena;
}

void Nivel::actualizarEscena() {
    if (jugador) {
        jugador->actualizarPosicion();
        
        for (Enemigo* enemigo : enemigos) {
            if (enemigo) {
                QList<QGraphicsItem*> colisionando = jugador->collidingItems();
                if (colisionando.contains(enemigo)) {
                    jugador->recibirDano(20);
                }
            }
        }
        
        jugador->actualizarAnimacion();
    }
    
    for (Enemigo* enemigo : enemigos) {
        if (enemigo) {
            enemigo->actualizarPosicion();
            enemigo->actualizarAnimacion();
            
            if (enemigo->pos().x() > 600 || enemigo->pos().x() < 0) {
            }
        }
    }
    
    for (Arma* arma : armas) {
        if (arma) {
        }
    }
    
    for (Rehen* rehen : rehenes) {
        if (rehen) {
            rehen->actualizarAnimacion();
            if (rehen->isLiberado()) {
            }
        }
    }
}