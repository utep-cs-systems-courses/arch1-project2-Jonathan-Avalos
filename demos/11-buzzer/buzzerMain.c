
#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

//Defining all the leds and their bits
#define RED_LED BIT6
#define GREEN_LED BIT0
#define LEDS (BIT0 | BIT6)

//Defining all switches and their bits
#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3
#define SWITCHES (BIT0 | BIT1 | BIT2 | BIT3)
int main() {
    configureClocks();
    enableWDTInterrupts();
    
    P1DIR |= LEDS;
    P1OUT &= ~LEDS;
    
    P2REN |= SWITCHES;
    P2IE |= SWITCHES;
    P2OUT |= SWITCHES;
    P2DIR &= ~SWITCHES;
    
    buzzer_init();
    
    or_sr(0x18);          // CPU off, GIE on
}
int switches = 0;
int state = 0;
void
switch_interrupt_handler()
{

  char p1val = P2IN;
  switches = (~p1val & SWITCHES);
  
  P2IES |= (p1val & SWITCHES);
  P2IES &= (p1val | ~SWITCHES);
 
  if (switches & SW1) {
    state = 1;
  }
  else if(switches & SW2){
    state = 2;
  }
  else if(switches & SW3){
    state = 3;
  }
  else if(switches & SW4){
    state = 4;
    
  }
  else{
    state = 0;
  }

  switch(state){
  case 0:
    buzzer_set_period(0);
    P1OUT &= ~LEDS;
    break;
  case 1:
    buzzer_set_period(8000);
    P1OUT |= RED_LED;
    P1OUT &= ~GREEN_LED;
    break;
  case 3:
    buzzer_set_period(10000);
    P1OUT |= GREEN_LED;
    P1OUT &= ~RED_LED;
    break;
  case 2:
    buzzer_set_period(12000);
    P1OUT |= LEDS;
    break;
  case 4:
    buzzer_set_period(6000);
    
    break;

    
  }
}

void
__interrupt_vec(PORT2_VECTOR) Port_2()
{
  if (P2IFG & SWITCHES) { 
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();

  }

}

int second = 1;
int count = 0;
int temp_second = 0;
int speed = 125;

void
__interrupt_vec(WDT_VECTOR) WDT()
{
  if(count < 25){
    buzzer_set_period(0);
  }
  else{
  
    if (count >= speed){
      count = 0;
      second++;
    }
    if(second >= 25){
      second = 0;
      speed = 125;
    }
    switch(second){
    case 0:
      buzzer_set_period(0); 
      break;
    case 1:   // ^E
    case 2:
    case 3:
    case 5:
    case 16:
      buzzer_set_period(1516);
      break;
    case 4:  // ^C
    case 8:
    case 22:
      buzzer_set_period(1911);
      break;
    case 6:   // ^G
    case 17:
    case 20:
      buzzer_set_period(1275);
      break;
    case 7:
    case 9:
      buzzer_set_period(2551);
      break;
    case 10:
      buzzer_set_period(3033);
      break;
    case 11:
    case 14:
      speed = 80;
      buzzer_set_period(2272);
      break;
    case 12:
    case 24:
      buzzer_set_period(2024);
      break;
    case 13:
      buzzer_set_period(2145);
      break;
    case 15:
      buzzer_set_period(2551);
      break;
    case 18:
      buzzer_set_period(1136);
      break;
    case 19:
      buzzer_set_period(1431);
      break;
    case 21:
      speed = 65;
      buzzer_set_period(1516);
    case 23:
      buzzer_set_period(1702);
      break;
    }
  }

  count++;
}
