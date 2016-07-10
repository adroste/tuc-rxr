#include "Texture.h"

#include <assert.h>

#include "stb_image/stb_image.h"
#include "../../Utility/Exception.h"
#include "../../Utility/GL_Exception.h"

void Texture::load(const std::string & filename)
{
	int numComponents;
	char* data = reinterpret_cast<char*>(
		stbi_load(filename.c_str(),
	    &m_width, &m_height, &numComponents, 4));

	// TODO replace with proper exception
	if (!data)
		throw Exception("missing: " + filename);

	// copy data
	m_pRawData = std::unique_ptr<char[]>(new char[m_width * m_height * 4]);
	memcpy(m_pRawData.get(), data, m_width * m_height * 4);

	stbi_image_free(data);
}

void Texture::create()
{
	assert(m_texIndex == 0);
	glGenTextures(1, &m_texIndex);
	glBindTexture(GL_TEXTURE_2D, m_texIndex);

	// repeat texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// use best linear filter
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// transfer bytes to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pRawData.get());

	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::dispose()
{
	if(m_texIndex != 0)
	{
		glDeleteTextures(1, &m_texIndex);
		m_texIndex = 0;
	}
}

int Texture::getWidth() const
{
	return m_width;
}

int Texture::getHeight() const
{
	return m_height;
}

void Texture::bind(unsigned int id) const
{
	assert(id <= 31);

	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, m_texIndex);
	
	glCheck("Texture::bind");
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
