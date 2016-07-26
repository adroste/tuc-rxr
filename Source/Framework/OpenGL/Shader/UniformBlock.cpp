#include "UniformBlock.h"
#include <cassert>

UniformBlock::UniformBlock(Shader & refShader, const std::string& blockName)
	:
	m_shader(refShader),
	m_blockName(blockName)
{
	static GLuint bindPoint = 1;
	m_bindingPoint = bindPoint++; // these numbers must be uniquely given by the application
}

UniformBlock::~UniformBlock()
{
}

void UniformBlock::create()
{
	assert(m_shader.m_program);

	m_blockLocation = glGetUniformBlockIndex(m_shader.m_program, m_blockName.c_str());
	if (m_blockLocation == GL_INVALID_INDEX)
		throw Exception("UniformBlock::create cannot find uniform block " + m_blockName);

	glGetActiveUniformBlockiv(m_shader.m_program, m_blockLocation, GL_UNIFORM_BLOCK_DATA_SIZE, &m_blockSize);

	m_pBuffer = std::unique_ptr<char[]>(new char[m_blockSize]);

	// create buffer object
	assert(m_vbo == 0);
	glGenBuffers(1, &m_vbo);

	glCheck("UniformBlock::create");
}

void UniformBlock::dispose()
{
	if(m_vbo)
	{
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0;
	}
}

void UniformBlock::flush()
{
	glUniformBlockBinding(getShaderProgramm(), m_blockLocation, m_bindingPoint);

	glBindBuffer(GL_UNIFORM_BUFFER, m_vbo);
	glBufferData(GL_UNIFORM_BUFFER, m_blockSize, m_pBuffer.get(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_vbo);
	glCheck("UniformBlock::flush");
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLuint UniformBlock::getShaderProgramm() const
{
	return m_shader.m_program;
}
