/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <tusb.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include "hardware/dma.h"

#include "atari_pico.pio.h"
#include "combat.h"
#include "spaceinvaders.h"

#define IRQ0 0
#define SM0 0
#define SM1 1
#define SM2 2

void isr()
{
    uint32_t addr = pio_sm_get(pio0, SM0);

    if(addr & 0x1000)
    {
        // Addressing the 2K ROM
        //pio_sm_put(pio0, SM0, combat[addr & 0x07FF]);

        // Addressing the 4K ROM
        pio_sm_put(pio0, SM0, spaceinvaders[addr & 0x0FFF]);
    }
    else
    {
        // Not addressing the ROM
        pio_sm_put(pio0, SM0, 0x00);
    }
    pio_interrupt_clear(pio0, IRQ0);
}

int main() {
    stdio_init_all();

    // Wait for com port to connect
    //while (!tud_cdc_connected()) { sleep_ms(100);  }
    //printf("Running...");
    set_sys_clock_khz(250000, true);

    PIO pio = pio0;
    // Data pindirs 0 to 4
    uint offset = pio_add_program(pio, &atari_pico_0_4_pindirs_program);
    atari_pico_pindirs_0_4_program_init(pio, SM1, offset);
    pio_sm_set_enabled(pio, SM1, true);

    // Data pindirs 5 to 7
    offset = pio_add_program(pio, &atari_pico_5_7_pindirs_program);
    atari_pico_pindirs_5_7_program_init(pio, SM2, offset);
    pio_sm_set_enabled(pio, SM2, true);

    // ROM look up
    offset = pio_add_program(pio, &atari_pico_program);

    // Set up IRQ
    irq_set_exclusive_handler(PIO0_IRQ_0, isr);
    irq_set_priority(PIO0_IRQ_0, 0x00);
    irq_set_enabled(PIO0_IRQ_0, true);
    pio_set_irq0_source_enabled(pio, pis_interrupt0, true);

    atari_pico_program_init(pio, SM0, offset);
    pio_sm_set_enabled(pio, SM0, true);
}
