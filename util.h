#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#define err( r, ... ) \
do { \
	fprintf( stderr, "Error on line %d of file \"%s\":\n", __LINE__, __FILE__ ); \
	fprintf( stderr, __VA_ARGS__ ); \
	exit( r ); \
} while ( 0 )

#define warn( ... ) \
do { \
	fprintf( stderr, "Warning on line %d of file \"%s\":\n", __LINE__, __FILE__ ); \
	fprintf( stderr, __VA_ARGS__ ); \
} while ( 0 )

char * fromFile ( char * filename )
{
	FILE * fp = fopen( filename, "r" );
	if ( fp == NULL ) err ( -1, "File \"%s\" could not be opened\n", filename );

	fseek( fp, 0L, SEEK_END ); // go to end of file
	long int fsize = ftell( fp ) + 1; // get filesize in bytes, plus one extra for safety
	if ( fsize < 0 ) err ( -1, "Couldn't obtain size of file \"%s\"\n", filename );
	rewind( fp );

	char * buf = malloc( sizeof( char ) * fsize ); // allocate buffer to fsize
	if ( buf == NULL ) err ( -1, "Failed to allocate memory for file buffer\"%s\"\n", filename );

	fread( buf, fsize, 1, fp ); // read file into buffer
	buf[ fsize - 1 ] = '\0'; // set final element to null terminator

	fclose( fp );

	return buf;
}

void toFile ( char * data, char * filename )
{
	FILE * fp = fopen( filename, "w" );
	if ( fp == NULL ) err ( -1, "File \"%s\" could not be opened for writing\n", filename );

	fprintf( fp, "%s", data );

	fclose( fp );
}

void toBinFile ( char * data, char * filename )
{
	FILE * fp = fopen( filename, "wb" );
	if ( fp == NULL ) err ( -1, "File \"%s\" could not be opened for writing\n", filename );

	/* TODO */

	fclose( fp );
}

int linesIn( char * str )
{
	int lines = 0;
	while ( *(str++) != '\0' )
	{
		if ( *str == '\n' ) lines++;
	}
	return lines;
}




