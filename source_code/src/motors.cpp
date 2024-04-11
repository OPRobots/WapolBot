#include "motors.h"

int vel = 100;


int velocidad_base = 100;

int velD = 0;
int velI = 0;

void motorsSetup(){
  pinMode(MotorIalante, OUTPUT);
  pinMode(MotorIatras, OUTPUT);
  pinMode(MotorDalante, OUTPUT);
  pinMode(MotorDatras, OUTPUT);
}

void setVelD(int val){
    velD = val;
}

void setVelI(int val){
    velI = val;
}

void setVel(int val){
    vel = val;
}

int getVelD(){
    return velD;
}

int getVelI(){
    return velI;
}
int getVel(){
    return vel;
}


int getVelBase(){
    return velocidad_base;
}

void motorDerechoAdelante(){
    analogWrite(MotorDalante, 255);
    analogWrite(MotorDatras, 255 - velD);
}

void motorIzquierdoAdelante(){
    analogWrite(MotorIalante, 255);
    analogWrite(MotorIatras, 255 - velI);
}

void motorDerechoAtras(){
    analogWrite(MotorDatras, 255);
    analogWrite(MotorDalante, 255 - velD);
}

void motorIzquierdoAtras(){
    analogWrite(MotorIatras, 255);
    analogWrite(MotorIalante, 255 - velI);
}

void motorIzquierdoStop(){
    analogWrite(MotorIalante, 255);
    analogWrite(MotorIatras, 255);
}

void motorDerechoStop(){
    analogWrite(MotorDatras, 255);
    analogWrite(MotorDalante, 255);
}

void motoresAdelante(){
    motorDerechoAdelante();
    motorIzquierdoAdelante();
}

void motoresAtras(){
    motorDerechoAtras();
    motorIzquierdoAtras();
}

void motoresStop(){
    motorIzquierdoStop();
    motorDerechoStop();
}

void motoresGiroDerecha(){
    motorIzquierdoAdelante();
}

void motoresGiroIzquierda(){
    motorDerechoAdelante();
}

void motoresGiroDerechaCerrado(){
    motorDerechoAtras();
    motorIzquierdoAdelante();
}

void motoresGiroIzquierdaCerrado(){
    motorDerechoAdelante();
    motorIzquierdoAtras();
}

void motoresSusto(){
    motoresAdelante();
    delay(30);
    motoresStop();
}

void motoresGirar45Izquierda(){
    motoresGiroIzquierdaCerrado();
    delay(100);
    motoresStop();
}

void motoresGirar45Derecha(){
    motoresGiroDerechaCerrado();
    delay(100);
    motoresStop();
}

void motoresGirar90Izquierda(){
    motoresGiroIzquierdaCerrado();
    delay(150);
    motoresStop();
}

void motoresGirar90Derecha(){
    motoresGiroDerechaCerrado();
    delay(150);
    motoresStop();
}

void motoresGirar180(){
    motoresGiroIzquierdaCerrado();
    delay(275);
    motoresStop();
}