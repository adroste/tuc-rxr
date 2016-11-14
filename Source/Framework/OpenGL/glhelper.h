#pragma once
#include "../../glew/glew.h"
#include <algorithm>

namespace gl
{
	using deleter = void(*)(GLsizei, GLuint*);
	using generator = void(*)(GLsizei, GLuint*);

	template <generator gen, deleter del>
	class Data
	{
	public:
		Data()
		{
			gen(1, &m_id);
		}
		Data(const Data&) = delete;
		Data(Data&& o)
		{
			swap(o);
		}
		Data& operator=(const Data&) = delete;
		Data& operator=(Data&& o)
		{
			swap(o);
			return *this;
		}
		~Data()
		{
			if (m_id)
			{
				del(1, &m_id);
				m_id = 0;
			}
		}
		GLuint get() const
		{
			return m_id;
		}
		void swap(Data<gen,del>& o)
		{
			std::swap(m_id, o.m_id);
		}
		static deleter getDeleter()
		{
			return del;
		}
	private:
		GLuint m_id = 0;
	};

#define GENERIC_GEN_DELETE(Name)	inline void gen##Name(GLsizei s, GLuint* d) { glGen##Name(s,d);}\
									inline void delete##Name(GLsizei s, GLuint* d) {glDelete##Name(s,d);}
	GENERIC_GEN_DELETE(Buffers);
	GENERIC_GEN_DELETE(Textures);

#undef GENERIC_GEN_DELETE

	using Buffer = Data<genBuffers, deleteBuffers>;
	using Texture = Data<genTextures, deleteTextures>;
}
