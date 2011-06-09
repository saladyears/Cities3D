//===============================================================================================
// 3D
// Copyright (c), Firelight Technologies Pty, Ltd, 1999-2004.
//
// This test shows EAX, DS3D and Software all being used together and the simple commands needed
// to set up some 3d audio.
// This application also displays the use of FSOUND_GetDriverCaps to get information on the 
// 3D capabilities of the selected driver
//===============================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../api/inc/fmod.h"
#include "../../api/inc/fmod_errors.h"	// optional
#include "../../api/inc/wincompat.h"

#define INTERFACE_UPDATETIME 50		// 50ms update for interface

/*
[
	[DESCRIPTION]

	[PARAMETERS]
 
	[RETURN_VALUE]

	[REMARKS]

	[SEE_ALSO]
]
*/
void Close(FSOUND_SAMPLE *samp1, FSOUND_SAMPLE *samp2, FSOUND_SAMPLE *samp3)
{
	// you dont need to free samples if you let fsound's sample manager look after samples, as 
	// it will free them all for you.
	FSOUND_Sample_Free(samp1);	
	FSOUND_Sample_Free(samp2);
	FSOUND_Sample_Free(samp3);

	FSOUND_Close();
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
	FSOUND_SAMPLE	*samp1 = NULL, *samp2 = NULL, *samp3 = NULL;
	char			key, listenerflag = 1;
	int             i = 0, channel1 = -1, channel2 = -1;
	float			listenerpos[3] =  { 0,0,0 }; 
    
	if (FSOUND_GetVersion() < FMOD_VERSION)
	{
		printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
		return 1;
	}

	{
		unsigned int caps = 0;

		FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);
		
		printf("---------------------------------------------------------\n");	
		printf("Driver capabilities\n");
		printf("---------------------------------------------------------\n");	
		if (!caps)
			printf("- This driver will support software mode only.\n  It does not properly support 3D sound hardware.\n");
		if (caps & FSOUND_CAPS_HARDWARE)
			printf("- Driver supports hardware 3D sound!\n");
		if (caps & FSOUND_CAPS_EAX2)
			printf("- Driver supports EAX 2 reverb!\n");
		if (caps & FSOUND_CAPS_EAX3)
			printf("- Driver supports EAX 3 reverb!\n");
		printf("---------------------------------------------------------\n");	
	}

	// ==========================================================================================
	// INITIALIZE
	// ==========================================================================================
	if (!FSOUND_Init(44100, 32, 0))
	{
		printf("Init: %s\n", FMOD_ErrorString(FSOUND_GetError()));
		return 1;
	}

	// ==========================================================================================
	// LOAD SAMPLES
	// ==========================================================================================

	// ==========================================================================================
	// 3D MONO 
	// ==========================================================================================

#if defined(__MACH__) || defined(WIN32)
	samp1 = FSOUND_Sample_Load(FSOUND_FREE, "../../media/drumloop.wav", FSOUND_HW3D, 0, 0);
#else
	samp1 = FSOUND_Sample_Load(FSOUND_FREE, ":::media:drumloop.wav", FSOUND_HW3D, 0, 0);
#endif	
 	if (!samp1)
	{
		printf("samp1: %s\n", FMOD_ErrorString(FSOUND_GetError()));
		Close(samp1, samp2, samp3);
		return 1;
	}

	// increasing mindistance makes it louder in 3d space
	FSOUND_Sample_SetMinMaxDistance(samp1, 4.0f, 1000.0f);	
	FSOUND_Sample_SetMode(samp1, FSOUND_LOOP_NORMAL);

	// ==========================================================================================
	// 3D MONO 
	// ==========================================================================================
#if defined(__MACH__) || defined(WIN32)
	samp2 = FSOUND_Sample_Load(FSOUND_UNMANAGED, "../../media/jungle.wav", FSOUND_HW3D, 0, 0);
#else
	samp2 = FSOUND_Sample_Load(FSOUND_UNMANAGED, ":::media:jungle.wav", FSOUND_HW3D, 0, 0);
#endif
	if (!samp2)
	{
		printf("samp2: %s\n", FMOD_ErrorString(FSOUND_GetError()));
		Close(samp1, samp2, samp3);
		return 1;
	}
	// increasing mindistance makes it louder in 3d space
	FSOUND_Sample_SetMinMaxDistance(samp2, 4.0f, 1000.0f);
	FSOUND_Sample_SetMode(samp2, FSOUND_LOOP_NORMAL);

	// ==========================================================================================
	// 2D STEREO
	// ==========================================================================================
#if defined(__MACH__) || defined(WIN32)
	samp3 = FSOUND_Sample_Load(FSOUND_UNMANAGED, "../../media/chimes.wav", FSOUND_2D, 0, 0);
#else	
	samp3 = FSOUND_Sample_Load(FSOUND_UNMANAGED, ":::media:chimes.wav", FSOUND_2D, 0, 0);
