/************************************************************* 
 This sketch makes the pin PB4 toggle every second
 (if you have a 16MHz crystal attached). It uses Timer0
 or Timer0B, and divide the clock frequncy by 1024.
 The divided frequencys period is multiplied with the
 number og counts Timer0/Timer0B can take before it overflows.
 The number is then multiplied by 61, and gives approximately
 1 second.  

 16MHz / 1024 = 15625 hz        We divide the 16 MHz clock by 1024
 1/15625 = 0.000064s            Period time
 256 * 0.000064 = 0.016384      
 0.016384 * 61 = 0.999424 ≈ 1s   
 X = 0.999424 / 0.016384 where X is approximately 61
**************************************************************/ 


#include <avr/io.h>

unsigned int timeCount;


int main (void)
{
  DDRB |= 0x10; //Set PB4 as output, ignore the rest

  #if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)  
    TCCR0 = 0x07; // clock frequency / 1024 
    OCR0 = 0x00;  // Output compare
    TCNT0 = 0; // Start to count from zero
    TIMSK = 0x01; // Enable overflow interrupt

  #elseif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
    TCCR0B = 0x05; // clock frequency / 1024 
    OCR0B = 0x00;  // Output compare
    TCNT0 = 0; // Start to counter 0 to zero
    TIMSK0 = 0x01; // Enable overflow interrupt
  #endif
  
  sei(); //Enable global interrupts
  
  while(1); // Infinite loop  
  
}



ISR(TIMER0_OVF_vect) //Timer 0 overflow vector - this run every time timer0 overflows
{
  timeCount++;
  if(timeCount == 61) //Timer overflown for the 61th time
  {
    PORTB ^= 0x10; //toggle PB4
    timeCount = 0;
  }
}

