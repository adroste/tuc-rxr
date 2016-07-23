#include "Drawing.h"
#include "../../glew/glew.h"
#include "../../glm/gtc/matrix_transform.inl"

Drawing::Drawing()
	:
	m_trans(m_shCube,"Transforms")
{}

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
	setModel( glm::translate(glm::vec3(pos.x, pos.y, z)) * glm::scale(glm::vec3(scalar, scalar, scalar)));

	m_shCube.setColor(c);
	m_shCube.bind();
	m_meshCube.draw();
	m_shCube.unbind();
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
		return style == Font::Style::Headline ? m_fontHeadS : m_fontTextS;
	case Font::Size::M:
		return style == Font::Style::Headline ? m_fontHeadM : m_fontTextM;
	case Font::Size::L:
		return style == Font::Style::Headline ? m_fontHeadL : m_fontTextL;
	default:
		assert("Drawing::getFont invalid usage" == nullptr);
		throw Exception("Drawing::getFont");
	}
}

void Drawing::create()
{
	m_meshCube.create();
	m_shCube.create();
	m_shButton.create();

	// fonts
	m_fontHeadS.create();
	m_fontHeadM.create();
	m_fontHeadL.create();
	m_fontTextS.create();
	m_fontTextM.create();
	m_fontTextL.create();


	m_trans.create();
}

void Drawing::dispose()
{
	m_meshCube.dispose();
	m_shCube.dispose();
	m_shButton.dispose();

	// fonts
	m_fontHeadS.dispose();
	m_fontHeadM.dispose();
	m_fontHeadL.dispose();
	m_fontTextS.dispose();
	m_fontTextM.dispose();
	m_fontTextL.dispose();

	m_trans.dispose();
}

void Drawing::init(FT_Library ftlib)
{
	m_shCube.load();
	m_shButton.load();

	// fonts
	m_fontHeadS.load(ftlib, "data/Font/DevinneSwash.ttf", float(Font::Size::S));
	m_fontHeadM.load(ftlib, "data/Font/DevinneSwash.ttf", float(Font::Size::M));
	m_fontHeadL.load(ftlib, "data/Font/DevinneSwash.ttf", float(Font::Size::L));
	m_fontTextS.load(ftlib, "data/Font/Domestic_Manners.ttf", float(Font::Size::S));
	m_fontTextM.load(ftlib, "data/Font/Domestic_Manners.ttf", float(Font::Size::M));
	m_fontTextL.load(ftlib, "data/Font/Domestic_Manners.ttf", float(Font::Size::L));
}
