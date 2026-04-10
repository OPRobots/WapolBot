#include "pid.h"

// Variables PID
float kp = 0.8; //0.8; //0.4!
float kd = 1; //1;
// float ki = 0;

int posicion = 0;
int posicion_anterior = 0;

int proporcional = 0;
int derivada = 0;
// int integral = 0;

int correccion = 0;
int velocidad = 60;

unsigned long millisPID = 0;
int tiempoPID = 10;

bool prints = false;
bool speedActive = true;

int lastSeen = 0;

unsigned long tiempoDeteccion = 0;
unsigned long ultimoIncremento = 0;
bool rivalDetectado = false;
int incremento = 2;
unsigned long retardo = 300;
unsigned long pasoTiempo = 100;


unsigned long rivalLostMs = 0;
int scanDirection = 1; // 1=derecha, -1=izquierda
const unsigned long SCAN_DELAY_MS = 250;
const int SCAN_VEL = 35;

void pidSetup() { 
    millisPID = millis(); 
    velocidad = getVel();
}

void enablePrintsPid() { prints = true; }

void enableSpeedPid() { speedActive = true; }

void dissableSpeedPid() { speedActive = false; }

void doPid() {
    if (motoresManiobra_isActive()) {
        millisPID = millis();
        return;
    }
    if (millis() >= millisPID + tiempoPID) {
        posicion = proporcionalSimple();

        proporcional = posicion;
        derivada = posicion - posicion_anterior;
        correccion = ((kp * proporcional) + (kd * derivada));

        posicion_anterior = posicion;
        
        if (proporcional == 0 && sharpCentroCerca()) {
            if (!rivalDetectado) {
                rivalDetectado = true;
                tiempoDeteccion = millis();
                setVel(getVelBase());  
            }

            if (millis() - tiempoDeteccion > retardo) {
                if (millis() - ultimoIncremento > pasoTiempo) {
                    setVel(getVel() + incremento);
                    ultimoIncremento = millis();
                }
            }

            set_led(RGB_TOP, 0, 0, 255);

        } else {
            rivalDetectado = false;
            setVel(getVelBase()); 
        }

        if(speedActive){
            if(posicion == 10 || posicion == -10){
                // Congelar dirección de giro al perder rival
                if (rivalLostMs == 0) {
                    rivalLostMs = millis();
                    scanDirection = (lastSeen >= 0) ? 1 : -1;
                    motoresStop();
                } else if (millis() - rivalLostMs >= SCAN_DELAY_MS) {
                    setVelD(SCAN_VEL);
                    setVelI(SCAN_VEL);
                    if (scanDirection == 1) {
                        motoresGiroDerechaCerrado();
                    } else {
                        motoresGiroIzquierdaCerrado();
                    }
                } else {
                    motoresStop();
                }
            }else {
                rivalLostMs = 0;
                int vel = getVel();
                if (!rivalDetectado && proporcional == 0) {
                    vel = getVelBase() - 20; 
                }
                movimiento(posicion, correccion, limitSpeed(vel - correccion),
                    limitSpeed(vel + correccion));
            }
        }else{
            movimiento(posicion, correccion, limitSpeed(correccion * -1),
                   limitSpeed(correccion));
        }
        
        millisPID = millis();
    }
}
/*
int proporcionalPesos() {
    int c = sharpCentroDistancia();
    int i = sharpIzquierdoDistancia();
    int d = sharpDerechoDistancia();
    int margin = 15;

    if ((i < margin) && !(c < margin) && !(d < margin)) {
        lastSeen = -1;
        return -((35 - i) * 50 / 35) - 50;
    } else if ((i < margin) && (c < margin) && !(d < margin)) {
        return -(70 - i - c) * 50 / 70;
    } else if (!(i < margin) && (c < margin) && !(d < margin)) {
        return 0;
    } else if (!(i < margin) && (c < margin) && (d < margin)) {
        return (70 - c - d) * 50 / 70;
    } else if (!(i < margin) && !(c < margin) && (d < margin)) {
        lastSeen = 1;
        return ((35 - d) * 50 / 35) + 50;
    }

    else if (!(i < margin) && !(c < margin) && !(d < margin)) {
        if (lastSeen == 1) {
            return 100;
        } else {
            return -100;
        }
    } else if ((i < margin) && (c < margin) && (d < margin)) {
        return 0;
    } else {
        return 0;
    }
}
*/
int proporcionalSimple() {
    int c = sharpCentroCerca();
    int i = sharpIzquierdoCerca();
    int d = sharpDerechoCerca();

    if(i){
        ledsLeft(HIGH);
    }else{
        ledsLeft(LOW);
    }
    if(c){
        set_led(RGB_TOP, 255, 0, 0);
    }else{
        ledsUp(LOW);
    }
    if(d){
        ledsRight(HIGH);
    }else{
        ledsRight(LOW);
    }
    if (i && !c && !d) {
        lastSeen = -1;
        return -100;
    } else if (i && c && !d) {
        lastSeen = -1;
        return -50;
    } else if (!i && c && !d) {
        return 0;
    } else if (!i && c && d) {
        lastSeen = 1;
        return 50;
    } else if (!i && !c && d) {
        lastSeen = 1;
        return 100;
    }

    else if ((!i && !c && !d)) {
        if (lastSeen == 1) {
            return 10;
        } else {
            return -10;
        }
        return 5;
    } else if (i && c && d) {
        return 5;
    } else {
        return 5;
    }
}

void movimiento(int pos, int correccion, int velI, int velD) {
    
    setVelI(abs(velI));
    setVelD(abs(velD));
   /* if (prints) {
        Serial.print(" Posicion: ");
        Serial.print(pos);
        Serial.print("\t Correccion: ");
        Serial.print(correccion);
        Serial.print("\t velocidades - Izq ");
        Serial.print(velI);
        Serial.print("\t - Der ");
        Serial.println(velD);
    }*/
    if (velI > 0 && velD < 0) {
        motoresGiroDerechaCerrado();
    } else if (velI < 0 && velD > 0) {
        motoresGiroIzquierdaCerrado();
    } else {
        if (speedActive) {
            motoresAdelante();
        } else {
            motoresStop();
        }
    }
}

int limitSpeed(int speed) {
    if (speed > velocidadMaxima) {
        return velocidadMaxima;
    } else if (speed < velocidadMaxima * -1) {
        return velocidadMaxima * -1;
    } else {
        return speed;
    }
}