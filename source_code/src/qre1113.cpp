#include "qre1113.h"

int qre1113Izquierdo(){
    return analogRead(pinQRE1113Izquierda);
}

int qre1113Derecho(){
    return analogRead(pinQRE1113Derecha);
}

int qre1113IzquierdoMapeado(){
    return map(analogRead(pinQRE1113Izquierda), 0, 1023, 0, 255); 
}

int qre1113DerechoMapeado(){
    return map(analogRead(pinQRE1113Derecha), 0, 1023, 0, 255); 
}


bool qre1113IzquierdoBlanco(){
    int valor = qre1113IzquierdoMapeado(); // cambiar para ring normal a true / false
    if(valor > 190){
        return true;
    }else{
        return false;
    }
}

bool qre1113DerechoBlanco(){
    int valor = qre1113DerechoMapeado(); // cambiar para ring normal a true / false
    if(valor > 190){
        return true;
    }else{
        return false;
    }
}

bool qre1113Blancos(){
    if(qre1113DerechoBlanco() && qre1113IzquierdoBlanco()){
        return true;
    }else{
        return false;
    }
}