#ifndef __SZTEHLO_GAMEBOY_BUTTONS__
#define __SZTEHLO_GAMEBOY_BUTTONS__


#define PORT_SW_A   PORTB4
#define PORT_SW_B   PORTB3

// these values are updated by the read_inputs function
uint16_t btn_right_btn1_btn2 = 0;
uint16_t btn_up_left_down = 0;

// each bit represents a single button. It is 1 if the button is pushed down, 0 if not pushed down.
uint8_t button_state = 0;
uint8_t button_state_old = 0;

#define _BUTTON_UP_BIT 0
#define _BUTTON_DOWN_BIT 1
#define _BUTTON_LEFT_BIT 2
#define _BUTTON_RIGHT_BIT 3
#define _BUTTON_ACTION_1_BIT 4
#define _BUTTON_ACTION_2_BIT 5

#define SET_BUTTON_BIT_HIGH(BIT) button_state |= (1 << BIT)
#define SET_BUTTON_BIT_LOW(BIT) button_state &= ~(1 << BIT)


#define BUTTON_LEFT     (0x01 & (button_state >> _BUTTON_LEFT_BIT))
#define BUTTON_RIGHT    (0x01 & (button_state >> _BUTTON_RIGHT_BIT))
#define BUTTON_DOWN     (0x01 & (button_state >> _BUTTON_DOWN_BIT))
#define BUTTON_UP       (0x01 & (button_state >> _BUTTON_UP_BIT))
#define BUTTON_ACTION_1 (0x01 & (button_state >> _BUTTON_ACTION_1_BIT))
#define BUTTON_ACTION_2 (0x01 & (button_state >> _BUTTON_ACTION_2_BIT))


#define BUTTON_PUSHED_LEFT     (button_state & ~button_state_old & (0x01 << _BUTTON_LEFT_BIT))
#define BUTTON_PUSHED_RIGHT    (button_state & ~button_state_old & (0x01 << _BUTTON_RIGHT_BIT))
#define BUTTON_PUSHED_DOWN     (button_state & ~button_state_old & (0x01 << _BUTTON_DOWN_BIT))  
#define BUTTON_PUSHED_UP       (button_state & ~button_state_old & (0x01 << _BUTTON_UP_BIT))
#define BUTTON_PUSHED_ACTION_1 (button_state & ~button_state_old & (0x01 << _BUTTON_ACTION_1_BIT))
#define BUTTON_PUSHED_ACTION_2 (button_state & ~button_state_old & (0x01 << _BUTTON_ACTION_2_BIT))


void buttonsInit() {    
  // ADCSRA bits:
  //    ADEN  = 1        : ADC enabled
  //    ADSC  = 1        : ADC conversion started
  //    ADATE = 0        : no auto-trigger
  //    ADIF  = 0        : interrupt flag (set to 1 when ADC completes)
  //    ADIE  = 0        : interrupt disabled
  //    ADPS[2:0] = 000  : ADC prescaler select, use the smallest ADC division
  ADCSRA = 0b10000000;
}

void buttonsChangeAdcInputChannel(uint8_t pin) {
  // REFS[2..0 ]= 0    : VCC used as voltage reference (disconnected from AREF/PB0)
  // ADLAR = 0         : Left-adjust the results
  // MUX[3..0] = 0011  : single ended input, 0011 for PB3, 0010 for PB4 and 0000 for PB5
  if(pin == PB3) {
    ADMUX = 0b00000011;
  } else {
    ADMUX = 0b00000010;
  }
}

void buttonReadStableAdc(uint16_t* adc) {
  uint16_t old_adc = *adc;
  while(1) {
    uint16_t old_adc = *adc;

    // start the ADC conversion
    ADCSRA |= (1 << ADSC); 

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

void buttonReadInput(uint8_t pin, uint16_t* adc, uint8_t bit0, uint8_t bit1, uint8_t bit2) {
  buttonsChangeAdcInputChannel(pin);
  buttonReadStableAdc(adc);

  if (*adc > 546) {
    SET_BUTTON_BIT_LOW(2);
    if (*adc > 747) {
      SET_BUTTON_BIT_LOW(1);
      if (*adc > 920) SET_BUTTON_BIT_LOW(0);
      else SET_BUTTON_BIT_HIGH(0);
    } else {
      SET_BUTTON_BIT_HIGH(1);
      if (*adc > 628) SET_BUTTON_BIT_LOW(0);
      else SET_BUTTON_BIT_HIGH(0);
    }
  } else {
    SET_BUTTON_BIT_HIGH(2);
    if (*adc > 430) {
      SET_BUTTON_BIT_LOW(1);
      if (*adc > 483) SET_BUTTON_BIT_LOW(0);
      else SET_BUTTON_BIT_HIGH(0);
    } else {
      SET_BUTTON_BIT_HIGH(1);
      if (*adc > 388) SET_BUTTON_BIT_LOW(0);
      else SET_BUTTON_BIT_HIGH(0);
    }
  }
}

void buttonReadAllInputs() { 
  button_state_old = button_state;
  buttonReadInput(PB3, &btn_up_left_down, _BUTTON_UP_BIT, _BUTTON_LEFT_BIT, _BUTTON_DOWN_BIT);
  buttonReadInput(PB4, &btn_right_btn1_btn2, _BUTTON_RIGHT_BIT, _BUTTON_ACTION_1_BIT, _BUTTON_ACTION_2_BIT);
}

#endif // __SZTEHLO_GAMEBOY_BUTTONS__