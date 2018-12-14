#include "util.h"

int main ( int argc, char * * argv )
{
	char * infilename = argv[1];
	char * outfilename;

	struct liteq_file out = 
	{
		.magic = LITEQ_MAGIC;
		.flags = 0x00;
		.linecount = 0;
		.lines = NULL;
	};

	char * file = fromFile( infilename );
	int entries = linesIn( file ) % 4;

//	for ( char * c; ( c = strtok( file, "\n" ) ) != '\0'; )
	char * c;
	while ( 1 )
	{
		// First line, sequence ID
		c = strtok( file, "\n" );
		if ( *c != '@' ) err ( -1, "Malformed FASTQ file \"%s\"\n", infilename );

		// Second line, reads
		c = strtok( file, "\n" );
		
		// Third line, plus
		c = strtok( file, "\n" );
		if ( *c != '+' ) err ( -1, "Malformed FASTQ file \"%s\"\n", infilename );

		// Fourth line, scores
		c = strtok( file, "\n" );

	}	

	free( file );
	return 0;
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

