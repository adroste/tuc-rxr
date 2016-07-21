#pragma once
#include "Shader.h"
#include <memory>

class UniformBlock : public GLObject
{
public:
	UniformBlock(Shader& refShader, const std::string& blockName);
	virtual ~UniformBlock();

	// important: init shader first
	void create() override;
	void dispose() override;

	// send data to gpu
	void flush();
protected:
	GLuint getShaderProgramm() const;

protected:
	Shader& m_shader;
	const std::string m_blockName;
	GLuint m_blockLocation = 0;
	GLint m_blockSize = 0;
	GLuint m_vbo = 0;

	std::unique_ptr<char[]> m_pBuffer;
};
