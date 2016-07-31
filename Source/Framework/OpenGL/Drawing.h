#pragma once
#include "GLObject.h"
#include "../../Utility/Rect.h"
#include "../Color.h"
#include "Geometry/MeshCube.h"
#include "Shader/ShaderCube.h"
#include "Shader/ShaderButton.h"
#include "Font.h"
#include "Shader/UniformBlockTransforms.h"
#include "Shader/UniformBlockMaterial.h"
#include "Shader/UniformBlockLight.h"
#include "../../Game/Shared/CubeDesc.h"
#include "Camera.h"
#include "Shader/UniformBlockMapInfo.h"
#include "../../Utility/Point3.h"
#include "VolumeTextureMap.h"
#include "Texture.h"
#include "Shader/ShaderButtonSide.h"

class Drawing : public GLObject
{
	friend class Graphics;
	Drawing();
public:
	~Drawing() {}

	virtual void create() override;
	virtual void dispose() override;

	void init(FT_Library ftlib);

	void rect(const RectF& r, const Color& c);
	void button(const RectF& r, bool down);

	// cubes
	void coloredCube(const PointF& pos, float scalar, const Color& c, float z = 0.0f);
	void shaderedCube(const glm::mat4& mat, Shader& shader);
	void setCubeMaterial(const Color& diffuse, const Color& specular, float gloss);
	void setMapInfo(const Point3S& dim);
	void setLights(const Color& ambient, const std::vector<UniformBlockLight::LightSource>& lights, const glm::vec3& eye);

	// camera
	void setCamera(const glm::mat4& mat);
	void setProjection(const glm::mat4& mat);
	void setModel(const glm::mat4& mat);

	Camera& getUiCam();

	// shader
	ShaderCube& getShaderCubeMap();

	// static
	static Font& getFont(Font::Style style, Font::Size size);
private:
	static Drawing& get();

private:
	Camera m_uiCam;

	MeshCube m_meshCube;

	ShaderCube m_shCubeMap;
	ShaderButton m_shButton;
	ShaderButtonSide m_shButtonSide;

	UniformBlockTransforms m_trans;
	UniformBlockMaterial m_material;
	UniformBlockLight m_lights;
	UniformBlockMapInfo m_mapInfo;

	VolumeTextureMap m_volumeTextureMap;

	// fonts
	Font m_fontHeadS;
	Font m_fontHeadM;
	Font m_fontHeadL;
	Font m_fontTextS;
	Font m_fontTextM;
	Font m_fontTextL;

	// textures
	Texture m_texBtnSide;
	Texture m_texBtnMid;
	Texture m_texBtnBumpSide;
	Texture m_texBtnBumpMid;
	Texture m_texBtnBumpSideDown;
	Texture m_texBtnBumpMidDown;
};
