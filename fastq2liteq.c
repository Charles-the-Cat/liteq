#include "liteq.h"
#include "util.h"

#define DEBUG 0

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

size_t mystrlen( char * s )
{
	int n = 0;
	for ( ; s[n] != '\0'; n++ ) ;
	return n;
}

/* FIXME: I think \n s are included in reads, strip them! */
// TODO: sanity check for huge fastq files that would exceed memory capacity
int main ( int argc, char * * argv )
{
	char * infilename = argv[1];
	char * outfilename = argv[2];

	struct liteq_file out = 
	{
		.magic = LITEQ_MAGIC,
		.flags = 0x00,
		.linecount = 0,
		.lines = NULL
	};

	FILE * infp = fopen( infilename, "r" );
	if ( infp == NULL ) err ( -1, "File \"%s\" could not be opened\n", infilename );
	
	out.linecount = linesInFile( infp ) / 4; // truncate up to 3 trailing blank lines

	struct interm_entry * interms = malloc( out.linecount * sizeof(struct interm_entry) );

	/* guarantee getline's very first args are NULL and 0 so it allocates correctly */ 
	size_t getline_len = 0;	// FIXME: we may need to make this a pointer to many zero values
	for ( uint32_t i = 0; i < out.linecount; i++ ) // TODO: replace with memsetting in zeros?
	{
		interms[i].seqidline  = NULL; 
		interms[i].readsline  = NULL; 
		interms[i].plusline   = NULL;
		interms[i].scoresline = NULL; 
	} 
	for ( uint32_t i = 0; i < out.linecount; i++ )
	{
		getline( &interms[i].seqidline,  &getline_len, infp ); 
		getline( &interms[i].readsline,  &getline_len, infp ); 
		getline( &interms[i].plusline,   &getline_len, infp );
		getline( &interms[i].scoresline, &getline_len, infp ); 
	}
	/* strip the final '\n' character that getline() preserves */
	for ( uint32_t i = 0; i < out.linecount; i++ )
	{
		interms[i].seqidline[  strlen( interms[i].seqidline )  - 1 ] = '\0'; 
		interms[i].readsline[  strlen( interms[i].readsline )  - 1 ] = '\0'; 
		interms[i].plusline[   strlen( interms[i].plusline )   - 1 ] = '\0';
		interms[i].scoresline[ strlen( interms[i].scoresline ) - 1 ] = '\0'; 
	} 
#if DEBUG
	for ( uint32_t i = 0; i < out.linecount; i++ )
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
#endif	
	out.lines = malloc( out.linecount * sizeof(struct liteq_line) );

	for ( uint32_t i = 0; i < out.linecount; i++ )
	{
		out.lines[i].readcount = (uint32_t)mystrlen( interms[i].readsline ); 		
		
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

	free( interms ); // TODO: must iterate?
	
	/* file output stuff here */
	FILE * outfp = fopen( outfilename, "wb" );
	if ( outfp == NULL ) err ( -1, "File \"%s\" could not be opened for writing\n", outfilename );

	/* write header */
	fwrite( &out.magic, 	sizeof(uint16_t), 1, outfp );
	fwrite( &out.flags, 	sizeof(uint8_t) , 1, outfp );
	fwrite( &out.linecount, sizeof(uint32_t), 1, outfp ); 

	/* write entries */
	for ( uint32_t i = 0; i < out.linecount; i++ )
	{
		fwrite( &out.lines[i].readcount, sizeof(uint16_t), 1, outfp );
		// TODO: naive, O(n^2)? approach 
		for ( int j = 0; j < out.lines[i].readcount; j++ )
		{
			fwrite( &out.lines[i].reads[j], sizeof(uint8_t), 1, outfp );
		}

		/* fwrite( out.lines[i].reads, */
	}

	fclose( infp  );	
	fclose( outfp );
	
	for ( uint32_t i = 0; i < out.linecount; i++ ) free( out.lines[i].reads );
	free( out.lines );
	return 0;
}



