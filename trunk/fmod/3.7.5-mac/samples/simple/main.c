/*===============================================================================================
 SIMPLE
 Copyright (c), Firelight Technologies Pty, Ltd 1999-2004.

 This example demonstrates some fundamental FMOD usage, including device enumeration, output
 mode selection, user file I/O callbacks, loading and playing samples and a music file, and
 calling some runtime manipulation and information functions.
===============================================================================================*/

#include <stdio.h>

#ifdef __MACH__
    #include <Carbon/Carbon.h>
#else
    #include <Carbon.h>    /* For MPAllocateAligned() */
#endif

#include "../../api/inc/fmod.h"
#include "../../api/inc/fmod_errors.h"	/* optional */
#include "../../api/inc/wincompat.h" /* this is just for this example - keypress etc */

/*
    File callbacks
*/
unsigned int myopen(const char *name)
{
	return (unsigned int)fopen(name, "rb");
}

void myclose(unsigned int handle)
{
	fclose((FILE *)handle);
}

int myread(void *buffer, int size, unsigned int handle)
{
	return fread(buffer, 1, size, (FILE *)handle);
}

int myseek(unsigned int handle, int pos, signed char mode)
{
	return fseek((FILE *)handle, pos, mode);
}

int mytell(unsigned int handle)
{
	return ftell((FILE *)handle);
}

/*
    Memory allocation callbacks
*/
void *myalloc(unsigned int size)
{
    printf("FMOD Malloc'ed %d bytes\n", size);

    return malloc(size);
}

void *myrealloc(void *data, unsigned int size)
{
    printf("FMOD Realloced'ed %d bytes\n", size);

    return realloc(data, size);
}

void  myfree(void *ptr)
{
    printf("FMOD freed some memory\n");

    free(ptr);
}


/*
[
	[DESCRIPTION]

	[PARAMETERS]
 
	[RETURN_VALUE]

	[REMARKS]

	[SEE_ALSO]
]
*/
int main()
{
    void          *mem;
    FMUSIC_MODULE *mod = 0;
    FSOUND_SAMPLE *samp1 = 0, *samp2 = 0, *samp3 = 0;
    int key;
 
    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        return 1;
    }

    /*
        Set custom file callbacks?  This doesnt have to be done, its just here as an example.
    */
#if 0
    FSOUND_File_SetCallbacks(myopen, myclose, myread, myseek, mytell);
#endif

    /*
        Set custom memory callbacks?  This is optional as well of course.
    */
#if 0
    /* user callbacks */
    if (!FSOUND_SetMemorySystem(NULL, 0, myalloc, myrealloc, myfree))
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 1;
    }   
#else
    /* internal memory management - give it 1 mb and no more mallocs will come from fmod */
    if (MPLibraryIsLoaded())
    {
        mem = MPAllocateAligned(1*1024*1024, kMPAllocateDefaultAligned, kMPAllocateClearMask);
        if (!mem)
        {
            printf("Couldn't allocate memory!\n");
            return -1;
        }
    }
    if (!FSOUND_SetMemorySystem(mem, 1*1024*1024, NULL, NULL, NULL)) 
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 1;
    }
#endif

    /*
        INITIALIZE
    */
    if (!FSOUND_Init(44100, 32, FSOUND_INIT_USEDEFAULTMIDISYNTH))
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 1;
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
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 1;
    }
    /*
        LOAD SAMPLES
    */

    /* PCM,44,100 Hz, 8 Bit, Mono */
#if defined(__MACH__) || defined(WIN32)
    samp1 = FSOUND_Sample_Load(FSOUND_UNMANAGED, "../../media/drumloop.wav", FSOUND_NORMAL | FSOUND_HW2D, 0, 0);   /* hardware? why not, just to show it can be done */
#else
    samp1 = FSOUND_Sample_Load(FSOUND_UNMANAGED, ":::media:drumloop.wav", FSOUND_NORMAL | FSOUND_HW2D, 0, 0);   /* hardware? why not, just to show it can be done */
#endif
    if (!samp1)
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 1;
    }

    FSOUND_Sample_SetMode(samp1, FSOUND_LOOP_OFF);  /* this wav has loop points in it which turns looping on.. turn it off! */

    /* PCM,22,050 Hz, 16 Bit, Mono */
#if defined(__MACH__) || defined(WIN32)
    samp2 = FSOUND_Sample_Load(FSOUND_UNMANAGED, "../../media/jungle.wav", FSOUND_NORMAL, 0, 0);
#else
    samp2 = FSOUND_Sample_Load(FSOUND_UNMANAGED, ":::media:jungle.wav", FSOUND_NORMAL, 0, 0);
#endif	
    if (!samp2)
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 1;
    }

    /* PCM,22,050 Hz, 8 Bit, Stereo */
#if defined(__MACH__) || defined(WIN32)
    samp3 = FSOUND_Sample_Load(FSOUND_UNMANAGED, "../../media/chimes.wav", FSOUND_NORMAL, 0, 0);
#else
    samp3 = FSOUND_Sample_Load(FSOUND_UNMANAGED, ":::media:chimes.wav", FSOUND_NORMAL, 0, 0);
