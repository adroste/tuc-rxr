#pragma once
#include <string>
#include "../../SDL/SDL_mixer.h"

class MusicFile
{
public:
	MusicFile(const std::string& filename, float vol);
	~MusicFile();

	void play(bool repeat);
	void stop();
	bool isPlaying() const;
	void setVolume(float vol);
	void setMute(bool mute);
	int getChannel() const;

private:
	void setVolumeNoSave(int vol) const;
private:
	bool m_isMuted = false;
	Mix_Chunk* m_pMusic = nullptr;
	int m_curVol;
	int m_channel = -1;

	bool bPaused = false;
};