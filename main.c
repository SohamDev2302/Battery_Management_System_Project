#include <xc.h>

#define _XTAL_FREQ 20000000

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define GREEN_LED RB0
#define RED_LED   RB1
#define BUZZER    RB2
#define CHG_CTRL RB3
#define DSG_CTRL RB4

#define BATT_OVERVOLTAGE   860   // about 4.2V
#define BATT_UNDERVOLTAGE  614   // about 3.0V

void adc_init(void)
{
    ADCON0 = 0x41;   // ADC ON, AN0 selected
    ADCON1 = 0x8E;   // Right justified, AN0 analog, Vref = Vdd/Vss
}

unsigned int adc_read(void)
{
    __delay_ms(2);
    GO_nDONE = 1;
    while(GO_nDONE);

    return (((unsigned int)ADRESH << 8) | ADRESL);
}

void main(void)
{
    unsigned int batt_adc;

    TRISA = 0xFF;   // PORTA input
    TRISB = 0x00;   // PORTB output
    PORTB = 0x00;

    adc_init();

    while(1)
    {
        batt_adc = adc_read();

        if(batt_adc > BATT_OVERVOLTAGE)
        {
            GREEN_LED = 0;
            RED_LED   = 1;
            BUZZER    = 0;
            CHG_CTRL  = 0;   // block charging
            DSG_CTRL  = 1;   // allow discharging
        }
        else if(batt_adc < BATT_UNDERVOLTAGE)
        {
            GREEN_LED = 0;
            RED_LED   = 0;
            BUZZER    = 1;
            CHG_CTRL  = 1;   // allow charging
            DSG_CTRL  = 0;   // block discharging
        }
        else
        {
            GREEN_LED = 1;
            RED_LED   = 0;
            BUZZER    = 0;
            CHG_CTRL  = 1;   // allow charging
            DSG_CTRL  = 1;   // allow discharging
        }

        __delay_ms(200);
    }
}