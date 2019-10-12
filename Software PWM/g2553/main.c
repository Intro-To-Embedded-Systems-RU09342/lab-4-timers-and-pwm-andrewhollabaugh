/*
 * Software PWM program for MSP430G2553
 * Fades LED0 using software PWM, button press changes fade amount
 * Turns on LED1 when button is pressed
 *
 * Created: 10/12/19
 * Last Edited: 10/12/19
 * Author: Andrew Hollabaugh
 */

#include <msp430.h> //msp identifiers

//use P1.0 for led0; define generic names for gpio registers and gpio pin
#define LED0_DIR_R P1DIR
#define LED0_OUT_R P1OUT
#define LED0_PIN 0

//use P1.6 for led1; define generic names for gpio registers and gpio pin
#define LED1_DIR_R P1DIR
#define LED1_OUT_R P1OUT
#define LED1_PIN 6

//use P1.3 for button; define generic names for gpio registers and gpio pin
#define BUTTON_DIR_R P1DIR
#define BUTTON_IN_R P1IN
#define BUTTON_PIN 3

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //stop watchdog timer

    //led setup
    LED0_DIR_R |= (1 << LED0_PIN); //set led direction to output
    LED0_OUT_R &= ~(1 << LED0_PIN); //ensure LED is off
    LED1_DIR_R |= (1 << LED1_PIN); //set led direction to output
    LED1_OUT_R &= ~(1 << LED1_PIN); //ensure LED is off

    //button setup
    BUTTON_DIR_R &= ~(1 << BUTTON_PIN); //set button direction to input
    P1IFG = 0x00; //clear interrupt flags
    P1IES |= (1 << BUTTON_PIN); //set interrupt to be generated on rising edge of signal (button is pressed)
    P1IE |= (1 << BUTTON_PIN); //enable interrupt
  
    //timer0 setup, no prescaling, used for debouncing
    TA0CTL |= TASSEL_2; //select SMCLK as clk src
    TA0CTL |= MC_2; //set to continuous mode
    TA0CCR0 = 0x1388; //set value timer will reset at; 5ms debounce time
    TA0CCTL0 |= CCIE; //enable compare 0 interrupt
    TA0CTL |= TAIE; //enable timer interrupts

    //timer1 setup, no prescaling, used for PWM
    TA1CTL |= TASSEL_2; //select SMCLK as clk src
    TA1CTL |= MC_1; //set to up mode
    TA1CCR0 = 999; //top value for 1 kHz pulse
    TA1CCR1 = 499; //start at 50% duty cycle
    TA1CCTL0 |= CCIE; //enable compare 0 interrupt
    TA1CCTL1 |= CCIE; //enable compare 1 interrupt
    TA1CTL |= TAIE;
    
    __bis_SR_register(LPM0_bits + GIE); //enter low power mode and enable interrupts

    while(1); //loop infinitely
}

//interrupt vector for button interrupt, debounce
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
{
    P1IFG &= ~(1 << BUTTON_PIN); //clear interrupt flag

    TA0R = 0; //reset timer
    TA0CTL &= ~MC_3; //start timer in continuous mode
    TA0CTL |= MC_2;
}

//interrupt vector for timerA0, CCR0
void __attribute__((interrupt(TIMER0_A0_VECTOR))) timer0_A0 (void)
{
    TA0CTL &= ~TAIFG; //clear interrupt flag

    TA0CTL &= ~MC_3; //stop debounce timer

    //button is pressed
    if(!(BUTTON_IN_R & (1 << BUTTON_PIN)) && (P1IES & (1 << BUTTON_PIN)))
    {
        P1IES &= ~(1 << BUTTON_PIN); //change button interrupt edge slope so interrupt is generated on next edge
        LED1_OUT_R |= (1 << LED1_PIN); //turn on led1

        if(TA1CCR1 <= 900) //check if duty cycle can be further increased
        {
            TA1CCR1 += 100; //increase duty cycle
            TA1CTL |= MC_1; //set timer to up mode
        }
        else
        {
            TA1CCR1 = 0; //set duty cycle back to 0
            LED1_OUT_R &= ~(1 << LED1_PIN); //turn off led0
            TA1CTL &= ~MC_3; //stop timer
        }
    }
    //button is released
    if((BUTTON_IN_R & (1 << BUTTON_PIN)) && !(P1IES & (1 << BUTTON_PIN)))
    {
        P1IES |= (1 << BUTTON_PIN); //change button interrupt edge slope
        LED1_OUT_R &= ~(1 << LED1_PIN); //turn off led1
    }
}

/*
 * timer0 interrupts - software PWM for led0
 * two compare registers, CCR0 stays at 999, CCR1 changes based on desired duty cycle
 * CCR0 interrupt turns led on, CCR1 interrupt turns led off
*/
//interrupt vector for timerA1, CCR0
void __attribute__((interrupt(TIMER1_A0_VECTOR))) timer1_A0 (void)
{
    TA1CTL &= ~TAIFG; //clear interrupt flag
    LED0_OUT_R |= (1 << LED0_PIN); //turn on led0
}

//interrupt vector for timerA1, CCR1
void __attribute__((interrupt(TIMER1_A1_VECTOR))) timer1_A1 (void)
{
    if(TA1IV == 0x02) //if interrupt was generated from CCR1 compare
        LED0_OUT_R &= ~(1 << LED0_PIN); //turn off led0
}

