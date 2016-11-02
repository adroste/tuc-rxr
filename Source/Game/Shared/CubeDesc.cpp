#include "CubeDesc.h"
#include <map>

ENUM_CONVERT(CubeShader,CubeShader::Default, 
{ CubeShader::Default,"default" },
{ CubeShader::Transparent,"transparent" },
{ CubeShader::Water,"water" }
)

ENUM_CONVERT(BlockType, BlockType::Solid,
{ BlockType::Immortal,"immortal" },
{ BlockType::Transparent,"transparent" },
{ BlockType::Solid,"solid" },
{ BlockType::Liquid,"liquid" },
)
