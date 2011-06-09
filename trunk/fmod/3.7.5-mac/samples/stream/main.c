//===============================================================================================
// STREAM
// Copyright (c), Firelight Technologies Pty, Ltd, 1999-2004.
//
// This example takes a command line parameter, a wav/mp2/mp3/ogg etc file, and uses the streamer 
// system to play it back.
//===============================================================================================

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "../../api/inc/fmod.h"
#include "../../api/inc/fmod_errors.h"	// optional
#include "../../api/inc/wincompat.h"

int channel = -1;

void *myopen(const char *name)
{
    return (void *)fopen(name, "rb");
}

void myclose(void *handle)
{
    fclose((FILE *)handle);
}

int myread(void *buffer, int size, void *handle)
{
    return fread(buffer, 1, size, (FILE *)handle);
}

int myseek(void *handle, int pos, signed char mode)
{
    return fseek((FILE *)handle, pos, mode);
}

int mytell(void *handle)
{
    return ftell((FILE *)handle);
}


/*
[
	[DESCRIPTION]
	End of stream user callback, initialized with FSOUND_Stream_SetEndCallback or 
	FSOUND_Stream_SetSynchCallback

	[PARAMETERS]
	'stream'	A pointer to the stream that ended.
	'buff'		This is NULL for end of stream callbacks, or a string for synch callbacks.
	'len'		This is reserved and is always 0 for end and synch callbacks. ignore.
	'param'		This is the value passed to FSOUND_Stream_SetEndCallback or 
	            FSOUND_Stream_SetSynchCallback as a user data value.
 
	[RETURN_VALUE]
	TRUE or FALSE, the value is ignored.

	[REMARKS]

	[SEE_ALSO]
]
*/
signed char endcallback(FSOUND_STREAM *stream, void *buff, int len, void *param)
{
	// end of stream callback doesnt have a 'buff' value, if it doesnt it could be a synch point.
	if (buff)
	{
		printf("\nSYNCHPOINT : \"%s\"\n", buff);
	}
	else
	{
		printf("\nSTREAM ENDED!!\n");
	}

	return 1;
}

/*
[
	[DESCRIPTION]
	main entry point into streamer example.

	[PARAMETERS]
	'argc'	Number of command line parameters.
	'argv'	Parameter list
 
	[RETURN_VALUE]
	void

	[REMARKS]

	[SEE_ALSO]
]
*/
int main(int argc, char *argv[])
{
	FSOUND_STREAM *stream;
	FSOUND_SAMPLE *sptr;
	char key;

	if (FSOUND_GetVersion() < FMOD_VERSION)
	{
		printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
		return 1;
	}
	
	if (argc < 2)
	{
		printf("-------------------------------------------------------------\n");
		printf("FMOD Streamer example.\n");
		printf("Copyright (c) Firelight Technologies Pty, Ltd, 1999-2004.\n");
		printf("-------------------------------------------------------------\n");
		printf("Syntax: stream infile.[mp2 mp3 wav ogg wma asf]\n\n");
		return 1;
	}

    // Set custom file callbacks?  This doesnt have to be done, its just here as an example.
	FSOUND_File_SetCallbacks(myopen, myclose, myread, myseek, mytell);

	// ==========================================================================================
	// INITIALIZE
	// ==========================================================================================
	if (!FSOUND_Init(44100, 32, 0))
	{
		printf("Error!\n");
		printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
		FSOUND_Close();
		return 1;
	}
	
	FSOUND_Stream_SetBufferSize(1000);

	// ==========================================================================================
	// OPEN STREAM (use #if 1 for streaming from memory)
	// ==========================================================================================
#if 0
	{
		FILE *	fp;
		int		length;
		char *	data;

		fp = fopen(argv[1], "rb");
		if (!fp)
		{
			printf("Error!\n");
			printf("File Not Found\n");
			FSOUND_Close();
			return 1;
		}
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		data = (char *)malloc(length);
		fread(data, length, 1, fp);
		fclose(fp);

		stream = FSOUND_Stream_Open(data, FSOUND_NORMAL | FSOUND_MPEGACCURATE | FSOUND_LOADMEMORY , 0, length);
	}
#else

	stream = FSOUND_Stream_Open(argv[1], FSOUND_NORMAL | FSOUND_MPEGACCURATE, 0, 0);
	if (!stream)
	{
		printf("Error!\n");
		printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
		FSOUND_Close();

		return 1;
	}

#endif

	// ==========================================================================================
	// SET AN END OF STREAM CALLBACK AND RIFF SYNCH POINTS CALLBACK
	// ==========================================================================================
	FSOUND_Stream_SetEndCallback(stream, endcallback, 0);
	FSOUND_Stream_SetSyncCallback(stream, endcallback, 0);
   

	printf("=========================================================================\n");
	printf("Press SPACE to pause/unpause\n");
	printf("Press 'f'   to fast forward 2 seconds\n");
	printf("Press ESC   to quit\n");
	printf("=========================================================================\n");
	printf("Playing stream...\n\n");

   
	sptr = FSOUND_Stream_GetSample(stream);
	if (sptr)
	{
		int freq;
		FSOUND_Sample_GetDefaults(sptr, &freq, NULL, NULL, NULL);
		printf("Name      : %s\n", FSOUND_Sample_GetName(sptr));
		printf("Frequency : %d\n\n", freq);
	}

    key = 0;
    do
    {
        if (channel < 0)
        {
            // ==========================================================================================
            // PLAY STREAM
            // ==========================================================================================
            channel = FSOUND_Stream_PlayEx(FSOUND_FREE, stream, NULL, 1);
            FSOUND_SetPaused(channel, 0);
        }

        if (kbhit())
        {
            key = getch();
            if (key == ' ')
            {
                FSOUND_SetPaused(channel, !FSOUND_GetPaused(channel));
            }
            if (key == 'f')
            {
                FSOUND_Stream_SetTime(stream, FSOUND_Stream_GetTime(stream) + 2000);
            }
        }

        printf("\rpos %6d/%6d time %02d:%02d/%02d:%02d cpu %5.02f%%   ", FSOUND_Stream_GetPosition(stream), 
                                                                         FSOUND_Stream_GetLength(stream), 
                                                                         FSOUND_Stream_GetTime(stream) / 1000 / 60, 
                                                                         FSOUND_Stream_GetTime(stream) / 1000 % 60, 
                                                                         FSOUND_Stream_GetLengthMs(stream) / 1000 / 60, 
                                                                         FSOUND_Stream_GetLengthMs(stream) / 1000 % 60, 
                                                                         FSOUND_GetCPUUsage());
		fflush(stdout);                                                                         
		Sleep(2000);
		
		FSOUND_Update();    
	} while (key != 27);

	printf("\n");

	FSOUND_Stream_Close(stream);
	FSOUND_Close();

    return 0;
}
