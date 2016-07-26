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
	m_dim = dim;
	// generate emtpy array on cpu side
	m_pData = std::unique_ptr<unsigned char[]>(new unsigned char[dim.x * dim.y * dim.z]);
	memset(m_pData.get(), 0, dim.x * dim.y * dim.z * sizeof(char));
}

void VolumeTextureMap::create()
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_3D, m_texture);

	// clamp texture
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);

	// linear filetring for soft shadows
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// transfer bytes to gpu
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, m_dim.width, m_dim.height, m_dim.depth,
		0, GL_RED, GL_UNSIGNED_BYTE, m_pData.get());
	glCheck("glTexImage3D");

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

void VolumeTextureMap::bind(unsigned id) const
{
	assert(id < 32);
	assert(m_texture);

	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_3D, m_texture);

	// TODO resend if changes were made
	if(m_changed)
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, m_dim.width, m_dim.height, m_dim.depth,
			0, GL_RED, GL_UNSIGNED_BYTE, m_pData.get());
	}

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
	m_pData[m_dim.width * (m_dim.height * idx.z + idx.y) + idx.x] = static_cast<unsigned char>(val * 255);
	m_changed = true;
}
