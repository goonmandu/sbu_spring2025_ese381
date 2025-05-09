#pragma once

#include <stdint.h>

/**
 * Toggles capitalization of an ASCII character.
 * All other characters are not affected.
 * Example: a becomes A, R becomes r, { becomes {.
 *
 * @param c Character to toggle the capitalization of.
 * @return Resulting character.
 */
uint8_t complement_case(uint8_t c);