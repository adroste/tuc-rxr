#pragma once
#include <string>
#include "../../SDL/SDL_mixer.h"

class MusicFile
{
private:
	enum class State
	{
		None,
		FadeIn,
		FadeOut
	};

public:
	MusicFile(const std::string& filename, float vol);
	~MusicFile();

	void play(bool repeat);
	void stop();
	bool isPlaying() const;
	void setVolume(float vol);
	void setMute(bool mute);
	int getChannel() const;
	void fadeIn(float secs);
	void fadeOut(float secs);
	void update(float dt);

private:
	void setVolumeNoSave(int vol);
	float fadeInFunc(float x) const;

private:
	bool m_isMuted = false;
	Mix_Chunk* m_pMusic = nullptr;
	int m_curVol;
	int m_channel = -1;
	bool m_isPaused = false;
	State m_curState = State::None;
	float m_fadeMax = 0.0f;
	float m_fadeCur = 0.0f;
};