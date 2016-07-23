#pragma once
#include "GLObject.h"
#include "Texture.h"

class FramebufferObject : public GLObject
{
public:
	FramebufferObject(GLsizei width, GLsizei height, bool hasDepth);
	virtual ~FramebufferObject();

	void create() override;
	void dispose() override;

	void bind();
	Texture getTexture();

	void setTextureFilter(GLint filter);

private:
	GLuint m_texture = 0;
	GLuint m_depth = 0;
	GLuint m_fbo = 0;
	const GLsizei m_width, m_height;

	GLint m_minMagFilter = GL_NEAREST;
	bool m_hasDepth;
};