#include "Nextion.h"
#include "NexText.h"
#include "NexNumber.h"
#include "NexButton.h"
volatile int NbtopsFan; // Measures flow sensor pulses
unsigned char flowsensor = 2; // Sensor Input
unsigned int calc;
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
  NULL
  };

char p;
uint32_t y = 0;
int x = 0;
uint32_t tope = 0;



int pulsos = 0;
void contarPulsos(){
  pulsos++;
  }
void b0PushCallback(void *ptr){
  dbSerialPrintln("play");  
}

void b1PushCallback(void *ptr){
  dbSerialPrintln("pause");
  }
void setup(void) {
  nexInit();
  b1.attachPush(b1PushCallback,&b1);
  b0.attachPush(b0PushCallback,&b0);
  dbSerialPrint("Setup done");
  pinMode(flowsensor,INPUT_PULLUP);
  digitalWrite(2,HIGH);
  attachInterrupt(0,contarPulsos,RISING);
  sei();  
}
void loop(void) {
  sei();
  delay(1000);
  calc = pulsos;
  nexLoop(nex_listen_list);
  if(calc>=18){
  y = y +1 ;
  fn.getValue(&tope);
  if(y > tope){
    cli();    
    }
  }
  lt.setValue(y);
  //dbSerialPrintln(calc);
  pulsos = 0;
  cli();

}
