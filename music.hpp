#pragma once

#include "SDL_mixer.h"
#include "math.hpp"

#include <string>

namespace kd
{
	class Music
	{
	public:
		std::string filename;
		Mix_Music *music;
		uint32 start_time;
		bool playing;
		float bpm;

		Music(std::string filename, float bpm)
			: filename(filename), bpm(bpm), music(0), playing(false), start_time(0)
		{
		}

		~Music() { if(music) Mix_FreeMusic(music); }

		void init()
		{
			Mix_Init(MIX_INIT_OGG);

	        if (Mix_OpenAudio(44100,AUDIO_S16SYS, 2, 512) != 0)
	        {
	            fprintf(stderr, "Ei saatu audioo :( %s\n", Mix_GetError());
	            exit(1);
	        }

			music = Mix_LoadMUS(filename.c_str());

			if (!music)
		    {
		        printf("Ei saatu musaa :( %s\n", Mix_GetError());
		        exit(1);
		    }
		}

		void play()
		{
			if (Mix_PlayMusic(music, 1) == 0)
			{
				start_time = SDL_GetTicks();
				playing = true;
			}
			else
			{
				printf("Musa ei soi :/ %s\n", Mix_GetError());
			}
		}

		void setEndHook(void (*music_finished)())
		{
			Mix_HookMusicFinished(music_finished);
		}

		uint32 getTicks()
		{
			if (playing)
			{
				return SDL_GetTicks() - start_time;
			}

			return 0;
		}

		float getTime()
		{
			if (playing)
			{
				return float(getTicks()) / 1000.0;
			}

			return -1.f;
		}

		int getBeats()
		{
			const uint32 beat = 60000 / bpm;

			return getTicks() / beat;
		}

		float getBeat(bool closest=false)
		{
			const uint32 beat = 60000 / bpm;
			const uint32 halfbeat = beat / 2;

			if (closest)
				return float(absi((getTicks() % beat) - halfbeat)) / float(halfbeat);
			else
				return float(beat - (getTicks() % beat)) / float(beat);
		}
	};
}