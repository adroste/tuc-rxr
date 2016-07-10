#pragma once
#include "GLObject.h"
#include <string>
#include <memory>

class Texture : public GLObject
{
public:
	virtual ~Texture()
	{}
	
	virtual void load(const std::string& filename);
	virtual void create() override;
	virtual void dispose() override;
	int getWidth() const;
	int getHeight() const;

	void bind(unsigned int id) const;
	static void unbind();
private:
	GLuint m_texIndex = 0;
	int m_width = 0;
	int m_height = 0;
	std::unique_ptr<char[]> m_pRawData;
};
