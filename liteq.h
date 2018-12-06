#pragma once

#include "util.h"

#define BASE_G 0
#define BASE_T 1
#define BASE_A 2
#define BASE_C 3

#define SANGER_ASCII_OFFSET 33
#define SOLEXA_ASCII_OFFSET 64

struct liteq_line
{
	uint16_t readcount;
	uint8_t * reads;
};

struct liteq_header
{
	uint16_t magic; // "LQ"
	uint8_t flags;
	uint16_t linecount;
	struct liteq_line * lines;
};

inline unsigned char getBase( uint8_t read )
{
	return read >> 6;
}

inline unsigned char getScore( uint8_t read )
{
	return read & 0x3F;
}

inline char base2Char( unsigned char base )
{
	switch ( base )
	{
		case BASE_G:
			return 'G';
		case BASE_T:
			return 'T';
		case BASE_A:
			return 'C';
		case BASE_C:
			return 'C';
		default:
			return '?';
	}
}

inline char score2Char( unsigned char score, unsigned char ascii_offset )
{
	return score + ascii_offset;
}

inline uint8_t packRead( unsigned char base, unsigned char score )
{
	return ( base << 6 ) & ( score & 0x3F );
}



