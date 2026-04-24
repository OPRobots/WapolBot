#include "sensors.h"

const int MAGNITUD_FILTRO = 100;

int s_rival_1 = 0;
int s_rival_2 = 0;
int s_rival_3 = 0;

int Filtro_s1[MAGNITUD_FILTRO] = { 0 };
int Filtro_s2[MAGNITUD_FILTRO] = { 0 };
int Filtro_s3[MAGNITUD_FILTRO] = { 0 };

int filtro_oldvalues[3];
int filtro_suma[3];

int i_s;

void filtro_sensores2() {
  filtro_oldvalues[0] = Filtro_s1[i_s];
  filtro_oldvalues[1] = Filtro_s2[i_s];
  filtro_oldvalues[2] = Filtro_s3[i_s];

  filtro_suma[0] -= filtro_oldvalues[0];
  filtro_suma[1] -= filtro_oldvalues[1];
  filtro_suma[2] -= filtro_oldvalues[2];

  Filtro_s1[i_s] = analogRead(S_RIVAL_I);
  Filtro_s2[i_s] = analogRead(S_RIVAL_C);
  Filtro_s3[i_s] = analogRead(S_RIVAL_D);

  filtro_suma[0] += Filtro_s1[i_s];
  filtro_suma[1] += Filtro_s2[i_s];
  filtro_suma[2] += Filtro_s3[i_s];

  i_s = (i_s + 1) % MAGNITUD_FILTRO; // Avanza el índice circularmente cuando supera MAGNITUD FILTRO vuelve a ser 0

  s_rival_1 = filtro_suma[0] / MAGNITUD_FILTRO;
  s_rival_2 = filtro_suma[1] / MAGNITUD_FILTRO;
  s_rival_3 = filtro_suma[2] / MAGNITUD_FILTRO;
}


int sensor1_analog() {
  return s_rival_1;
}
int sensor2_analog() {
  return s_rival_2;
}
int sensor3_analog() {
  return s_rival_3;
}

void print_sensores_1() {
  static unsigned long last_print1 = 0;
  if (millis() - last_print1 < 500) return;
  last_print1 = millis();
  if(analogRead(S_RIVAL_I) == 0){
    Serial.print(" ");
    Serial.println("no va");
  }else{
    Serial.print("LinIzq:");
    Serial.print(digitalRead(S_LINEA_I));
    Serial.print("\n");
    Serial.print("FrontIzq:");
    Serial.print(analogRead(S_RIVAL_I));
    Serial.print("\n");
    Serial.print("FrontCen:");
    Serial.print(analogRead(S_RIVAL_C));
    Serial.print("\n");
    Serial.print("FrontDer:");
    Serial.print(analogRead(S_RIVAL_D));
    Serial.print("\n");
    Serial.print("LinDer:");
    Serial.print(digitalRead(S_LINEA_D));
    Serial.print("\n");

  }
}


void print_sensores_2() {
  static unsigned long last_print2 = 0;
  if (millis() - last_print2 < 500) return;
  last_print2 = millis();
  if(analogRead(S_RIVAL_I) == 0){
    Serial.print(" ");
    Serial.println("no va");
  }else{
    Serial.print("Sensores: ");
    Serial.print(digitalRead(S_LINEA_I));
    Serial.print(" ");
    Serial.print(sharpIzquierdoCerca());
    Serial.print(" ");
    Serial.print(sharpCentroCerca());
    Serial.print(" ");
    Serial.print(sharpDerechoCerca());
    Serial.print(" ");
    Serial.println(digitalRead(S_LINEA_D));

  }
}




const int MAGNITUD_FILTRO_SUELO = 10;

int lecturas[2][MAGNITUD_FILTRO_SUELO] = {0};
int indice[2] = {0};
int total[2] = {0};



bool filtrarSensoresSueloI(){
    total[0] -= lecturas[0][indice[0]];
    lecturas[0][indice[0]] = digitalRead(S_LINEA_I);
    total[0] += digitalRead(S_LINEA_I);
    indice[0] = (indice[0] + 1) % MAGNITUD_FILTRO_SUELO;
    double promedio = total[0] / (double)MAGNITUD_FILTRO_SUELO;
    return (promedio > 0.7) ? true : false;
}

bool filtrarSensoresSueloD(){
    total[1] -= lecturas[1][indice[1]];
    lecturas[1][indice[1]] = digitalRead(S_LINEA_D);
    total[1] += digitalRead(S_LINEA_D);
    indice[1] = (indice[1] + 1) % MAGNITUD_FILTRO_SUELO;
    double promedio2 = total[1] / (double)MAGNITUD_FILTRO_SUELO;
    return (promedio2 > 0.7) ? true : false;
}







bool qre1113IzquierdoBlanco(){
    return filtrarSensoresSueloI();
}

bool qre1113DerechoBlanco(){
    return filtrarSensoresSueloD();
}

bool qre1113Blancos(){
    if(qre1113IzquierdoBlanco() && qre1113DerechoBlanco()){
        return true;
    }else{
        return false;
    }
}









float voltageToDistanceIC(int sensorValue) {
  float voltage = sensorValue * (3.3 / SENSORS_MAX);
  if (voltage < 0.4) return 30;  // Si el voltaje es muy bajo, la distancia está fuera del rango
  if (voltage > 2.6) return 4;   // Si el voltaje es muy alto, la distancia está fuera del rango
  return 10 / (voltage - 0.11);
}

float voltageToDistanceD(int sensorValue) { // el rarito
  float voltage = sensorValue * (3.3 / SENSORS_MAX);
  if (voltage < 0.45) return 30;  // Si el voltaje es muy bajo, la distancia está fuera del rango
  if (voltage > 2.6) return 4;   // Si el voltaje es muy alto, la distancia está fuera del rango
  return 11 / (voltage - 0.1);
}





int sharpCentroDistancia(){
    return voltageToDistanceIC(sensor2_analog());
}

int sharpIzquierdoDistancia(){
    return voltageToDistanceIC(sensor1_analog());
}

int sharpDerechoDistancia(){
    return voltageToDistanceD(sensor3_analog());
}

int limitedDistance(int val){
    if(val >= 35){
        return 35;
    }
    return val;
}

bool sharpCentroDetectado(){
    int distancia = sharpCentroDistancia();
    if((distancia < 29) && (distancia >= 17)){
        return true;
    }else{
        return false;
    }
}

bool sharpIzquierdoDetectado(){
    int distancia = sharpIzquierdoDistancia();
    if((distancia < 29) && (distancia >= 17)){
        return true;
    }else{
        return false;
    }
}

bool sharpDerechoDetectado(){
    int distancia = sharpDerechoDistancia();
    if((distancia < 29) && (distancia >= 17)){
        return true;
    }else{
        return false;
    }
}



//TODO ver cerca 

bool sharpCentroCerca(){
    int distancia = sharpCentroDistancia();
    if((distancia < 17) && (distancia >= 0)){
        return true;
    }else{
        return false;
    }
}

bool sharpIzquierdoCerca(){
    int distancia = sharpIzquierdoDistancia();
    if((distancia < 17) && (distancia >= 0)){
        return true;
    }else{
        return false;
    }
}

bool sharpDerechoCerca(){
    int distancia = sharpDerechoDistancia();
    if((distancia < 17) && (distancia >= 0)){
        return true;
    }else{
        return false;
    }
}