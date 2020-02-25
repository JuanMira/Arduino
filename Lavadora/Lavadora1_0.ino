#include "Nextion.h"
#include "NexHotspot.h"
#include "NexButton.h"
#include "NexTimer.h";
#include "NexNumber.h";
NexHotspot hot0 = NexHotspot(0,1,"m0");
NexHotspot hot1 = NexHotspot(1,1,"m0");
NexTimer v1 = NexTimer(5,5,"tm0");
NexButton b0 = NexButton(5,14,"b0");
NexButton b1 = NexButton(5,19,"b1");
NexNumber n0 = NexNumber(5,8,"n0");
NexNumber n1 = NexNumber(5,9,"n1");
NexNumber n2 = NexNumber(5,10,"n2");
NexButton b2 = NexButton(5,22,"b3");
NexTouch *nex_listen_list[]={
  &hot0,
  &hot1,
  &b0,  
  &b1,
  &b2,
  NULL
  };
char buffer[100] = {0};
uint32_t num = 0;
uint32_t num1 = 0;
uint32_t num2 = 0;
uint32_t num3 = 0;
//boton pausa
uint32_t num12 = 0;
//Salida forzosa
void b2PopCallback(void *ptr){
  digitalWrite(10,HIGH);
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
}  
//control de los t1 a t3 
void b0PopCallback(void *ptr){
  //if(!b2PopCallback){
  //dbSerialPrintln("Se presiono el boton play");
  //trae valor de t1
  n0.getValue(&num);
  digitalWrite(10,LOW);

    while(num>0){
    dbSerialPrintln(num);
    delay(1000);
    n0.getValue(&num);
    if(num==0){
    digitalWrite(10,HIGH);}
      }
    delay(50);
    n1.getValue(&num1); 
    digitalWrite(11,LOW);
    //trae valor de t2
    while(num1>0){    
    dbSerialPrintln(num1);
    delay(880);
    n1.getValue(&num1);
    if(num1==0){
      digitalWrite(11,HIGH);
      }    
      }
      delay(50);
   //trae el valor de t3
    n2.getValue(&num2);
    digitalWrite(12,LOW);
    while(num2>0){
    dbSerialPrintln(num2);
    delay(800);
    n2.getValue(&num2);
    if(num2==0){
    digitalWrite(12,HIGH);
      }
    }
    if(num==0&&num1==0&&num2==0){
      digitalWrite(12,HIGH);
      }
    //digitalWrite(12,HIGH);
  //}
}
    //dbSerialPrint(num);
 
void hot0PushCallback(void *ptr)
{
  dbSerialPrintln("hot0PushCallback");  
  dbSerialPrint("ptr=");
  dbSerialPrintln((uint32_t)ptr);
}

void hot0PopCallback(void *ptr)
{
  dbSerialPrintln("hot0PopCallback");
  dbSerialPrint("ptr=");  
  dbSerialPrintln((uint32_t)ptr);
}

void hot1PopCallback(void *ptr){
  digitalWrite(13,HIGH);
  delay(2000);
  digitalWrite(13,LOW);
  
  }



void setup(void) {
  // put your setup code here, to run once:
  nexInit();
  hot0.attachPush(hot0PushCallback,&hot0);
  hot0.attachPop(hot0PopCallback,&hot0);
  hot1.attachPop(hot1PopCallback,&hot1);
  b2.attachPop(b2PopCallback,&b2);
  b0.attachPop(b0PopCallback,&b0);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  digitalWrite(10,HIGH);
  digitalWrite(11,HIGH);
  digitalWrite(12,HIGH);
  dbSerialPrintln("Setup Done");
}

void loop(void) {
  // put your main code here, to run repeatedly:
  nexLoop(nex_listen_list);

}
