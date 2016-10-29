#pragma once
#include "CubeBase.h"
#include "CubeDesc.h"
#include <memory>
#include "../Client/ShaderMaterialHolder.h"

class CubeDefault : public CubeBase
{
public:
	CubeDefault(const CubeDesc&, std::shared_ptr<const MaterialInfo>);
	virtual ~CubeDefault() override;
	virtual size_t getMaterialIndex() override;
private:
	CubeDesc m_cd;
	std::shared_ptr<const MaterialInfo> m_pMat;
};
