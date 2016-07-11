#include "Sound.h"

#include "../../SDL/SDL.h"
#include "../../SDL/SDL_mixer.h"
#include "../../Utility/MIX_Exception.h"
#include "../../System/Log.h"
#include "MusicFile.h"
#include <assert.h>
#include <vector>
#include <memory>

static std::vector<size_t> m_playingChannels;
static std::unique_ptr<MusicFile> m_musicFiles[size_t(Sound::Music::Size)];

static void soundCallback(int channel)
{
	
}

// returns -1 if no channel is free
static int getNextFreeChannel()
{
	static auto curChannel = 0;
	auto startChan = curChannel;
	do
	{
		if (m_playingChannels[curChannel] == 0)
		{
			m_playingChannels[curChannel] = 1; // use this channel
			return curChannel;
		}
		curChannel = (curChannel + 1) % m_playingChannels.size();
	} while (startChan != curChannel);
	return -1;
}

void Sound::init()
{
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
		throw MIX_Exception("Sound::init ogg");

	if (Mix_OpenAudio(SAMPLERATE, MIX_DEFAULT_FORMAT, CHANNELS, 4096) != 0)
		throw MIX_Exception("Sound::init open audio");

	auto num = Mix_AllocateChannels(32);
	Log::info("Sound::init " + std::to_string(num) + " audio channels allocated");

	m_playingChannels.assign(num, 0);

	Mix_ChannelFinished(soundCallback);
}

void Sound::quit()
{
	Log::info("Sound::quit");
	for (auto& m : m_musicFiles)
		m.reset();

	Mix_CloseAudio();
	Mix_Quit();
}

void Sound::loadFiles()
{
	m_musicFiles[size_t(Sound::Music::Theme)] = std::unique_ptr<MusicFile>
		(new MusicFile("data/Sound/theme.ogg", 1.0f));
}

void Sound::playMusic(Music track)
{
	m_musicFiles[size_t(track)]->play(getNextFreeChannel(), true);
}
