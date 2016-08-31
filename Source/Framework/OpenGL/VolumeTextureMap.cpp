#include "VolumeTextureMap.h"
#include "../../System/Exceptions/GL_Exception.h"

VolumeTextureMap::VolumeTextureMap()
{
}

VolumeTextureMap::~VolumeTextureMap()
{
}

void VolumeTextureMap::init(Point3S dim)
{
	LockGuard g(m_muTex);

	m_dim = dim;
	// generate emtpy array on cpu side
	m_pData = std::unique_ptr<unsigned char[]>(new unsigned char[dim.x * dim.y * dim.z]);
	memset(m_pData.get(), 0, dim.x * dim.y * dim.z * sizeof(char));

	m_changed = true;
}

void VolumeTextureMap::resize(Point3S dim)
{
	assert(m_pData);
	auto newDim = std::unique_ptr<unsigned char[]>(new unsigned char[dim.size()]);
	memset(newDim.get(), 0, dim.size() * sizeof(char));

	LockGuard g(m_muTex);

	for(size_t x = 0; x < std::min(dim.x,m_dim.x); x++)
		for (size_t y = 0; y < std::min(dim.y, m_dim.y); y++)
			for (size_t z = 0; z < std::min(dim.z, m_dim.z); z++)
			{
				newDim[dim.calcIndex({ x,y,z })] = m_pData[m_dim.calcIndex({ x,y,z })];
			}

	m_pData = std::move(newDim);
	m_dim = dim;

	m_changed = true;
}

void VolumeTextureMap::create()
{
	assert(m_texture == 0);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_3D, m_texture);

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
	if(m_texture)
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}

void VolumeTextureMap::updateGPU()
{
	// note: texture is already bound
	LockGuard g(m_muTex);
	if(m_changed)
	{
		// transfer bytes to gpu
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, m_dim.width, m_dim.height, m_dim.depth,
			0, GL_RED, GL_UNSIGNED_BYTE, m_pData.get());
		m_changed = false;
	}
}

void VolumeTextureMap::bind(unsigned id)
{
	assert(id < 32);
	assert(m_texture);

	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_3D, m_texture);

	updateGPU();

	glCheck("VolumeTextureMap::bind");
}

void VolumeTextureMap::unbind()
{
	glBindTexture(GL_TEXTURE_3D, 0);
}

void VolumeTextureMap::setValue(const Point3S& idx, float val)
{
	assert(idx.x < m_dim.x);
	assert(idx.y < m_dim.y);
	assert(idx.z < m_dim.z);
	LockGuard g(m_muTex);
	m_pData[m_dim.calcIndex(idx)] = static_cast<unsigned char>(val * 255);
	m_changed = true;
}
