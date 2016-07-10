#pragma once

class Sound
{
public:
	enum class Music
	{
		Theme,
		Size
	};

public:
	static void init();
	static void quit();

	static void loadFiles();
	static void playMusic(Music track);
public:
	static const int SAMPLERATE = 44100;
	static const int BITRATE = 16;
	static const int CHANNELS = 2;
};