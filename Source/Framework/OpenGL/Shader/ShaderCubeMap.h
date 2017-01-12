#pragma once
#include "Shader.h"
#include "../../Color.h"
#include "ShaderCubeUniforms.h"

enum class AssetAnimation;

class ShaderCubeMap : public ShaderCubeUniforms, public Shader::ILoadable
{
public:
	virtual ~ShaderCubeMap() {}
	void load() override;
	void setAnimation(AssetAnimation a);
protected:
	virtual void loadUniforms() override;

private:
	GLint m_animation = -1;
};
