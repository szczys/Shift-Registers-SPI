/********************************
* Driving shift registers using	*
* an AVR chip's hardwarre SPI	*
*				*
* http://jumptuck.com 		*
*				*
********************************/

#include <avr/io.h>
#include <util/delay.h>


#define SHIFT_REGISTER DDRB
#define SHIFT_PORT PORTB
#define DATA (1<<PD3)		//MOSI (SI)
#define LATCH (1<<PB2)		//SS   (RCK)
#define CLOCK (1<<PB5)		//SCK  (SCK)

void init_IO(void){
  //Setup IO
  SHIFT_REGISTER |= (DATA | LATCH | CLOCK);	//Set control pins as outputs
  SHIFT_PORT &= ~(DATA | LATCH | CLOCK);		//Set control pins low
}

void init_SPI(void){
  //Setup SPI
  SPCR = (1<<SPE) | (1<<MSTR);	//Start SPI as Master
}

void spi_send(unsigned char byte){
  SPDR = byte;			//Shift in some data
  while(!(SPSR & (1<<SPIF)));	//Wait for SPI process to finish
}

int main(void)
{
  init_IO();
  init_SPI();
 
  unsigned int binary_counter = 0;

  while(1)			
  {
    //Pull LATCH low (Important: this is necessary to start the SPI transfer!)
    SHIFT_PORT &= ~LATCH;

    spi_send((unsigned char)(binary_counter>>8));
    spi_send((unsigned char)binary_counter);
    

    //Toggle latch to copy data to the storage register
    SHIFT_PORT |= LATCH;
    SHIFT_PORT &= ~LATCH;

    //increment the counter for next time
    binary_counter++;

    //wait for a little bit before repeating everything
    _delay_ms(200);
  }
}
