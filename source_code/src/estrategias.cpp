#include "estrategias.h"

#include "motors.h"
#include "pid.h"
#include "qre1113.h"
#include "sharps.h"

void estrategiaSetup() {
    setVelD(getVel());
    setVelI(getVel());
    motoresStop();
    delay(3000);
}

void estrategiaBase() {
    if (qre1113IzquierdoBlanco() || qre1113DerechoBlanco()) {
        motoresStop();
        if (qre1113Blancos()) {
            motoresAtras();
            delay(100);
            motoresGirar180();
        } else if (qre1113IzquierdoBlanco()) {
            motoresGirar90Derecha();
        } else if (qre1113DerechoBlanco()) {
            motoresGirar90Izquierda();
        }
    } else {
        doPid();
    }
}

void estrategiaMirarAtras() {
    motoresGirar180();
}

void estrategiaMirarLadoDerecha() {
    motoresGirar90Derecha();
}

void estrategiaMirarLadoIzquierda() {
    motoresGirar90Izquierda();
}

void estrategiaCaja() {
    motoresAdelante();
    delay(1500);
}

void estrategiaRadar(){
    dissableSpeedPid();
    while(true){
        doPid();
    }
}

/*
  vel++;
  if(getVel() > 255){
    setVel(0);
  }
  //delay(1000);
  setVelD(getVel());
  setVelI(getVel());
  if (getVel() >= 0) {
    //motoresGiroDerecha();
  }
  //motoresGiroIzquierdaCerrado();
  //motoresStop();
*/