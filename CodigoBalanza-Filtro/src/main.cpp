#include <Arduino.h>
#include <LiquidCrystal.h>

#define TiempoMuestreoMS 9.09

const char pinRs = 8;         // PB0
const char pinRw = 9;         // PB1
const char pinE = 10;         // PB2
const char pinD0 = 4;         // PD4
const char pinD1 = 5;         // PD5
const char pinD2 = 6;         // PD6
const char pinD3 = 7;         // PD7
const char pinADC = A5;       // PC5

// Logo UTN

byte UTN[8] {

  B10101,
  B01110,
  B00100,
  B11111,
  B00100,
  B01110,
  B10101,
  B00000
  
};

//

//const float CoefEntrada[5] = {0.2194, 0.8778, 1.3167, 0.8778, 0.2194};
//const float CoefSalida[5] = {1, -1.1989, -0.9437, -0.3194, -0.049};

const float CoefEntrada[5] = {0.1237, 0.4949, 0.7423, 0.4949, 0.1237};
const float CoefSalida[5] = {1, -0.3566, -0.5262, -0.07653, -0.02015};

float ArrayEntrada[5] = {0, 0, 0, 0, 0};
float ArraySalida[5] = {0, 0, 0, 0, 0}; 

LiquidCrystal lcd(pinRs, pinRw, pinE, pinD0, pinD1, pinD2, pinD3);

unsigned long TiempoMedido = 0;

void calculopeso(float valorADC){
  float Peso = (valorADC) * 30 / 1024;

  lcd.setCursor(0,0);
  lcd.print("     ");
  lcd.print(Peso, 2);
  lcd.print(" Kg");
}

void datosEntrada(){
  TiempoMedido = millis();
  ArrayEntrada[0] = float(analogRead(pinADC));
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
  pinMode(pinRs, OUTPUT);
  pinMode(pinRw, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinD0, OUTPUT);
  pinMode(pinD1, OUTPUT);
  pinMode(pinD2, OUTPUT);
  pinMode(pinD3, OUTPUT);
}

void inicializacionLCD(){
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
  lcd.print("  UTN FR SFCO");
  lcd.write(byte(0));
}

void setup() {
  pines();
  lcd.createChar(0, UTN);
  //lcd.clear();
  inicializacionLCD();
  TiempoMedido = millis();
  //Serial.begin(9600);
}

void loop() {
  if(millis() - TiempoMedido >= TiempoMuestreoMS){
    datosEntrada();
    filtro();
    calculopeso(ArraySalida[0]);
    corrimientoArray();
    //Serial.print("L"); 
  }
}
