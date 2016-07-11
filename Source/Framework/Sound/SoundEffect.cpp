#include "SoundEffect.h"
#include <assert.h>

SoundEffect::SoundEffect(const std::string & filename, float vol)
{
}

SoundEffect::~SoundEffect()
{
	stop();
}

void SoundEffect::play(int channel)
{
}

void SoundEffect::stop()
{
	m_isStopping = true;

	LockGuard g(m_muSounds);
	m_pSounds.clear();
	g.unlock();
}

void SoundEffect::setVolume(float vol)
{
	m_curVol = vol;
}

int SoundEffect::collectGarbage(int channel)
{
	if (m_isStopping)
		return 0; // files we be deleted anyway
	
	// delete sounds that are not playing anymore
	LockGuard g(m_muSounds);

	int nDeleted = 0;
	m_pSounds.remove_if([&nDeleted,channel](const std::unique_ptr<MusicFile>& file)
	{
		if(file->getChannel() == channel)
		{
			++nDeleted;
			return true;
		}
		return false;
	});
	
	// only one sound should have played on this channel
	assert(nDeleted <= 1);
	return nDeleted;
}
