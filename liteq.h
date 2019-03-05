#pragma once

#include "util.h"

#define BASE_G 0
#define BASE_T 1
#define BASE_A 2
#define BASE_C 3

#define SANGER_ASCII_OFFSET 33
#define SOLEXA_ASCII_OFFSET 64

#define LITEQ_MAGIC 0x4C51

struct liteq_line
{
	uint16_t readcount;
	uint8_t * reads;
};

struct liteq_file
{
	uint16_t magic; // "LQ", use LITEQ_MAGIC
	uint8_t flags;
	uint16_t linecount;
	struct liteq_line * lines;
};

struct liteq_line * liteqLineAllocUniform( int readcount_per_line )
{
	return malloc( sizeof(uint8_t) * readcount_per_line );
}

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
			return 'A';
		case BASE_C:
			return 'C';
		default:
			return '?';
	}
}

inline unsigned char char2Base( char ch )
{
	switch ( ch )
	{
		case 'G':
			return BASE_G;
		case 'T':
			return BASE_T;
		case 'A':
			return BASE_A;
		case 'C':
			return BASE_C;
		default:
			return 0;
	}
}

inline char score2Char( unsigned char score, unsigned char ascii_offset )
{
	return score + ascii_offset;
}

inline unsigned char char2Score( char ch, unsigned char ascii_offset )
{
	return ch - ascii_offset;
}

inline uint8_t packRead( unsigned char base, unsigned char score )
{
	return ( base << 6 ) | ( score & 0x3F );
}



