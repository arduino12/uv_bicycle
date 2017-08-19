/* auto_cts.ino
	Pull cts line high when uart communication is detected, so uart data will be bufferd.
	Then pull cts line low and ignore uart communication, so all the bufferd uart data will be sent at once */

#define SERIAL_CTS_PIN				(2)
#define SERIAL_CTS_ON_MS			(100)
#define SERIAL_CTS_OFF_MS			(50)
#define SERIAL_BAUDRATE				(115200)
#define SERIAL_BAUDRATE_PRESCALE	(F_CPU / 16 / SERIAL_BAUDRATE - 1)


/* flag to indicate uart communication */
volatile bool rx_detected = false;;


ISR(USART_RX_vect){
	/* must read UDR0 to clear interrupt */
    uint8_t rx_data = UDR0;
    rx_detected = true;
}

void setup() {
	/* set cts line as output */
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(SERIAL_CTS_PIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	digitalWrite(SERIAL_CTS_PIN, LOW);
	/* disable global interrupts */
	cli();
	/* set uart baudrate */
    UBRR0L = SERIAL_BAUDRATE_PRESCALE & 0xFF;
    UBRR0H = SERIAL_BAUDRATE_PRESCALE >> 8;
	/* enable uart rx interrupt */
    UCSR0B = _BV(RXCIE0) | _BV(RXEN0);
	/* set uart config: 8 data bits (no parity bit, 1 stop bit) */
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
	/* enable global interrupts */
	sei();
}

void loop() {
	uint8_t state = 0;
	uint32_t cur_ms;
	uint32_t state_ms;
	
	while (true) {
		cur_ms = millis();

		switch (state) {
			case 0: /* idle state: wait for uart communication */
				if (rx_detected) {
					digitalWrite(SERIAL_CTS_PIN, HIGH);
					digitalWrite(LED_BUILTIN, HIGH);
					state_ms = cur_ms + SERIAL_CTS_ON_MS;
					state = 1;
				}
			break;
			case 1: /* cts state: wait befor pulling cts line low */
				if (cur_ms > state_ms) {
					digitalWrite(SERIAL_CTS_PIN, LOW);
					state_ms = cur_ms + SERIAL_CTS_OFF_MS;
					state = 2;
				}
			break;
			case 2: /* ignore state: allow uart communication befor going back to idle state */
				if (cur_ms > state_ms) {
					digitalWrite(LED_BUILTIN, LOW);
					rx_detected = false;
					state = 0;
				}
			break;
		}
	}
}
