#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void err ( const char * msg, int ret )
{
	fputs( msg, stderr );
	exit( ret );
}

void warn ( const char * msg )
{
	fputs( msg, stderr );
}


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





