/* mbed Microcontroller Library
 * Copyright (c) 2006-2014 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STDIO_UART_TX	U0Tx
#define STDIO_UART_RX	U0Rx
#define STDIO_UART      UART_0
#define STDIO_BAUD      9600

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

typedef enum {
	PA_0,
	PA_1,
	PA_2,
	PA_3,
	PA_4,
	PA_5,
	PA_6,
	PA_7,

	PB_0,
	PB_1,
	PB_2,
	PB_3,
	PB_4,
	PB_5,
	PB_6,
	PB_7,

	PC_0,
	PC_1,
	PC_2,
	PC_3,
	PC_4,
	PC_5,
	PC_6,
	PC_7,

	PD_0,
	PD_1,
	PD_2,
	PD_3,
	PD_4,
	PD_5,
	PD_6,
	PD_7,

	PE_0,
	PE_1,
	PE_2,
	PE_3,
	PE_4,
	PE_5,

	PF_0,
	PF_1,
	PF_2,
	PF_3,
	PF_4,

	U0Rx = PA_0,
	U0Tx = PA_1,

	U1Rx = PB_0,
	U1Tx = PB_1,

	U2Rx = PD_6,
	U2Tx = PD_7,

	U3Rx = PC_6,
	U3Tx = PC_7,

	U4Rx = PC_4,
	U4Tx = PC_5,

	U5Rx = PE_4,
	U5Tx = PE_5,

	U6Rx = PD_4,
	U6Tx = PD_5,

	U7Rx = PE_0,
	U7Tx = PE_1,

    // Not connected
    NC = (int)0xFFFFFFFF,
} PinName;

typedef enum{
	PIN_NULL = 0x00,
	PIN0 = 0x01,
	PIN1 = 0x02,
	PIN2 = 0x04,
	PIN3 = 0x08,
	PIN4 = 0x10,
	PIN5 = 0x20,
	PIN6 = 0x40,
	PIN7 = 0x80
}PinValue;


typedef enum {
    PullUp,
    PullDown,
    PullNone,
    Repeater,
    OpenDrain,
    PullDefault
} PinMode;

typedef struct port_s{
	uint8_t sonm;
}port_t;


#ifdef __cplusplus
}
#endif

#endif
