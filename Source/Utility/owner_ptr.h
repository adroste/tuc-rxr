#pragma once
#include <memory>
#include <cassert>
#include <functional>
#include <vector>
#include <string>

/*
USAGE:

owner_ptr: heap pointer (new)
- create owner_ptr (and move it to its final position if known)
- create ref_ptr with owner_ptr.getRef()

derived_ptr: refable structure build into class
- add derived_ptr to your class e.g. class myClass : public derived_ptr<myClass> {};
- call derived_ptr<myClass>::resetRefs(); in destructor of myClass
- create ref_ptr with myClass.getRef()

refable: structure containing your class
- when creating your class, wrap it like this: refable<myClass> mc(...) (... are arguments for the myClass constructor)
- create ref_ptr with mc.getRef()

TIPP:

- set ref_ptr abandon callback to prevent deletion of ref_pointer whilst using it. (lock a mutex with the callback function)
- callback will be called BEFORE owner_ptr is reset (reset = valid pointer to nullptr)
- after the ref_ptr is reset, he will remain nullptr

WHICH SHALL I USE?

- you have something that can be nullptr or has to be allocated on the heap -> use owner_ptr
- you want to create the object on the stack:
- you don't want to integrate it into your class: use refable
- you want to acces functions withouth -> or * and don't mind
integrating it into your class: use derived_ptr

WARNINGS:

- if you move the owner_ptr, the ref_ptrs will be reassigned automatically to the owner_ptr's new position.
Nevertheless you should not do this when accessing ref_ptr from a seperate thread!
- set the callback in ref_ptr AFTER it has a valid owner_ptr, the callback will be lost otherwise!
*/


template <class T>
class refable_ptr;

template <class T>
class owner_ptr;

template <class T>
class derived_ptr;

template <class T>
class refable;

template <class T>
class ref_ptr
{
	friend class refable_ptr<T>;
	friend class owner_ptr<T>;
	friend class derived_ptr<T>;
	friend class refable<T>;

	ref_ptr(refable_ptr<T>*);
public:
	ref_ptr();
	ref_ptr(std::nullptr_t);
	ref_ptr(const ref_ptr&);
	ref_ptr& operator=(const ref_ptr&);
	ref_ptr(ref_ptr&&);
	ref_ptr& operator=(ref_ptr&&);
	~ref_ptr();
	T* operator->();
	const T* operator->() const;
	T* get();
	const T* get() const;
	T& operator*();
	const T& operator*() const;
	void copy(const ref_ptr&);
	void swap(ref_ptr&);
	void release();
	operator bool() const;
	// callback will be called BEFORE owner is destructed and BEFORE reference to owner ist deleted. Locking a mutex would be possible
	void setAbandonCallback(std::function<void(ref_ptr<T>&)> func);
	void resetCallback();

	bool operator==(const ref_ptr<T>& o) const;
	bool operator==(const T* o) const;
	bool operator!=(const ref_ptr<T>& o) const;
	bool operator!=(const T* o) const;

	// assignement for different type
	template<class T2>
	ref_ptr(const ref_ptr<T2>&);
	template<class T2>
	ref_ptr<T>& operator=(const ref_ptr<T2>&);
	template<class T2>
	void copy(const ref_ptr<T2>&);
private:
	void abandon();
	void notify();
private:
	refable_ptr<T>* m_owner = nullptr;
	std::function<void(ref_ptr<T>&)> m_reset_callback;
};

template <class T>
class refable_ptr
{
	friend class ref_ptr<T>;

public:
	virtual ~refable_ptr()
	{}
	ref_ptr<T> getRef()
	{
		ref_ptr<T> ref = ref_ptr<T>(this);
		return ref;
	}

protected:
	void resetRefs()
	{
		while(m_refs.size())
		{
			size_t sz = m_refs.size();
			m_refs.back()->abandon();
			// do this in case ref_ptr calls remove in abandon callback
			if (sz == m_refs.size())
				m_refs.pop_back();
		}
		m_refs.clear();
	}
	bool hasRefs() const
	{
		return m_refs.size() != 0;
	}
	void swap(refable_ptr<T>& o) noexcept
	{
		std::swap(m_refs, o.m_refs);
		// update pointers in m_refs
		refresh();
		o.refresh();
	}

private:
	virtual T* get() = 0;
	virtual const T* get() const = 0;

