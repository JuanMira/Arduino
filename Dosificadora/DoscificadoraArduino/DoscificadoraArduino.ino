#include "Nextion.h"
#include "NexText.h"
#include "NexNumber.h"
#include "NexButton.h"
//#include <EEPROM.h>
volatile int pulsos; // Measures flow sensor pulses
volatile int  flowsensor = 2; // Sensor Input
float factorConversion = 7.1;
NexButton b0 = NexButton(1,3,"b0");
NexButton b1 = NexButton(1,4,"b1");
NexText n1 = NexText(1,2,"t1"); 
NexNumber lt = NexNumber(1,6,"n1");
NexNumber fn = NexNumber(1,5,"n0");

char buffer[100] = {0};

NexTouch *nex_listen_list[]={
  &b0,
  &n1,
  &b1,
  &lt,
  &fn,
  NULL
  };

char p;
uint32_t y = 0;
int x = 0;
uint32_t tope = 0;
uint32_t start = 0;
int obs = 0;
int z =0;
long dt = 0;
long t0 = 0;
float volumen = 0;

//int pulsos = 0;

void contarPulsos(){
  pulsos++;
}

int obtenerFrecuencia(){
  int frecuencia;
  pulsos = 0;
  interrupts();
  delay(1000);
  noInterrupts();
  frecuencia = pulsos;
  return frecuencia;
}
void b0PushCallback(void *ptr){
  fn.getValue(&start);
  
  if(start!=0){
  //dbSerialPrintln("Valvula Play"); 
  digitalWrite(3,LOW); 
  delay(50);
  digitalWrite(4,LOW);
  //dbSerialPrintln("Bomba Play");
  }
}

void b1PushCallback(void *ptr){
  //dbSerialPrintln("Bomba pausada");
  digitalWrite(4,HIGH);
  //delay(20);
  digitalWrite(3,HIGH);
  //dbSerialPrintln("Valvula pausada");
  
  }
  
void setup(void) {
  //EEPROM.write(0,z+1);
 // obs = EEPROM.read(0);
  //Serial.begin(9600);
  nexInit();
  b1.attachPush(b1PushCallback,&b1);
  b0.attachPush(b0PushCallback,&b0);
  //dbSerialPrint("Setup done");
  pinMode(flowsensor,INPUT);
  pinMode(3,OUTPUT);//pin 3 balbula pin 4 bomba
  pinMode(4,OUTPUT);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  //digitalWrite(12,HIGH);
  attachInterrupt(0,contarPulsos,RISING);
  t0 = millis();
  sei();  
}
void loop() {
  nexLoop(nex_listen_list);
  sei();
  delay(20);
  nexLoop(nex_listen_list);
  float frecuencia = obtenerFrecuencia();
  float caudal_L_m = frecuencia/factorConversion;
  dt  = millis()-t0;
  t0 = millis();
  volumen = volumen+(caudal_L_m/60)*(dt/1000);
  
    if(volumen > tope){
      //dbSerialPrintln("Boma 0");  
      digitalWrite(4,HIGH);
      //delay(50);
      //dbSerialPrintln("valvula 0");
      digitalWrite(3,HIGH);
      //delay(500);
      volumen = 0;
      lt.setValue(volumen);
      fn.setValue(volumen);    
      //dbSerialPrintln(y); 
    }
    lt.setValue(volumen);
}  
 
