#include "glFramebufferObject.h"
#include <assert.h>
#include "../../System/Exceptions/GL_Exception.h"

FramebufferObject::FramebufferObject(bool hasDepth, size_t nColorAttachments)
	:
	m_hasDepth(hasDepth)
{
	for (size_t i = 0; i < nColorAttachments; i++)
	{
		m_colorAttachments.push_back(gl::Texture2D());
		m_colorInfo.push_back(TexInfo());
	}
}

FramebufferObject::~FramebufferObject()
{
	FramebufferObject::dispose();
}

void FramebufferObject::setTexture(size_t slot, GLenum minMag, GLenum format, GLenum internalFormat, GLenum type)
{
	assert(slot < m_colorInfo.size());
	TexInfo& tex = m_colorInfo[slot];
	tex.minMag = minMag;
	tex.format = format;
	tex.internalFormat = internalFormat;
	tex.type = type;
}

void FramebufferObject::create()
{
	// generate framebuffer
	if (m_fbo.get()) return;

	m_fbo.create();
	m_fbo.bind();
	// attach 2d texture to framebuffer
	for(size_t i = 0; i < m_colorAttachments.size(); i++)
	{
		gl::Texture2D& t = m_colorAttachments[i];
		const TexInfo& in = m_colorInfo[i];
		// create textures
		assert(!t.get());
		t.create();
		t.bind();

		glTexImage2D(GL_TEXTURE_2D, 0, in.format, m_width, m_height, 0, in.internalFormat, in.type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, in.minMag);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, in.minMag);
		t.unbind();

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, t.get(), 0);
	}

	// attach render buffer
	if (m_hasDepth)
	{
		m_depth.create();
		m_depth.bind();
		
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth.get());
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

	m_fbo.unbind();
	glCheck("FramebufferObject::create");
}

void FramebufferObject::dispose()
{
	for (auto& t : m_colorAttachments)
		t.dispose();
	m_depth.dispose();
	m_fbo.dispose();
}

void FramebufferObject::resize(GLsizei width, GLsizei height)
{
	dispose();
	m_width = width;
	m_height = height;
	create();
}

void FramebufferObject::bind(bool clear)
{
	m_fbo.bind();

	if(clear)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 ,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7, GL_COLOR_ATTACHMENT8 ,
		GL_COLOR_ATTACHMENT9, GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11 ,
		GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14 };
	assert(m_colorAttachments.size() < 16);
	glDrawBuffers(m_colorAttachments.size(), buffers);
	if (clear)
	{
		if(m_hasDepth)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		else
			glClear(GL_COLOR_BUFFER_BIT);
	}
}

void FramebufferObject::unbind()
{
	m_fbo.unbind();
}

void FramebufferObject::bindTexture(size_t slot, size_t target)
{
	assert(slot < m_colorAttachments.size());
	glActiveTexture( GL_TEXTURE0 + target);
	m_colorAttachments[slot].bind();
}

void FramebufferObject::getDepthFrom(FramebufferObject& fbo)
{
	assert(!m_hasDepth);
	assert(fbo.m_hasDepth);

	m_fbo.bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo.m_depth.get());
	m_fbo.unbind();
	glCheck("FramebufferObject::getDepthFrom");
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
