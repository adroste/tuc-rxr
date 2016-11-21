#include "glFramebufferObject.h"
#include <assert.h>
#include "../../System/Exceptions/GL_Exception.h"

FramebufferObject::FramebufferObject(GLsizei width, GLsizei height, bool hasDepth, bool hasColor2)
	:
	m_width(width),
	m_height(height),
	m_hasDepth(hasDepth),
	m_hasColor2(hasColor2)
{

}

FramebufferObject::~FramebufferObject()
{
	FramebufferObject::dispose();
}

void FramebufferObject::create()
{
	if (m_texture.get())
		return; // already inititialized
	assert(m_fbo == 0);

	// generate texture to store pixels
	m_texture.create();
	m_texture.bind();

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
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.get(), 0);
	// attach render buffer
	if (m_hasDepth)
	{
		/*glGenRenderbuffers(1, &m_depth);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth);*/
		m_depth.create();
		m_depth.bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth.get(), 0);
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
	m_texture.dispose();
	if(m_fbo)
	{
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}
	m_depth.dispose();
}

void FramebufferObject::resize(GLsizei width, GLsizei height)
{
	dispose();
	m_width = width;
	m_height = height;
	create();
}

void FramebufferObject::bind()
{
	assert(m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Texture FramebufferObject::getTexture()
{
	// end drawing
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// now we can use the texture

	Texture tex(std::move(m_texture), m_width, m_height);
	m_texture.dispose(); // move texture owner
	return std::move(tex);
}

Texture FramebufferObject::getDepth()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Texture tex(std::move(m_depth), m_width, m_height);
	m_depth.dispose();
	return std::move(tex);
}

void FramebufferObject::setTextureFilter(GLint filter)
{
	m_minMagFilter = filter;
	
	if(m_texture.get())
	{
		glBindTexture(GL_TEXTURE_2D, m_texture.get());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glCheck("FramebufferObject::setTextureFilter");
}

void FramebufferObject::drawRect()
{
	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
	}
	glEndSafe();
	glEnable(GL_CULL_FACE);
}
