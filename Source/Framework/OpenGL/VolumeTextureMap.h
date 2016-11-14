#pragma once
#include "GLObject.h"
#include "../../Utility/Point3.h"
#include <memory>
#include "../../Utility/Mutex.h"

class VolumeTextureMap : public GLObject
{
public:
	VolumeTextureMap();
	virtual ~VolumeTextureMap();
	void resize(Point3S dim);

	virtual void create() override;
	virtual void dispose() override;

	bool isCreated() const;
	void bind(unsigned int id);
	static void unbind();
	void setValue(const Point3S& idx, float val);
protected:
	void updateGPU();
private:
	Point3S m_curDim;
	Point3S m_newDim;
	GLuint m_texture = 0;
	std::unique_ptr<unsigned char[]> m_pData;
	bool m_changed = true;
	Mutex m_muTex;
};
