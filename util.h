#pragma once

#include <stdio.h>
#include <stdlib.h>
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
	if ( fp == NULL ) err ( "File could not be opened\n", -1 );

	fseek( fp, 0L, SEEK_END ); // go to end of file
	long int fsize = ftell( fp ) + 1; // get filesize in bytes, plus one extra for safety
	if ( fsize < 0 ) err ( "Couldn't obtain size of file\n", -1 );
	rewind( fp );

	char * buf = malloc( sizeof( char ) * fsize ); // allocate buffer to fsize
	if ( buf == NULL ) err ( "Failed to allocate memory for file buffer\n", -1 );

	fread( buf, fsize, 1, fp ); // read file into buffer
	buf[ fsize - 1 ] = '\0'; // set final element to null terminator

	fclose( fp );

	return buf;
}

void toFile ( char * data, char * filename )
{
	FILE * fp = fopen( filename, "w" );
	if ( fp == NULL ) err ( "File could not be opened for writing\n", -1 );

	fprintf( filename, "%s", data );

	fclose( fp );
}





