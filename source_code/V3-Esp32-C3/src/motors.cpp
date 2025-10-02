#include "motors.h"

/**
 * @brief Inicializa el timer controlador de los motores
 *
 */
void init_motors() {
  // Configuración de los canales PWM del Timer
  ledcSetup(PWM_MOTOR_RIGHT_A, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);
  ledcSetup(PWM_MOTOR_RIGHT_B, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);
  ledcSetup(PWM_MOTOR_LEFT_A, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);
  ledcSetup(PWM_MOTOR_LEFT_B, PWM_MOTORS_HZ, PWM_MOTORS_RESOLUTION);

  // Asignación de los pines a los canales PWM
  ledcAttachPin(MOTOR_RIGHT_A, PWM_MOTOR_RIGHT_B);
  ledcAttachPin(MOTOR_RIGHT_B, PWM_MOTOR_RIGHT_A);
  ledcAttachPin(MOTOR_LEFT_A, PWM_MOTOR_LEFT_B);
  ledcAttachPin(MOTOR_LEFT_B, PWM_MOTOR_LEFT_A);

  // Establece el valor inicial de los canales PWM
  ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MIN);
  ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MIN);

}

int vel = 60;//60
int velocidad_base = 60;//60

int velD = 0;
int velI = 0;

void setVelD(int val){
    velD = val;
}

void setVelI(int val){
    velI = val;
}

void setVels(int val){
    velD = val;
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

void motorIzquierdoAdelante(){
    ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MAX);
    ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * velD / 100));
    //Serial.print("velD: ");
    //Serial.println(velD);
}

void motorDerechoAdelante(){
    ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * velI / 100));
    ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MAX);
    //Serial.print("velI: ");
    //Serial.println(velI);
}

void motorIzquierdoAtras(){
    ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * velD / 100));
    ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MAX);
    //Serial.print("-velD: ");
    //Serial.println(velD);
}

void motorDerechoAtras(){
    ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MAX);
    ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MAX - (PWM_MOTORS_MAX * velI / 100));
    //Serial.print("-velI: ");
    //Serial.println(velI);
}

void motorIzquierdoStop(){
    ledcWrite(PWM_MOTOR_LEFT_A, PWM_MOTORS_MIN);
    ledcWrite(PWM_MOTOR_LEFT_B, PWM_MOTORS_MIN);
}

void motorDerechoStop(){
    ledcWrite(PWM_MOTOR_RIGHT_A, PWM_MOTORS_MIN);
    ledcWrite(PWM_MOTOR_RIGHT_B, PWM_MOTORS_MIN);
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
    delay(85);
    motoresStop();
}

void motoresGirar45Derecha(){
    motoresGiroDerechaCerrado();
    delay(80);
    motoresStop();
}

void motoresGirar90Izquierda(){
    motoresGiroIzquierdaCerrado();
    delay(140);
    motoresStop();
}

void motoresGirar90Derecha(){
    motoresGiroDerechaCerrado();
    delay(140);
    motoresStop();
}

void motoresGirar180(){
    motoresGiroIzquierdaCerrado();
    delay(260);
    motoresStop();
}