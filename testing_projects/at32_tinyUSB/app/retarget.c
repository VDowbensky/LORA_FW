#include "retarget.h"

static bool initialized = false;    /**< Initialize UART/LEUART */

extern uint32_t Receive_length;
volatile int txCount = 0;

int RETARGET_WriteChar(char c)
{
	uint16_t next = (cdc_tx_ring.head + 1) % cdc_tx_ring.size;
	// якщо буфер ќ—№-ќ—№ ѕ≈–≈ѕќ¬Ќ»“№—я (залишилос€ мало м≥сц€)
	// або €кщо в≥н уже повний (next == tail)
	while (next == cdc_tx_ring.tail) 
	{
		// ѕримусово виштовхуЇмо дан≥ в USB, зв≥льн€ючи м≥сце в к≥льцевому буфер≥
		flush_tx_to_usb();
	}
	// “епер м≥сце точно Ї, безпечно пушимо з вимкненн€м переривань
	__disable_irq();
	ringbuf_push(&cdc_tx_ring, (uint8_t)c);
	__enable_irq();
	return c;
}


void RETARGET_Init(void)
{
#if !defined(__CROSSWORKS_ARM) && defined(__GNUC__)
  setvbuf(stdout, NULL, _IONBF, 0); 
	setvbuf(stdin, NULL, _IONBF, 0);
#endif
  initialized = true;
}

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

PUTCHAR_PROTOTYPE
{
	RETARGET_WriteChar((uint8_t)ch);
	return ch;
}

int RETARGET_ReadChar(void)
{
	__disable_irq();
	int ret = ringbuf_pop(&cdc_rx_ring);
	__enable_irq();
	return ret;
}

//int RETARGET_WriteChar(char c)
//{
//	__disable_irq(); 
//	ringbuf_push(&cdc_tx_ring,c);
//	__enable_irq(); 
//	return c;
//}

int stdout_putchar(int c, FILE * stream)
{
	RETARGET_WriteChar(c);
	return c; //return the character written to denote a successfull write
}

int stdin_getchar(FILE * stream)
{
	char c = RETARGET_ReadChar();
	return c;
}

int fgetc(FILE *f)
{
	int ch;
	// „екаЇмо, поки в к≥льцевому буфер≥ з'€витьс€ символ з USB
	while ((ch = RETARGET_ReadChar()) == -1) tud_task(); // ѕрокручуЇмо USB стек п≥д час оч≥куванн€
	return ch;
}