#endif
    if (!samp3)
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 1;
    }

    /*
        DISPLAY HELP
    */

    printf("FSOUND Output Method : ");
    switch (FSOUND_GetOutput())
    {
        case FSOUND_OUTPUT_NOSOUND:	printf("FSOUND_OUTPUT_NOSOUND\n"); break;
        case FSOUND_OUTPUT_WINMM:	printf("FSOUND_OUTPUT_WINMM\n"); break;
        case FSOUND_OUTPUT_DSOUND:	printf("FSOUND_OUTPUT_DSOUND\n"); break;
        case FSOUND_OUTPUT_ASIO:	printf("FSOUND_OUTPUT_ASIO\n"); break;
        case FSOUND_OUTPUT_MAC:	    printf("FSOUND_OUTPUT_MAC\n"); break;
    };
    printf("FSOUND Driver        : %s\n", FSOUND_GetDriverName(FSOUND_GetDriver()));

    printf("=========================================================================\n");
    printf("Press 1       Play 16bit sound at any time\n");
    printf("      2       Play 8bit sound at any time (limited to 3 at a time)\n");
    printf("      3       Play 16bit STEREO sound at any time\n");
    printf("      <       Rewind mod back 1 order\n");
    printf("      >       FastForward mod forward 1 order\n");
    printf("      SPACE   Pause/unpause music at any time\n");
    printf("      ESC     Quit\n"); 
    printf("=========================================================================\n");
    printf("Playing \"%s\"...\n", FMUSIC_GetName(mod));

    {
        int count;
        for (count=0; count < FMUSIC_GetNumSamples(mod) && count < 20; count+=2)
        {
            const char *a,*b;
            a = FSOUND_Sample_GetName(FMUSIC_GetSample(mod, count));
            b = FSOUND_Sample_GetName(FMUSIC_GetSample(mod, count+1));
            if (!a)
                a = "";
            if (!b)
                b = "";
            printf("%02d %-33s ", count, a);
	    printf("%02d %-33s\n", count+1, b);
        }
    }

    FSOUND_Sample_SetMaxPlaybacks(samp2, 3);

    /*
        START PLAYING MUSIC!
    */
    FMUSIC_SetMasterVolume(mod, 192);
    FMUSIC_SetLooping(mod, FALSE);
    FMUSIC_PlaySong(mod);

    do
    {
        static float speed = 1.0f;
        key = 0;

	printf("\rorder = %d/%d, row = %d/%d time = %d.%02d finished %d channels = %d cpu = %.02f%%     ", 
                FMUSIC_GetOrder(mod), 
                FMUSIC_GetNumOrders(mod), 
                FMUSIC_GetRow(mod), 
                FMUSIC_GetPatternLength(mod, FMUSIC_GetOrder(mod)), 
                FMUSIC_GetTime(mod) / 1000,  
                FMUSIC_GetTime(mod) % 1000 / 10, 
                FMUSIC_IsFinished(mod), 
                FSOUND_GetChannelsPlaying(), 
                FSOUND_GetCPUUsage());
        fflush(stdout);

        if (kbhit())
        {
            key = getch();

            if (key == ' ') 
            {
                 FMUSIC_SetPaused(mod, !FMUSIC_GetPaused(mod));
            }
            if (key == '1') 
            {
                FSOUND_PlaySound(FSOUND_FREE, samp1);
            }
            if (key == '2') 
            {
                int channel;

                channel = FSOUND_PlaySoundEx(FSOUND_FREE, samp2, NULL, TRUE);
                FSOUND_SetCurrentPosition(channel, FSOUND_Sample_GetLength(samp2)-1);
                FSOUND_SetFrequency(channel, -22050);   /* Play it backwards! */
                FSOUND_SetVolume(channel, 255);
                FSOUND_SetPan(channel, 255);			/* pan it all the way to the right */
                FSOUND_SetPaused(channel, FALSE);
            }
            if (key == '3') 
            {
                int channel;

                channel = FSOUND_PlaySoundEx(FSOUND_FREE, samp3, NULL, TRUE);
                FSOUND_SetPaused(channel, FALSE);
            }
            if (key == '>') 
            {
                FMUSIC_SetOrder(mod, FMUSIC_GetOrder(mod)+1);
            }
            if (key == '<') 
            {
                FMUSIC_SetOrder(mod, FMUSIC_GetOrder(mod)-1);
            }
            if (key == 'f')
            {
                speed += 0.1f;
                FMUSIC_SetMasterSpeed(mod, speed);
            }
            if (key == 's')
            {
                speed -= 0.1f;
                FMUSIC_SetMasterSpeed(mod, speed);
            }
        }

        Sleep(100);

    } while (key != 27);

    FMUSIC_StopSong(mod);

    printf("\n");

    /*
        CLEANUP AND SHUTDOWN
    */

    FSOUND_Sample_Free(samp1);
    FSOUND_Sample_Free(samp2);
    FSOUND_Sample_Free(samp3);
    FMUSIC_FreeSong(mod);

    FSOUND_Close();
  
    return 0;
}


