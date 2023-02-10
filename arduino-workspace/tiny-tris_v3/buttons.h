#ifndef __STEHLO_GAMEBOY_BUTTONS__
#define __STEHLO_GAMEBOY_BUTTONS__


#define PORT_SW_A   PORTB4
#define PORT_SW_B   PORTB3

// these values are updated by the read_inputs function
uint16_t btn_right_btn1_btn2 = 0;
uint16_t btn_up_left_down = 0;
uint16_t prev_btn_right_btn1_btn2 = 0;
uint16_t prev_btn_up_left_down = 0;


uint8_t button_state[] = {0, 0, 0, 0, 0, 0};

#define _BUTTON_UP 0
#define _BUTTON_LEFT 1
#define _BUTTON_DOWN 2
#define _BUTTON_RIGHT 3
#define _BUTTON_BTN1 4
#define _BUTTON_BTN2 5

#define TINYJOYPAD_LEFT  (button_state[_BUTTON_LEFT])
#define TINYJOYPAD_RIGHT (button_state[_BUTTON_RIGHT])
#define TINYJOYPAD_DOWN (button_state[_BUTTON_DOWN])
#define TINYJOYPAD_UP  (button_state[_BUTTON_UP])
#define BUTTON_DOWN (button_state[_BUTTON_BTN1])
#define BUTTON_UP (!button_state[_BUTTON_BTN1])

/*
TINYJOYPAD_LEFT  (analogRead(A0)>=750)&&(analogRead(A0)<950)
TINYJOYPAD_RIGHT (analogRead(A0)>500)&&(analogRead(A0)<750)
TINYJOYPAD_DOWN (analogRead(A3)>=750)&&(analogRead(A3)<950)
TINYJOYPAD_UP  (analogRead(A3)>500)&&(analogRead(A3)<750)
BUTTON_DOWN (digitalRead(1)==0)
BUTTON_UP (digitalRead(1)==1) 
*/


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

void read_stable_adc(uint16_t* adc) {
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

void read_input(uint8_t pin, uint16_t* adc, uint8_t* button_state) {
  change_gameboy_input_channel(pin);
  read_stable_adc(adc);

  if (*adc > 546) {
    button_state[2] = 0;
    if (*adc > 747) {
      button_state[1] = 0;
      if (*adc > 920) button_state[0] = 0;
      else button_state[0] = 1;
    } else {
      button_state[1] = 1;
      if (*adc > 628) button_state[0] = 0;
      else button_state[0] = 1;
    }
  } else {
    button_state[2] = 1;
    if (*adc > 430) {
      button_state[1] = 0;
      if (*adc > 483) button_state[0] = 0;
      else button_state[0] = 1;
    } else {
      button_state[1] = 1;
      if (*adc > 388) button_state[0] = 0;
      else button_state[0] = 1;
    }
  }
}


void read_all_inputs() { 
  read_input(PB3, &btn_up_left_down, button_state);
  read_input(PB4, &btn_right_btn1_btn2, button_state+3);
}

#endif // __STEHLO_GAMEBOY_BUTTONS__