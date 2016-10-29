#include "CubeDefault.h"

CubeDefault::CubeDefault(const CubeDesc& cd, std::shared_ptr<const MaterialInfo> mi)
	: m_cd(cd), m_pMat(mi)
{
}

CubeDefault::~CubeDefault()
{

}

size_t CubeDefault::getMaterialIndex()
{
	assert(m_pMat);
	return m_pMat->id;
}