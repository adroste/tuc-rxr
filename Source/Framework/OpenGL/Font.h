#pragma once
#include "Shader/Shader.h"
#include "../../Utility/Point.h"
#include "../Color.h"
#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H

class Font : public Shader
{
public:
	Font();
	virtual ~Font() {}

	void load(struct FT_LibraryRec_* ft, const std::string& filename, float scalar);
	virtual void create() override;
	virtual void dispose() override;

	void write(const std::string& txt, PointF pos) const;
	PointF getDim(const char* txt) const;
	PointF getDim(const std::string& txt) const;
	void setColor(const Color& c);
protected:
	virtual void loadUniforms() override;
	void load() override;
private:
	void findMaxBearing(float scalar);
	void loadTextures();
private:
	static const int CHAR_START = 32;
	static const int CHAR_END = 126;

	FT_LibraryRec_*	m_ft = nullptr;
	FT_FaceRec_*	m_face = nullptr;

	GLint	m_uniLocColor = -1;
	GLint	m_uniLocTexture = -1;

	std::unique_ptr<char[]> m_rawData;
	GLuint	m_textureArray[CHAR_END - CHAR_START];

	static const int m_nChars = 128;
	FT_Glyph_Metrics m_metrics[m_nChars];
			
	float m_scaleFactor = 0.0f;
	int m_maxBearing = 0;
	int m_maxHeight = 0;

	float m_fontHeight = 0.0f; // max fontheigh on display
};
