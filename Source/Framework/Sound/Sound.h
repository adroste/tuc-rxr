#pragma once

class Sound
{
public:
	enum class Music
	{
		Theme,
		Hey,
		Size
	};

	enum class Sfx
	{
		Plop,
		Size
	};

public:
	static void init();
	static void quit();

	static void loadFiles();
	static void playMusic(Music track);
	static void playSound(Sfx sound);

	static void update(float dt);
	
	static void setMusicVolume(float vol);
	static void setSoundVolume(float vol);
public:
	static const int SAMPLERATE = 44100;
	static const int BITRATE = 16;
	static const int CHANNELS = 2;
};