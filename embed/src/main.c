#include <msp430g2553.h>
#include <stdint.h>

#define SHIFT_SRCLK BIT0  // P2.0: shift register, shift register clock
#define SHIFT_RCLK  BIT1  // P2.1: shift register, storage register clock
#define SHIFT_SER   BIT2  // P2.2: shift register, serial data input

static void display(unsigned int digit, unsigned int number);

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  // stop watchdog timer

    P1DIR = 0xFF;  // set all port 1 to output
    P1OUT = 0;     // set all port 1 outputs to low
    P2DIR = 0xFF;  // set all port 2 to output
    P2OUT = 0;     // set all port 2 outputs to low

    DCOCTL = CALDCO_8MHZ;            // select frequency range
    BCSCTL1 = CALBC1_8MHZ | DIVA_0;  // select frequency, ALCK = MCLK

    P2DIR |= 0x18 | SHIFT_RCLK | SHIFT_SRCLK | SHIFT_SER;

    volatile unsigned int i;
    unsigned int k = 0;

    while(1)
    {
        display(3, k);
        k = (k == 9) ? 0 : k + 1;
        for(i=100000; i>0; i--);
    }

}

void display(unsigned int digit, unsigned int number) {
    uint8_t byte2send = (1 << (4 + digit)) | number;

    unsigned int i;
    for (i = 8; i; --i) {
        P2OUT &= ~SHIFT_SRCLK;                // SRCLK LOW
        if ((byte2send >> (i - 1)) & BIT0) {
            P2OUT |= SHIFT_SER;               // SHIFT_SER HIGH
        }
        else {
            P2OUT &= ~SHIFT_SER;              // SHIFT_SER LOW
        }
        P2OUT |= SHIFT_SRCLK;                 // SRCLK HIGH
    }
    P2OUT |= SHIFT_RCLK;                      // RCLK HIGH
    P2OUT &= ~SHIFT_RCLK;                     // RCLK LOW
}
