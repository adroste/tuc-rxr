#pragma once
#include "../../glew/glew.h"
#include <algorithm>
#include <cassert>

namespace gl
{
	using deleter = void(*)(GLsizei, GLuint*);
	using generator = void(*)(GLsizei, GLuint*);

	template <generator, deleter>
	class Data;
	class Disposeable
	{
		template <generator, deleter>
		friend Data<generator,deleter>;

		Disposeable(GLuint id, deleter del)
			:
		m_deleter(del),m_id(id)
		{}
	public:
		Disposeable()
		{}
		Disposeable(const Disposeable&) = delete;
		Disposeable& operator=(const Disposeable&) = delete;
		Disposeable(Disposeable&& m) noexcept
		{
			swap(m);
		}
		Disposeable& operator=(Disposeable&& m) noexcept
		{
			swap(m);
			return *this;
		}
		~Disposeable()
		{
			if(m_id)
			{
				assert(m_deleter);
				m_deleter(1, &m_id);
				m_id = 0;
			}
		}
		void swap(Disposeable& o) noexcept
		{
			std::swap(m_deleter, o.m_deleter);
			std::swap(m_id, o.m_id);
		}
	private:
		deleter m_deleter = nullptr;
		GLuint m_id = 0;
	};

	template <generator gen, deleter del>
	class Data
	{
	public:
		Data()
		{
			gen(1, &m_id);
		}
		Data(const Data&) = delete;
		Data(Data&& o) noexcept
		{
			swap(o);
		}
		Data& operator=(const Data&) = delete;
		Data& operator=(Data&& o) noexcept
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
		void swap(Data<gen,del>& o) noexcept
		{
			std::swap(m_id, o.m_id);
		}
		static deleter getDeleter()
		{
			return del;
		}
		Disposeable dispose()
		{
			Disposeable d(m_id, del);
			m_id = 0;
			return d;
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
