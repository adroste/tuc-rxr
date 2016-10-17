#include "Sound.h"

#include "../../SDL/SDL.h"
#include "../../SDL/SDL_mixer.h"
#include "../../System/Exceptions/MIX_Exception.h"
#include "../../System/Log.h"
#include "MusicFile.h"
#include <vector>
#include <memory>
#include "../../Utility/Tools.h"
#include "../../Utility/Exception.h"

static const float MUSIC_FADE = 3.0f;
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

	m_musicFiles[size_t(Sound::Music::Hey)] = std::unique_ptr<MusicFile>
		(new MusicFile("data/Sound/hey.ogg", 1.0f));

	m_soundFiles[size_t(Sound::Sfx::Plop)] = std::unique_ptr<MusicFile>
		(new MusicFile("data/Sound/plop.ogg", 1.0f));

	m_soundFiles[size_t(Sound::Sfx::Knock)] = std::unique_ptr<MusicFile>
		(new MusicFile("data/Sound/knock.wav", 1.0f));
}

void Sound::playMusic(Music track)
{
	if (track == m_curTrack) return;

	if (m_curTrack != Sound::Music::Size)
	{
		// something is playing
		m_musicFiles[size_t(m_curTrack)]->fadeOut(MUSIC_FADE);
	}
	m_curTrack = track;
	m_musicFiles[size_t(track)]->fadeIn(MUSIC_FADE);
}

void Sound::playSound(Sfx sound)
{
	m_soundFiles[size_t(sound)]->play(false);
}

void Sound::update(float dt)
{
	for (auto& m : m_musicFiles)
		m->update(dt);
}

void Sound::setMusicVolume(float vol)
{
	vol = tool::clamp(vol, 0.0f, 1.0f);
	for (auto& m : m_musicFiles)
		m->setVolume(vol);
}

void Sound::setSoundVolume(float vol)
{
	vol = tool::clamp(vol, 0.0f, 1.0f);
	for (auto& m : m_soundFiles)
		m->setVolume(vol);
}
