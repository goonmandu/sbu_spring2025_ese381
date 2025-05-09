#include "ascii.h"

uint8_t complement_case(uint8_t c) {
	if ('A' <= c && c <= 'Z') {
		return c + 32;
	} else if ('a' <= c && c <= 'z') {
		return c - 32;
	} else {
		return c;
	}
}
