#include "Drawing.h"
#include "../../glew/glew.h"
#include "../../glm/gtc/matrix_transform.inl"
#include "../Framework.h"
#include "../../System/System.h"

static Drawing* m_curInstance = nullptr;
static size_t m_drawThreadID = 0;

Drawing::Drawing()
	:
	m_uiCam({ Framework::STD_DRAW_X / 2, Framework::STD_DRAW_Y / 2 }, 1.0f, 1000.0f),
	m_trans({ &m_shHSVPicker, &m_shCubeMap, &m_shButton,
	&m_fontHeadS, &m_fontHeadM, &m_fontHeadL,
	&m_fontTextS, &m_fontTextM, &m_fontTextL,
	&m_shColor
	}, "Transforms"),
	m_material({ &m_shCubeMap }, "Material"),
	m_lights({ &m_shCubeMap }, "Lights"),
	m_mapInfo({ &m_shCubeMap }, "MapInfo"),
	m_blockFramework({&m_shButton },"Framework")
{
	m_curInstance = this;
	m_drawThreadID = System::getThreadID();
}

void Drawing::rect(const RectF & r, const Color & c)
{
	m_shColor.setColor(c);
	m_shColor.bind();

	m_trans.flush();
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex3f(r.x2, r.y1, 0.0f);
		glVertex3f(r.x1, r.y1, 0.0f);
		glVertex3f(r.x2, r.y2, 0.0f);
		glVertex3f(r.x1, r.y2, 0.0f);
	}
	glEndSafe();

	m_shColor.unbind();
}

void Drawing::line(PointF p1, PointF p2, float thickness, const Color& color)
{
	if (p2.x < p1.x)
		std::swap(p1, p2);

	glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(p1.x, p1.y, 0.0f));
	translate = glm::rotate(translate, atanf((p2.y - p1.y) / (p2.x - p1.x)), glm::vec3(0.0f, 0.0f, 1.0f));
	translate = glm::translate(translate, glm::vec3(-thickness / 2.0f, -thickness / 2.0f, 0.0f));
	translate = glm::scale(translate, glm::vec3((p1 - p2).length() + thickness, thickness, 1.0f));

	m_trans.pushModel(translate);
	m_trans.flush();

	m_shColor.setColor(color);
	m_shColor.bind();

	m_trans.flush();
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex2f(0.0f, 1.0f);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(1.0f, 0.0f);
	}
	glEndSafe();
	
	m_shColor.unbind();

	m_trans.popModel();
	m_trans.flush();
}

void Drawing::line(const glm::vec3& p1, const glm::vec3& p2, float thikness, const Color& c)
{
	glLineWidth(thikness);
	m_shColor.setColor(c);

	m_trans.flush();
	m_shColor.bind();
	glBegin(GL_LINES);
	{
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
	}
	glEndSafe();
	m_shColor.unbind();
}

void Drawing::buttonRoyal(const RectF& r, bool down)
{
	Texture& bumpMid = down ? m_texBtnBumpMidDown : m_texBtnBumpMid;
	Texture& bumpLeft = down ? m_texBtnBumpLeftDown : m_texBtnBumpLeft;
	Texture& bumpRight = down ? m_texBtnBumpRightDown : m_texBtnBumpRight;

	float sideWidth = float(m_texBtnSide.getWidth()) / float(m_texBtnSide.getHeight()) * r.getHeight();

	auto mid = r.getMidpoint();
	m_shButton.setLightPos({ mid.x,/*mid.y*/r.y1 - 30.0f,r.getHeight() / 4.0f });

	m_shButton.bind();
	m_texBtnSide.bind(0);
	bumpLeft.bind(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_trans.flush();
	glBegin(GL_TRIANGLE_STRIP);
	{
		// left side
		glVertex4f(r.x1 + sideWidth, r.y1, 1.0f, 0.0f);
		glVertex4f(r.x1, r.y1, 0.0f, 0.0f);
		glVertex4f(r.x1 + sideWidth, r.y2, 1.0f, 1.0f);
		glVertex4f(r.x1, r.y2, 0.0f, 1.0f);
	}
	glEndSafe();

	bumpRight.bind(1);
	glBegin(GL_TRIANGLE_STRIP);
	{
		// right side
		glVertex4f(r.x2, r.y1, 0.0f, 0.0f);
		glVertex4f(r.x2 - sideWidth, r.y1, 1.0f, 0.0f);
		glVertex4f(r.x2, r.y2, 0.0f, 1.0f);
		glVertex4f(r.x2 - sideWidth, r.y2, 1.0f, 1.0f);
	}
	glEndSafe();

	m_texBtnMid.bind(0);
	bumpMid.bind(1);

	sideWidth -= 1.0f;
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex4f(r.x2 - sideWidth, r.y1, 0.5f, 0.0f);
		glVertex4f(r.x1 + sideWidth, r.y1, 0.5f, 0.0f);
		glVertex4f(r.x2 - sideWidth, r.y2, 0.5f, 1.0f);
		glVertex4f(r.x1 + sideWidth, r.y2, 0.5f, 1.0f);
	}
	glEndSafe();

	m_shButton.unbind();

	glDisable(GL_BLEND);
}

