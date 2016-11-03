/*!

License Public Domain (CC0) 2016 Julio Tentor

To the extent possible under law, Julio Tentor has waived all copyright and related
or neighboring rights. This work is published from: Jujuy - Argentina.

http://creativecommons.org/publicdomain/zero/1.0/

*/
#pragma once
#ifndef _ENDIANNESS_H_
#define _ENDIANNESS_H_

#include <stdint.h>

typedef union Endian16 {
	int16_t i;
	unsigned char c[2];
} Endian16;

typedef union Endian32 {
	int32_t i;
	unsigned char c[4];
} Endian32;

inline int16_t littleEndian16(unsigned char b[]) {
	return (int16_t)b[0] | ((int16_t)b[1] << 8);
}
inline int16_t littleEndian16(unsigned char b0, unsigned char b1) {
	return (int16_t)b0 | ((int16_t)b1 << 8);
}

inline int16_t bigEndian16(unsigned char b[]) {
	return ((int16_t)b[0] << 8) | (int16_t)b[1];
}
inline int16_t bigEndian16(unsigned char b0, unsigned char b1) {
	return ((int16_t)b0 << 8) | (int16_t)b1;
}

inline int16_t reverseEndian16(int16_t value) {
	return (value << 8) | (value >> 8);
}


inline int32_t littleEndian32(unsigned char b[]) {
	return (int32_t)b[0] | ((int32_t)b[1] << 8) | ((int32_t)b[2] << 16) | ((int32_t)b[3] << 24);
}
inline int32_t littleEndian32(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3) {
	return (int32_t)b0 | ((int32_t)b1 << 8) | ((int32_t)b2 << 16) | ((int32_t)b3 << 24);
}

inline int32_t bigEndian32(unsigned char b[]) {
	return ((int32_t)b[0] << 24) | ((int32_t)b[1] << 16) | ((int32_t)b[2] << 8) | (int32_t)b[3];
}
inline int32_t bigEndian32(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3) {
	return ((int32_t)b0 << 24) | ((int32_t)b1 << 16) | ((int32_t)b2 << 8) | (int32_t)b3;
}

inline int32_t reverseEndian32(int32_t value) {
	uint32_t temp = (value << 16) | (value >> 16);
	return ((temp << 8) & 0xff00ff00) | ((temp >> 8) & 0x00ff00ff);
}

#endif	// _ENDIANNESS_H_

