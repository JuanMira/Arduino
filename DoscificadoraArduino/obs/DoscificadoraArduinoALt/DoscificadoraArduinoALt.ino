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
uint32_t start = 0;
int play = 0;
int pulsos = 0;
void contarPulsos(){
  pulsos++;
  }
void b0PushCallback(void *ptr){
  fn.getValue(&start);
  if(start!=0){
  //dbSerialPrintln("Valvula Play"); 
  digitalWrite(3,LOW); 
  //delay(50);
  digitalWrite(4,LOW);
  //dbSerialPrintln("Bomba Play");
  play = 1;
  
  }
}

void b1PushCallback(void *ptr){
  //dbSerialPrintln("Bomba pausada");
  digitalWrite(4,HIGH);
  //delay(20);
  digitalWrite(3,HIGH);
  //dbSerialPrintln("Valvula pausada");
  play = 0;
  }
  
void setup(void) {
  //EEPROM.write(z+1);
  Serial.begin(9600);
  nexInit();
  b1.attachPush(b1PushCallback,&b1);
  b0.attachPush(b0PushCallback,&b0);
  //dbSerialPrint("Setup done");
  pinMode(flowsensor,INPUT_PULLUP);
  pinMode(3,OUTPUT);//pin 3 balbula pin 4 bomba
  pinMode(4,OUTPUT);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  //digitalWrite(12,HIGH);
  attachInterrupt(0,contarPulsos,RISING);
  sei();  
  
}
void loop() {
  sei();
  nexLoop(nex_listen_list);  
  //calc = pulsos;
  fn.getValue(&tope);
  if(play == 1){
    y = y + 1;
    delay(4053);   
    if(y>=tope){
      //dbSerialPrintln("Boma 0");  
      digitalWrite(4,HIGH);
      //delay(50);
      //dbSerialPrintln("valvula 0");
      digitalWrite(3,HIGH);
      //delay(500);
      y = 0;
      play=0;
      //lt.setValue(y);
      //fn.setValue(y);    
      //dbSerialPrintln(y); 
    }
    lt.setValue(y);
  }
  /*
  if(calc>=18){
    calc = 0;
    pulsos = 0;
    y = y + 1 ;
    fn.getValue(&tope);
    if(y > tope){
      //dbSerialPrintln("Boma 0");  
      digitalWrite(4,HIGH);
      //delay(50);
      //dbSerialPrintln("valvula 0");
      digitalWrite(3,HIGH);
      //delay(500);
      y = 0;
      lt.setValue(y);
      fn.setValue(y);    
      //dbSerialPrintln(y); 
    }
  }
  //dbSerialPrint(y);
  lt.setValue(y);
  dbSerialPrintln(pulsos);
  //pulsos = 0;
*/
}
