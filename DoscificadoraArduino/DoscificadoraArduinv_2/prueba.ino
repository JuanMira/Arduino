#include <doxygen.h>
#include <NexButton.h>
#include <NexCheckbox.h>
#include <NexConfig.h>
#include <NexCrop.h>
#include <NexDualStateButton.h>
#include <NexGauge.h>
#include <NexGpio.h>
#include <NexHardware.h>
#include <NexHotspot.h>
#include <NexNumber.h>
#include <NexObject.h>
#include <NexPage.h>
#include <NexPicture.h>
#include <NexProgressBar.h>
#include <NexRadio.h>
#include <NexRtc.h>
#include <NexScrolltext.h>
#include <NexSlider.h>
#include <NexText.h>
#include <NexTimer.h>
#include <Nextion.h>
#include <NexTouch.h>
#include <NexUpload.h>
#include <NexVariable.h>
#include <NexWaveform.h>

#include "Nextion.h"

NexDSButton play_pause = NexDSButton(1,1,"bt0");
NexButton reset = NexButton(1,5,"b0");
NexNumber meta = NexNumber(1,3,"n0");
NexNumber setear = NexNumber(1,4,"n1");

char buffer[100] = {0};


int pulsos;
int flowSensor = 2;
float factorConversion = 7.1;
long t0 = 0;
long dt = 0;
float volumen  = 0;
uint32_t tope = 0;

NexTouch *nex_listen_list[]={
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
	uint32_t dual_state;
	NexDSButton *btn = (NexDSButton *)ptr;
	memset(buffer,0,sizeof(buffer));

	play_pause.getValue(&dual_state);
	meta.getValue(&tope);
	if(tope>0){
		if(dual_state){
			digitalWrite(8, LOW);
			state = true;
		}else{
			digitalWrite(8, HIGH);
			state = false;
		}
	}
}

void resetPushCallback(void *ptr){
	digitalWrite(8, LOW);
	state = false;
}

void setup()
{
	nexInit();
	play_pause.attachPush(play_pausePushCallback,&play_pause);
	reset.attachPush(resetPushCallback,&reset);
	Serial.begin(9600);
	pinMode(8, OUTPUT);
	pinMode(flowSensor, INPUT);
	digitalWrite(8, HIGH);
	attachInterrupt(0, contarPulsos, RISING);
	t0=millis();
}

void void loop()
{
	sei();
	dt=millis()-t0;
	t0=millis();
	nexLoop(nex_listen_list);
	meta.getValue(&tope);
	setear.setValue(volumen);
	if(state){
		int frecuencia = obtenerFrecuencia();
		Serial.println(volumen);
		float caudal_l_m = volumen + (caudal_l_m/60) * (dt/1000);
		if(volumen>tope){
			digitalWrite(8, HIGH);
			volumen = 0;
		}
	}
}