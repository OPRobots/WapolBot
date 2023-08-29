#include <Arduino.h>

#include "buttons.h"
#include "leds.h"
#include "menu.h"
#include "motors.h"
#include "qre1113.h"
#include "sharps.h"

/*
// runs once
void setup() {
  ledsSetup();
  motorsSetup();
  buttonsSetup();
}

// runs forever
void loop() {
  setVelD(getVel());
  setVelI(getVel());
  menu1();
}
*/

/*
#define Kp 1
#define Kd 1
#define velIzqBase 0
#define velDerBase 0
#define NUM_SENSORES  5

int errorAnterior = 0;
const int values[NUM_SENSORES] = {-1250, -250, 0, 250, 1250};
*/

#define velocidadMaxima 250

#define NUM_SENSORES 3
long lectura[NUM_SENSORES];
// Variables PID
float kp = 0.8; // 2
float kd = 1;   // 0
float ki = 0;
unsigned int sensores[NUM_SENSORES];
int posicion = 0;
int posicion_anterior = 0;
int proporcional = 0;
int derivada = 0;
int integral = 0;
int correccion = 0;
int velocidad = 50;

long millisPID = 0;
int tiempoPID = 10; // 40

bool prints = false;

int lastSeen = 0;

// runs once
void setup() {
  ledsSetup();
  motorsSetup();
  Serial.begin(9600);
  millisPID = millis();
}

// runs forever
void loop() {
  // setVelD(getVel());
  // setVelI(getVel());

  /* version con pesos ponderados
  *
  unsigned int sensores[NUM_SENSORES];
  int position = calcPos(sensores); // get calibrated readings along with the
  line position, refer to the QTR Sensors Arduino Library for more details on
  line position. int error = position; int motorSpeed = Kp * error + Kd * (error
  + errorAnterior); errorAnterior = error; int rightMotorSpeed =
  limitSpeed(velDerBase - motorSpeed); int leftMotorSpeed =
  limitSpeed(velIzqBase + motorSpeed); movimiento(position, leftMotorSpeed,
  rightMotorSpeed);
  *
  */

  if (millis() >= millisPID + tiempoPID) {
    posicion = proporcionalSimple();

    proporcional = posicion;
    derivada = posicion - posicion_anterior;
    // integral = integral + (posicion / 10);
    // integral= constrain(integral,-500,500);

    correccion = ((kp * proporcional) + (kd * derivada)); // + (ki * integral));

    posicion_anterior = posicion;

    movimiento(posicion, correccion, limitSpeed(velocidad + correccion),
               limitSpeed(velocidad - correccion));

    millisPID = millis();
  }
}

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
    return 50;
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
  }
}

int proporcionalSimple() {
  int c = sharpCentroCerca();
  int i = sharpIzquierdoCerca();
  int d = sharpDerechoCerca();

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
      return 150;
    } else {
      return -150;
    }
  } else if (i && c && d) {
    return 0;
  }
}

void movimiento(int pos, int correccion, int velI, int velD) {
  setVelI(abs(velI));
  setVelD(abs(velD));

  if (prints) {
    Serial.print(" Posicion: ");
    Serial.print(pos);
    Serial.print("\t Correccion: ");
    Serial.print(correccion);
    Serial.print("\t velocidades - Izq ");
  }
  if (velI > 0 && velD < 0) {
    if (prints) {
      Serial.print(velI);
      Serial.print("\t - Der ");
      Serial.print(velD);
    }
    motoresGiroDerechaCerrado();
  } else if (velI < 0 && velD > 0) {
    if (prints) {
      Serial.print(velI);
      Serial.print("\t - Der ");
      Serial.print(velD);
    }
    motoresGiroIzquierdaCerrado();
  } else {
    if (prints) {
      Serial.print(velI);
      Serial.print("\t - Der ");
      Serial.print(velD);
    }
    motoresAdelante();
  }
  if (prints) {
    Serial.println("");
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

/*
int calcPos(int snsrs[]){
  snsrs[0] = sharpIzquierdoDistancia();
  snsrs[2] = sharpCentroDistancia();
  snsrs[4] = sharpDerechoDistancia();
  if(snsrs[0]<34){
    snsrs[1] = (sharpIzquierdoDistancia()*2 + sharpCentroDistancia())/3;
  }else{
    snsrs[1] = 35;
  }
  if(snsrs[4]<34){
    snsrs[3] = (sharpCentroDistancia() + sharpDerechoDistancia()*2)/3;
  }else{
    snsrs[3] = 35;
  }

  Serial.print("sensores: ");
  Serial.print(snsrs[0]);
  Serial.print("\t - ");
  Serial.print(snsrs[1]);
  Serial.print("\t - ");
  Serial.print(snsrs[2]);
  Serial.print("\t - ");
  Serial.print(snsrs[3]);
  Serial.print("\t - ");
  Serial.print(snsrs[4]);
  Serial.print("\t -.-.-.- ");

  int val = (values[0]*snsrs[0] + values[1]*snsrs[1] + values[2]*snsrs[2] +
values[3]*snsrs[3] + values[4]*snsrs[4]) / (snsrs[0] + snsrs[1] + snsrs[2] +
snsrs[3] + snsrs[4] ); val = map(val, -850, 850, -100, 100); return val;
}




//sharpCentro;
//sharpIzquierda;
//sharpDerecha;

*/