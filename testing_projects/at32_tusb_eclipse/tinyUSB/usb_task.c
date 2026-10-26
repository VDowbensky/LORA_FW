#include "usb_task.h"

static void cdc_task(void);

uint8_t tud_rx_buffer[TUD_RXBUFSIZE];
volatile uint32_t tud_rxcount = 0;

// Допоміжна функція для швидкого проштовхування TX даних в USB
void flush_tx_to_usb(void)
{
	if (tud_cdc_connected()) 
	{
		uint8_t usb_tx_buf[64]; // Розмір пакета Full-Speed
		uint32_t bytes_to_send = 0;
		// Забираємо байти, скільки влізе в пакет
		while (bytes_to_send < sizeof(usb_tx_buf)) 
		{
			int32_t ch = ringbuf_pop(&cdc_tx_ring);
			if (ch == -1) break;
			usb_tx_buf[bytes_to_send++] = (uint8_t)ch;
		}
		if (bytes_to_send > 0) 
		{
			tud_cdc_write(usb_tx_buf, bytes_to_send);
			tud_cdc_write_flush();
		}
	}
	// Прокручуємо внутрішній стек
	tud_task(); 
}

void tud_mount_cb(void) 
{
  //blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void) 
{
  //blink_interval_ms = BLINK_NOT_MOUNTED;
}

static void cdc_task(void) 
{
// connected() check for DTR bit
// Most but not all terminal client set this when making connection
// if(!tud_cdc_n_connected(0)) return;
	uint8_t buf[64];
	if(tud_cdc_n_available(0))
	{
		uint32_t cnt = tud_cdc_n_read(0,buf,64);
		//tud_cdc_n_read_flush(0);
		for(uint32_t i = 0; i < cnt; i++) ringbuf_push(&cdc_rx_ring,buf[i]);
	}
	
	//send pending TX data
	uint16_t txcnt = 0;
	while(1)
	{
		tud_cdc_n_write_flush(0);
		int32_t v = ringbuf_pop(&cdc_tx_ring);
		if(v == -1) break;
		tud_cdc_n_write(0,&v,1);
		txcnt++;
	}
	//if(txcnt != 0) 
	//tud_cdc_n_write_flush(0);
	return;
}

//void cdc_task(void)
//{
//	// 1. Завжди викликаємо таск TinyUSB
//	tud_task();
//	// 2. НАПРАВЛЕННЯ: З TinyUSB CDC -> у ваш cdc_rx_ring (Прийом)
//	if (tud_cdc_available()) 
//	{
//		uint8_t usb_rx_byte;
//		// Читаємо по одному байту з TinyUSB і відразу пушимо в кільцевий буфер
//		// (tud_cdc_read повертає кількість прочитаних байт, тут 0 або 1)
//		while (tud_cdc_read(&usb_rx_byte, 1) > 0) ringbuf_push(&cdc_rx_ring, usb_rx_byte);
//	}
//	// 3. НАПРАВЛЕННЯ: З вашого cdc_tx_ring -> в TinyUSB CDC (Передача)
//	//if (tud_cdc_connected()) 
//	{
//		uint8_t usb_tx_buf[64]; // Розмір Full-Speed USB пакета для CDC
//		uint32_t bytes_to_send = 0;
//		// Витягуємо байти з кільцевого буфера, поки вони є і поки є місце в локальному пакеті
//		while (bytes_to_send < sizeof(usb_tx_buf)) 
//		{
//			int32_t ch = ringbuf_pop(&cdc_tx_ring);
//			if (ch == -1) break; // Буфер порожній, виходимо з циклу
//			usb_tx_buf[bytes_to_send++] = (uint8_t)ch;
//		}
//		// Якщо щось дістали з буфера — відправляємо в USB
//		if (bytes_to_send > 0) 
//		{
//			tud_cdc_write(usb_tx_buf, bytes_to_send);
//			tud_cdc_write_flush(); // Негайно проштовхуємо дані в шину
//		}
//	}
//}

//void cdc_task(void)
//{
//	tud_task();
//	// Прийом з USB в RX буфер
//	if (tud_cdc_available()) 
//	{
//		uint8_t usb_rx_byte;
//		while (tud_cdc_read(&usb_rx_byte, 1) > 0) 
//		{
//			__disable_irq();
//			ringbuf_push(&cdc_rx_ring, usb_rx_byte);
//			__enable_irq();
//		}
//	}
//	// Передача з TX буфера в USB (тепер викликає спільну функцію)
//	__disable_irq();
//	bool tx_has_data = (cdc_tx_ring.head != cdc_tx_ring.tail);
//	__enable_irq();
//	if (tx_has_data) flush_tx_to_usb();
//}



void usb_task(void)
{
	//tud_task();
	cdc_task();
}

void cdc_print(uint8_t itf,char* str)
{
	while(*str != 0)
	{
		tud_cdc_n_write_char(itf,*str);
		str++;
	}
	tud_cdc_n_write_flush(itf);
}
