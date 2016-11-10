#include "Texture.h"

#include <assert.h>

#include "stb_image/stb_image.h"
#include "../../System/Exceptions/Exception.h"
#include "../../System/Exceptions/GL_Exception.h"
#include "../../System/Exceptions/ExceptionMissingFile.h"

Texture::Texture()
{
}

Texture::Texture(GLuint texture, GLsizei width, GLsizei height)
	:
	m_texIndex(texture),
	m_width(width),
	m_height(height)
{
}

Texture::Texture(Texture&& move)
{
	swap(move);
}

Texture& Texture::operator=(Texture&& move)
{
	swap(move);
	return *this;
}

Texture::~Texture()
{
	Texture::dispose();
}

void Texture::load(const std::string & fileName)
{
	int numComponents;
	char* data = reinterpret_cast<char*>(
		stbi_load(fileName.c_str(),
	    &m_width, &m_height, &numComponents, 4));

	if (!data)
		throw ExceptionMissingFile("Texture::load", fileName);

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

	// transfer bytes to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pRawData.get());

	glGenerateMipmap(GL_TEXTURE_2D);

	// use best linear filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

void Texture::swap(Texture& o)
{
	std::swap(m_width, o.m_width);
	std::swap(m_height, o.m_height);
	std::swap(m_texIndex, o.m_texIndex);
	std::swap(m_pRawData, o.m_pRawData);
}
