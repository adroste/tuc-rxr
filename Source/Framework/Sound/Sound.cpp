#include "Sound.h"

#include "../../SDL/SDL.h"
#include "../../SDL/SDL_mixer.h"
#include "../../Utility/MIX_Exception.h"
#include "../../System/Log.h"
#include "MusicFile.h"
#include <assert.h>
#include <vector>
#include <memory>

static std::unique_ptr<MusicFile> m_musicFiles[size_t(Sound::Music::Size)];
static std::unique_ptr<MusicFile> m_soundFiles[size_t(Sound::Sfx::Size)];
static Sound::Music m_curTrack = Sound::Music::Size;

void Sound::init()
{
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
		throw MIX_Exception("Sound::init ogg");

	if (Mix_OpenAudio(SAMPLERATE, MIX_DEFAULT_FORMAT, CHANNELS, 4096) != 0)
		throw MIX_Exception("Sound::init open audio");

	auto num = Mix_AllocateChannels(32);
	Log::info("Sound::init " + std::to_string(num) + " audio channels allocated");
}

void Sound::quit()
{
	Log::info("Sound::quit");
	for (auto& m : m_musicFiles)
		m.reset();

	for (auto& m : m_soundFiles)
		m.reset();

	Mix_CloseAudio();
	Mix_Quit();
}

void Sound::loadFiles()
{
	m_musicFiles[size_t(Sound::Music::Theme)] = std::unique_ptr<MusicFile>
		(new MusicFile("data/Sound/theme.ogg", 1.0f));

	m_soundFiles[size_t(Sound::Sfx::Plop)] = std::unique_ptr<MusicFile>
		(new MusicFile("data/Sound/plop.ogg", 1.0f));
}

void Sound::playMusic(Music track)
{
	if(m_curTrack != Sound::Music::Size)
	{
		// something is playing
		m_musicFiles[size_t(m_curTrack)]->stop();
	}
	m_curTrack = track;
	m_musicFiles[size_t(track)]->play(true);
}

void Sound::playSound(Sfx sound)
{
	m_soundFiles[size_t(sound)]->play(true);
}