#endif	
	if (!samp3)
	{
		printf("samp3: %s\n", FMOD_ErrorString(FSOUND_GetError()));
		Close(samp1, samp2, samp3);
		return 1;
	}

	// ==========================================================================================
	// DISPLAY HELP
	// ==========================================================================================

	printf("FSOUND Output Method : ");
	switch (FSOUND_GetOutput())
	{
            case FSOUND_OUTPUT_NOSOUND:	printf("FSOUND_OUTPUT_NOSOUND\n"); break;
	    case FSOUND_OUTPUT_MAC:     printf("FSOUND_OUTPUT_MAC\n"); break;	   
	};

	printf("FSOUND Driver        : ");
	printf("%s\n", FSOUND_GetDriverName(FSOUND_GetDriver()));
	printf("Hardware 3D channels : %d\n", FSOUND_GetNumHardwareChannels());

	printf("=========================================================================\n");
	printf("Press 1		Pause/Unpause 16bit 3D sound at any time\n");
	printf("      2		Pause/Unpause 8bit 3D sound at any time\n");
	printf("      3		Play 16bit STEREO 2D sound at any time\n");
	printf("      4		Change to EAX Reverb mode CONCERTHALL (DirectSound/SBLive only)\n");
	printf("      5		Change to EAX Reverb mode SEWERPIPE (DirectSound/SBLive only)\n");
	printf("      6		Change to EAX Reverb mode PSYCHOTIC (DirectSound/SBLive only)\n");
	printf("      <		Move listener left (in still mode)\n");
	printf("      >		Move listener right (in still mode)\n");
	printf("      SPACE Stop/Start listener automatic movement\n");
	printf("      ESC	Quit\n");
	printf("=========================================================================\n");

	// ==========================================================================================
	// PLAY 2 LOOPING SOUNDS
	// ==========================================================================================

	{
		float pos[3] = { -10.0f, -0.0f, 0.0f };
		float vel[3] = { 0,0,0 };

		channel1 = FSOUND_PlaySoundEx(FSOUND_FREE, samp1, NULL, TRUE);
		FSOUND_3D_SetAttributes(channel1, pos, vel);
		FSOUND_SetPaused(channel1, FALSE);
	}
	{
		float pos[3] = { 15.0f, -0.0f, -0.0f };
		float vel[3] = { 0,0,0 };

		channel2 = FSOUND_PlaySoundEx(FSOUND_FREE, samp2, NULL, TRUE);
		FSOUND_3D_SetAttributes(channel2, pos, vel);
        FSOUND_SetVolume(channel2, 128);
		FSOUND_SetPaused(channel2, FALSE);
	}

	// ==========================================================================================
	// MAIN LOOP
	// ==========================================================================================

	do
	{
		if (kbhit())
		{
			key = getch();

			if (key == '1') 
            {
				FSOUND_SetPaused(channel1, !FSOUND_GetPaused(channel1));
            }
			if (key == '2') 
            {
				FSOUND_SetPaused(channel2, !FSOUND_GetPaused(channel2));
            }
			if (key == '3') 
            {
				FSOUND_PlaySoundEx(FSOUND_FREE, samp3, NULL, FALSE);
            }
			if (key == '4') 
            {
                FSOUND_REVERB_PROPERTIES props = FSOUND_PRESET_CONCERTHALL;
				FSOUND_Reverb_SetProperties(&props);
            }
			if (key == '5') 
            {
                FSOUND_REVERB_PROPERTIES props = FSOUND_PRESET_SEWERPIPE;
				FSOUND_Reverb_SetProperties(&props);
            }
			if (key == '6')
            {
                FSOUND_REVERB_PROPERTIES props = FSOUND_PRESET_PSYCHOTIC;
				FSOUND_Reverb_SetProperties(&props);
            }

			if (key == ' ')
            {
				listenerflag = !listenerflag;
            }

			if (!listenerflag)
			{
				if (key == '<') 
				{
					listenerpos[0] -= 1.0f;
					if (listenerpos[0] < -35)
						listenerpos[0] = -35;
				}
				if (key == '>') 
				{
					listenerpos[0] += 1.0f;
					if (listenerpos[0] > 30)
						listenerpos[0] = 30;
				}
			}
		}


		// ==========================================================================================
		// UPDATE THE LISTENER
		// ==========================================================================================
		{
			static float t = 0;
			static float lastpos[3] = { 0,0,0 };
			float vel[3];

			if (listenerflag)
				listenerpos[0] = ((float)sin(t*0.05f) * 33.0f); // left right pingpong

			// ********* NOTE ******* READ NEXT COMMENT!!!!!
			// vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).
			vel[0] = (listenerpos[0]-lastpos[0]) * (1000 / INTERFACE_UPDATETIME);
			vel[1] = (listenerpos[1]-lastpos[1]) * (1000 / INTERFACE_UPDATETIME);
			vel[2] = (listenerpos[2]-lastpos[2]) * (1000 / INTERFACE_UPDATETIME);

			// store pos for next time
			lastpos[0] = listenerpos[0];
			lastpos[1] = listenerpos[1];
			lastpos[2] = listenerpos[2];

			FSOUND_3D_Listener_SetAttributes(&listenerpos[0], &vel[0], 0, 0, 1.0f, 0, 1.0f, 0);

			t += (30 * (1.0f / (float)INTERFACE_UPDATETIME));	// t is just a time value .. it increments in 30m/s steps in this example

			// print out a small visual display
			{
				char s[80];

				sprintf(s, "|.......................<1>......................<2>....................|");
				
                		s[(int)listenerpos[0]+35] = 'L';
				
				printf("\r%s", s);
				fflush(stdout);
			}			
		}

		FSOUND_Update();

		Sleep(INTERFACE_UPDATETIME-1);	// -2 for printf etc

	} while (key != 27);

	printf("\n");

	// ==========================================================================================
	// CLEANUP AND SHUTDOWN
	// ==========================================================================================

	Close(samp1, samp2, samp3);
   
    return 0;
}
