/*
 RECORD.EXE
 Copyright (c), Firelight Technologies Pty, Ltd, 2000-2003.

 This example shows how to record data to a static sample, or record dynamically, and have
 a dsp unit processing the result.
 The reverb below is taken from /samples/fmod/fmod.c 
*/

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32) || defined(__WATCOMC__)
  #include <conio.h>
  #include <windows.h>
#else
  #include "../../api/inc/wincompat.h"
  #include <string.h>
#endif

#include "../../api/inc/fmod.h"
#include "../../api/inc/fmod_errors.h"	/* optional */

#define	ENABLEREVERB	TRUE
#define RECORDRATE      44100
#define RECORDLEN       (RECORDRATE * 5)    /* 5 seconds at RECORDRATE khz */

#define REVERB_NUMTAPS	7

typedef struct
{
	FSOUND_DSPUNIT	*Unit;
	char			*historybuff;		/* storage space for tap history */
	char			*workarea;			/* a place to hold 1 buffer worth of data (for reverb) */
	int				delayms;			/* delay of reverb tab in milliseconds */
	int				volume;				/* volume of reverb tab */
	int				pan;				/* pan of reverb tab */
	int				historyoffset;		/* running offset into history buffer */
	int				historylen;			/* size of history buffer in SAMPLES */
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
void * F_CALLBACKAPI DSP_ReverbCallback(void *originalbuffer, void *newbuffer, int length, void *param)
{
    int           mixertype = FSOUND_GetMixer();
    REVERBTAP    *tap = (REVERBTAP *)param;

    #define MIXBUFFERFORMAT signed short /* This determines the format of the mixbuffer being passed in.  change if you want to support int32 or float32 */

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
            int val = (int)dest[count];
            dest[count] = (signed short)(val < -32768 ? -32768 : val > 32767 ? 32767 : val);
        }

        src  = (MIXBUFFERFORMAT *)((char *)newbuffer + ((tap->historylen - tap->historyoffset) * sizeof(MIXBUFFERFORMAT)));
        dest = (signed short *)tap->historybuff;
        for (count=0; count < startlen * 2; count++)
        {
            int val = (int)dest[count];
            dest[count] = (signed short)(val < -32768 ? -32768 : val > 32767 ? 32767 : val);
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
            int val = (int)dest[count];
            dest[count] = (signed short)(val < -32768 ? -32768 : val > 32767 ? 32767 : val);
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

		DSP_ReverbTap[count].historybuff	= (char *)calloc(DSP_ReverbTap[count].historylen, 4);	/* * 4 is for 16bit stereo (mmx only) */
		DSP_ReverbTap[count].workarea		= NULL;
		DSP_ReverbTap[count].Unit			= FSOUND_DSP_Create(&DSP_ReverbCallback, FSOUND_DSP_DEFAULTPRIORITY_USER+20+(count*2),	&DSP_ReverbTap[count]);

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


int main(int argc, char *argv[])
{
	FSOUND_SAMPLE *samp1;
	signed char key;
	int driver, i, channel, originalfreq;

	if (FSOUND_GetVersion() < FMOD_VERSION)
	{
		printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
		return 0;
	}

	/*
	    SELECT OUTPUT METHOD
	*/
	
    printf("---------------------------------------------------------\n");	
    printf("Output Type\n");	
    printf("---------------------------------------------------------\n");	
#if defined(WIN32) || defined(__CYGWIN32__) || defined(__WATCOMC__)
    printf("1 - Direct Sound\n");
    printf("2 - Windows Multimedia Waveout\n");
    printf("3 - NoSound\n");
#elif defined(__linux__)
    printf("1 - OSS - Open Sound System\n");
    printf("2 - ESD - Elightment Sound Daemon\n");
    printf("3 - ALSA 0.9 - Advanced Linux Sound Architecture\n");   
#endif
	printf("---------------------------------------------------------\n");	/* print driver names */
	printf("Press a corresponding number or ESC to quit\n");

	do
	{
		key = getch();
	} while (key != 27 && key < '1' && key > '4');
	
	switch (key)
	{
#if defined(WIN32) || defined(__CYGWIN32__) || defined(__WATCOMC__)
		case '1' :	FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
					break;
		case '2' :	FSOUND_SetOutput(FSOUND_OUTPUT_WINMM);
					break;
		case '3' :	FSOUND_SetOutput(FSOUND_OUTPUT_NOSOUND);
					break;
#elif defined(__linux__)
		case '1' :	FSOUND_SetOutput(FSOUND_OUTPUT_OSS);
					break;
		case '2' :	FSOUND_SetOutput(FSOUND_OUTPUT_ESD);
					break;
		case '3' :	FSOUND_SetOutput(FSOUND_OUTPUT_ALSA);
					break;
#endif
		default :	return 0;
	}
	
	/*
	    SELECT OUTPUT DRIVER
	*/

	/* The following list are the drivers for the output method selected above. */
    printf("---------------------------------------------------------\n");	
    switch (FSOUND_GetOutput())
    {
        case FSOUND_OUTPUT_NOSOUND:    printf("NoSound"); break;
        case FSOUND_OUTPUT_WINMM:      printf("Windows Multimedia Waveout"); break;
        case FSOUND_OUTPUT_DSOUND:     printf("Direct Sound"); break;
        case FSOUND_OUTPUT_OSS:        printf("Open Sound System"); break;
        case FSOUND_OUTPUT_ESD:        printf("Enlightment Sound Daemon"); break;
        case FSOUND_OUTPUT_ALSA:       printf("ALSA"); break;       
    };
	printf(" Driver list\n");	
	printf("---------------------------------------------------------\n");	

	for (i=0; i < FSOUND_GetNumDrivers(); i++) 
	{
		printf("%d - %s\n", i+1, FSOUND_GetDriverName(i));	/* print driver names */
	}
	printf("---------------------------------------------------------\n");	/* print driver names */
	printf("Press a corresponding number or ESC to quit\n");

	do
	{
		key = getch();
		if (key == 27) 
		{
			FSOUND_Close();
			return 0;
		}
		driver = key - '1';
	} while (driver < 0 || driver >= FSOUND_GetNumDrivers());

	FSOUND_SetDriver(driver);					/* Select sound card (0 = default) */

	/*
	    SELECT MIXER
	*/

	FSOUND_SetMixer(FSOUND_MIXER_QUALITY_AUTODETECT);
		
	/*
	    INITIALIZE
	*/
	if (!FSOUND_Init(44100, 64, FSOUND_INIT_ACCURATEVULEVELS))
	{
		printf("Error!\n");
		printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
		return 0;
	}


	/*
	    SELECT INPUT DRIVER (can be done before or after init)
	*/

	/* The following list are the drivers for the output method selected above. */
	printf("---------------------------------------------------------\n");	
	switch (FSOUND_GetOutput())
	{
        case FSOUND_OUTPUT_NOSOUND:    printf("NoSound"); break;
        case FSOUND_OUTPUT_WINMM:      printf("Windows Multimedia Waveout"); break;
        case FSOUND_OUTPUT_DSOUND:     printf("Direct Sound"); break;
        case FSOUND_OUTPUT_OSS:        printf("Open Sound System"); break;
        case FSOUND_OUTPUT_ESD:        printf("Enlightment Sound Daemon"); break;
        case FSOUND_OUTPUT_ALSA:       printf("ALSA"); break;       
	};
	printf(" Recording device driver list\n");	
	printf("---------------------------------------------------------\n");	

	for (i=0; i < FSOUND_Record_GetNumDrivers(); i++) 
	{
		printf("%d - %s\n", i+1, FSOUND_Record_GetDriverName(i));	/* print driver names */
	}
	printf("---------------------------------------------------------\n");	/* print driver names */
	printf("Press a corresponding number or ESC to quit\n");

	do
	{
		key = getch();
		if (key == 27) 
			return 0;
		driver = key - '1';
	} while (driver < 0 || driver >= FSOUND_Record_GetNumDrivers());

	if (!FSOUND_Record_SetDriver(driver))	/* Select input sound card (0 = default) */
	{
		printf("Error!\n");
		printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
		FSOUND_Close();
		return 0;
	}
	
	/*
	    DISPLAY HELP
	*/

	printf("FSOUND Output Method : ");
	switch (FSOUND_GetOutput())
	{
        case FSOUND_OUTPUT_NOSOUND:    printf("FSOUND_OUTPUT_NOSOUND\n"); break;
        case FSOUND_OUTPUT_WINMM:      printf("FSOUND_OUTPUT_WINMM\n"); break;
        case FSOUND_OUTPUT_DSOUND:     printf("FSOUND_OUTPUT_DSOUND\n"); break;
        case FSOUND_OUTPUT_OSS:        printf("FSOUND_OUTPUT_OSS\n"); break;
        case FSOUND_OUTPUT_ESD:        printf("FSOUND_OUTPUT_ESD\n"); break;
        case FSOUND_OUTPUT_ALSA:       printf("FSOUND_OUTPUT_ALSA\n"); break;       
	};

	printf("FSOUND Mixer         : ");
	switch (FSOUND_GetMixer())
	{
		case FSOUND_MIXER_BLENDMODE:	printf("FSOUND_MIXER_BLENDMODE\n"); break;
		case FSOUND_MIXER_MMXP5:		printf("FSOUND_MIXER_MMXP5\n"); break;
		case FSOUND_MIXER_MMXP6:		printf("FSOUND_MIXER_MMXP6\n"); break;
		case FSOUND_MIXER_QUALITY_FPU:	printf("FSOUND_MIXER_QUALITY_FPU\n"); break;
		case FSOUND_MIXER_QUALITY_MMXP5:printf("FSOUND_MIXER_QUALITY_MMXP5\n"); break;
		case FSOUND_MIXER_QUALITY_MMXP6:printf("FSOUND_MIXER_QUALITY_MMXP6\n"); break;
	};
	printf("FSOUND Driver        : %s\n", FSOUND_GetDriverName(FSOUND_GetDriver()));
	printf("FSOUND Record Driver : %s\n", FSOUND_Record_GetDriverName(FSOUND_Record_GetDriver()));

	/*
	    RECORD INTO A STATIC SAMPLE
	*/

	/*
        Create a sample to record into
    */
    samp1 = FSOUND_Sample_Alloc(FSOUND_UNMANAGED, RECORDLEN, FSOUND_STEREO | FSOUND_16BITS , RECORDRATE, 255, 128, 255);
    if (!samp1)
    {
        printf("Error!\n");
		printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        FSOUND_Close();
        return 0;
    }

	printf("\n");
	printf("=========================================================================\n");
	printf("Press a key to start recording 5 seconds worth of data\n");
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
		printf("Recording position = %d\r", FSOUND_Record_GetPosition());
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
		printf("Playback position = %d\r", FSOUND_GetCurrentPosition(channel));
		Sleep(50);
	} while (FSOUND_IsPlaying(channel) && !kbhit());

    if (FSOUND_GetOutput() == FSOUND_OUTPUT_OSS)
    {
        FSOUND_Sample_Free(samp1);
        FSOUND_Close();
        return 0;
    }

	/*
	    REALTIME FULL DUPLEX RECORD / PLAYBACK!
	*/

	printf("\n=========================================================================\n");
	printf("Press a key to do some full duplex realtime recording!\n");
	printf("(with reverb for mmx users)\n");
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
    #define RECORD_DELAY_MS       25
    #define RECORD_DELAY_SAMPLES  (RECORDRATE * RECORD_DELAY_MS / 1000)

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

			printf("Play=%6d Rec=%6d (gap=%6d, freqchange=%6d hz) VU:%-15s\r", playpos, recordpos,  diff, FSOUND_GetFrequency(channel) - originalfreq, vu);
		}

		Sleep(10);
	} while (!kbhit());

    getch();

	FSOUND_StopSound(channel);
	FSOUND_Record_Stop();

#ifdef ENABLEREVERB		
	CloseReverb();
#endif

	/*
	    CLEANUP AND SHUTDOWN
	*/

    FSOUND_Sample_Free(samp1);
    FSOUND_Close();
    return 0;
}

