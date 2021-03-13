#include "Nextion.h"
#include "NexNumber.h"
#include "NexButton.h"
#include "NexNumber.h"

volatile int pulsos;
volatile int flowsensor = 2;
float factorConversion = 7.1;
NexButton play = NexButton(1,3,"b0");
NexButton pause = NexButton(1,4,"b1");
NexNumber meta = NexNumber(1,5,"n0");
NexNumber setear = NexNumber(1,6,"n1");

char buffer[100] = {0};

bool led = false;
long t0 = 0;
long dt = 0;
float volumen = 0;
uint32_t tope = 0;

NexTouch *nex_listen_list[] = {
  &play,
  &meta,  
  &pause,
  NULL
};

void contarPulsos(){
  pulsos++;
}


void playPushCallback(void *ptr){
  digitalWrite(4,LOW);  digitalWrite(13,HIGH);
}


void pausePushCallback(void *ptr){
  digitalWrite(4,HIGH);
  digitalWrite(13,LOW);  
}


void setup() {
  nexInit();
  play.attachPush(playPushCallback,&play);
  pause.attachPush(pausePushCallback,&pause);
  pinMode(13,OUTPUT);
  pinMode(3,OUTPUT);  
  pinMode(flowsensor,INPUT);
  digitalWrite(4,HIGH);  
  attachInterrupt(0,contarPulsos,RISING);
  t0 = millis();
} 
  
void loop() {    
  nexLoop(nex_listen_list);      
  sei(); 
  dt = millis()-t0;
  t0 = millis();      
  int frecuencia;
  pulsos = 0; 
  interrupts();
  delay(1000);
  noInterrupts();
  frecuencia = pulsos;  
  float caudal_L_m = frecuencia/factorConversion;
  volumen = volumen + (caudal_L_m/60) * (dt/1000);
  Serial.println(volumen);  
}
