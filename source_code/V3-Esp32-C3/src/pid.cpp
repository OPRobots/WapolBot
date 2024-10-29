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

int tiempoSusto = 1000;
int millisSusto = 0;

bool prints = false;
bool speedActive = true;

int lastSeen = 0;

void pidSetup() { 
    millisPID = millis(); 
    velocidad = getVel();

    millisSusto = millis();
}

void enablePrintsPid() { prints = true; }

void enableSpeedPid() { speedActive = true; }

void dissableSpeedPid() { speedActive = false; }

void doPid() {
    if (millis() >= millisPID + tiempoPID) {
        posicion = proporcionalSimple();

        proporcional = posicion;
        derivada = posicion - posicion_anterior;
        correccion = ((kp * proporcional) + (kd * derivada));

        posicion_anterior = posicion;
        if (proporcional == 0 && sharpCentroCerca()) {
            velocidad += 2;
            set_led(RGB_TOP, 0, 0, 255);
        } else {
            velocidad = getVelBase();
        }
        if(speedActive){
            if(posicion == 10 || posicion == -10){
                motoresStop();
            }else {
                movimiento(posicion, correccion, limitSpeed(velocidad + correccion),
                    limitSpeed(velocidad - correccion));
            }
        }else{
            movimiento(posicion, correccion, limitSpeed(correccion),
                   limitSpeed(correccion * -1));
        }
        
        millisPID = millis();
    }
    
    if (millis() >= millisSusto + tiempoSusto) {
        if(posicion == 10 || posicion == -10){
            motoresSusto();
        }
        millisSusto = millis();
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