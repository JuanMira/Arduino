#include "Nextion.h"
#include "NexDualStateButton.h"
#include "NexButton.h"
#include "NexNumber.h"
NexDSButton play_pause = NexDSButton(1, 1, "bt0");
NexButton reset = NexButton(1, 5, "b0");
NexNumber meta = NexNumber(1, 3, "n0");
NexNumber setear = NexNumber(1, 4, "n1");
char buffer[100] = {0};
volatile int pulsos;
uint32_t tope = 0;
NexTouch *nex_listen_list[] = {
  &play_pause,
  &reset,
  NULL
};
float volumen;
bool state = false;
unsigned int litrosPorHora;
unsigned char sensorDeFlujo = 2;

unsigned long tiempoAnterior;

unsigned long pulsosAcumulados;

float litros;
void flujo ()

{
  pulsos++;
}

void play_pausePushCallback(void *ptr) {
  int satate = 0;
  uint32_t dual_state;
  NexDSButton *btn = (NexDSButton *)ptr;
  memset(buffer, 0, sizeof(buffer));

  play_pause.getValue(&dual_state);
  meta.getValue(&tope);
  if (tope > 0) {
    if (dual_state) {
      digitalWrite(8, LOW);
      Serial.println("PLAY");
      state = true;
    } else {
      digitalWrite(8, HIGH);
      Serial.println("PAUSE");
      state = false;
    }
  }
}

void resetPushCallback(void *ptr) {
  litros = 0;
  digitalWrite(8, HIGH);
  state = false;
  Serial.println("reset");
}

void setup()
{
  nexInit();
  play_pause.attachPush(play_pausePushCallback, &play_pause);
  reset.attachPush(resetPushCallback, &reset);
  pinMode(sensorDeFlujo, INPUT);
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  attachInterrupt(0, flujo, RISING); // Setup Interrupt
  digitalWrite(8, HIGH);
  interrupts();
  tiempoAnterior = millis();
}



void loop ()
{
  nexLoop(nex_listen_list);
  
  if (state) {
    tiempoAnterior = millis();        
    pulsosAcumulados += pulsos;
    litrosPorHora = (pulsos * 60 / 9.7);
    pulsos = 0;
    setear.setValue(litros);
    meta.getValue(&tope);    
    delay(400);        
    litros = pulsosAcumulados * 1.0 / 400;        
    if(litros>tope){
      litros = 0;
      pulsosAcumulados = 0;
      digitalWrite(8,HIGH);
      state= false;
      setear.setValue(litros);      
    }
  }    
}
