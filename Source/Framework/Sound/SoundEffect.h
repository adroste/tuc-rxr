#pragma once
#include "MusicFile.h"
#include <memory>
#include "../../Utility/Mutex.h"
#include <list>

class SoundEffect
{
public:
	SoundEffect(const std::string& filename, float vol);
	~SoundEffect();
	void play(int channel);
	void stop();
	void setVolume(float vol);
	// deletes finished sounds
	int collectGarbage(int channel);
private:
	float m_curVol;
	std::list< std::unique_ptr< MusicFile >> m_pSounds;

	bool m_isStopping = false;
	Mutex m_muSounds;
};
