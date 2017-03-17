#include "touch.h"


uint16_t readX(void) {
	uint32_t xs;
	uint16_t samples[NUMSAMPLES];
	uint8_t i;

	DDRA = (0 << XP)|(1 << YP)|(0 << XM)|(1 << YM);
	PORTA &= ~(1 << XP);
	PORTA &= ~(1 << XM);

	PORTA |= (1 << YP);
	PORTA &= ~(1 << YM);

	// Read from channel X+ (ADC3 here)
	ADMUX = (0 << REFS1)|(1 << REFS0)|(0 << ADLAR)|(1 << MUX1)|(1 << MUX0);

	_delay_us(20);


	for(i = 0; i < NUMSAMPLES; i++) {
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & (1 << ADSC));
		samples[i] = ADC;
	}

	int16_t diff = samples[1] - samples[0];

	if(diff < 4 && diff > -4) {
		xs = (samples[0] + samples[1]) >> 1; // Average 2 samples
		xs += 1195;
		xs *= 323;
		xs /= 820;

		return (uint16_t) xs;
	} else {
		return 0;
	}

	

}

uint16_t readY(void) {
	uint32_t ys;
	uint16_t samples[NUMSAMPLES];
	uint8_t i;

	DDRA = (1 << XP)|(0 << YP)|(1 << XM)|(0 << YM);
	PORTA &= ~(1 << YP);
	PORTA &= ~(1 << YM);

	PORTA |= (1 << XP);
	PORTA &= ~(1 << XM);

	// Read from channel Y+ (ADC2 here)
	ADMUX = (0 << REFS1)|(1 << REFS0)|(0 << ADLAR)|(1 << MUX1)|(0 << MUX0);

	_delay_us(20);


	for(i = 0; i < NUMSAMPLES; i++) {
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & (1 << ADSC));
		samples[i] = ADC;
	}

	int16_t diff = samples[1] - samples[0];

	if(diff < 5 && diff > -5) {
		samples[1] = (samples[0] + samples[1]) >> 1; // Average 2 samples
		ys = (1023 - samples[1]);
		ys += 1331;
		ys *= 243;
		ys /= 710;

		return (uint16_t) ys;
	} else {
		return 0;
	}

	

}

uint16_t readZ(void) {
	int32_t z1, z2, z;

	DDRA = (1 << XP)|(0 << YP)|(0 << XM)|(1 << YM);
	PORTA &= ~(1 << XP);
	PORTA |= (1 << YM);
	PORTA &= ~(1 << YP);

	// Read from channel X- (ADC1 here)
	ADMUX = (0 << REFS1)|(1 << REFS0)|(0 << ADLAR)|(0 << MUX1)|(1 << MUX0);
	_delay_us(20);


	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	z1 = ADC;

	// Read from channel Y+ (ADC2 here)
	ADMUX = (0 << REFS1)|(1 << REFS0)|(0 << ADLAR)|(1 << MUX1)|(0 << MUX0);
	_delay_us(20);


	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	z2 = ADC;
	

	z = (1023 - (z2-z1));
	return (uint16_t) z;

}

uint8_t isInXSpan(uint16_t xp, uint16_t xspan, uint16_t wspan) {
	if(xp > xspan && xp < (xspan + wspan)) {
		return 1;
	} else {
		return 0;
	}
}

uint8_t isInYSpan(uint16_t yp, uint16_t yspan, uint16_t hspan) {
	if(yp > yspan && yp < (yspan + hspan)) {
		return 1;
	} else {
		return 0;
	}
}

uint8_t isInBox(uint16_t xp, uint16_t yp, uint16_t xbox, uint16_t ybox, uint16_t wbox, uint16_t hbox) {

	uint8_t xbool = isInXSpan(xp, xbox, wbox);
	uint8_t ybool = isInYSpan(yp, ybox, hbox);

	return (xbool & ybool);

}


// TODO Rewrite without buffers
void checkBoxBounds(uint16_t xp, uint16_t yp) {

	uint8_t i, j, tmp;

	for(i = 0; i < 2; i++) {
		
		for(j = 0; j < 4; j++) {

			tmp = isInBox(xp, yp, 20+j*70, 36+i*92, 70, 92);
				
			if(tmp) {
				boxCount[(i*4)+j]++;
			}

		}

	}

}