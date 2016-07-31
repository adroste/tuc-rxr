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
	m_trans({ &m_shCubeMap, /*&m_shButton,*/
	&m_fontHeadS, &m_fontHeadM, &m_fontHeadL,
	&m_fontTextS, &m_fontTextM, &m_fontTextL }, "Transforms"),
	m_material({ &m_shCubeMap }, "Material"),
	m_lights({ &m_shCubeMap }, "Lights"),
	m_mapInfo({ &m_shCubeMap }, "MapInfo")
{
	m_curInstance = this;
	m_drawThreadID = System::getThreadID();
}

void Drawing::rect(const RectF & r, const Color & c)
{
	glColor4f(c.r, c.g, c.b, c.a);

	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex3f(r.x2, r.y1, 0.0f);
		glVertex3f(r.x1, r.y1, 0.0f);
		glVertex3f(r.x2, r.y2, 0.0f);
		glVertex3f(r.x1, r.y2, 0.0f);
	}
	glEndSafe();
}

void Drawing::button(const RectF& r, float border)
{
	m_shButton.bind();

	glBegin(GL_TRIANGLE_STRIP);
	{
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(r.x2, r.y1, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(r.x1, r.y1, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(r.x2, r.y2, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(r.x1, r.y2, 0.0f);
	}
	glEndSafe();

	m_shButton.unbind();
}

void Drawing::coloredCube(const PointF& pos, float scalar, const Color& c, float z)
{
	//scalar += 0.1f;
	//setModel( glm::translate(glm::vec3(pos.x, pos.y, z)) * glm::scale(glm::vec3(scalar, scalar, scalar)));

	setCubeMaterial(c, Color::Black(), 1.0f);
	m_shCubeMap.bind();
	m_meshCube.draw();
	m_shCubeMap.unbind();
}

void Drawing::shaderedCube(const glm::mat4& mat, Shader& shader)
{
	setModel(mat);

	shader.bind();
	m_meshCube.draw();
	shader.unbind();
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

void Drawing::setCamera(const glm::mat4& mat)
{
	m_trans.setCamera(mat);
	m_trans.flush();
}

void Drawing::setProjection(const glm::mat4& mat)
{
	m_trans.setProjection(mat);
	m_trans.flush();
}

void Drawing::setModel(const glm::mat4& mat)
{
	m_trans.setModel(mat);
	m_trans.flush();
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

ShaderCube& Drawing::getShaderCubeMap()
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
	m_meshCube.create();
	m_shCubeMap.create();
	m_shButton.create();

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
}

void Drawing::dispose()
{
	m_meshCube.dispose();
	m_shCubeMap.dispose();
	m_shButton.dispose();

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
}

void Drawing::init(FT_Library ftlib)
{
	m_shCubeMap.load();
	m_shButton.load();

	// fonts
	m_fontHeadS.load(ftlib, "data/Font/DevinneSwash.ttf", float(Font::Size::S));
	m_fontHeadM.load(ftlib, "data/Font/DevinneSwash.ttf", float(Font::Size::M));
	m_fontHeadL.load(ftlib, "data/Font/DevinneSwash.ttf", float(Font::Size::L));
	m_fontTextS.load(ftlib, "data/Font/Domestic_Manners.ttf", float(Font::Size::S));
	m_fontTextM.load(ftlib, "data/Font/Domestic_Manners.ttf", float(Font::Size::M));
	m_fontTextL.load(ftlib, "data/Font/Domestic_Manners.ttf", float(Font::Size::L));
}
