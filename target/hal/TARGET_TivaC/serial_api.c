#include "serial_api.h"
#include "cmsis_nvic.h"

#define UART_NUM    8


static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

struct serial_global_data_s {
    uint32_t serial_irq_id;
    uint8_t rx_irq_set_api;
};

static struct serial_global_data_s uart_data[UART_NUM];

static PortName getUartParam(serial_t *obj, PinName tx, PinName rx);
static inline void uart_irq(uint32_t iir, uint32_t index, UART0_Type *puart);
static void UART0_Init(void);
static void UART1_Init(void);
static void UART2_Init(void);
static void UART3_Init(void);
static void UART4_Init(void);
static void UART5_Init(void);
static void UART6_Init(void);
static void UART7_Init(void);
static void UART0_Irq(void);
static void UART1_Irq(void);
static void UART2_Irq(void);
static void UART3_Irq(void);
static void UART4_Irq(void);
static void UART5_Irq(void);
static void UART6_Irq(void);
static void UART7_Irq(void);


void serial_init(serial_t *obj, PinName tx, PinName rx)
{
	PortName uart_port = PORT_NC;
	uart_port = getUartParam(obj,tx,rx);
	MBED_ASSERT(obj->port != (PortName)PORT_NC);

	switch(uart_port)
	{
	case (portA):
		SYSCTL->RCGCGPIO |= 0x01;
			break;
	case (portB):
		SYSCTL->RCGCGPIO |= 0x02;
			break;
	case (portC):
		SYSCTL->RCGCGPIO |= 0x04;
			break;
	case (portD):
		SYSCTL->RCGCGPIO |= 0x08;
			break;
	case (portE):
		SYSCTL->RCGCGPIO |= 0x10;
			break;
	case (portF):
		SYSCTL->RCGCGPIO |= 0x20;
			break;
	default:
		break;
	}

	switch(obj->uart)
	{
	case (UART_0):
			UART0_Init();
			obj->puart = UART0;
			break;
	case (UART_1):
			UART1_Init();
			obj->puart = UART1;
			break;
	case (UART_2):
			UART2_Init();
			obj->puart = UART2;
			break;
	case (UART_3):
		    UART3_Init();
			obj->puart = UART3;
			break;
	case (UART_4):
			UART4_Init();
			obj->puart = UART4;
			break;
	case (UART_5):
			UART5_Init();
			obj->puart = UART5;
			break;
	case (UART_6):
			UART6_Init();
			obj->puart = UART6;
			break;
	case (UART_7):
			UART7_Init();
			obj->puart = UART7;
			break;
	}
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
	IRQn_Type irq_n = (IRQn_Type)0;
	uint32_t vector = 0;

	if (RxIrq == irq)
	        uart_data[obj->index].rx_irq_set_api = enable;


	switch(obj->uart)
	{
	case (UART_0):
		irq_n=UART0_IRQn;
	vector = (uint32_t)&UART0_Irq;
			break;
	case (UART_1):
		irq_n=UART1_IRQn;
	vector = (uint32_t)&UART1_Irq;
			break;
	case (UART_2):
		irq_n=UART2_IRQn;
	vector = (uint32_t)&UART2_Irq;
			break;
	case (UART_3):
		irq_n=UART3_IRQn;
	vector = (uint32_t)&UART3_Irq;
			break;
	case (UART_4):
		irq_n=UART4_IRQn;
	vector = (uint32_t)&UART4_Irq;
			break;
	case (UART_5):
		irq_n=UART5_IRQn;
	vector = (uint32_t)&UART5_Irq;
			break;
	case (UART_6):
		irq_n=UART6_IRQn;
	vector = (uint32_t)&UART6_Irq;
			break;
	case (UART_7):
		irq_n=UART7_IRQn;
	vector = (uint32_t)&UART7_Irq;
			break;
	}

	if (enable == TRUE)
	{
		obj->puart->IM |= (1<<4);
		NVIC_SetVector(irq_n, (uint32_t)vector);
		NVIC_EnableIRQ(irq_n);
	}
	else if ((irq == TxIrq)||(uart_data[obj->index].rx_irq_set_api = FALSE))
	{
		NVIC_DisableIRQ(irq_n);
	}
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    uart_data[obj->index].serial_irq_id = id;
}

int  serial_getc(serial_t *obj)
{
	int data;
	while (serial_readable(obj) == FALSE);
	data = ((UART5->DR)&0xFF);
	return data;
}

void serial_putc(serial_t *obj, int c)
{
	while (serial_writable(obj) == FALSE);
	obj->puart->DR = (c&0xFF);
}

