#include "Drawing.h"
#include "../../glew/glew.h"
#include "../../glm/gtc/matrix_transform.inl"
#include "../Framework.h"
#include "../../System/System.h"

static Drawing* m_curInstance = nullptr;
static size_t m_drawThreadID = 0;

void glHelperDisposeCallback(gl::Disposeable d)
{
	if(System::getThreadID() !=  Framework::getDrawThreadID())
	{
		// queue disposable	
		if(m_curInstance)
		{
			m_curInstance->addToDisposeStack(std::move(d));
		}
	}
	// else delete immediatly (destructor will do this)
}

Drawing::Drawing()
	:
	m_uiCam(Framework::getScreenCenter(), 1.0f, 1000.0f),
	m_trans({ &m_shCubeMap, &m_shCube,
		&m_fontHeadS, &m_fontHeadM, &m_fontHeadL,
		&m_fontTextS, &m_fontTextM, &m_fontTextL,
		&m_shColor, &m_shHSVPickerCircle, &m_shHSVPickerSquare, &m_shButton,
		&m_shDisc, &m_shColor2
	}, "Transforms"),
	m_material({ &m_shCubeMap,  &m_shCube }, "Material"),
	m_lights({ &m_shCubeMap,  &m_shCube }, "Lights"),
	m_mapInfo({ &m_shCubeMap,  &m_shCube }, "MapInfo"),
	
	m_blockFramework({&m_shButton,
		&m_fontHeadS, &m_fontHeadM, &m_fontHeadL,
		&m_fontTextS, &m_fontTextM, &m_fontTextL,
		&m_shColor, &m_shDisc, &m_shColor2,
		&m_shCubeMap

	},"Framework"),

	m_shaders({
		&m_shCubeMap, &m_shCube, &m_shButton, &m_shColor, &m_shColor2,
		&m_shHSVPickerCircle, &m_shHSVPickerSquare, &m_shDisc,
		&m_fontHeadS, &m_fontHeadM, &m_fontHeadL, &m_fontTextS, &m_fontTextM, &m_fontTextL,
		&m_shFxaa, &m_shBloom1, &m_shBloom2, &m_shBloom3
	}),
	m_blockFramebuffer({
		&m_shFxaa, &m_shBloom2
	},"Framebuffer"),
	m_frontFbo(0,0,true,true)
{
	m_curInstance = this;
	m_drawThreadID = System::getThreadID();
}

Drawing::~Drawing()
{
	m_curInstance = nullptr;
}

void Drawing::rect(const RectF & r, const Color & c)
{
	m_shColor.setColor(c);
	m_shColor.bind();

	prepareDraw();
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

	prepareDraw();
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex2f(0.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(0.0f, 0.0f);		
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

	prepareDraw();
	m_shColor.bind();
	glBegin(GL_LINES);
	{
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
	}
	glEndSafe();
	m_shColor.unbind();
}

void Drawing::disc(const PointF& midPos, float r, const Color& color)
{
	m_shDisc.setColor(color);
	prepareDraw();
	m_shDisc.bind();
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex4f(midPos.x - r, midPos.y - r, -1.0f, -1.0f);
		glVertex4f(midPos.x - r, midPos.y + r, -1.0f, 1.0f);
		glVertex4f(midPos.x + r, midPos.y - r, 1.0f, -1.0f);
		glVertex4f(midPos.x + r, midPos.y + r, 1.0f, 1.0f);
	}
	glEndSafe();
	m_shDisc.unbind();
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

	prepareDraw();
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

	m_texBtnSide.unbind();
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

	m_texBtnMid.unbind();
	m_shButton.unbind();

	glDisable(GL_BLEND);
}

void Drawing::hsvPicker(const PointF& midPos, float r, const Color& color)
{
	m_shHSVPickerCircle.bind();
	prepareDraw();
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex4f(midPos.x - r, midPos.y - r, -1.0f, -1.0f);
		glVertex4f(midPos.x - r, midPos.y + r, -1.0f, 1.0f);
		glVertex4f(midPos.x + r, midPos.y - r, 1.0f, -1.0f);
		glVertex4f(midPos.x + r, midPos.y + r, 1.0f, 1.0f);	
	}
	glEndSafe();
	m_shHSVPickerCircle.unbind();

	m_shHSVPickerSquare.setColor(color);
	m_shHSVPickerSquare.bind();
	glBegin(GL_TRIANGLE_STRIP);
	{
		float d = r * 0.75f * 0.707107f;
		glVertex4f(midPos.x - d, midPos.y - d, -1.0f, -1.0f);
		glVertex4f(midPos.x - d, midPos.y + d, -1.0f, 1.0f);
		glVertex4f(midPos.x + d, midPos.y - d, 1.0f, -1.0f);
		glVertex4f(midPos.x + d, midPos.y + d, 1.0f, 1.0f);
	}
	glEndSafe();
	m_shHSVPickerSquare.unbind();
}

