#define ENABLE_SOUND	1
#include "types.h"
#include "mem.h"
#include "audio.h"
#include "debug3do.h"
#include "effectshandler.h"
#include "event.h"
#include "nodes.h"
#include "kernelnodes.h"

/* added for spoolsound */
#include "debug.h"
#include "operror.h"
#include "filefunctions.h"
#include "music.h"

#define	DBUG(x)	/* PRT(x) */

#define     MAXVOICES   8
#define     NUMVOICES   8
#define     NUMCHANNELS 8
#define     NUMSAMPLERS 8
#define     MAXAMPLITUDE MAXDSPAMPLITUDE
#define     MAX_PITCH   60
#define     PITCH_RANGE 40

int32 PlaySoundFile (char *FileName, int32 BufSize, int32 NumReps);
void SpoolSoundFileThread( void );
void musix( void );

/*
** Allocate enough space so that you don't get stack overflows.
** An overflow will be characterized by seemingly random crashes
** that defy all attempts at logical analysis.  You might want to
** start big then reduce the size till you crash, then double it.
*/
#define STACKSIZE (10000)

#define CHECKPTR(val,name) \
	if (val == 0) \
	{ \
		Result = -1; \
		ERR(("Failure in %s\n", name)); \
		goto error; \
	}

#define	DBUG(x)	/* PRT(x) */

#define NUMBLOCKS (64)
#define BLOCKSIZE (2048)
#define BUFSIZE (NUMBLOCKS*BLOCKSIZE) 
#define NUMBUFFS  (2)
//#define MAXAMPLITUDE (0x7FFF)



/********* Globals for Thread **********/
char *gFileName;
int32 gSignal1;
Item gMainTaskItem;
int32 gNumReps;




/**************************************************************************
** Entry point for background thread.
**************************************************************************/
void SpoolSoundFileThread( void )
{
	int32 Result;
	
	/* Initialize audio, return if error. */ 
	if (OpenAudioFolio())
	{
		ERR(("Audio Folio could not be opened!\n"));
	}

	Result = PlaySoundFile ( gFileName, BUFSIZE, gNumReps);
	SendSignal( gMainTaskItem, gSignal1 );

	CloseAudioFolio();
	WaitSignal(0);   /* Waits forever. Don't return! */
	
}

int32 PlaySoundFile (char *FileName, int32 BufSize, int32 NumReps)
{
	int32 Result=0;
	SoundFilePlayer *sfp;
	int32 SignalIn, SignalsNeeded;
	int32 LoopCount;

		
	for( LoopCount = 0; LoopCount < NumReps; LoopCount++)
	{
		PRT(("Loop #%d\n", LoopCount));
		
		sfp = OpenSoundFile(FileName, NUMBUFFS, BufSize);
		CHECKPTR(sfp, "OpenSoundFile");

		Result = StartSoundFile( sfp, MAXAMPLITUDE );
		CHECKRESULT(Result,"StartSoundFile");
	
/* Keep playing until no more samples. */
		SignalIn = 0;
		SignalsNeeded = 0;
		do
		{
			if (SignalsNeeded) SignalIn = WaitSignal(SignalsNeeded);
			Result = ServiceSoundFile(sfp, SignalIn, &SignalsNeeded);
			CHECKRESULT(Result,"ServiceSoundFile");
		} while (SignalsNeeded);

		Result = StopSoundFile (sfp);
		CHECKRESULT(Result,"StopSoundFile");
		
	Result = CloseSoundFile (sfp);
	CHECKRESULT(Result,"CloseSoundFile");
	
	}
	
	return 0;
	
error:
	return (Result);
}

void musix()
{ 	
  	int32 Priority;
	Item SpoolerThread;
    gFileName = "BounceFolder/Sound/globebnce.aiff";
    //gFileName = "Music/music_16_stereo.aiff";
    

	//PRT(("Usage: %s <samplefile>\n", argv[0]));
	
/* Get sample name from command line. */
/*
	if (argc > 1)
	{
		gFileName = (char *) argv[1];
	}
	else
	{
		
	}
*/
	
	gNumReps = 4;
	//if( argc > 2) gNumReps = atoi(argv[2]);
	PRT(("Play file %s %d times.\n", gFileName, gNumReps));
	
/* Initialize audio, return if error. */ 
	//if (OpenAudioFolio())
	//{
		//ERR(("Audio Folio could not be opened!\n"));
		//return(-1);
	//}
	
/* Get parent task Item so that thread can signal back. */
	gMainTaskItem = KernelBase->kb_CurrentTask->t.n_Item;
	
/* Allocate a signal for each thread to notify parent task. */
	gSignal1 = AllocSignal( 0 );
	CHECKRESULT(gSignal1,"AllocSignal");

	Priority = 180;
	SpoolerThread = CreateThread("SoundSpooler", Priority, SpoolSoundFileThread, STACKSIZE);
	CHECKRESULT(SpoolerThread,"CreateThread");

/* Do nothing for now but we could easily go off and do other stuff here!. */
/* OR together signals from other sources for a multi event top level */


    //PRT(("Foreground waiting for signal from background spooler.\n"));
	//WaitSignal( gSignal1 );
    //PRT(("Background spooler finished.\n"));
/*	
	CloseAudioFolio();
	DeleteThread( SpoolerThread );
	PRT(("Playback complete.\n"));
 */
}