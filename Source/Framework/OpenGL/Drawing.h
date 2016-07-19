#pragma once
#include "GLObject.h"
#include "../../Utility/Rect.h"
#include "../Color.h"
#include "Geometry/MeshCube.h"
#include "Shader/ShaderCube.h"
#include "Shader/ShaderButton.h"

class Drawing : public GLObject
{
public:
	Drawing();
	~Drawing() {}

	virtual void create() override;
	virtual void dispose() override;

	void rect(const RectF& r, const Color& c);
	void button(const RectF& r, float border);

	// cubes
	void coloredCube(const PointF& pos, float scalar, const Color& c);

private:
	MeshCube m_meshCube;
	ShaderCube m_shCube;
	ShaderButton m_shButton;
};