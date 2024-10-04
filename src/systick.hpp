#pragma once

#include <stdint.h>

void systick_init();
bool systick_1hz();
bool systick_10hz();

uint32_t systick_get_ms();
