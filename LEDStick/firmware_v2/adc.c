#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint16_t adc_value = 0;

ISR(ADC_vect)
{
    adc_value = ADC;
}

    


//*****************************************************************************
// 
void ADC_Init (void)
{
	DIDR0 |= (1<<ADC3D);
	ADMUX |= (1<<REFS1)|(1<<REFS0)|(1<<MUX1)|(1<<MUX0);
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
