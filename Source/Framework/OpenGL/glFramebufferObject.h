#pragma once
#include "GLObject.h"
#include "Texture.h"
#include <vector>

class FramebufferObject : public GLObject
{
public:
	FramebufferObject(bool hasDepth, size_t nColorAttachments);
	virtual ~FramebufferObject();

	// Format -> GL_RGBA8
	void setTexture(size_t slot, GLenum minMag = GL_NEAREST, GLenum format = GL_RGBA8, GLenum internalFormat = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);
	// note: textures have to be initialized 
	void create() override;
	void dispose() override;
	void resize(GLsizei width, GLsizei height);

	void bind(bool clear = true);
	void unbind();
	void bindTexture(size_t slot, size_t target);

	void getDepthFrom(FramebufferObject& fbo);
	static void drawRect();
private:
	struct TexInfo
	{
		GLenum minMag = GL_NEAREST;
		GLenum format = GL_RGBA8;
		GLenum internalFormat = GL_RGBA;
		GLenum type = GL_UNSIGNED_BYTE;
	};
	std::vector<gl::Texture2D> m_colorAttachments;
	std::vector<TexInfo> m_colorInfo;
	bool m_hasDepth;
	gl::Texture2D m_depth;

	//GLuint m_fbo = 0;
	gl::Framebuffer m_fbo;
	GLsizei m_width = 0, m_height = 0;

};
