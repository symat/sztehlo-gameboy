
#define PORT_SW_A   PORTB4
#define PORT_SW_B   PORTB3

// these values are updated by the read_inputs function
uint16_t btn_right_btn1_btn2 = 0;
uint16_t btn_top_left_down = 0;


void init_gameboy_inputs() {    
  // ADCSRA bits:
  //    ADEN  = 1        : ADC enabled
  //    ADSC  = 1        : ADC conversion started
  //    ADATE = 0        : no auto-trigger
  //    ADIF  = 0        : interrupt flag (set to 1 when ADC completes)
  //    ADIE  = 0        : interrupt disabled
  //    ADPS[2:0] = 000  : ADC prescaler select, use the smallest ADC division
  ADCSRA = 0b10000000;
}

void change_gameboy_input_channel(uint8_t pin) {
  // REFS[2..0 ]= 0    : VCC used as voltage reference (disconnected from AREF/PB0)
  // ADLAR = 0         : Left-adjust the results
  // MUX[3..0] = 0011  : single ended input, 0011 for PB3, 0010 for PB4 and 0000 for PB5
  if(pin == PB3) {
    ADMUX = 0b00000011;
  } else {
    ADMUX = 0b00000010;
  }
}



void read_inputs(uint8_t pin, uint16_t* adc) {
  change_gameboy_input_channel(pin);

  while(1) {
    uint16_t old_adc = *adc;

    ADCSRA |= (1 << ADSC); // start the ADC conversion

    // wait for ADC
    while((ADCSRA & (1 << ADSC)) > 0) ;

    // read ADC value
    uint8_t adc_low8bit = ADCL;
    *adc = ((ADCH<<8)|adc_low8bit) % 1024; 

    // the adc value doesn't change 'immediately', also there is some bouncing due to the push button
    // so we repeat the read until the adc value stabilize
    if(old_adc != *adc) delay(1);
    else break;
  }
}


void read_all_inputs() { 
  read_inputs(PB3, &btn_top_left_down);
  read_inputs(PB4, &btn_right_btn1_btn2);
  // update_state();


}

