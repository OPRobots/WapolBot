#include "estrategias.h"

#include "motors.h"
#include "pid.h"
#include "qre1113.h"
#include "sharps.h"
#include "leds.h"
#include "buttons.h"

bool near = false;

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

void estrategiaSharps(){
    while(true){
        if (buttonsLeftPressed()) {
            near = !near;
            delay(200);
        }
        if(near){
            ledsGreen(HIGH);
            if(sharpIzquierdoCerca()){
                ledsLeft(HIGH);
            }else{
                ledsLeft(LOW);
            }
            if(sharpCentroCerca()){
                ledsUp(HIGH);
            }else{
                ledsUp(LOW);
            }
            if(sharpDerechoCerca()){
                ledsRight(HIGH);
            }else{
                ledsRight(LOW);
            }
        }else{
            ledsGreen(LOW);
            if(sharpIzquierdoDetectado()){
                ledsLeft(HIGH);
            }else{
                ledsLeft(LOW);
            }
            if(sharpCentroDetectado()){
                ledsUp(HIGH);
            }else{
                ledsUp(LOW);
            }
            if(sharpDerechoDetectado()){
                ledsRight(HIGH);
            }else{
                ledsRight(LOW);
            }
        }
        Serial.print("Distancia: ");
        Serial.print(sharpIzquierdoDistancia());
        Serial.print(" - ");
        Serial.print(sharpCentroDistancia());
        Serial.print(" - ");
        Serial.print(sharpDerechoDistancia());
        Serial.println(" cm");
    }
}

void estrategiaQre1113(){
    while(true){
        if(qre1113IzquierdoBlanco()){
            ledsLeft(HIGH);
        }else{
            ledsLeft(LOW);
        }
        if(qre1113DerechoBlanco()){
            ledsRight(HIGH);
        }else{
            ledsRight(LOW);
        }
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