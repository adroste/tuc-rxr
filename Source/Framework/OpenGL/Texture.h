#pragma once
#include "GLObject.h"
#include <string>
#include <memory>

class Texture : public GLObject
{
public:
	Texture();
	Texture(GLuint texture, GLsizei width, GLsizei height);
	Texture(Texture&& move);
	Texture& operator=(Texture&& move);
	
	virtual ~Texture();
	
	virtual void load(const std::string& filename);
	virtual void create() override;
	virtual void dispose() override;
	int getWidth() const;
	int getHeight() const;

	void bind(unsigned int id) const;
	static void unbind();

	void swap(Texture& o);
private:
	GLuint m_texIndex = 0;
	int m_width = 0;
	int m_height = 0;
	std::unique_ptr<char[]> m_pRawData;
};
