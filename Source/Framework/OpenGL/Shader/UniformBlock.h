#pragma once
#include "Shader.h"
#include <memory>
#include <cassert>

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

	template <class T>
	void updateVar(const T& var, size_t offset)
	{
		assert(offset + sizeof(T) < size_t(m_blockSize));
		memcpy(m_pBuffer.get() + offset, &var, sizeof(T));
	}
	void updateArray(const void* src, size_t len, size_t offset)
	{
		assert(offset + len < size_t(m_blockSize));
		memcpy(m_pBuffer.get() + offset, src, len);
	}
protected:
	Shader& m_shader;
	const std::string m_blockName;
	GLuint m_blockLocation = 0;
	GLint m_blockSize = 0;
	GLuint m_vbo = 0;
	GLuint m_bindingPoint;

	std::unique_ptr<char[]> m_pBuffer;
};
