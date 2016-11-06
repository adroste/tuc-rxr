#include "FramebufferObject.h"
#include <assert.h>
#include "../../System/Exceptions/GL_Exception.h"

FramebufferObject::FramebufferObject(GLsizei width, GLsizei height, bool hasDepth)
	:
	m_width(width),
	m_height(height),
	m_hasDepth(hasDepth)
{
}

FramebufferObject::~FramebufferObject()
{
	FramebufferObject::dispose();
}

void FramebufferObject::create()
{
	assert(m_texture == 0);
	assert(m_depth == 0);
	assert(m_fbo == 0);

	// generate texture to store pixels
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_minMagFilter);

	// generate empty image
	// important: if dimansions of framebuffer are bigger than viewport call glViewport before rendering to texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	// generate framebuffer
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	// attach 2d texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

	// attach render buffer
	if (m_hasDepth)
	{
		glGenRenderbuffers(1, &m_depth);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth);
	}

	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		break; // succes
	case GL_FRAMEBUFFER_UNDEFINED:
		throw Exception("GL_FRAMEBUFFER_UNDEFINED");
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		throw Exception("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		throw Exception("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
	default:
		throw Exception("no framebuffer support?");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheck("FramebufferObject::create");
}

void FramebufferObject::dispose()
{
	if(m_texture)
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
	if(m_fbo)
	{
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}
	if(m_depth)
	{
		glDeleteRenderbuffers(1, &m_depth);
		m_depth = 0;
	}
}

void FramebufferObject::bind()
{
	assert(m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

Texture FramebufferObject::getTexture()
{
	assert(m_texture);

	// end drawing
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// now we can use the texture

	Texture tex(m_texture, m_width, m_height);
	m_texture = 0; // move texture owner
	return std::move(tex);
}

void FramebufferObject::setTextureFilter(GLint filter)
{
	m_minMagFilter = filter;
	
	if(m_texture)
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glCheck("FramebufferObject::setTextureFilter");
}
