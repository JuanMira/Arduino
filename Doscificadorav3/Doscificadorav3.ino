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
bool state = true;
unsigned int litrosPorHora;
unsigned char sensorDeFlujo = 2;
int p = 4;
unsigned long tiempoAnterior;

unsigned long pulsosAcumulados;

unsigned int litros;
void flujo ()

{
  pulsos++;
}

//se obtiene el valor del dual state button
void play_pausePushCallback(void *ptr) {
  int satate = 0;
  uint32_t dual_state;
  NexDSButton *btn = (NexDSButton *)ptr;
  memset(buffer, 0, sizeof(buffer));

  play_pause.getValue(&dual_state);
  meta.getValue(&tope);
  if (tope > 0) {
    if (dual_state) {
      digitalWrite(p, LOW);
      Serial.println("PLAY");
      state = true;
    } else {
      digitalWrite(p, HIGH);
      Serial.println("PAUSE");
      state = false;
    }
  }
}

//boton de reset
void resetPushCallback(void *ptr) {
  litros = 0;
  digitalWrite(p, HIGH);
  state = false;
  Serial.println("reset");
  setear.setValue(0);
}

void setup() {
  nexInit();
  play_pause.attachPush(play_pausePushCallback, &play_pause);
  reset.attachPush(resetPushCallback, &reset);
  pinMode(sensorDeFlujo, INPUT);
  Serial.begin(9600);
  pinMode(p, OUTPUT);
  attachInterrupt(0, flujo, RISING); // Setup Interrupt
  digitalWrite(p, HIGH);
  interrupts();
  tiempoAnterior = millis();

}

void loop() {
  nexLoop(nex_listen_list);
  
  if (state) {
    tiempoAnterior = millis();        
    pulsosAcumulados += pulsos;
    litros = (pulsos * 60 / 7.5);//factor conversion de litros por hora
    pulsos = 0;
    setear.setValue(litros);
    meta.getValue(&tope);    
    delay(400);        
    litrosPorHora = pulsosAcumulados/7.5; //cantidad de pulsos por litros            
    if(litros>tope){
      litrosPorHora = 0;
      pulsosAcumulados = 0;
      digitalWrite(p,HIGH);
      state= false;
      setear.setValue(litrosPorHora);      
    }
  }    
}
