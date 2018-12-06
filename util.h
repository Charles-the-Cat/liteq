#pragma once

#include <stdio.h>
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
	char * buf = NULL;

	FILE * fp = fopen( filename, "r" );
	if ( fp == NULL ) err ( "File could not be opened\n", -1 );

	fseek( fp, 0L, SEEK_END ); // go to end of file
	long int fsize = ftell( fp ); // get filesize in bytes
	if ( fsize < 0 ) err ( "Couldn't obtain size of file\n", -1 );
	rewind( fp );

	buf = malloc( sizeof( char ) * ( fsize + 1 ) ); // allocate buffer to fsize, plus one extra for safety

	fread( buf, fsize + 1, 1, fp ); // read file into buffer

	fclose( fp );

	return buf;
}