	void reg(ref_ptr<T>* p)
	{
		m_refs.push_back(p);
	}

	void unreg(ref_ptr<T>* p)
	{
		for (auto e = m_refs.begin(), end = m_refs.end(); e != end; ++e)
		{
			if (*e == p)
			{
				m_refs.erase(e);
				return;
			}
		}
	}

	void refresh()
	{
		for (ref_ptr<T>* p : m_refs)
			p->m_owner = this;

	}

private:
	std::vector<ref_ptr<T>*> m_refs;
};

template <class T>
class derived_ptr : public refable_ptr<T>
{

protected:
	derived_ptr()
	{}
public:
	virtual ~derived_ptr()
	{
		// call resetRefs in destructor of T !
		assert(!refable_ptr<T>::hasRefs());
	}

	// for now you cant duplicate or swap dat shit
	derived_ptr(const derived_ptr&) = delete;
	derived_ptr& operator=(const derived_ptr&) = delete;
	derived_ptr(derived_ptr&&) = delete;
	derived_ptr& operator=(derived_ptr&&) = delete;
private:
	virtual T* get() override final
	{
		return (T*)(this);
	}

	virtual const T* get() const override final
	{
		return (T*)(this);
	}
};

template <class T>
class refable : public refable_ptr<T>
{
public:
	template<typename ...Args>
	explicit refable(Args&&... args)
		:
		obj(std::forward<Args>(args)...)
	{}
	~refable()
	{
		refable_ptr<T>::resetRefs();
	}
	refable(const refable&) = delete;
	refable& operator=(const refable&) = delete;
	refable(refable&& o)
	{
		swap(o);
	}
	refable& operator=(refable&& o)
	{
		swap(o);
		return *this;
	}
	T* operator->()
	{
		return &obj;
	}

	const T* operator->() const
	{
		return &obj;
	}

	T& operator*()
	{
		return obj;
	}

	const T& operator*() const
	{
		return obj;
	}
	void swap(refable& o)
	{
		std::swap(obj, o.obj);
		refable_ptr<T>::swap(o);
	}
private:
	virtual T* get() override
	{
		return &obj;
	}
	virtual const T* get() const override
	{
		return &obj;
	}
private:
	T obj;
};

template <class T>
class owner_ptr : public refable_ptr<T>
{
	friend ref_ptr<T>;
public:
	owner_ptr()
		:
		m_ptr(nullptr)
	{
	}

	owner_ptr(std::nullptr_t)
		:
		m_ptr(nullptr)
	{
	}

	explicit owner_ptr(T* p)
		:
		m_ptr(p)
	{
	}

	~owner_ptr()
	{
		reset();
	}

	owner_ptr(const owner_ptr&) = delete;
	owner_ptr& operator=(const owner_ptr&) = delete;

	owner_ptr(owner_ptr&& m) noexcept
		:
		m_ptr(nullptr)
	{
		swap(m);
	}

	owner_ptr& operator=(owner_ptr&& m) noexcept
	{
		swap(m);
		return *this;
	}

	T* operator->()
	{
		return m_ptr;
	}

	const T* operator->() const
	{
		return m_ptr;
	}

	T& operator*()
	{
		assert(m_ptr);
		return *m_ptr;
	}

	const T& operator*() const
	{
		assert(m_ptr);
		return *m_ptr;
	}

	T* get() override final
	{
		return m_ptr;
	}

	const T* get() const override final
	{
		return m_ptr;
	}

	void reset()
	{
		refable_ptr<T>::resetRefs();

		delete m_ptr;
		m_ptr = nullptr;
	}

	void swap(owner_ptr& o) noexcept
	{
		std::swap(m_ptr, o.m_ptr);
		refable_ptr<T>::swap(o);
	}

	operator bool() const
	{
		return m_ptr != nullptr;
	}

private:
	T* m_ptr = nullptr;
};

template <class T>
ref_ptr<T>::ref_ptr(refable_ptr<T>* o)
	: m_owner(o)
{
	if (m_owner)
		m_owner->reg(this);
}

template <class T>
ref_ptr<T>::ref_ptr()
	: m_owner(nullptr)
{
}

