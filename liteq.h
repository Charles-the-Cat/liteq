#pragma once

#include <stdio.h>
#include <inttypes.h>

#define BASE_G 0
#define BASE_T 1
#define BASE_A 2
#define BASE_C 3

void err ( const char * msg, int ret )
{
	fprintf( stderr, msg );
	exit( ret );
}

void warn ( const char * msg )
{
	fprintf( stderr, msg );
}


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

void open_file ()
{
	FILE * fp = fopen( "here's some text", "r" );
	if ( fp == NULL )
}





