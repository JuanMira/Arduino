#include "Nextion.h"
#include "NexNumber.h"
#include "NexButton.h"
#include "NexText.h"
#include "ListLib.h"

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
  /*digitalWrite(t1o, HIGH);
  digitalWrite(t2o, HIGH);
  digitalWrite(t3o, HIGH);
  digitalWrite(Bo, HIGH);
  */Serial.print("Pausa");
  salir = false;
}

/*
  void b0PushCallback(void *ptr) {
  digitalWrite(t1o, HIGH);
  digitalWrite(t2o, HIGH);
  digitalWrite(t3o, HIGH);
  Serial.print("Abortar");
  salir = false;
  }*/

//play button
void b1PushCallback(void *ptr) {
  salir = true;
  /*
    delay(50);
    B.getValue(&Bv);
    valB = Bv;
    while (Bv > 0) {
    delay(750);
    B.getValue(&Bv);
    if (Bv == 0) {
      digitalWrite(Bo, HIGH);
      Serial.print(valid);
      Serial.print(",");
      Serial.print(valt1);
      Serial.print(",");
      Serial.print(valt2);
      Serial.print(",");
      Serial.print(valB);
      Serial.print(",");
      Serial.println(valt3);
      Serial.print(",");
      Serial.println(resV);
    }
    }*/
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

void washProcess() {
  if (salir == true) {
    while (true) {
      valt1 = t1v;
      resV = analogReadFunction();
      res.setValue(resV);
      digitalWrite(t1o, LOW);
      digitalWrite(t2o, HIGH);
      digitalWrite(t3o, HIGH);
      t1.getValue(&t1v);
      while (t1v > 0) {
        delay(1000);
        t1.getValue(&t1v);
        if (t1v == 0) {
          digitalWrite(t1o, HIGH);
          digitalWrite(t2o, LOW);
          digitalWrite(t3o, HIGH);
        }
      }

      t2.getValue(&t2v);
      valt2 = t2v;

      digitalWrite(t1o, HIGH);
      digitalWrite(t2o, LOW);
      digitalWrite(t3o, HIGH);

      while (t2v > 0) {
        delay(880);
        t2.getValue(&t2v);
        if (t2v == 0) {
          digitalWrite(t2o, HIGH);
          t1Exit == 1;
        }
      }

      t3.getValue(&t3v);
      valt3 = t3v;
      digitalWrite(t1o, HIGH);
      digitalWrite(t2o, HIGH);
      digitalWrite(t3o, LOW);
      while (t3v > 0) {
        delay(800);
        t3.getValue(&t3v);
        if (t3v == 1) {
          delay(1000);
          t3v = t3v - 1;
        }
        if (t3v == 0) {
          digitalWrite(t3o, HIGH);
          digitalWrite(Bo, HIGH);
          idT.getText(buffer, sizeof(buffer));
          Serial.print(",");
          Serial.print(valt1);
          Serial.print(",");
          Serial.print(valt2);
          Serial.print(",");
          Serial.print(valt3);
          Serial.print(",");
          Serial.print(resV);
          Serial.print(",");
          Serial.println(valB);
          delay(1700);
          t1.getValue(&t1v);
          digitalWrite(t1o, HIGH);
          digitalWrite(t2o, HIGH);
          digitalWrite(t3o, HIGH);
        }
      }
    }

  }
}


void setup() {
  // put your setup code here, to run once:
  nexInit();
  Serial.begin(9600);
  play.attachPush(b1PushCallback, &play);
  pause.attachPush(b2PushCallback, &pause);
  //abortar.attachPush(b0PushCallback, &abortar);
  pinMode(t1o, OUTPUT);
  pinMode(t2o, OUTPUT);
  pinMode(t3o, OUTPUT);
  pinMode(Bo, OUTPUT);
  digitalWrite(t1o, HIGH);
  digitalWrite(t2o, HIGH);
  digitalWrite(t3o, HIGH);
  digitalWrite(Bo, HIGH);
  //create a new list
}


void loop() {
  nexLoop(nex_listen_list);
  if (salir == true)
    washProcess();
  else {
    digitalWrite(t1o, HIGH);
    digitalWrite(t2o, HIGH);
    digitalWrite(t3o, HIGH);
  }

}
