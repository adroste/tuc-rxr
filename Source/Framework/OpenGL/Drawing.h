#pragma once
#include "GLObject.h"
#include "../../Utility/Rect.h"
#include "../Color.h"
#include "Geometry/MeshCube.h"
#include "Shader/ShaderCube.h"
#include "Shader/ShaderButton.h"
#include "Font.h"
#include "Shader/UniformBlockTransforms.h"
#include "../../Game/Shared/CubeDesc.h"
#include "Camera.h"

class Drawing : public GLObject
{
public:
	Drawing();
	~Drawing() {}

	virtual void create() override;
	virtual void dispose() override;

	void init(FT_Library ftlib);

	void rect(const RectF& r, const Color& c);
	void button(const RectF& r, float border);

	// cubes
	void coloredCube(const PointF& pos, float scalar, const Color& c, float z = 0.0f);
	void shaderedCube(const glm::mat4& mat, Shader& shader);

	// camera
	void setCamera(const glm::mat4& mat);
	void setProjection(const glm::mat4& mat);
	void setModel(const glm::mat4& mat);

	Font& getFont(Font::Style style, Font::Size size);
	Shader& getCubeShader(CubeShader s);

	Camera& getUiCam();

private:
	Camera m_uiCam;

	MeshCube m_meshCube;
	ShaderCube m_shCube;
	ShaderButton m_shButton;

	UniformBlockTransforms m_trans;

	// fonts
	Font m_fontHeadS;
	Font m_fontHeadM;
	Font m_fontHeadL;
	Font m_fontTextS;
	Font m_fontTextM;
	Font m_fontTextL;
};
