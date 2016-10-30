#pragma once
#include "Shader.h"
#include "../../Color.h"

class ShaderCubeMap : public Shader, public Shader::ILoadable
{
public:
	virtual ~ShaderCubeMap() {}
	void load() override;
	void setChunkHeight(size_t h);
protected:
	virtual void loadUniforms() override;
private:
	GLint m_chHeight = -1;
};
