#include "main.h"

volatile uint16_t boxCount[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile enum states state = MAIN;
volatile uint8_t prepumpPipe = 0;
volatile uint8_t cryoPipe = 0;
volatile int32_t numberHolder = 0;
volatile uint16_t potentiom = 0;


void init(void) {

   // Set MOSI, SCK, PB0, PB1, PB2 as output
   // PB4 has to set to output for the SPI to work
   DDRB = (1<< PB4) | (1<<PB5)|(1<<PB7) | (1<<PB0)|(1<<PB1)|(1<<PB2);

   // Set output ports
   DDRC = 0xFF;

   // Set PWM outputs (TODO)
   //DDRD = (1<<PD7)|(0 << PD6);
   //PORTD = (1 << PD6);
   // Initialize SPI command register
   SPCR = (0<<SPIE)|(1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<CPOL)|(0<<SPR1)|(0<<SPR0);
   SPSR = (1<<SPI2X);

   // Initialize ADC
   ADCSRA = (1 << ADEN)|(0 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
   // Enable global interrupts
   sei();

}

void blinkLED(void) {
   PORTC |= (1 << PC0);
   _delay_ms(50);
   PORTC &= ~(1 << PC0);
   _delay_ms(50);
}

void testTouch(void) {
   uint16_t xpos = 0, ypos = 0, z = 0;


   while(1) {
      // Flashes LED and induces delay in loop
      //blinkLED();

      drawCross(xpos, ypos, COLOR_BG);

      z = readZ();
      fillRect(20, 8, 32, 8, COLOR_BG);
      printNum(20, 8, z, COLOR_FG);
      if(z > 500) {
         xpos = readX();
         ypos = readY();
      } else {
         xpos = 0;
         ypos = 0;
      }


      drawCross(xpos, ypos, COLOR_BG);
      drawTestUI();
      checkBoxBounds(xpos-512, ypos-512);
      printToBoxes();
      if(xpos > 0) {
         printNum(224, 24, xpos-512, COLOR_FG);
      }

      if(ypos > 0) {
         printNum(264, 24, ypos-512, COLOR_FG);   
      }

      if(boxCount[0] > boxCount[1]) {
         printStr(90, 24, "Test string", 11, COLOR_FG);
      } else {
         printStr(90, 24, "           ", 11, COLOR_FG);
      }
      
      printNum(24, 24, z, 0x0000);
   }
}

void testGUI(void) {
   uint16_t xpos = 0, ypos = 0, z = 0;
   uint8_t redraw = 1, pressed = 0;
   //uint8_t i = 0, j = 0;



   while(1) {
      
      if(redraw) {
         drawScreen();
         redraw = 0;
      }

      z = readZ();

      if(z > 300 && !pressed) {
         // xpos and ypos values smaller than 512 (maybe 500) should not be allowed! Put do{}while()-loop around both and sample until legit value received?
         xpos = readX();
         ypos = readY();
         redraw = 1;
         pressed = 1;
         printNum(240, 80, xpos, COLOR_FG);
         printNum(240, 90, ypos, COLOR_FG);
         buttons(xpos - 512, ypos - 512);
      } else {
         xpos = 0;
         ypos = 0;
      }

      if(z < 200) {
         pressed = 0;
      }

/*
      pidWrite(i);
      j++;
      if(j == 12) {
         j = 0;
         i++;
         i = i % 256;
      }
*/

      //_delay_ms(1);
   }
}
/*
void pidLoop(void) {
   uint16_t xpos = 0, ypos = 0, z = 0;
   uint8_t redraw = 1, pressed = 0;
   uint16_t i = 0;

   pidInit();

   //pidCh1Write(128);
   //pidCh2Write(64);
   while(1) {
      
      if(redraw) {
         drawCurrentState(COLOR_FG);
         redraw = 0;
      }

      z = readZ();

      if(z > 500 && !pressed) {
         xpos = readX();
         ypos = readY();
         redraw = 1;
         pressed = 1;
         pidButtons(xpos - 512, ypos - 512);
      } else {
         xpos = 0;
         ypos = 0;
      }

      if(z < 200) {
         pressed = 0;
      }

      //potentiom = pidRead();
      readPos();
      redraw = 1;

      adjustHeading();
      //adjustHeadingSimple();

      pidWrite(i);
      j++;
      if(j == 12) {
         j = 0;
         i++;
         i = i % 256;
      }

      _delay_ms(1);
   }
}*/

int main (void)
{

	init();
   PORTC |= (1 << PC4);
   _delay_ms(300);
   PORTC &= ~(1 << PC4);
   _delay_ms(300);

   tft_reset();

   cls(COLOR_BG);

   //pidLoop();
   testGUI();
   //testTouch();
}
