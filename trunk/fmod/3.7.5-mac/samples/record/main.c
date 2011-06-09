/*
 RECORD
 Copyright (c), Firelight Technologies Pty, Ltd, 2000-2004.

 This example shows how to record data to a static sample, or record dynamically, and have
 a dsp unit processing the result.
 The reverb below is taken from /samples/fmod/fmod.c 
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef __MACH__
    #include <Carbon/Carbon.h>
#else
    #include <Carbon.h>    /* For MPAllocateAligned() */
#endif

#include "../../api/inc/fmod.h"
#include "../../api/inc/fmod_errors.h"	/* optional */
#include "../../api/inc/wincompat.h"

#define	ENABLEREVERB	TRUE
#define RECORDLEN		(44100*5)		/* 5 seconds at 44khz */

#define REVERB_NUMTAPS	7

typedef struct
{
	FSOUND_DSPUNIT  *Unit;
	char			*historybuff;       /* storage space for tap history */
	char            *workarea;          /* a place to hold 1 buffer worth of data (for reverb) */
	int             delayms;            /* delay of reverb tab in milliseconds */
	int             volume;             /* volume of reverb tab */
	int             pan;                /* pan of reverb tab */
	int             historyoffset;      /* running offset into history buffer */
	int             historylen;         /* size of history buffer in SAMPLES */
} REVERBTAP;

/*
    Reverb stuff
*/
REVERBTAP		DSP_ReverbTap[REVERB_NUMTAPS];

/*
[
	[DESCRIPTION]
	Callback to mix in one reverb tap.  It copies the buffer into its own history buffer also.

	[PARAMETERS]
	'originalbuffer'	Pointer to the original mixbuffer, not any buffers passed down 
						through the dsp chain.  They are in newbuffer.
	'newbuffer'			Pointer to buffer passed from previous DSP unit.
	'length'			Length in SAMPLES of buffer being passed.
	'param'				User parameter.  In this case it is a pointer to DSP_LowPassBuffer.
 
	[RETURN_VALUE]
	a pointer to the buffer that was passed in, with a tap mixed into it.

	[REMARKS]
]
*/
void *DSP_ReverbCallback(void *originalbuffer, void *newbuffer, int length, void *param)
{
    int           mixertype = FSOUND_GetMixer();
    REVERBTAP    *tap = (REVERBTAP *)param;

    #define MIXBUFFERFORMAT signed int  /* This determines the format of the mixbuffer being passed in.  change if you want to support int32 or float32 */

    /*
        must be 16bit stereo integer buffer.. sorry fpu (32bit float) dont support this.
    */
    if (mixertype == FSOUND_MIXER_BLENDMODE || mixertype == FSOUND_MIXER_QUALITY_FPU)
    {
        return newbuffer;
    }


    /*
        Reverb history buffer is a ringbuffer.  If the length makes the copy wrap, then split the copy 
        into end part, and start part.. 
    */
    if (tap->historyoffset + length > tap->historylen)
    {
        int              taillen  = tap->historylen - tap->historyoffset;
        int              startlen = length - taillen;
        int              count;
        signed short    *dest;
        MIXBUFFERFORMAT *src;

        /*
            Mix a scaled version of history buffer into output
        */
        FSOUND_DSP_MixBuffers(newbuffer, tap->historybuff + (tap->historyoffset << 2),
                              taillen,  44100, tap->volume, tap->pan, FSOUND_STEREO | FSOUND_16BITS);
        FSOUND_DSP_MixBuffers((char *)newbuffer+((tap->historylen - tap->historyoffset) << 2), tap->historybuff, 
                              startlen, 44100, tap->volume, tap->pan, FSOUND_STEREO | FSOUND_16BITS);

        /*
            Now copy input into reverb/history buffer 
        */
        src  = (MIXBUFFERFORMAT *)newbuffer;
        dest = (signed short *)(tap->historybuff + (tap->historyoffset << 2));
        for (count=0; count < taillen * 2; count++)     /* *2 for stereo */
        {
            dest[count] = (signed short)(src[count] < -32768 ? -32768 : src[count] > 32767 ? 32767 : src[count]);
        }

        src  = (MIXBUFFERFORMAT *)((char *)newbuffer + ((tap->historylen - tap->historyoffset) * sizeof(MIXBUFFERFORMAT)));
        dest = (signed short *)tap->historybuff;
        for (count=0; count < startlen * 2; count++)
        {
            dest[count] = (signed short)(src[count] < -32768 ? -32768 : src[count] > 32767 ? 32767 : src[count]);
        }
    }
    /*
        No wrapping reverb buffer, just write dest
    */
    else
    {
        int              count;
        signed short    *dest;
        MIXBUFFERFORMAT *src;

        /*
            Mix a scaled version of history buffer into output
        */
        FSOUND_DSP_MixBuffers(newbuffer, tap->historybuff + (tap->historyoffset << 2), 
                              length, 44100, tap->volume, tap->pan, FSOUND_STEREO | FSOUND_16BITS);

        /*
            Now copy input into reverb/history buffer 
        */
        src  = (MIXBUFFERFORMAT *)newbuffer;
        dest = (signed short *)(tap->historybuff + (tap->historyoffset << 2));
        for (count=0; count < length * 2; count++)
        {
            dest[count] = (signed short)(src[count] < -32768 ? -32768 : src[count] > 32767 ? 32767 : src[count]);
        }
    }


    tap->historyoffset += length;
    if (tap->historyoffset >= tap->historylen) 
    {
        tap->historyoffset -= tap->historylen;
    }

    /*
        Reverb history has been mixed into new buffer, so return it.
    */
    return newbuffer;
}



