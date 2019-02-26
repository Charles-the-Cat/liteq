#include "util.h"

#define NEWLINE_STRING "\n"

/* an intermediate form for a single fastq file entry. */
struct interm_entry
{
	char * seqidline;
	char * readsline;
	char * plusline;
	char * scoresline;
};

// TODO: sanity check for huge fastq files that would exceed memory capacity
int main ( int argc, char * * argv )
{
//	char * infilename = argv[1];
//	char * outfilename;

	struct liteq_file out = 
	{
		.magic = LITEQ_MAGIC;
		.flags = 0x00;
		.linecount = 0;
		.lines = NULL;
	};

	char * file = fromFile( infilename );
	
	int entries = linesIn( file ) / 4; // truncate up to 3 trailing blank lines

	out.linecount = entries; // 

	struct interm_entry * interms = malloc( entries * sizeof(struct interm_entry) );

	for ( int i = 0; i < entries; i++ )
	{
		interms[i].seqidline  = strtok( file, NEWLINE_STRING );
		interms[i].readsline  = strtok( file, NEWLINE_STRING );
		interms[i].plusline   = strtok( file, NEWLINE_STRING );
		interms[i].scoresline = strtok( file, NEWLINE_STRING );
	}

	free( file );

	struct liteq_line * outlines = malloc( entries * sizeof(struct liteq_line) );	

	for ( int i = 0; i < entries; i++ )
	{
		outlines[i].readcount = strlen( interms[i].readsline );
	}

	free( interms );
	free( outlines );
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