void Drawing::hsvPicker(const PointF& midPos, float r, const Color& color)
{
	m_shHSVPicker.bind();
	m_trans.flush();
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex4f(midPos.x - r, midPos.y - r, -1.0f, -1.0f);
		glVertex4f(midPos.x - r, midPos.y + r, -1.0f, 1.0f);
		glVertex4f(midPos.x + r, midPos.y - r, 1.0f, -1.0f);
		glVertex4f(midPos.x + r, midPos.y + r, 1.0f, 1.0f);	
	}
	glEndSafe();
	m_shHSVPicker.unbind();

	m_shColor2.bind();

	glBegin(GL_TRIANGLE_STRIP);
	{
		int d = r * 0.8 * 0.707107;
		glVertexAttrib4f(1, 1.0f, 1.0f, 1.0f, 1.0f);
		glVertex2f(midPos.x - d, midPos.y - d);
		glVertexAttrib4f(1, 0.0f, 0.0f, 0.0f, 1.0f);
		glVertex2f(midPos.x - d, midPos.y + d);
		glVertexAttrib4f(1, color.r, color.g, color.b, 1.0f);
		glVertex2f(midPos.x + d, midPos.y - d);
		glVertexAttrib4f(1, 0.0f, 0.0f, 0.0f, 1.0f);
		glVertex2f(midPos.x + d, midPos.y + d);

		/*glVertexAttrib4f(1, 0.0f, 0.0f, 0.0f, 1.0f);
		glVertex2f(midPos.x - d, midPos.y + d);
		glVertexAttrib4f(1, 0.0f, 0.0f, 0.0f, 1.0f);
		glVertex2f(midPos.x + d, midPos.y + d);
		glVertexAttrib4f(1, 1.0f, 1.0f, 1.0f, 1.0f);
		glVertex2f(midPos.x - d, midPos.y - d);
		glVertexAttrib4f(1, color.r, color.g, color.b, 1.0f);
		glVertex2f(midPos.x + d, midPos.y - d);*/
	}
	glEndSafe();
	
	m_shColor2.unbind();
}

void Drawing::shaderedCube(const glm::mat4& mat, Shader& shader)
{
	//setModel(mat);
	m_trans.pushModel(mat);
	m_trans.flush();

	shader.bind();
	m_meshCube.draw();
	shader.unbind();

	m_trans.popModel();
	m_trans.flush();
}

void Drawing::setCubeMaterial(const Color& diffuse, const Color& specular, float gloss)
{
	m_material.updateMaterial(glm::vec3(diffuse.r, diffuse.g, diffuse.b), glm::vec3(specular.r, specular.g, specular.b), gloss);
}

void Drawing::setMapInfo(const Point3S& dim)
{
	m_mapInfo.setInfo({ float(dim.x), float(dim.y), float(dim.z) });
}

void Drawing::setLights(const Color& ambient, const std::vector<UniformBlockLight::LightSource>& lights, const glm::vec3& eye)
{
	m_lights.updateLights(ambient.toVec3(), eye, lights);
}

UniformBlockTransforms& Drawing::getTransform()
{
	return m_trans;
}