int  serial_readable(serial_t *obj)
{
	int return_val;
	if (((obj->puart->FR)&0x10)== 0)
	{
		return_val = TRUE;
	}
	else if(((obj->puart->FR)&0x10)== 0x10)
	{
		return_val = FALSE;
	}
	return return_val;
}

int  serial_writable(serial_t *obj)
{
	int return_val;
	if(((obj->puart->FR)&0x20)==0)
	{
		return_val = TRUE;
	}
	else if (((obj->puart->FR)&0x20)==0x20)
	{
		return_val = FALSE;
	}
	return return_val;
}

void serial_clear(serial_t *obj)
{

}

void serial_free(serial_t *obj)
{
    uart_data[obj->index].serial_irq_id = 0;
}
/**********************************************************************************
**                           Local Implementation
**********************************************************************************/

static PortName getUartParam(serial_t *obj, PinName tx, PinName rx)
{
	PortName return_val;

	if((tx == U0Tx)&&(rx == U0Rx))
	{
		return_val = portA;
		obj->uart = UART_0;
		obj->index = 0;
		SYSCTL->RCGCUART |= 0x01;
	}
	else if((tx == U1Tx)&&(rx == U1Rx))
	{
		return_val = portB;
		obj->uart = UART_1;
		obj->index = 1;
		SYSCTL->RCGCUART |= 0x02;
	}
	else if((tx == U2Tx)&&(rx == U2Rx))
	{
		return_val = portD;
		obj->uart = UART_2;
		obj->index = 2;
		SYSCTL->RCGCUART |= 0x04;
	}
	else if((tx == U3Tx)&&(rx == U3Rx))
	{
		return_val = portC;
		obj->uart = UART_3;
		obj->index = 3;
		SYSCTL->RCGCUART |= 0x08;
	}
	else if((tx == U4Tx)&&(rx == U4Rx))
	{
		return_val = portC;
		obj->uart = UART_4;
		obj->index = 4;
		SYSCTL->RCGCUART |= 0x10;
	}
	else if((tx == U5Tx)&&(rx == U5Rx))
	{
		return_val = portE;
		obj->uart = UART_5;
		obj->index = 5;
		SYSCTL->RCGCUART |= 0x20;
	}
	else if((tx == U6Tx)&&(rx == U6Rx))
	{
		return_val = portD;
		obj->uart = UART_6;
		obj->index = 6;
		SYSCTL->RCGCUART |= 0x40;
	}
	else if((tx == U7Tx)&&(rx == U7Rx))
	{
		return_val = portE;
		obj->uart = UART_7;
		obj->index = 7;
		SYSCTL->RCGCUART |= 0x80;
	}
	else
	{
		return_val = PORT_NC;
	}
	return return_val;
}

static inline void uart_irq(uint32_t iir, uint32_t index, UART0_Type *puart)
{
	SerialIrq irq_type;
    switch (iir)
    {
        case ((uint32_t)0x20): irq_type = TxIrq; break;
        case ((uint32_t)0x10): irq_type = RxIrq; break;
        case ((uint32_t)0x30): irq_type = RxIrq; break;
        default: return;
    }

    if(irq_type == RxIrq)
    {
    	puart->ICR = (uint32_t)0x30;
    }

    if(uart_data[index].serial_irq_id != 0)
    {
    	if ((irq_type != RxIrq) || (uart_data[index].rx_irq_set_api))
    	{
    		irq_handler(uart_data[index].serial_irq_id, irq_type);
    	}
    }

}
static void UART0_Irq(void)
{
	uart_irq(((UART0->RIS)&0x30), 0, UART0);
}

static void UART1_Irq(void)
{
	uart_irq(((UART1->RIS)&0x30), 1, UART1);
}

static void UART2_Irq(void)
{
	uart_irq(((UART2->RIS)&0x30), 2, UART2);
}

static void UART3_Irq(void)
{
	uart_irq(((UART3->RIS)&0x30), 3, UART3);
}

static void UART4_Irq(void)
{
	uart_irq(((UART4->RIS)&0x30), 4, UART4);
}

static void UART5_Irq(void)
{
	uart_irq(((UART5->RIS)&0x30), 5, UART5);
}

static void UART6_Irq(void)
{
	uart_irq(((UART6->RIS)&0x30), 6, UART6);
}

static void UART7_Irq(void)
{
	uart_irq(((UART7->RIS)&0x30), 7, UART7);
}


