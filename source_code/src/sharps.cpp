#include "sharps.h"

/* Model :
  GP2Y-0A02-YK0F --> 20150
  GP2Y-0A21-YK0F --> 1080
  GP2Y-0A71-0K0F --> 100500
  GP2Y-A41-SK0F ---> 430
*/
SharpIR sharpCentro = SharpIR(pinSharpCentro, 430);
SharpIR sharpIzquierdo = SharpIR(pinSharpIzquierda, 430);
SharpIR sharpDerecho = SharpIR(pinSharpDerecha, 430);

int sharpCentroDistancia(){
    return limitedDistance(sharpCentro.distance());
}

int sharpIzquierdoDistancia(){
    return limitedDistance(sharpIzquierdo.distance());
}

int sharpDerechoDistancia(){
    return limitedDistance(sharpDerecho.distance());
}

int limitedDistance(int val){
    if(val >= 35){
        return 35;
    }
    return val;
}

bool sharpCentroDetectado(){
    int distancia = sharpCentroDistancia();
    if((distancia < 35) && (distancia >= 20)){
        return true;
    }else{
        return false;
    }
}

bool sharpIzquierdoDetectado(){
    int distancia = sharpIzquierdoDistancia();
    if((distancia < 35) && (distancia >= 20)){
        return true;
    }else{
        return false;
    }
}

bool sharpDerechoDetectado(){
    int distancia = sharpDerechoDistancia();
    if((distancia < 35) && (distancia >= 20)){
        return true;
    }else{
        return false;
    }
}

bool sharpCentroCerca(){
    int distancia = sharpCentroDistancia();
    if((distancia < 20) && (distancia >= 0)){
        return true;
    }else{
        return false;
    }
}

bool sharpIzquierdoCerca(){
    int distancia = sharpIzquierdoDistancia();
    if((distancia < 20) && (distancia >= 0)){
        return true;
    }else{
        return false;
    }
}

bool sharpDerechoCerca(){
    int distancia = sharpDerechoDistancia();
    if((distancia < 20) && (distancia >= 0)){
        return true;
    }else{
        return false;
    }
}
