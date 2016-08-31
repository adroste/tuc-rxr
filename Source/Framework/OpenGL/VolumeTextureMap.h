#pragma once
#include "GLObject.h"
#include "../../Utility/Point3.h"
#include <memory>
#include "../../Utility/Mutex.h"

/*
	Game Usage:
	1. call init which creates an empty map

	Editor Usage:
	1. init
	2. resize instead of init
*/
class VolumeTextureMap : public GLObject
{
public:
	VolumeTextureMap();
	virtual ~VolumeTextureMap();
	void init(Point3S dim);
	void resize(Point3S dim);

	virtual void create() override;
	virtual void dispose() override;

	void bind(unsigned int id);
	static void unbind();
	void setValue(const Point3S& idx, float val);
protected:
	void updateGPU();
private:
	Point3S m_dim;
	GLuint m_texture = 0;
	std::unique_ptr<unsigned char[]> m_pData;
	bool m_changed = true;
	Mutex m_muTex;
};