static void UART0_Init(void)
{
	GPIOA->AFSEL |= ((1<<0)|(1<<1));
	GPIOA->DR2R	 |= ((1<<0)|(1<<1));
	GPIOA->DEN 	 |= ((1<<0)|(1<<1));
	GPIOA->PCTL	 |= ((1<<0)|(1<<4));

	UART0->CTL	&= ~0x01;
	UART0->IBRD  = 43;
	UART0->FBRD  = 26;
	UART0->LCRH  = 0x70;
	UART0->CC    = 0x00;
	UART0->CTL   = (1<<0)|(1<<8)|(1<<9);
}

static void UART1_Init(void)
{
	GPIOB->AFSEL |= ((1<<0)|(1<<1));
	GPIOB->DR2R	 |= ((1<<0)|(1<<1));
	GPIOB->DEN 	 |= ((1<<0)|(1<<1));
	GPIOB->PCTL	 |= ((1<<0)|(1<<4));

	UART1->CTL	&= ~0x01;
	UART1->IBRD  = 43;
	UART1->FBRD  = 26;
	UART1->LCRH  = 0x70;
	UART1->CC    = 0x00;
	UART1->CTL   = (1<<0)|(1<<8)|(1<<9);
}

static void UART2_Init(void)
{
	GPIOD->AFSEL |= ((1<<6)|(1<<7));
	GPIOD->DR2R	 |= ((1<<6)|(1<<7));
	GPIOD->DEN 	 |= ((1<<6)|(1<<7));
	GPIOD->PCTL	 |= ((1<<24)|(1<<28));

	UART2->CTL	&= ~0x01;
	UART2->IBRD  = 43;
	UART2->FBRD  = 26;
	UART2->LCRH  = 0x70;
	UART2->CC    = 0x00;
	UART2->CTL   = (1<<0)|(1<<8)|(1<<9);
}

static void UART3_Init(void)
{
	GPIOC->AFSEL |= ((1<<6)|(1<<7));
	GPIOC->DR2R	 |= ((1<<6)|(1<<7));
	GPIOC->DEN 	 |= ((1<<6)|(1<<7));
	GPIOC->PCTL	 |= ((1<<24)|(1<<28));

	UART3->CTL	&= ~0x01;
	UART3->IBRD  = 43;
	UART3->FBRD  = 26;
	UART3->LCRH  = 0x70;
	UART3->CC    = 0x00;
	UART3->CTL   = (1<<0)|(1<<8)|(1<<9);
}

static void UART4_Init(void)
{
	GPIOC->AFSEL |= ((1<<4)|(1<<5));
	GPIOC->DR2R	 |= ((1<<4)|(1<<5));
	GPIOC->DEN 	 |= ((1<<4)|(1<<5));
	GPIOC->PCTL	 |= ((1<<16)|(1<<20));

	UART4->CTL	&= ~0x01;
	UART4->IBRD  = 43;
	UART4->FBRD  = 26;
	UART4->LCRH  = 0x70;
	UART4->CC    = 0x00;
	UART4->CTL   = (1<<0)|(1<<8)|(1<<9);
}

static void UART5_Init(void)
{
	GPIOE->AFSEL |= ((1<<4)|(1<<5));
	GPIOE->DR2R	 |= ((1<<4)|(1<<5));
	GPIOE->DEN 	 |= ((1<<4)|(1<<5));
	GPIOE->PCTL	 |= ((1<<16)|(1<<20));

	UART5->CTL	&= ~0x01;
	UART5->IBRD  = 43;
	UART5->FBRD  = 26;
	UART5->LCRH  = 0x70;
	UART5->CC    = 0x00;
	UART5->CTL   = (1<<0)|(1<<8)|(1<<9);
}

static void UART6_Init(void)
{
	GPIOD->AFSEL |= ((1<<4)|(1<<5));
	GPIOD->DR2R	 |= ((1<<4)|(1<<5));
	GPIOD->DEN 	 |= ((1<<4)|(1<<5));
	GPIOD->PCTL	 |= ((1<<16)|(1<<20));

	UART6->CTL	&= ~0x01;
	UART6->IBRD  = 43;
	UART6->FBRD  = 26;
	UART6->LCRH  = 0x70;
	UART6->CC    = 0x00;
	UART6->CTL   = (1<<0)|(1<<8)|(1<<9);
}

static void UART7_Init(void)
{
	GPIOE->AFSEL |= ((1<<0)|(1<<1));
	GPIOE->DR2R	 |= ((1<<0)|(1<<1));
	GPIOE->DEN 	 |= ((1<<0)|(1<<1));
	GPIOE->PCTL	 |= ((1<<0)|(1<<4));

	UART7->CTL	&= ~0x01;
	UART7->IBRD  = 43;
	UART7->FBRD  = 26;
	UART7->LCRH  = 0x70;
	UART7->CC    = 0x00;
	UART7->CTL   = (1<<0)|(1<<8)|(1<<9);
}




