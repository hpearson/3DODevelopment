#include "types.h"
#include "item.h"
#include "mem.h"
#include "nodes.h"
#include "debug.h"
#include "list.h"
#include "device.h"
#include "driver.h"
#include "kernel.h"
#include "kernelnodes.h"
#include "io.h"
#include "graphics.h"
#include "filestream.h"
#include "filestreamfunctions.h"
#include "filesystem.h"
//#include "filesystemdefs.h"
#include "discdata.h"
#include "event.h"

#include "folio.h"
#include "task.h"
#include "semaphore.h"
#include "strings.h"
#include "stdlib.h"

#include "directory.h"
#include "directoryfunctions.h"

#include "filefunctions.h"
#include "Form3DO.h"
#include "Init3DO.h"
#include "Parse3DO.h"
#include "Utils3DO.h"


long ReadAFile( char *filename, char *buf, long count );
char *AllocAndLoadFile( char *filename, ulong memtype );



/* === RJ's Idiosyncracies === */
#define NOT !
#define FOREVER for(;;)
#define SetFlag(v,f) ((v)|=(f))
#define ClearFlag(v,f) ((v)&=~(f))
#define FlagIsSet(v,f) ((bool)(((v)&(f))!=0))
#define FlagIsClear(v,f) ((bool)(((v)&(f))==0))
#define Error(s) kprintf("ERROR:  %s\n",s)
#define Error2(s,arg) kprintf("ERROR:  %s %s\n",s,arg)
#define Warning(s) kprintf("WARNING:  %s\n",s)


int32 AllocAndLoadLen;
char RootName[256];
int32 oldbits = 0;
int32 upcount = 0;
int32 downcount = 0;



long
ReadAFile( char *filename, char *buf, long count )
/* Reads count bytes from the filename file into 
 * the specified buffer.  Returns the actual length of 
 * the read, 
 */
{
	Stream *stream;
	char name[256];

	strcpy( name, "^/" );
	strcat( name, filename );

	stream = OpenDiskStream( name, 0 );
	if ( stream != NULL )
		{
		if ( ( count = ReadDiskStream( stream, buf, count ) ) < 0 ) 
			Error2( "Couldn't read file:", filename );
		CloseDiskStream( stream );
		}
	else 
		{
		Error2( "file doesn't exist:", filename );
		count = -1;
		}
	return( count );
}


char *
AllocAndLoadFile( char *filename, ulong memtype )
/* This handy little routine allocates a buffer that's large enough for 
 * the named file, then loads that file into the buffer, and 
 */
{
	long len;
	char *buf;
	Stream *stream;
	char name[256];

	strcpy( name, "^/" );
	strcat( name, filename );

	buf = NULL;
	stream = OpenDiskStream( name, 0 );
	if ( stream != NULL )
		{
		len = stream->st_FileLength;
		AllocAndLoadLen = len;
		if ( len > 0 )
			{
			len = ( len + 3 ) & ~3L;
			buf = (char *)ALLOCMEM( (int)len, memtype );
			if ( buf )
				{
				if ( ReadDiskStream( stream, buf, len ) < 0 ) 
					Error2( "Couldn't read file:", filename );
				}
			else Error2( "Out of memory loading file:", filename );
			}
		else Error2( "file is empty:", filename );
		CloseDiskStream( stream );
		}
	else Error2( "file doesn't exist:", filename );
	return( buf );
}

