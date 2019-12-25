//Led xmas_tree

#include "stm8s.h"

#define ARR_P 16

#define LED0_ON GPIOD->ODR &=(~0x4)
#define LED0_OFF GPIOD->ODR |=0x4
#define LED1_ON GPIOD->ODR &=(~0x10)
#define LED1_OFF GPIOD->ODR |=0x10
#define LED2_ON GPIOD->ODR &=(~0x20)
#define LED2_OFF GPIOD->ODR |=0x20
#define LED3_ON GPIOD->ODR &=(~0x40)
#define LED3_OFF GPIOD->ODR |=0x40
#define LED4_ON GPIOA->ODR &=(~0x2)
#define LED4_OFF GPIOA->ODR |=0x2
#define LED5_ON GPIOA->ODR &=(~0x4)
#define LED5_OFF GPIOA->ODR |=0x4
#define LED6_ON GPIOA->ODR &=(~0x8)
#define LED6_OFF GPIOA->ODR |=0x8
#define LED7_ON GPIOB->ODR &=(~0x20)
#define LED7_OFF GPIOB->ODR |=0x20
#define LED8_ON GPIOB->ODR &=(~0x10)
#define LED8_OFF GPIOB->ODR |=0x10
#define LED9_ON GPIOC->ODR &=(~0x8)
#define LED9_OFF GPIOC->ODR |=0x8
#define LEDA_ON GPIOC->ODR &=(~0x10)
#define LEDA_OFF GPIOC->ODR |=0x10
#define LEDB_ON GPIOC->ODR &=(~0x20)
#define LEDB_OFF GPIOC->ODR |=0x20
#define LEDC_ON GPIOC->ODR &=(~0x40)
#define LEDC_OFF GPIOC->ODR |=0x40
#define LEDD_ON GPIOC->ODR &=(~0x80)
#define LEDD_OFF GPIOC->ODR |=0x80

#define LED_ON GPIOA->ODR &=(~0xE);GPIOB->ODR &=(~0x30);GPIOC->ODR &=(~0xF8);GPIOD->ODR &=(~0x74);
#define LED_OFF GPIOA->ODR |=0xE;GPIOB->ODR |=0x30;GPIOC->ODR |=0xF8;GPIOD->ODR |=0x74;

#define LED_INIT() GPIOA->DDR |=0xE;GPIOB->DDR |=0x30;GPIOC->DDR |=0xF8;GPIOD->DDR |=0x74;GPIOA->CR1 &=(~0xE);GPIOB->CR1 &=(~0x30);GPIOC->CR1 &=(~0xF8);GPIOD->CR1 &=(~0x74);GPIOA->CR2 |=0xE;GPIOB->CR2 |=0x30;GPIOC->CR2 |=0xF8;GPIOD->CR2 |=0x74;

#define BRIGHT TIM2->CCR2L
#define BRIGHT_HI TIM2->CCR2L=16;
#define BRIGHT_LO TIM2->CCR2L=0;
uint8_t const TIM4_PSCR=0x04;
uint8_t const TIM4_PERIOD=124;

volatile uint16_t idle_timer;
unsigned int TimingDelay; //mSec counter
volatile uint16_t idle_timer=0; //Idle_timer
uint16_t sec_time,led;
uint8_t j=0;
bool sec_flag=0;

