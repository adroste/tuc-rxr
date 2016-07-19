#pragma once
#include "GLObject.h"
#include "../../Utility/Rect.h"
#include "../Color.h"
#include "Geometry/MeshCube.h"
#include "Shader/ShaderCube.h"

class Drawing : public GLObject
{
public:
	Drawing();
	~Drawing() {}

	virtual void create() override;
	virtual void dispose() override;

	void rect(const RectF& r, const Color& c);

	// cubes
	void coloredCube(const PointF& pos, float scalar, const Color& c);

private:
	MeshCube m_meshCube;
	ShaderCube m_shCube;
};