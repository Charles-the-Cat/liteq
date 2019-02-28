#include "liteq.h"
#include "util.h"

#define NEWLINE_STRING "\n\r"

/* an intermediate form for a single fastq file entry. */
struct interm_entry
{
	char * seqidline;
	char * readsline;
	char * plusline;
	char * scoresline;
};

int linesIn( char * str )
{
	int lines = 0;
	while ( *(str++) != '\0' )
	{
		if ( *str == '\n' ) lines++;
	}
	return lines;
}

void liteqDisplay( struct liteq_file disp )
{
	printf( "Magic Number: %x\n", 
}

// TODO: sanity check for huge fastq files that would exceed memory capacity
int main ( int argc, char * * argv )
{
	char * infilename = argv[1];
//	char * outfilename;

	struct liteq_file out = 
	{
		.magic = LITEQ_MAGIC,
		.flags = 0x00,
		.linecount = 0,
		.lines = NULL
	};

	char * file = fromFile( infilename );
	
	int entries = linesIn( file ) / 4; // truncate up to 3 trailing blank lines

	out.linecount = entries; // 

	struct interm_entry * interms = malloc( entries * sizeof(struct interm_entry) );

	for ( int i = 0; i < entries; i++ )
	{
		interms[i].seqidline  = strtok( i == 0 ? file : NULL, NEWLINE_STRING );
		interms[i].readsline  = strtok( NULL, NEWLINE_STRING );
		interms[i].plusline   = strtok( NULL, NEWLINE_STRING );
		interms[i].scoresline = strtok( NULL, NEWLINE_STRING );
	}

	for ( int i = 0; i < entries; i++ )
	{
		printf
		( 
			"DEBUG:\n %s\n %s\n %s\n %s\n", 
			interms[i].seqidline, 
			interms[i].readsline,
			interms[i].plusline,
			interms[i].scoresline
		);
	}
	
	free( file );

	struct liteq_line * outlines = malloc( entries * sizeof(struct liteq_line) );	
	out.lines = outlines;

	for ( int i = 0; i < entries; i++ )
	{
		outlines[i].readcount = strlen( interms[i].readsline );

		/* TODO: make a single call to malloc instead of iterating */
		outlines[i].reads = malloc( outlines[i].readcount * sizeof(uint8_t) );

		for ( int j = 0; j < outlines[i].readcount; j++ )
		{
			outlines[i].reads[j] = packread
			(
				char2Base(  interms[i].readsline[j]  ),
				char2Score( interms[i].scoresline[j], SANGER_ASCII_OFFSET )
			);
		}

	}

	for ( int i = 0; i < entries; i++ )
	{
	}
	
	
	for ( int i = 0; i < entries; i++ ) free( outlines[i].reads );
	free( interms );
	free( outlines );
	return 0;
}



