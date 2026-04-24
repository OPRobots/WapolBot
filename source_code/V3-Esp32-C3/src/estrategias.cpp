#include "estrategias.h"

#include "motors.h"
#include "pid.h"
#include "sensors.h"
#include "leds.h"
#include "buttons.h"

bool near = false;

enum BaseState { BASE_COMBAT, BASE_RETRO, BASE_GIRO };
static BaseState base_state = BASE_COMBAT;
static unsigned long base_retro_ms = 0;

void estrategiaSetup() {
    setVelD(getVel());
    setVelI(getVel());
    motoresStop();
}

void estrategiaBase() {
    if (!is_starting()) {
        motoresStop();
        base_state = BASE_COMBAT;
        return;
    }

    switch (base_state) {
        case BASE_COMBAT:
            if (motoresManiobra_isActive()) return;
            if (qre1113IzquierdoBlanco() || qre1113DerechoBlanco()) {
                motoresStop();
                velocidad = getVelBase();
                if (qre1113Blancos()) {
                    motoresAtras();
                    base_retro_ms = millis();
                    base_state = BASE_RETRO;
                } else if (qre1113IzquierdoBlanco()) {
                    motoresGirar45Izquierda();
                    base_state = BASE_GIRO;
                } else if (qre1113DerechoBlanco()) {
                    motoresGirar45Derecha();
                    base_state = BASE_GIRO;
                }
            } else {
                filtro_sensores2();
                //dissableSpeedPid(); //esto sobra pero si no, se sale, toca buscar mejores motores xD
                doPid();
            }
            break;

        case BASE_RETRO:
            if (millis() - base_retro_ms >= 80) {
                motoresGirar180();
                base_state = BASE_GIRO;
            }
            break;

        case BASE_GIRO:
            if (!motoresManiobra_isActive()) {
                base_state = BASE_COMBAT;
            }
            break;
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
    motoresManiobra_begin(300);
}

void estrategiaRadar(){
    dissableSpeedPid();
    while(true){
        filtro_sensores2();
        doPid();
    }
}

void estrategiaSharps(){
    clear_leds();
    while(true){
        filtro_sensores2();
        BTN_STATES btn_state = get_btn_pressed_state();
        if (btn_state == BTN_PRESSED) {
            near = !near;
        }
        if(near){
            //ledsGreen(HIGH);
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
            //ledsGreen(LOW);
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
    clear_leds();
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

void estrategiaMotores(){
    while(true){
        BTN_STATES btn_state = get_btn_pressed_state();
        if (btn_state == BTN_PRESSED) {
            near = !near;
        }
        if(near){
            if(qre1113IzquierdoBlanco()){
                ledsLeft(HIGH);
                motorIzquierdoAdelante();
            }else{
                ledsLeft(LOW);
                motorIzquierdoStop();
            }
            if(qre1113DerechoBlanco()){
                ledsRight(HIGH);
                motorDerechoAdelante();
            }else{
                ledsRight(LOW);
                motorDerechoStop();
            }
        }else{
            if(qre1113IzquierdoBlanco()){
                ledsLeft(HIGH); 
                motorIzquierdoAtras();
            }else{
                ledsLeft(LOW);
                motorIzquierdoStop();
            }
            if(qre1113DerechoBlanco()){
                ledsRight(HIGH);
                motorDerechoAtras();
            }else{
                ledsRight(LOW);
                motorDerechoStop();
            }
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