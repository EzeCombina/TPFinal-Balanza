#include <Arduino.h>
//#include <LiquidCrystal.h>

#define TiempoMuestreoMS 9.09

const char pinADC = A5;       // PC5

//const float CoefEntrada[5] = {0.2194, 0.8778, 1.3167, 0.8778, 0.2194};
//const float CoefSalida[5] = {1, -1.1989, -0.9437, -0.3194, -0.049};

const float CoefEntrada[5] = {0.1237, 0.4949, 0.7423, 0.4949, 0.1237};
const float CoefSalida[5] = {1, -0.3566, -0.5262, -0.07653, -0.02015};

float ArrayEntrada[5] = {0, 0, 0, 0, 0};
float ArraySalida[5] = {0, 0, 0, 0, 0}; 

unsigned long TiempoMedido = 0;

void datosEntrada(){
  TiempoMedido = millis();
  ArrayEntrada[0] = float(analogRead(pinADC));
  Serial.print(ArrayEntrada[0]);
  Serial.print(" ");
}

void filtro(){
  float sumatoria = CoefEntrada[0]*ArrayEntrada[0];
  for(int i = 1; i < 5; i++){
    sumatoria += CoefEntrada[i] * ArrayEntrada[i] + CoefSalida[i] * ArraySalida[i];
  }
  ArraySalida[0] = sumatoria;
}

void corrimientoArray(){
  for(int i = 4; i > 0; i--){
    ArraySalida[i] = ArraySalida[i-1];
    ArrayEntrada[i] = ArrayEntrada[i-1];
  }
}

void pines(){
  pinMode(pinADC, INPUT);
}

void setup() {
  pines();
  //lcd.createChar(0, UTN);
  //lcd.clear();
  //inicializacionLCD();
  TiempoMedido = millis();
  Serial.begin(9600);
}

void loop() {
  if(millis() - TiempoMedido >= TiempoMuestreoMS){
    datosEntrada();
    filtro();
    Serial.println(ArraySalida[0]);
    corrimientoArray();
    //Serial.print("L"); 
  }
}
