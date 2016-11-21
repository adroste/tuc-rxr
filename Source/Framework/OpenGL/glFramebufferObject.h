#pragma once
#include "GLObject.h"
#include "Texture.h"

class FramebufferObject : public GLObject
{
public:
	FramebufferObject(GLsizei width, GLsizei height, bool hasDepth, bool hasColor2);
	virtual ~FramebufferObject();

	void create() override;
	void dispose() override;
	void resize(GLsizei width, GLsizei height);

	void bind();
	Texture getTexture();
	Texture getDepth();

	void setTextureFilter(GLint filter);
	static void drawRect();
private:
	gl::Texture2D m_texture;
	//GLuint m_texture = 0;
	gl::Texture2D m_depth;
	GLuint m_fbo = 0;
	GLsizei m_width, m_height;

	GLint m_minMagFilter = GL_NEAREST;
	bool m_hasDepth;
	bool m_hasColor2;
};