template <class T>
ref_ptr<T>::ref_ptr(std::nullptr_t)
	:
	m_owner(nullptr)
{
}

template <class T>
ref_ptr<T>::ref_ptr(const ref_ptr& c)
{
	copy(c);
}

template <class T>
ref_ptr<T>& ref_ptr<T>::operator=(const ref_ptr& c)
{
	copy(c);
	return *this;
}

template <class T>
ref_ptr<T>::ref_ptr(ref_ptr&& m)
{
	swap(m);
}

template <class T>
ref_ptr<T>& ref_ptr<T>::operator=(ref_ptr&& m)
{
	swap(m);
	return *this;
}

template <class T>
ref_ptr<T>::~ref_ptr()
{
	release();
}

template <class T>
T* ref_ptr<T>::operator->()
{
	return get();
}

template <class T>
const T* ref_ptr<T>::operator->() const
{
	return get();
}

template <class T>
T* ref_ptr<T>::get()
{
	if (m_owner)
		return m_owner->get();
	return nullptr;
}

template <class T>
const T* ref_ptr<T>::get() const
{
	if (m_owner)
		return m_owner->get();
	return nullptr;
}

template <class T>
T& ref_ptr<T>::operator*()
{
	assert(m_owner);
	assert(m_owner->get());
	return *m_owner->get();
}

template <class T>
const T& ref_ptr<T>::operator*() const
{
	assert(m_owner);
	assert(m_owner->get());
	return *m_owner->get();
}

template <class T>
void ref_ptr<T>::swap(ref_ptr& o)
{
	// unregister
	if (o.m_owner)
		o.m_owner->unreg(&o);
	if (m_owner)
		m_owner->unreg(this);

	// swap
	std::swap(o.m_owner, m_owner);
	std::swap(o.m_reset_callback, m_reset_callback);

	// reregister
	if (m_owner)
		m_owner->reg(this);
	if (o.m_owner)
		o.m_owner->reg(&o);
}

template <class T>
void ref_ptr<T>::release()
{
	if (m_owner)
	{
		// release owner
		m_owner->unreg(this);
		m_owner = nullptr;
	}
}

template <class T>
ref_ptr<T>::operator bool() const
{
	if (!m_owner)
		return false;
	return m_owner->get() != nullptr;
}

template <class T>
void ref_ptr<T>::setAbandonCallback(std::function<void(ref_ptr<T>&)> func)
{
	m_reset_callback = func;
}

template <class T>
void ref_ptr<T>::resetCallback()
{
	decltype(m_reset_callback) emptyFunc;
	m_reset_callback.swap(emptyFunc);
}

template <class T>
bool ref_ptr<T>::operator==(const T* o) const
{
	if (m_owner)
		return m_owner->get() == o;
	return o == nullptr;
}

template <class T>
bool ref_ptr<T>::operator!=(const ref_ptr<T>& o) const
{
	return !(*this == o);
}

template <class T>
bool ref_ptr<T>::operator!=(const T* o) const
{
	return !(*this == o);
}

template <class T>
template <class T2>
ref_ptr<T>::ref_ptr(const ref_ptr<T2>& o)
{
	copy(o);
}

template <class T>
template <class T2>
ref_ptr<T>& ref_ptr<T>::operator=(const ref_ptr<T2>& o)
{
	copy(o);
	return *this;
}

template <class T>
bool ref_ptr<T>::operator==(const ref_ptr<T>& o) const
{
	return o.m_owner == m_owner;
}

template <class T>
void ref_ptr<T>::copy(const ref_ptr& c)
{
	// release old
	release();
	m_owner = c.m_owner;
	m_reset_callback = c.m_reset_callback;
	if (m_owner)
		m_owner->reg(this);
}

template <class T>
template <class T2>
void ref_ptr<T>::copy(const ref_ptr<T2>& o)
{
	static_assert(std::is_base_of<T, T2>::value, "cannot convert between different ref_ptr types");
	copy(*(const ref_ptr<T>*)(void*)(&o));
}

template <class T>
void ref_ptr<T>::abandon()
{
	if (m_owner)
	{
		notify();
		m_owner = nullptr;
	}
}

template <class T>
void ref_ptr<T>::notify()
{
	if (m_reset_callback)
		m_reset_callback(*this);
}
