#include "device.h"
#include "objects.h"
#include "gpio_api.h"

PortName getPortNameForPin(PinName pin);
uint8_t getPinValue(PinName pin);

void gpio_init(gpio_t *obj, PinName pin)
{
	uint8_t pin_value = 0x00;
	PortName init_port;

	init_port = getPortNameForPin(pin);
	pin_value = getPinValue(pin);
	obj->pin_val = pin_value;

	switch(init_port)
	{
	case portA:
		obj->port = portA;
		SYSCTL->RCGCGPIO |= 0x01;
		GPIOA->DEN |= (obj->pin_val);
		break;
	case portB:
		obj->port = portB;
		SYSCTL->RCGCGPIO |= 0x02;
		GPIOB->DEN |= (obj->pin_val);
		break;
	case portC:
		obj->port = portC;
		SYSCTL->RCGCGPIO |= 0x04;
		GPIOC->DEN |= (obj->pin_val);
		break;
	case portD:
		obj->port = portD;
		SYSCTL->RCGCGPIO |= 0x08;
		GPIOD->DEN |= (obj->pin_val);
		break;
	case portE:
		obj->port = portE;
		SYSCTL->RCGCGPIO |= 0x10;
		GPIOE->DEN |= (obj->pin_val);
		break;
	case portF:
		obj->port = portF;
		SYSCTL->RCGCGPIO |= 0x20;
		GPIOF->DEN |= (obj->pin_val);
		break;
	}
}


void gpio_write(gpio_t *obj, int value)
{
	PortName set_port;
    MBED_ASSERT(obj->pin != (PinName)NC);
    set_port = obj->port;
    if(value == 1)
    {
        switch(set_port)
        {
        case (portA):
    		GPIOA->DATA |= (obj->pin_val);
        		break;
        case (portB):
    		GPIOB->DATA |= (obj->pin_val);
        		break;
        case (portC):
    		GPIOC->DATA |= (obj->pin_val);
        		break;
        case (portD):
    		GPIOD->DATA |= (obj->pin_val);
        		break;
        case (portE):
    		GPIOE->DATA |= (obj->pin_val);
        		break;
        case (portF):
    		GPIOF->DATA |= (obj->pin_val);
        		break;
        }
    }
    else
    {
        switch(set_port)
        {
        case (portA):
    		GPIOA->DATA &= ~(obj->pin_val);
        		break;
        case (portB):
    		GPIOB->DATA &= ~(obj->pin_val);
        		break;
        case (portC):
    		GPIOC->DATA &= ~(obj->pin_val);
        		break;
        case (portD):
    		GPIOD->DATA &= ~(obj->pin_val);
        		break;
        case (portE):
    		GPIOE->DATA &= ~(obj->pin_val);
        		break;
        case (portF):
    		GPIOF->DATA &= ~(obj->pin_val);
        		break;
        }
    }

}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
	PortName set_port;
    MBED_ASSERT(obj->pin != (PinName)NC);
    set_port = obj->port;
    switch(set_port)
    {
    case (portA):
		GPIOA->DIR |= (obj->pin_val);
    		break;
    case (portB):
		GPIOB->DIR |= (obj->pin_val);
    		break;
    case (portC):
		GPIOC->DIR |= (obj->pin_val);
    		break;
    case (portD):
		GPIOD->DIR |= (obj->pin_val);
    		break;
    case (portE):
		GPIOE->DIR |= (obj->pin_val);
    		break;
    case (portF):
		GPIOF->DIR |= (obj->pin_val);
    		break;
    }
}


void gpio_mode(gpio_t *obj, PinMode mode)
{

}


/**Local Implementation**/
PortName getPortNameForPin(PinName pin)
{
	PortName return_name;
	if((pin == PA_0)|(pin == PA_1)|
	   (pin == PA_2)|(pin == PA_3)|
	   (pin == PA_4)|(pin == PA_5)|
	   (pin == PA_6)|(pin == PA_7))
	{
		return_name = portA;
	}
	else if((pin == PB_0)|(pin == PB_1)|
			(pin == PB_2)|(pin == PB_3)|
			(pin == PB_4)|(pin == PB_5)|
			(pin == PB_6)|(pin == PB_7))
	{
		return_name = portB;
	}
	else if((pin == PC_0)|(pin == PC_1)|
			(pin == PC_2)|(pin == PC_3)|
			(pin == PC_4)|(pin == PC_5)|
			(pin == PC_6)|(pin == PC_7))
	{
		return_name = portC;
	}
	else if((pin == PD_0)|(pin == PD_1)|
			(pin == PD_2)|(pin == PD_3)|
			(pin == PD_4)|(pin == PD_5)|
			(pin == PD_6)|(pin == PD_7))
	{
		return_name = portD;
	}
	else if((pin == PE_0)|(pin == PE_1)|
			(pin == PE_2)|(pin == PE_3)|
			(pin == PE_4)|(pin == PE_5))
	{
		return_name = portE;
	}
	else if((pin == PF_0)|(pin == PF_1)|
			(pin == PF_2)|(pin == PF_3)|
			(pin == PF_4))
	{
		return_name = portF;
	}
	return return_name;
}

uint8_t getPinValue(PinName pin)
{
	uint8_t return_value;

	if((pin==PA_0)||(pin==PB_0)||(pin==PC_0)||(pin==PD_0)||(pin==PE_0)||(pin==PF_0))
	{
		return_value = 0x01;
	}
	else if((pin==PA_1)||(pin==PB_1)||(pin==PC_1)||(pin==PD_1)||(pin==PE_1)||(pin==PF_1))
	{
		return_value = 0x02;
	}
	else if((pin==PA_2)||(pin==PB_2)||(pin==PC_2)||(pin==PD_2)||(pin==PE_2)||(pin==PF_2))
	{
		return_value = 0x04;
	}
	else if((pin==PA_3)||(pin==PB_3)||(pin==PC_3)||(pin==PD_3)||(pin==PE_3)||(pin==PF_3))
	{
		return_value = 0x08;
	}
	else if((pin==PA_4)||(pin==PB_4)||(pin==PC_4)||(pin==PD_4)||(pin==PE_4)||(pin==PF_4))
	{
		return_value = 0x10;
	}
	else if((pin==PA_5)||(pin==PB_5)||(pin==PC_5)||(pin==PD_5)||(pin==PE_5))
	{
		return_value = 0x20;
	}
	else if((pin==PA_6)||(pin==PB_6)||(pin==PC_6)||(pin==PD_6))
	{
		return_value = 0x40;
	}
	else if((pin==PA_7)||(pin==PB_7)||(pin==PC_7)||(pin==PD_7))
	{
		return_value = 0x80;
	}

	return return_value;
}
