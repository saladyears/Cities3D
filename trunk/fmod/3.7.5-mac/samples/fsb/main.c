//===============================================================================================
// FSB
// Copyright (c), Firelight Technologies Pty, Ltd, 1999-2004.
//
// This example demonstrates use of the FMOD Sample Bank format and also usage of the 
// FSOUND_Sample_SetDefaultsEx function.
//===============================================================================================

#include <string.h>
#include <stdio.h>

#include "../../api/inc/wincompat.h"
#include "../../api/inc/fmod.h"
#include "../../api/inc/fmod_errors.h"    // optional


/*
[
    [DESCRIPTION]

    [PARAMETERS]
 
    [RETURN_VALUE]

    [REMARKS]

    [SEE_ALSO]
]
*/
int main(int argc, char *argv[])
{
    FSOUND_SAMPLE  *sample;
    FMUSIC_MODULE  *mod;
    char            key;
    int             sampleindex = 0, variation = 1, lastopenstate = -1;

    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
        return 1;
    }

    // ==========================================================================================
    // INITIALIZE
    // ==========================================================================================
    if (!FSOUND_Init(44100, 32, FSOUND_INIT_USEDEFAULTMIDISYNTH))
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        FSOUND_Close();
        return 1;
    }

    // ==========================================================================================
    // OPEN FSB
    // ==========================================================================================
#if defined(__MACH__)
    mod = FMUSIC_LoadSongEx("../../media/footsteps.fsb", 0, 0, FSOUND_NONBLOCKING, 0, 0);
#else
    mod = FMUSIC_LoadSongEx(":::media:footsteps.fsb", 0, 0, FSOUND_NONBLOCKING, 0, 0);
#endif
    if (!mod)
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
        FSOUND_Close();
        return 1;
    }

    printf("=========================================================================\n");
    printf("Press SPACE to toggle pitch/volume variation\n");
    printf("Press ESC   to quit\n");
    printf("=========================================================================\n");
    printf("\n");
  
    key = 0;
    do
    {
        /*
            Set initial defaults for both samples. Do this only once as soon as the FSB has finished loading.
        */
        if ((lastopenstate != 0) && (FMUSIC_GetOpenState(mod) == 0))
        {     	
            sample = FMUSIC_GetSample(mod, 0);
            FSOUND_Sample_SetDefaultsEx(sample, -1, -1, -1, -1, 2000, 128, -1);
            sample = FMUSIC_GetSample(mod, 1);
            FSOUND_Sample_SetDefaultsEx(sample, -1, -1, -1, -1, 2000, 128, -1);
            lastopenstate = 0;
        }

        /*
            Play a sample from the FSB. Do this once every frame when the FSB has finished loading.
        */
        if (FMUSIC_GetOpenState(mod) == 0)
        {
            sample  = FMUSIC_GetSample(mod, sampleindex++ & 1);
            FSOUND_PlaySound(FSOUND_FREE, sample);
        }

        if (kbhit())
        {
            key = getch();
            if (key == ' ')
            {
                variation ^= 1;
            }

            /*
                Change the defaults/variations on both samples.
            */
            if (variation)
            {
                sample = FMUSIC_GetSample(mod, 0);
                FSOUND_Sample_SetDefaultsEx(sample, -1, -1, -1, -1, 2000, 128, -1);
                sample = FMUSIC_GetSample(mod, 1);
                FSOUND_Sample_SetDefaultsEx(sample, -1, -1, -1, -1, 2000, 128, -1);
            }
            else
            {
                sample = FMUSIC_GetSample(mod, 0);
                FSOUND_Sample_SetDefaultsEx(sample, -1, -1, -1, -1, 0, 0, 0);
                sample = FMUSIC_GetSample(mod, 1);
                FSOUND_Sample_SetDefaultsEx(sample, -1, -1, -1, -1, 0, 0, 0);
            }
        }

        printf("\rPitch/volume variation: %s", variation ? "on " : "off");
        fflush(stdout);

        Sleep(600 + (variation ? (rand() % 100) : 50));
    
    } while (key != 27);

    printf("\n");

    FMUSIC_FreeSong(mod);
    FSOUND_Close();

    return 0;
}
