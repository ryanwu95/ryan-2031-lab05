#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include "ifttt.h"

enum State {
	alarm_off, alarm_armed, alarm_arming, alarm_triggered, alarm_sounding
};
static enum State alarmState;

void init(){
	wiringPiSetup();
	pinMode(0,INPUT);
	pinMode(1,OUTPUT);
	pinMode(2,OUTPUT);
	pinMode(3,INPUT);
	pinMode(4,OUTPUT);
	pullUpDnControl(3,PUD_UP);
} 


void ALARM_OFF(){
	
	delay(500);
	printf("alarm off now\n");
	digitalWrite(1,HIGH);
	digitalWrite(2,LOW);
	digitalWrite(4,LOW);
	if(digitalRead(3) == 0){

		printf("Button pushed, Araming\n");
		alarmState = alarm_arming;
	}

}


void ALARM_ARMING(){
			
	
/*	time_t begain,end;
	begain = time(NULL);
	do{*/


	int i = 0;
	while(i <10 ){
		digitalWrite(1,HIGH);delay(500);
		digitalWrite(1,LOW);delay(500);
		/*end = time(NULL);*/
		i++;
		printf("Arming &d\n",i);
	}

	/*while( (end-begain).10);*/
	digitalWrite(2,HIGH);
	alarmState = alarm_armed;
}

void ALARM_ARMED(){
	printf("Alarm Armed\n");
	delay(500);
	digitalWrite(1,LOW);
	digitalWrite(2,HIGH);
	if (digitalRead(3) == 0){
		alarmState = alarm_off;
	}else if(digitalRead(0) == 1){
		alarmState = alarm_triggered;
	}
}


void ALARM_TRIGGERED(){

		
	double end,current,begain,gap;
	int led;
	printf("Alarm Trigered!!!\n");
	begain = (double) time(NULL);
	end = (double) time(NULL)+10;
	current = (double) time(NULL);
	digitalWrite(1,LOW);
	digitalWrite(2,LOW);
	gap = 2;
	led = 1;

	while(current <= end){
		if(digitalRead(3) == 0){
			alarmState = alarm_off;
			break;
		} 
		if(current - begain >= gap){
			if(led == 1){
				digitalWrite(1,HIGH);
				digitalWrite(2,HIGH);
				led =0;
			}else{
				digitalWrite(1,LOW);
				digitalWrite(2,LOW);
				led = 1;
			}
			begain = current;
		}
		current = (double) time(NULL);
	}
	alarmState = alarm_sounding;
}



void ALARM_SOUNDING(){
	
	double begain,current,interval;
	int on;
	printf("alarm sounding\n");
	ifttt ("https://maker.ifttt.com/trigger/alarm_triggered/with/key/bIEHY5yQV4rQls416JBymI","111","222","333");
	begain = (double) time(NULL);
	current = (double) time(NULL);
	interval = 1;
	on = 0;
	while(digitalRead(3) == 1){
		current = time(NULL);
		if(current - begain >= interval){
			if(on == 0){
				digitalWrite(1,HIGH);
				digitalWrite(2,LOW);
				digitalWrite(4,HIGH);
				on = 1;
			}else{
				digitalWrite(1,LOW);
				digitalWrite(2,HIGH);
				digitalWrite(4,LOW);
				on = 0;
			}
			begain = current;
		}
	
	}
	alarmState = alarm_off;
}



int main (int argc, char *argv[]){
	
	init();
	alarmState = alarm_off;
	while(1){
		if(alarmState == alarm_off){
			ALARM_OFF();
		}else if(alarmState == alarm_arming){
			ALARM_ARMING();
		}else if(alarmState == alarm_armed){
			ALARM_ARMED();
		}else if(alarmState == alarm_triggered){
			ALARM_TRIGGERED();
		}else if(alarmState == alarm_sounding){
			ALARM_SOUNDING();
		}


	}
	
	return 0;
}





