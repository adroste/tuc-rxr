#pragma once
#include "GLObject.h"
#include "../../Utility/Point3.h"
#include <memory>

class VolumeTextureMap : public GLObject
{
public:
	VolumeTextureMap();
	virtual ~VolumeTextureMap();
	void init(Point3S dim);

	virtual void create() override;
	virtual void dispose() override;

	void bind(unsigned int id) const;
	static void unbind();
private:
	Point3S m_dim;
	GLuint m_texture = 0;
	std::unique_ptr<unsigned char> m_pData;
};
