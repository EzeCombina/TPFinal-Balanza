#include <Arduino.h>
#include <LiquidCrystal.h>

#define TiempoMuestreoMS 9.09

const char pinRs = 8;         // PB0
const char pinRw = 9;         // PB1
const char pinE = 10;         // PB2
const char pinD4 = 4;         // PD4
const char pinD5 = 5;         // PD5
const char pinD6 = 6;         // PD6
const char pinD7 = 7;         // PD7
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

float Peso = 0.0;
float valorAntiguo = 0.0;

LiquidCrystal lcd(pinRs, pinRw, pinE, pinD4, pinD5, pinD6, pinD7);

unsigned long TiempoMedido = 0;

void calculopeso(float valorADC){

  //if((valorADC < valorAntiguo + 0.5) && (valorADC > valorAntiguo - 0.5))
  //{
  //  valorADC = valorAntiguo;
  //}

  // --- Cálculos para evitar errores en la medición --- Habria que realizar una función de linealización //

  valorADC = valorADC - 9.5;

  if(valorADC <= 0.1){

    Peso = 0.0;

  }else if(valorADC < 30)
  {
    
    Peso = (valorADC + 2) * 30 / 1023;
  
  }else if(valorADC > 100)
  {
    
    Peso = (valorADC + 10) * 30 / 1023;
    
  }else{Peso = (valorADC + 5) * 30 / 1023;}

  //valorAntiguo = valorADC;

  /*
  valorADC = valorADC - 11;
  if(valorADC <= 0.1){

    Peso = 0.0;

  }else{Peso = (valorADC + 5) * 30 / 1024;}
  */

  //float Peso = (valorADC + 1) * 30 / 1024;

  // --- Evitar que parpadee --- //

  if((Peso < valorAntiguo + 0.025) && (Peso > valorAntiguo - 0.025)) 
  {
    Peso = valorAntiguo;
  }

  lcd.setCursor(0,0);
  lcd.print("     ");
  lcd.print(Peso, 2);
  lcd.print(" kg    ");

  valorAntiguo = Peso;

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
  pinMode(pinD4, OUTPUT);
  pinMode(pinD5, OUTPUT);
  pinMode(pinD6, OUTPUT);
  pinMode(pinD7, OUTPUT);
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
  Serial.begin(9600);
}

void loop() {
  if(millis() - TiempoMedido >= TiempoMuestreoMS){
    datosEntrada();
    filtro();
    Serial.println(ArraySalida[0]);
    calculopeso(ArraySalida[0]);
    corrimientoArray(); 
  }
}
