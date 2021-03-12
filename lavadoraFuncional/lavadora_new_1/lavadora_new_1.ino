#include "Nextion.h"
#include "NexNumber.h"
#include "NexButton.h"
#include "NexText.h"

NexButton play = NexButton(7, 13, "b1");
NexButton pause = NexButton(7, 14, "b2");
NexButton abortar = NexButton(7, 8, "b0");
NexNumber t1 = NexNumber(7, 9, "n0");
NexNumber t2 = NexNumber(7, 10, "n1");
NexNumber t3 = NexNumber(7, 11, "n2");
NexNumber B = NexNumber(7, 12, "n3");
NexNumber id = NexNumber(7, 16, "n4");
NexNumber res = NexNumber(7, 17, "n5");
NexText idT = NexText(7, 15, "t4");

NexTouch *nex_listen_list[] = {
  &play,
  &abortar,
  &pause,
  &t1,
  &t2,
  &t3,
  &B,
  &id,
  &res,
  NULL
};

char buffer[100] = {0};

//"v" is for value like t1 Value, psdt t1 is the value from nextion screen
uint32_t t1v = 0;
uint32_t t2v = 0;
uint32_t t3v = 0;
uint32_t Bv = 0;
uint32_t Idv = 0;
uint32_t resV = 0;

//define the pins output. the "o" is for output
int t1o = 8;
int t2o = 9;
int t3o = 10;
int Bo = 11;

//excel values
int valt1;
int valt2;
int valt3;
int valB;
int valid;
uint16_t validt = "";
int t1Exit = 1;
//analogRead input values
float sumVal;
float prom;
bool salir = false;
float analogVal[10];

//pause button
void b2PushCallback(void *ptr) {
  Serial.println("Pausa");
  salir = false;
}

//play button
void b1PushCallback(void *ptr) {
  salir = true;
  Serial.println("Reproducir");
}

//abort button
void abortarPushCallback(void *ptr) {
  salir = false;
  Serial.println("Abortar");
}

float analogReadFunction() {
  float input;
  for (int i = 0; i < 10; i++) {
    input = analogRead(A0);
    if (input < 490)
      input = 490;
    float dataInput = ((float(input - 490 )) * 5 / 1024);
    analogVal[i] = dataInput;
    sumVal = analogVal[i] + sumVal;
    Serial.print("*******");
    Serial.print(input);
    Serial.println("*******");
    delay(1);
  }
  int cant = sizeof(analogVal);
  prom = sumVal / 10;
  sumVal = 0;
  Serial.println(prom);
  return prom;
}

void setup() {
  nexInit();
  Serial.begin(9600);
  play.attachPush(b1PushCallback, &play);
  pause.attachPush(b2PushCallback, &pause);
  abortar.attachPush(abortarPushCallback, &abortar);
  pinMode(t1o, OUTPUT);
  pinMode(t2o, OUTPUT);
  pinMode(t3o, OUTPUT);
  pinMode(Bo, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(t1o, HIGH);
  digitalWrite(t2o, HIGH);
  digitalWrite(t3o, HIGH);
  digitalWrite(Bo, HIGH);
  digitalWrite(13, LOW);
}


void loop() {
  nexLoop(nex_listen_list);    
  if (salir) {    
    digitalWrite(13, HIGH);
    t1.getValue(&t1v);
    if (t1v > 0) {      
      digitalWrite(t3o,HIGH);
      digitalWrite(t1o,LOW);
      Serial.println(t1v);
      delay(200);
      t1.getValue(&t1v);
      if (t1v == 0) {
        digitalWrite(t1o, HIGH);
        Serial.println("T1 over");
        delay(100);
      }
    }

    if (t1v == 0) {
      if (salir) {
        t2.getValue(&t2v);
        if (t2v > 0) {
          digitalWrite(t2o,LOW);
          Serial.println(t2v);
          t2.getValue(&t2v);
          delay(200);
          if (t2v == 0) {
            digitalWrite(t2o,HIGH);
            Serial.println("T2 over");
          }
        }
      }
    }

    if (t1v == 0 && t2v == 0) {
      if (salir) {
        digitalWrite(t2o,HIGH);
        t3.getValue(&t3v);        
        if (salir) {
          digitalWrite(t3o,LOW);
          Serial.println(t3v);
          delay(200);
          t3.getValue(&t3v);    
          if (t3v == 0) {
            digitalWrite(t3o,HIGH);
            Serial.println("T3 over");
          }
        }
      }
    }
  }
  else {
    digitalWrite(t1o,HIGH);
    digitalWrite(t2o,HIGH);
    digitalWrite(t3o,HIGH);
  }

}
