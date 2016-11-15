#include "VolumeTextureMap.h"
#include "../../System/Exceptions/GL_Exception.h"
#include "Drawing.h"
#include "../../System/System.h"
#include "../Framework.h"
#include "../../Game/Client/MapAsset.h"

VolumeTextureMap::VolumeTextureMap()
{

}

VolumeTextureMap::~VolumeTextureMap()
{
}

void VolumeTextureMap::resize(Point3S dim)
{
	MAIN_THREAD;
	auto newDim = std::unique_ptr<unsigned char[]>(new unsigned char[dim.size()]);
	memset(newDim.get(), 0, dim.size() * sizeof(char));

	for (size_t x = 0; x < std::min(dim.x, m_curDim.x); x++)
		for (size_t y = 0; y < std::min(dim.y, m_curDim.y); y++)
			for (size_t z = 0; z < std::min(dim.z, m_curDim.z); z++)
			{
				newDim[dim.calcIndex({x,y,z})] = m_pData[m_curDim.calcIndex({x,y,z})];
			}

	LockGuard g(m_muTex);

	m_pData = move(newDim);
	m_newDim = dim;

	m_changed = true;
}

void VolumeTextureMap::create()
{
	DRAW_THREAD;
	assert(Drawing::getThreadID() == System::getThreadID());

	m_texture.create();
	m_texture.bind();

	// clamp texture
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);

	// linear filetring for soft shadows
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glCheck("VolumeTextureMap::create");
}

void VolumeTextureMap::dispose()
{
	DRAW_THREAD;
	assert(Drawing::getThreadID() == System::getThreadID());
	m_texture.dispose();
}

bool VolumeTextureMap::isCreated() const
{
	return m_texture.get() != 0;
}

void VolumeTextureMap::updateGPU()
{
	DRAW_THREAD;
	// note: texture is already bound
	LockGuard g(m_muTex);
	if (m_changed)
	{
		// transfer bytes to gpu
		if(m_newDim != m_curDim)
		{
			m_curDim = m_newDim;
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, m_curDim.width, m_curDim.height, m_curDim.depth,
				0, GL_RED, GL_UNSIGNED_BYTE, m_pData.get());
		}
		else
		{
			// update without resizing (faster)
			glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, m_curDim.width, m_curDim.height, m_curDim.depth,
				GL_RED, GL_UNSIGNED_BYTE, m_pData.get());
		}
		m_changed = false;
	}
}

void VolumeTextureMap::bind(unsigned id)
{
	DRAW_THREAD;
	assert(id < 32);
	assert(Drawing::getThreadID() == System::getThreadID());

	glActiveTexture(GL_TEXTURE0 + id);
	m_texture.bind();

	updateGPU();

	glCheck("VolumeTextureMap::bind");
}

void VolumeTextureMap::unbind()
{
	DRAW_THREAD;
	assert(Drawing::getThreadID() == System::getThreadID());
	glBindTexture(GL_TEXTURE_3D, 0);
}

void VolumeTextureMap::setValue(const Point3S& idx, float val)
{
	MAIN_THREAD;
	assert(idx.x < m_newDim.x);
	assert(idx.y < m_newDim.y);
	assert(idx.z < m_newDim.z);

	LockGuard g(m_muTex);
	m_pData[m_newDim.calcIndex(idx)] = static_cast<unsigned char>(val * 255);
	m_changed = true;
}
