/*
 * Button debounce program for MSP430FR6989
 * Toggles state of LED when button is pressed, uses software to debounce button press
 *
 * Created: 10/12/19
 * Last Edited: 10/12/19
 * Author: Andrew Hollabaugh
 */

#include <msp430.h> //msp identifiers

//use P1.0 for led; define generic names for gpio registers and gpio pin
#define LED_DIR_R P1DIR
#define LED_OUT_R P1OUT
#define LED_PIN 0

//use P1.1 for button; define generic names for gpio registers and gpio pin
#define BUTTON_DIR_R P1DIR
#define BUTTON_OUT_R P1OUT
#define BUTTON_REN_R P1REN
#define BUTTON_IN_R P1IN
#define BUTTON_PIN 1

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //disable the GPIO power-on default high-impedance mode

    //led setup
    LED_DIR_R |= (1 << LED_PIN); //set led direction to output
    LED_OUT_R &= ~(1 << LED_PIN); //ensure LED is off

    //button setup
    BUTTON_DIR_R &= ~(1 << BUTTON_PIN); //set button direction to input
    BUTTON_REN_R |= (1 << BUTTON_PIN); //enable pullup resistor
    BUTTON_OUT_R |= (1 << BUTTON_PIN); //enable pullup resistor
    P1IFG = 0x00; //clear interrupt flags
    P1IES |= (1 << BUTTON_PIN); //set interrupt to be generated on rising edge of signal (button is pressed)
    P1IE |= (1 << BUTTON_PIN); //enable interrupt
  
    //timer0 setup, no prescaling
    TA0CTL |= TASSEL_2; //select SMCLK as clk src
    TA0CTL |= MC_2; //set to continuous mode
    TA0CCR0 = 0x1388; //set value timer will reset at; about 5ms debounce time
    TA0CCTL0 |= CCIE; //enable compare 0 interrupt
    TA0CTL |= TAIE; //enable timer interrupts
    
    __bis_SR_register(LPM0_bits + GIE); //enter low power mode and enable interrupts

    while(1); //loop infinitely
}

//interrupt vector for timerA0, CCR0
void __attribute__((interrupt(TIMER0_A0_VECTOR))) Timer_A0 (void)
{
    TA0CTL &= ~TAIFG; //clear interrupt flag

    TA0CTL &= ~MC_3; //stop timer

    if(!(BUTTON_IN_R & (1 << BUTTON_PIN)))
        LED_OUT_R ^= (1 << LED_PIN); //toggle LED output using XOR
}

//interrupt vector for button interrupt
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
{
    P1IFG &= ~(1 << BUTTON_PIN); //clear interrupt flag

    TA0R = 0; //reset timer
    TA0CTL &= ~MC_3; //start timer in continuous mode
    TA0CTL |= MC_2;
}