//=========Functions and Procedures==========
void sleep_10ms(uint16_t sleep_time) { // go to sleep mode
while(sleep_time) {
sleep_time--;
AWU->APR &=(~0x3F) | 0x30; //AWU->APR &=(~0x3F) | 0x30;
AWU->TBR |=0x4; //AWU->TBR |=0x4;  10 mcек
AWU->CSR |=AWU_CSR_AWUEN ;   //Enable interrupt.
_asm("halt");//Sleep go on
AWU->CSR &=(~AWU_CSR_AWUEN);
LED_INIT();
}
}
void TimingDelayDec(void) { // 1ms interrupt

if (TimingDelay != 0x00) {TimingDelay--;}
if (sec_time) { sec_time--;}
else {sec_time = 1000;sec_flag=1;}
if (sec_flag) { idle_timer++; } // 1 second flag
}
void DelayMs(int Delay_time) {// ms Timer

TimingDelay=Delay_time;
  while(TimingDelay!= 0x00);
}
void initial(void) {//Init GPIO & Timer
CLK->ICKR |= CLK_ICKR_HSIEN;
CLK->PCKENR1 =0;CLK->PCKENR2 =0;
CLK->PCKENR1 |= CLK_PCKENR1_TIM2 | CLK_PCKENR1_TIM4;
CLK->PCKENR2 |= CLK_PCKENR2_AWU;
//=============TIM4============
TIM4->PSCR = TIM4_PSCR;
TIM4->ARR = TIM4_PERIOD;
TIM4->SR1 &=~TIM4_SR1_UIF;
TIM4->IER |= TIM4_IER_UIE;
TIM4->CR1 |= TIM4_CR1_CEN; // Start tim4
_asm("rim");
LED_INIT();
//===========TIM2=================
TIM2->PSCR = 100;
TIM2->ARRH = 0;TIM2->ARRL = ARR_P;
TIM2->CNTRH=0; TIM2->CNTRL=0; //Сброс счетчика
TIM2->CCMR2 |= 0b110<<4 | TIM2_CCMR_OCxPE; //------
TIM2->CCER1 |= TIM1_CCER1_CC2P |TIM2_CCER1_CC2E; //Active polaryty 0
TIM2->CCR2H=0; TIM2->CCR2L=0;

TIM2->CR1 |= TIM2_CR1_CEN; // запустить таймер
TIM2->EGR |= TIM2_EGR_UG;
}
void led_mask(uint16_t leds) {// Leds mask

if(leds & 0x0001) {LED0_ON;}
  else {LED0_OFF;}
if(leds & 0x0002) {LED1_ON;}
  else {LED1_OFF;}
if(leds & 0x0004) {LED2_ON;}
  else {LED2_OFF;}
if(leds & 0x0008) {LED3_ON;}
  else {LED3_OFF;}
if(leds & 0x0010) {LED4_ON;}
  else {LED4_OFF;}
if(leds & 0x0020) {LED5_ON;}
  else {LED5_OFF;}
if(leds & 0x0040) {LED6_ON;}
  else {LED6_OFF;}
if(leds & 0x0080) {LED7_ON;}
  else {LED7_OFF;}
if(leds & 0x0100) {LED8_ON;}
  else {LED8_OFF;}
if(leds & 0x0200) {LED9_ON;}
  else {LED9_OFF;}
if(leds & 0x0400) {LEDA_ON;}
  else {LEDA_OFF;}
if(leds & 0x0800) {LEDB_ON;}
  else {LEDB_OFF;}
if(leds & 0x1000) {LEDC_ON;}
  else {LEDC_OFF;}
if(leds & 0x2000) {LEDD_ON;}
  else {LEDD_OFF;}
}


void flash_on(uint16_t time) {  
uint8_t i=0;
for (i=0;i<ARR_P+1;i++) {BRIGHT = i;DelayMs(time);}
}
void flash_off(uint16_t time) {
uint8_t i=0;
for (i=0;i<ARR_P+1;i++) {BRIGHT= ARR_P-i; DelayMs(time);}
}

