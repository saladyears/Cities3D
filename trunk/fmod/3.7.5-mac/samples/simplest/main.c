/*===============================================================================================
 SIMPLEST
 Copyright (c), Firelight Technologies Pty, Ltd, 1999,2004.

 This is the simplest way to play a song through FMOD.  It is basically Init, Load, Play!
===============================================================================================*/

#include <stdio.h>
#include <stdlib.h>

#include "../../api/inc/fmod.h"
#include "../../api/inc/fmod_errors.h"	/* optional */
#include "../../api/inc/wincompat.h"

int main()
{
	FMUSIC_MODULE *mod = NULL;

	if (FSOUND_GetVersion() < FMOD_VERSION)
	{
		printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
		exit(1);
	}

	/*
	    INITIALIZE
	*/
	if (!FSOUND_Init(44100, 64, FSOUND_INIT_GLOBALFOCUS))
	{
		printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
		exit(1);
	}
   
	/*
	    LOAD SONG
	*/

#if defined(__MACH__) || defined(WIN32)
	mod = FMUSIC_LoadSong("../../media/invtro94.s3m");
#else	
	mod = FMUSIC_LoadSong(":::media:invtro94.s3m");
#endif	
	if (!mod)
	{
		printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
		exit(1);
	}

	/*
	    PLAY SONG
	*/

	FMUSIC_PlaySong(mod);

	/*
	    UPDATE INTERFACE
	*/ 

	printf("Press any key to quit\n");
	printf("=========================================================================\n");
	printf("Playing %s...\n", FMUSIC_GetName(mod));
	do
	{
        printf("\rorder = %d/%d, row = %d/%d channels playing = %d cpu usage = %.02f%%     ", FMUSIC_GetOrder(mod), FMUSIC_GetNumOrders(mod), FMUSIC_GetRow(mod), FMUSIC_GetPatternLength(mod, FMUSIC_GetOrder(mod)), FSOUND_GetChannelsPlaying(), FSOUND_GetCPUUsage());
        fflush(stdout);
        Sleep(100);
	} while (!kbhit());

	getch();

	printf("\n");

	/*
	    FREE SONG AND SHUT DOWN
	*/

	FMUSIC_FreeSong(mod);
	FSOUND_Close();

    return 0;
}
