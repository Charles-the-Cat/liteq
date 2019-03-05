#include "liteq.h"
#include "util.h"

#define NEWLINE_STRING "\n\r"

#define HUGE_FILE

/* an intermediate form for a single fastq file entry. */
struct interm_entry
{
	char * seqidline;
	char * readsline;
	char * plusline;
	char * scoresline;
};


void liteqDebugDisplay( struct liteq_file disp )
{
	printf( "DEBUG DISPLAY of struct liteq_file\n" );
	printf( "Magic Number: 0x%X\n", disp.magic );
	printf( "Flags: 0x%X\n", disp.flags );
	printf( "Line Count: %d\n", disp.linecount );
	printf( "LINES ARE AS FOLLOWS:\n" );
	for ( int i = 0; i < disp.linecount; i++ )
	{
		printf( "\tLine of length %d:", disp.lines[i].readcount );
		for ( int j = 0; j < disp.lines[i].readcount; j++ )
		{
			printf( " %0X", disp.lines[i].reads[j] );
		}
		printf( "\n" );
	}
}

// TODO: sanity check for huge fastq files that would exceed memory capacity
int main ( int argc, char * * argv )
{
	char * infilename = argv[1];
	char * outfilename;

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
		interms[i].readsline  = strtok( 		NULL, NEWLINE_STRING );
		interms[i].plusline   = strtok( 		NULL, NEWLINE_STRING );
		interms[i].scoresline = strtok( 		NULL, NEWLINE_STRING );
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

	out.lines = malloc( entries * sizeof(struct liteq_line) );

	for ( int i = 0; i < entries; i++ )
	{
		out.lines[i].readcount = (uint16_t)strlen( interms[i].readsline );

		/* TODO: make a single call to malloc instead of iterating */
		out.lines[i].reads = malloc( out.lines[i].readcount * sizeof(uint8_t) );

		for ( int j = 0; j < out.lines[i].readcount; j++ )
		{
			out.lines[i].reads[j] = packRead
			(
				char2Base(  interms[i].readsline[j]  ),
				char2Score( interms[i].scoresline[j], SANGER_ASCII_OFFSET )
			);
		}
	}

	liteqDebugDisplay( out );

#if 1-1
	/* file output stuff here */

	FILE * outfp = fopen( outfile, "wb" );
	if ( outfp == NULL ) err ( -1, "File \"%s\" could not be opened for writing\n", filename );

	/* write header */
	fwrite( out.magic, sizeof(uint16_t), 1, outfp );
	fwrite( out.flags, sizeof(uint8_t) , 1, outfp );
	fwrite( out.flags, sizeof( 

	/* write entries */
	for ( int i = 0; i < out.linecount; i++ )
	{

	}

	fclose( outfp );

#endif	
	
	for ( int i = 0; i < entries; i++ ) free( out.lines[i].reads );
	free( out.lines );

	free( interms );
	return 0;
}