void SetupReverb()
{
	/*
	    REVERB SETUP
	*/
	/* something to fiddle with. */
	int delay[REVERB_NUMTAPS]	= { 131, 149, 173, 211, 281, 401, 457};	/* prime numbers make it sound good! */
	int volume[REVERB_NUMTAPS]	= { 120, 100,  95,  90,  80,  60,  50};
	int pan[REVERB_NUMTAPS]		= { 100, 128, 128, 152, 128, 100, 152};
	int count;

	for (count=0; count< REVERB_NUMTAPS; count++)
	{
		DSP_ReverbTap[count].delayms		= delay[count];	
		DSP_ReverbTap[count].volume			= volume[count];
		DSP_ReverbTap[count].pan			= pan[count];
		DSP_ReverbTap[count].historyoffset	= 0;
		DSP_ReverbTap[count].historylen		= (DSP_ReverbTap[count].delayms * 44100 / 1000);
		if (DSP_ReverbTap[count].historylen < FSOUND_DSP_GetBufferLength())
			DSP_ReverbTap[count].historylen = FSOUND_DSP_GetBufferLength();	/* just in case our calc is not the same. */

#ifdef __MACH__
        DSP_ReverbTap[count].historybuff    = (char *)calloc(DSP_ReverbTap[count].historylen, 4);
#else
		if (MPLibraryIsLoaded())
		{
		    DSP_ReverbTap[count].historybuff	= (char *)MPAllocateAligned(DSP_ReverbTap[count].historylen * 4,
		                                                    kMPAllocateDefaultAligned, kMPAllocateClearMask);	/* * 4 is for 16bit stereo (mmx only) */
		}
		if (!DSP_ReverbTap[count].historybuff)
		{
		    printf("Memory not allocated!");
		    exit (EXIT_FAILURE);
		}
#endif
		
		DSP_ReverbTap[count].workarea		= NULL;
		DSP_ReverbTap[count].Unit			= FSOUND_DSP_Create(&DSP_ReverbCallback, FSOUND_DSP_DEFAULTPRIORITY_USER+20+(count*2),	(void *)&DSP_ReverbTap[count]);

		FSOUND_DSP_SetActive(DSP_ReverbTap[count].Unit, TRUE);
	}
}

void CloseReverb()
{
	int count;

	for (count=0; count<REVERB_NUMTAPS; count++)
	{
		FSOUND_DSP_Free(DSP_ReverbTap[count].Unit);	
		DSP_ReverbTap[count].Unit = NULL;

		free(DSP_ReverbTap[count].historybuff);		
		DSP_ReverbTap[count].historybuff = NULL;

		free(DSP_ReverbTap[count].workarea);		
		DSP_ReverbTap[count].workarea = NULL;
	}
}


