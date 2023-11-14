#include <Arduino.h>
#include <math.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(A5, INPUT);
  //pinMode(A4, OUTPUT);
  analogReference(EXTERNAL);
  Serial.begin(9600);
}

// Valores de la señal
//float pi = 3.14159;
//int f = 1000;

// Valores para la discretizacion
int fs = 110;                    // Frecuencia de muestreo 
float Ts = 1/fs;                 // Tiempo de muestreo
float Tsms = Ts *1000;           // Tiempo de muestreo en ms
float TiempoMedido = Tsms;       // Timepo inicial
//float TiempoMuestra = 50;
int A = 0;
float Sumatoria = 0;

// El tamaño de los arrays es igual al orden del denominador + 1

const float CoefEntrada[5] = {0.1237, 0.4949, 0.7423, 0.4949, 0.1237};
const float CoefSalida[5] = {1 -0.3566 -0.5262 -0.07653 -0.02015};

float ArrayEntrada[5] = {0, 0, 0, 0, 0}; 
float ArraySalida[5] = {0, 0, 0, 0, 0};

void loop() {

  if(millis() - TiempoMedido == Tsms)
  {
    //Serial.print(analogRead(A5));
    ArrayEntrada[0] = float(analogRead(A5));
    Sumatoria = ArrayEntrada[0] * CoefEntrada[0];

    for(int i = 1; i < 5; i++)
    {
      Sumatoria += ArrayEntrada[i] * CoefEntrada[i] + ArraySalida[i] * CoefSalida[i]; 
    }

    ArraySalida[0] = Sumatoria;

    //Serial.print(" ");
    Serial.println(ArraySalida[0]);

    for(int j = 4; j > 0; j--)
    {
      ArrayEntrada[j] = ArrayEntrada[j-1];
      ArraySalida[j] = ArraySalida[j-1];
    }
    //Serial.println(ArraySalida[0]);
    //analogWrite(3, ArraySalida[0]);
    //if(A % 50 == 0)
    //{
    //  Serial.println(ArraySalida[0]);
    //}
    //A++;
    delay(2000);
    TiempoMedido = millis();
  }
    
}

/*

int fs = 110;                    // Frecuencia de muestreo 
float Ts = 1/fs;                 // Tiempo de muestreo
int A = 0;
 
#define SAMPLNG_PERIOD_MS 9.1

const char pinADCInput = A5;
//const char pinDACOutput = 25;

const float inputCoefficients[5] = {0.1237, 0.4949, 0.7423, 0.4949, 0.1237};
const float outputCoefficients[5] = {1 -0.3566 -0.5262 -0.07653 -0.02015};

float outputArray[5] = {0, 0, 0, 0, 0};
float inputArray[5] = {0, 0, 0, 0, 0}; 

unsigned long lastMeasurementTime = 0;

void getInputData(){
  lastMeasurementTime = millis();
  inputArray[0] = float(analogRead(pinADCInput));
}

void iirLowpassButterworthFilter(){
  float outputSum = inputCoefficients[0]*inputArray[0];
  for(int i = 1; i < 5; i++){
    outputSum += inputCoefficients[i] * inputArray[i] + outputCoefficients[i] * outputArray[i];
  }
  outputArray[0] = outputSum;
}

void arraysRightShift(){
  for(int i = 4; i > 0; i--){
    outputArray[i] = outputArray[i-1];
    inputArray[i] = inputArray[i-1];
  }
}

void pinInitalization(){
  pinMode(pinADCInput, INPUT);
  //pinMode(pinDACOutput, OUTPUT); 
  analogReference(EXTERNAL);
}

void setup() {
  pinInitalization();
  Serial.begin(9600);
  //analogReadResolution(8);
  lastMeasurementTime = millis();
}

void loop() {
  if(millis() - lastMeasurementTime >= SAMPLNG_PERIOD_MS){
    getInputData();
    iirLowpassButterworthFilter();
    //dacWrite(pinDACOutput, outputArray[0]);
    Serial.println(outputArray[0]);
    //if(A % 50 == 0)
    //{
    //  Serial.println(outputArray[0]);
    //}
    //A++;
    arraysRightShift(); 
  }
}

*/