Font& Drawing::getFont(Font::Style style, Font::Size size)
{
	switch (size)
	{
	case Font::Size::S: 
		return style == Font::Style::Headline ? get().m_fontHeadS : get().m_fontTextS;
	case Font::Size::M:
		return style == Font::Style::Headline ? get().m_fontHeadM : get().m_fontTextM;
	case Font::Size::L:
		return style == Font::Style::Headline ? get().m_fontHeadL : get().m_fontTextL;
	default:
		assert("Drawing::getFont invalid usage" == nullptr);
		throw Exception("Drawing::getFont");
	}
}

Camera& Drawing::getUiCam()
{
	return m_uiCam;
}

ShaderCubeMap& Drawing::getShaderCubeMap()
{
	return m_shCubeMap;
}

Drawing& Drawing::get()
{
	assert(m_curInstance);
	return *m_curInstance;
}

void Drawing::create()
{
	m_shHSVPicker.create();
	m_meshCube.create();
	m_shCubeMap.create();
	m_shButton.create();
	m_shColor.create();
	m_shColor2.create();

	// fonts
	m_fontHeadS.create();
	m_fontHeadM.create();
	m_fontHeadL.create();
	m_fontTextS.create();
	m_fontTextM.create();
	m_fontTextL.create();


	m_trans.create();
	m_material.create();
	m_lights.create();
	m_mapInfo.create();
	m_blockFramework.create();

	m_texBtnMid.create();
	m_texBtnSide.create();
	m_texBtnBumpLeftDown.create();
	m_texBtnBumpRightDown.create();
	m_texBtnBumpMid.create();
	m_texBtnBumpLeft.create();
	m_texBtnBumpRight.create();
	m_texBtnBumpMidDown.create();
}

void Drawing::dispose()
{
	m_shHSVPicker.dispose();
	m_meshCube.dispose();
	m_shCubeMap.dispose();
	m_shButton.dispose();
	m_shColor.dispose();
	m_shColor2.dispose();

	// fonts
	m_fontHeadS.dispose();
	m_fontHeadM.dispose();
	m_fontHeadL.dispose();
	m_fontTextS.dispose();
	m_fontTextM.dispose();
	m_fontTextL.dispose();

	m_trans.dispose();
	m_material.dispose();
	m_lights.dispose();
	m_mapInfo.dispose();
	m_blockFramework.dispose();

	m_texBtnMid.dispose();
	m_texBtnSide.dispose();
	m_texBtnBumpLeftDown.dispose();
	m_texBtnBumpRightDown.dispose();
	m_texBtnBumpMid.dispose();
	m_texBtnBumpLeft.dispose();
	m_texBtnBumpRight.dispose();
	m_texBtnBumpMidDown.dispose();
}

void Drawing::init(FT_Library ftlib)
{
	m_shHSVPicker.load();
	m_shCubeMap.load();
	m_shButton.load();
	m_shColor.load();
	m_shColor2.load();

	// fonts
	m_fontHeadS.load(ftlib, "data/Font/DevinneSwash.ttf", float(Font::Size::S));
	m_fontHeadM.load(ftlib, "data/Font/DevinneSwash.ttf", float(Font::Size::M));
	m_fontHeadL.load(ftlib, "data/Font/DevinneSwash.ttf", float(Font::Size::L));
	m_fontTextS.load(ftlib, "data/Font/Domestic_Manners.ttf", float(Font::Size::S));
	m_fontTextM.load(ftlib, "data/Font/Domestic_Manners.ttf", float(Font::Size::M));
	m_fontTextL.load(ftlib, "data/Font/Domestic_Manners.ttf", float(Font::Size::L));

	m_texBtnMid.load("data/Pic/btn_mid.png");
	m_texBtnSide.load("data/Pic/btn_side.png");
	m_texBtnBumpLeft.load("data/Pic/btn_bump_left.bmp");
	m_texBtnBumpRight.load("data/Pic/btn_bump_right.bmp");
	m_texBtnBumpMid.load("data/Pic/btn_bump_mid.bmp");
	m_texBtnBumpLeftDown.load("data/Pic/btn_bump_left_down.bmp");
	m_texBtnBumpRightDown.load("data/Pic/btn_bump_right_down.bmp");
	m_texBtnBumpMidDown.load("data/Pic/btn_bump_mid_down.bmp");
}