int main()
{
    FSOUND_SAMPLE *samp1;
    int channel, originalfreq;

    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        return 0;
    }
		
    /*
        INITIALIZE
    */
    if (!FSOUND_Init(44100, 16, FSOUND_INIT_ACCURATEVULEVELS))
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        return 0;
    }

    /*
        RECORD INTO A STATIC SAMPLE
    */

    /*
        Create a sample to record into
    */
    samp1 = FSOUND_Sample_Alloc(FSOUND_UNMANAGED, RECORDLEN, FSOUND_STEREO | FSOUND_16BITS , 44100, 255, 128, 255);

    printf("\n=========================================================================\n");
    printf("Press a key to start recording 5 seconds worth of 44khz 16bit data\n");
    printf("=========================================================================\n");

    getch();

    if (!FSOUND_Record_StartSample(samp1, FALSE))	/* it will record into this sample for 5 seconds then stop */
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));

        FSOUND_Close();
        return 0;
    }

    do
    {
        printf("\rRecording position = %d", FSOUND_Record_GetPosition());
        fflush(stdout);
        Sleep(50);
    } while (FSOUND_Record_GetPosition() < RECORDLEN && !kbhit());
	
    FSOUND_Record_Stop();	/* it already stopped anyway */

    printf("\n=========================================================================\n");
    printf("Press a key to play back recorded data\n");
    printf("=========================================================================\n");

    getch();

    channel = FSOUND_PlaySound(FSOUND_FREE, samp1);

    printf("Playing back sound...\n");

    do
    {
        printf("\rPlayback position = %d", FSOUND_GetCurrentPosition(channel));
        fflush(stdout);
        Sleep(50);
    } while (FSOUND_IsPlaying(channel) && !kbhit());

    /*
        REALTIME FULL DUPLEX RECORD / PLAYBACK!
    */

    printf("\n=========================================================================\n");
    printf("Press a key to do some full duplex realtime recording!\n");
    printf("=========================================================================\n");

    getch();

    FSOUND_Sample_SetMode(samp1, FSOUND_LOOP_NORMAL);	/* make it a looping sample */

    if (!FSOUND_Record_StartSample(samp1, TRUE))	/* start recording and make it loop also */
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));

        FSOUND_Close();
        return 0;
    }

    /*
        Increase this value if the sound sounds corrupted or the time between recording
        and hearing the result is longer than it should be..
    */
    #define RECORD_DELAY_MS			25
    #define RECORD_DELAY_SAMPLES	(44100 * RECORD_DELAY_MS / 1000)

    /*
        Let the record cursor move forward a little bit first before we try to play it
	    (the position jumps in blocks, so any non 0 value will mean 1 block has been recorded)
    */
    while (!FSOUND_Record_GetPosition()) 
    {
        Sleep(1);
    }

#ifdef ENABLEREVERB
    SetupReverb();
#endif

    channel = FSOUND_PlaySound(FSOUND_FREE, samp1);	/* play the sound */

    originalfreq = FSOUND_GetFrequency(channel);
		
/*	printf("initial delay = %d\n", FSOUND_GetCurrentPosition(channel) - FSOUND_Record_GetPosition()); */

    do
    {
        int playpos, recordpos, diff;
        static int oldrecordpos = 0, oldplaypos = 0;

        playpos = FSOUND_GetCurrentPosition(channel);
        recordpos = FSOUND_Record_GetPosition();

        /* 
            NOTE : As the recording and playback frequencies arent guarranteed to be exactly in 
		    sync, we have to adjust the playback frequency to keep the 2 cursors just enough 
		    apart not to overlap. (and sound corrupted)
		    This code tries to keep it inside a reasonable size window just behind the record
		    cursor. ie [........|play window|<-delay->|<-Record cursor.............] 
        */

        /*
            Dont do this code if either of the cursors just wrapped
        */
        if (playpos > oldplaypos && recordpos > oldrecordpos)	
        {
            diff = playpos - recordpos;

            if (diff > -RECORD_DELAY_SAMPLES)
            {
                FSOUND_SetFrequency(channel, originalfreq - 1000);	/* slow it down */
            }
            else if (diff < -(RECORD_DELAY_SAMPLES * 2))
            {
                FSOUND_SetFrequency(channel, originalfreq + 1000);	/* speed it up */
            }
            else
            {
                FSOUND_SetFrequency(channel, originalfreq);	
            }
        }

        oldplaypos = playpos;
        oldrecordpos = recordpos;

        /*
            Print some info and a VU meter (vu is smoothed)
        */
        {
            char vu[19];
            float vuval, l, r;
            static float smoothedvu = 0;

            FSOUND_GetCurrentLevels(channel, &l, &r);
            vuval = (l+r) * 0.5f;
            vuval *= 18.0f;

            #define VUSPEED 0.2f

            if (vuval > smoothedvu)
            {
                smoothedvu = vuval;
            }

            smoothedvu -= VUSPEED;
            if (smoothedvu < 0)
            {
                smoothedvu = 0;
            }

		    memset(vu, 0, 19);
		    memset(vu, '=', (int)(smoothedvu));

            printf("\rPlay=%6d Rec=%6d (gap=%6d, freqchange=%6d hz) VU:%-15s", playpos, recordpos,  diff, FSOUND_GetFrequency(channel) - originalfreq, vu);
	        fflush(stdout);
        }

        Sleep(10);
    } while (!kbhit());

    FSOUND_StopSound(channel);
    FSOUND_Record_Stop();

#ifdef ENABLEREVERB		
    CloseReverb();
#endif

    /*
        CLEANUP AND SHUTDOWN
    */

    FSOUND_Close();

    return 0;
}