void Drawing::shaderedCube(const glm::mat4& mat, Shader& shader)
{
	//setModel(mat);
	m_trans.pushModel(mat);
	prepareDraw();

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

void Drawing::setLights(const Color& ambient, const std::vector<LightSource>& lights, const glm::vec3& eye)
{
	m_lights.updateLights(ambient.toVec3(), eye, lights);
}

UniformBlockTransforms& Drawing::getTransform()
{
	return m_trans;
}

void Drawing::pushClippingRect(RectF rect)
{
	// transform rect points from model to world coords
	rect.p1 = m_trans.modelToWorld(rect.p1);
	rect.p2 = m_trans.modelToWorld(rect.p2);
	m_blockFramework.pushRect(rect);
}

void Drawing::popClippingRect()
{
	m_blockFramework.pop();
}

void Drawing::pushIgnoreRect()
{
	m_blockFramework.pushIgnore();
}

UniformBlockLight& Drawing::getLightUniform()
{
	return m_lights;
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

size_t Drawing::getThreadID()
{
	return m_drawThreadID;
}

Camera& Drawing::getUiCam()
{
	return m_uiCam;
}

Texture& Drawing::getWaterTexture()
{
	return m_texWater;
}

Texture& Drawing::getWaterfallTexture()
{
	return m_texWaterfall;
}

ShaderCubeMap& Drawing::getShaderCubeMap()
{
	return m_shCubeMap;
}

ShaderCube& Drawing::getShaderCubeDefault()
{
	return m_shCube;
}

MeshCube& Drawing::getCubeMesh()
{
	return m_meshCube;
}

//#define DISABLE_FBO

void Drawing::beginGameShader()
{
	assert(!m_gameActive);
	m_gameActive = true;

#ifndef DISABLE_FBO
	// dont draw before this call!
	m_frontFbo.setTextureFilter(GL_LINEAR);
	m_frontFbo.bind();
#endif
	//glClear(GL_COLOR_BUFFER_BIT);
}

void Drawing::endGameShader()
{
	assert(m_gameActive);
	m_gameActive = false;

#ifndef DISABLE_FBO
	//glFlush(); // flush beforehand to create game image
	// do framebuffer processing
	// anti aliasing, blooming etc.
	Texture texFrame = m_frontFbo.getTexture();
	Texture texDepth = m_frontFbo.getTexture2();
	m_frontFbo.dispose();
	m_frontFbo.create();

	glm::vec2 step;
	step.x = 1.0f / float(m_resolution.x);
	step.y = 1.0f / float(m_resolution.y);
	m_blockFramebuffer.setStep(step);

	// apply blooming
	FramebufferObject fbo(m_resolution.x, m_resolution.y, false, false);
	fbo.create();

	fbo.bind();
	// extract bloom color
	m_shBloom1.bind();
	texFrame.bind(0);
	texDepth.bind(1);

	FramebufferObject::drawRect();


	// blur in x
	Texture texBlured = fbo.getTexture();
	fbo.dispose();
	fbo.create();
	fbo.bind();
	m_shBloom2.setDir({ 1.0f,0.0f });
	m_shBloom2.bind();
	texBlured.bind(0);

	FramebufferObject::drawRect();


	// blur in y
	texBlured = fbo.getTexture();
	fbo.dispose();
	fbo.create();
	fbo.bind();
	m_shBloom2.setDir({ 0.0f,1.0f });
	m_shBloom2.bind();
	texBlured.bind(0);
	FramebufferObject::drawRect();



	// add images together
	texBlured = fbo.getTexture();
	fbo.dispose();
	fbo.create();
	fbo.bind();
	m_shBloom3.bind();
	texFrame.bind(0);
	texBlured.bind(1);
	FramebufferObject::drawRect();



	// apply fxaa
	texFrame = fbo.getTexture();
	//fbo.dispose();
	//fbo.create();
	texFrame.bind(0);
	m_shFxaa.bind();

	FramebufferObject::drawRect();
	fbo.dispose();

#endif
	// flush all commands because they will take a little longer...
	glFlush();
}

Drawing& Drawing::get()
{
	assert(m_curInstance);
	return *m_curInstance;
}

void Drawing::prepareDraw()
{
	m_trans.flush();
	m_blockFramework.flush();
}

void Drawing::beginFrame()
{
	
}

void Drawing::endFrame()
{
	
}

void Drawing::resize(GLsizei width, GLsizei height)
{
	m_frontFbo.resize(width, height);
	m_resolution = PointS(width, height);
}

void Drawing::addToDisposeStack(gl::Disposeable d)
{
	LockGuard m(m_muDisposeStack);
	m_disposeStack.push(std::move(d));
}

void Drawing::create()
{
	for (auto& s : m_shaders)
		s->create();

	m_meshCube.create();

	m_trans.create();
	m_material.create();
	m_lights.create();
	m_mapInfo.create();
	m_blockFramework.create();
	m_blockFramebuffer.create();

	m_texBtnMid.create();
	m_texBtnSide.create();
	m_texBtnBumpLeftDown.create();
	m_texBtnBumpRightDown.create();
	m_texBtnBumpMid.create();
	m_texBtnBumpLeft.create();
	m_texBtnBumpRight.create();
	m_texBtnBumpMidDown.create();

	m_texWater.create();
	m_texWaterfall.create();

	m_frontFbo.create();
}

void Drawing::dispose()
{
	// dispose shader
	for (auto& s : m_shaders)
		s->dispose();

	m_meshCube.dispose();

	m_trans.dispose();
	m_material.dispose();
	m_lights.dispose();
	m_mapInfo.dispose();
	m_blockFramework.dispose();
	m_blockFramebuffer.dispose();

	m_texBtnMid.dispose();
	m_texBtnSide.dispose();
	m_texBtnBumpLeftDown.dispose();
	m_texBtnBumpRightDown.dispose();
	m_texBtnBumpMid.dispose();
	m_texBtnBumpLeft.dispose();
	m_texBtnBumpRight.dispose();
	m_texBtnBumpMidDown.dispose();

	m_texWater.dispose();
	m_texWaterfall.dispose();

	m_frontFbo.dispose();
}

void Drawing::init(FT_Library ftlib)
{
	GLint bs = 0;
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &bs);


	for(auto& s : m_shaders)
	{
		auto l = dynamic_cast<Shader::ILoadable*>(s);
		if (l) l->load();
	}

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

	m_texWater.load("data/Pic/wave.bmp");
	m_texWaterfall.load("data/Pic/waterfall.bmp");
}
