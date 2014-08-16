/*

Test biblioteki RC-5 na ATmega8

Schemat testowy:
 - trzy diody LED anodami do pinów PB0, PD6 i PD7, a katodami przez rezystory
   220Ohm do GND (masa).
 - wyjœcie czujnika podczerwieni do pinu INT0.

Plik:		RC5_main.c
Kompilator: AVR-GCC 4.3.3

Data: 	2013.07.30
Autor: 	Dondu
www:	http://mikrokontrolery.blogspot.com/2011/03/RC5-IR-podczerwien-odbior-danych-przyklad-AVR-ATmega8.html

Schemat testowy dostêpny w linku powy¿ej.

*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

/* Za³¹cz bibliotekê RC-5*/
#include "dd_rc5.h"


/* LED testowy do pokazywania odebranych danych */
#define LED_PORT	PORTB
#define LED_DDR 	DDRB
#define LED_PIN 	PB0
#define LED_INI 	LED_DDR	 |=  (1<<LED_PIN)
#define LED_ON		LED_PORT |=  (1<<LED_PIN)
#define LED_OFF		LED_PORT &= ~(1<<LED_PIN)
#define LED_MIX		LED_PORT ^= (1<<LED_PIN)


/* LED testowy do pokazywania stanu bitu toggle */
#define LED_TOGGLE_BIT_PORT	PORTD
#define LED_TOGGLE_BIT_DDR 	DDRD
#define LED_TOGGLE_BIT_PIN 	PD7
#define LED_TOGGLE_BIT_INI 	LED_TOGGLE_BIT_DDR 	|= 	(1<<LED_TOGGLE_BIT_PIN)
#define LED_TOGGLE_BIT_ON	LED_TOGGLE_BIT_PORT |=  (1<<LED_TOGGLE_BIT_PIN)
#define LED_TOGGLE_BIT_OFF	LED_TOGGLE_BIT_PORT &= ~(1<<LED_TOGGLE_BIT_PIN)


/* Zmienne globalne z odebranymi danymi */
//volatile unsigned char dd_rc5_dane_odebrane = 0;
//volatile unsigned char dd_rc5_status = 0;


//-----------------------------------------------------------------------------

void test(void){

	//Funkcja testowa pokazuj¹ca na diodach LED odbierane dane z pilota RC5.
	//Funkcjê tê mo¿esz dowolnie skomplikowaæ lub usun¹æ.

	//unsigned char f;	//zmienna pomocnicza

	//sprawdzamy bit toggle	zapalaj¹c przypisan¹ mu diodê
	//gdy bit ten jest jedynk¹ i gasz¹c, gdy jest zerem
	if(dd_rc5_status & 0b100000)	LED_TOGGLE_BIT_ON;
	else LED_TOGGLE_BIT_OFF;

	//opóŸnienie w celu lepszej widocznoœci zmian diod LED
	_delay_ms(300);

	//dd_rc5_dane_odebrane = dd_rc5_status & 0b11111;

	//mrugnij diod¹ danych tyle razy jaki kod klawisza odebrano

	/*
	for(f=0; f<dd_rc5_dane_odebrane; f++){
		LED_ON;
		_delay_ms(50);
		LED_OFF;
		_delay_ms(300);
	}
    */


	// 12 - sygna³ POWER
    if (dd_rc5_dane_odebrane == 12) LED_MIX;
    //else {LED_OFF;}




	//Poniewa¿ odczytaliœmy odebrane dane zgaœ flagê gotowoœci
	//danych do odczytu, by umo¿liwiæ odebranie nastêpnych danych
	dd_rc5_status &= ~DD_RC5_STATUS_DANE_GOTOWE_DO_ODCZYTU;
	DD_RC5_WLACZ_DEKODOWANIE;

}


//-----------------------------------------------------------------------------

int main(void){

	//inicjuj LED-y testowe
	LED_INI;
	LED_TOGGLE_BIT_INI;


	//inicjuj timer oraz pin przerwania zewnêtrznego INTx
	dd_rc5_ini();

	//w³¹cz mo¿liwoœæ usypiania do trybu IDLE
	sleep_enable();
	set_sleep_mode(SLEEP_MODE_IDLE);

	//w³¹cz przerwania globalne
    sei();

	//pêtla g³ówna
    while(1){

		//tutaj mo¿esz dodaæ swój kod

		//sprawdŸ, czy dane s¹ gotowe do odczytu
		if(dd_rc5_status & DD_RC5_STATUS_DANE_GOTOWE_DO_ODCZYTU){
			//dane czekaj¹ na odczyt
			test();
		}

		sleep_cpu(); //œpij koteczku :-)


	}
}







