#include "Nextion.h"
#include "NexDualStateButton.h"
#include "NexButton.h"
#include "NexNumber.h"
int pulsos;
int flowsensor = 2;
float factorConversion = 7.1;
NexDSButton play_pause = NexDSButton(1,1,"bt0");
NexButton reset = NexButton(1,5,"b0");
NexNumber meta = NexNumber(1,3,"n0");
NexNumber setear = NexNumber(1,4,"n1");
char buffer[100] = {0};
long t0 = 0;
long dt = 0;
float volumen = 0;
uint32_t tope = 0;

NexTouch *nex_listen_list[] = {
  &play_pause,
  &reset,  
  NULL
};

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

bool state = false;

void play_pausePushCallback(void *ptr){
  int satate = 0;  
  uint32_t dual_state;
  NexDSButton *btn = (NexDSButton *)ptr; 
  memset(buffer,0,sizeof(buffer));

  play_pause.getValue(&dual_state);
  meta.getValue(&tope);
  if(tope>0){
    if(dual_state){
      digitalWrite(8,LOW);
      Serial.println("PLAY");
      state = true;      
    }else{
      digitalWrite(8,HIGH);  
      Serial.println("PAUSE");      
      state = false;
    }  
  }
}

void resetPushCallback(void *ptr){
  volumen = 0;
  digitalWrite(8,HIGH);
  state = false;
}

void setup() {
  nexInit();
  play_pause.attachPush(play_pausePushCallback,&play_pause);
  Serial.begin(9600);
  //play_pause.attachPop(play_pausePopCallback,&play_pause);
  reset.attachPush(resetPushCallback,&reset);
  pinMode(8,OUTPUT);
  pinMode(flowsensor,INPUT);  
  digitalWrite(8,HIGH); 
  attachInterrupt(0,contarPulsos,RISING);
  t0 = millis();
} 
  
void loop() {     
  sei();  
  dt = millis()-t0;
  t0 = millis();     
  nexLoop(nex_listen_list);       
  meta.getValue(&tope);
  setear.setValue(volumen);
  if(state){int frecuencia = obtenerFrecuencia();  
  Serial.println(volumen);
  float caudal_L_m = frecuencia/factorConversion;
  volumen = volumen + (caudal_L_m/60) * (dt/1000);  
    if(volumen>tope){   
      digitalWrite(8,HIGH);
      digitalWrite(13,LOW);
      volumen = volumen * 0;     
    }        
  }
}
