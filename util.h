#pragma once

#include <stdio.h>
#include <inttypes.h>

void err ( const char * msg, int ret )
{
	fprintf( stderr, msg );
	exit( ret );
}

void warn ( const char * msg )
{
	fprintf( stderr, msg );
}


void open_file ()
{
	FILE * fp = fopen( "here's some text", "r" );
	if ( fp == NULL )
}





