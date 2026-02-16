
#ifndef PORT_MEGA328P_H
#define PORT_MEGA328P_H

#ifdef __AVR_ATmega32__

#define port_sys_tick_cb TIMER1_COMPA_vect

#define port_init_systick_timer(duration) \
  do { \
    TCCR1B = (1<<WGM12);  /*CTC mode*/ \
    TIMSK |= (1<<OCIE1A); /*enable output compare interrupt for channel a*/ \
    TCNT1 = 0;            /*reset timer counter*/ \
    OCR1A = duration;     /*set the top of CTC on channel a*/ \
    TCCR1B |= (1<<CS10);  /*cs2..0=0b001, activate timer, 1:1 pre-scaler*/ \
  } while (0)

#define PORT_UDRE_VECT USART_UDRE_vect

#define PORT_RXC_VECT USART_RXC_vect

#define PORT_UDR UDR

#define port_set_uart_char_size(n) \
  if (n > 5) { \
  if (n == 9) UCSRC = (1<<URSEL) | (3<<UCSZ0); \
  else UCSRC = (1<<URSEL) | ((n-5)<<UCSZ0); }

#define port_set_uart_baud_rate(b) \
  do { \
    UBRRL = baud; \
    UBRRH = baud >> 8; \
  } while (0)

#define port_uart_init() \
  do { \
    UCSRB = (1<<RXCIE) | (1<<RXEN) | (1<<TXEN); \
    UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0); \
  } while (0)

#define port_enable_udre_interrupt() \
  UCSRB |= (1<<UDRIE)

#define port_disable_udre_interrupt() \
  UCSRB &= ~(1<<UDRIE)

#define port_enable_rxc_interrupt() \
  UCSRB |= (1<<RXCIE)

#define port_disable_rxc_interrupt() \
  UCSRB &= ~(1<<RXCIE)

#endif

#endif /* PORT_MEGA328P_H */