void roll_right(uint16_t time,uint16_t leds,uint8_t type) {
uint8_t i;
for (j=0;j<=14;j++)
{ if(!type) {led_mask(0x3FFF & (leds >>(14-j)));}
if(type==1) {led_mask(0x3FFF & (leds >>j));}
if(type==2) {led_mask(0x3FFF & (~(leds >>j)));}
DelayMs(time);
//sleep_10ms(time/10);
}
}
void roll_left(uint16_t time, uint16_t leds,uint8_t type) {
uint8_t i;
for (j=0;j<=14;j++)
{ if(!type) {led_mask(0x3FFF & leds<<j);}
if(type==1) {led_mask(0x3FFF & (leds |=(1<<j)));}
if(type==2) {led_mask(0x3FFF & (~(leds<<j)));}

DelayMs(time);
//sleep_10ms(time/10);
}

}
void roll_2side(uint16_t time,uint16_t leds) {
uint8_t i;
BRIGHT_HI;
for (j=0;j<=14;j++)
{
led_mask(((leds & 0x3F80)>>j) | ((leds&0x7F)<<j));
DelayMs(time);
}
}
void shadow_f_right(uint16_t time,uint16_t leds) {
uint8_t i;
BRIGHT_LO;
for (j=0;j<=14;j++)
{ led_mask(leds>>j);
for (i=0;i<ARR_P+1;i++) {BRIGHT = i;DelayMs(time);}
for (i=0;i<ARR_P+1;i++) {BRIGHT= ARR_P-i; DelayMs(time);}
}
}
void shadow_f_left(uint16_t time, uint16_t leds) {
uint8_t i;
BRIGHT_LO;
for (j=0;j<=14;j++)
{ led_mask(leds<<j);
for (i=0;i<ARR_P+1;i++) {BRIGHT = i;DelayMs(time);}
for (i=0;i<ARR_P+1;i++) {BRIGHT= ARR_P-i; DelayMs(time);}
//sleep_10ms(time/10);
}

}

//============Standart functions===========
void shadow_up(uint16_t tim,uint8_t br) {
BRIGHT=br;
roll_right(tim,0x2000,2); // бегущая тень снизу вверх
}
void shadow_down(uint16_t tim,uint8_t br) {
BRIGHT=br;
roll_left(tim,0x1,2); // бегущая тень сверху вниз
}
void run_up_single(uint16_t tim,uint8_t br) {
BRIGHT=br;
roll_right(tim,0x2000,1); //пробежал снизу вверх
}
void run_down_single(uint16_t tim,uint8_t br) {
BRIGHT=br;
roll_left(tim,0x1,0); // пробежал сверху вниз
}
void on_rise_up(uint16_t tim,uint8_t br) {
BRIGHT=br;
roll_right(tim,0x3FFF,2); // зажглись снизу вверх
}
void off_rise_up(uint16_t tim,uint8_t br) {
BRIGHT=br;
roll_right(tim,0x3FFF,1);// потухли снизу вверх
}
void on_fall_down(uint16_t tim,uint8_t br) {
BRIGHT=br;
roll_left(tim,0x3FFF,2); // зажглись сверху вниз
}
void off_fall_down(uint16_t tim,uint8_t br) {
BRIGHT=br;
roll_left(tim,0x3FFF,0); // потухли сверху вниз
}
//=========================================

//=====================-MAIN-=======================
int main(void) {
initial();

while(1){ //=========== main cycle=================
roll_2side(200,0x2001);
led_mask(0x1089); flash_on(20);flash_off(20); //ROW1
led_mask(0x2312); flash_on(20);flash_off(20); //ROW2
led_mask(0xC64); flash_on(20);flash_off(20); //ROW3
on_rise_up(100,ARR_P);
roll_2side(200,0x2805);
led_mask(0xf); flash_on(20);flash_off(20); //COL1
led_mask(0x01F0); flash_on(20);flash_off(20); //COL2
led_mask(0x3E00); flash_on(20);flash_off(20);//COL3
off_fall_down(100,ARR_P);
led_mask(0x3fff);flash_on(20);flash_off(20);
on_fall_down(100,ARR_P);
BRIGHT_HI;roll_right(100,0x3FFF,0);roll_left(100,0x3FFF,0);BRIGHT_LO;
off_rise_up(100,ARR_P);
shadow_f_right(10,0x2000);
shadow_f_left(10,1);

shadow_f_right(10,0x3FFF);
shadow_f_left(10,0x3FFF);

shadow_f_right(10,0x2AAA);
shadow_f_left(10,0x1555);

run_up_single(100,ARR_P);
BRIGHT_HI;
run_down_single(100,ARR_P);

BRIGHT_LO;sleep_10ms(100);


sec_flag=0; //drop the sec_flag
} // end of mine cycle
}

#ifdef USE_FULL_ASSERT

void assert_failed(u8* file, u32 line)
{
 //printf("Wrong parameters value: file %s on line %d\r\n", file, (int) line);
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/