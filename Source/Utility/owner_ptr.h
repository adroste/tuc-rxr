#pragma once
#include <memory>
#include <cassert>
#include <functional>

/*
	USAGE:
	
	- create owner_ptr (and move it to its final position if known)
	- create ref_ptr with owner_ptr.getRef()
	- set ref_ptr abandon callback to prevent deletion of ref_pointer whilst using it. (lock a mutex with the callback function)
	- callback will be called BEFORE owner_ptr is reset (reset = valid pointer to nullptr)
	- after the ref_ptr is reset, he will remain nullptr

	WARNINGS:

	- if you move the owner_ptr, the ref_ptrs will be reassigned automatically to the owner_ptr's new position. 
		Nevertheless you should not do this when accessing ref_ptr from a seperate thread!
	- set the callback in ref_ptr AFTER it has a valid owner_ptr, the callback will be lost otherwise!
*/

template <class T>
class owner_ptr;

template <class T>
class ref_ptr
{
	friend owner_ptr<T>;

	ref_ptr(owner_ptr<T>*);
public:
	ref_ptr();
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
private:
	void abandon();
	void notify();
private:
	owner_ptr<T>* m_owner = nullptr;
	std::function<void(ref_ptr<T>&)> m_reset_callback;
};


template <class T>
class owner_ptr
{
	friend ref_ptr<T>;
public:
	owner_ptr()
		:
		m_ptr(nullptr)
	{}
	owner_ptr(nullptr_t)
		:
		m_ptr(nullptr)
	{}
	explicit owner_ptr(T* p)
		:
		m_ptr(p)
	{}
	~owner_ptr()
	{
		reset();
	}

	owner_ptr(const owner_ptr&) = delete;
	owner_ptr& operator=(const owner_ptr&) = delete;
	owner_ptr(owner_ptr&& m)
		:
		m_ptr(nullptr)
	{
		swap(m);
	}
	owner_ptr& operator=(owner_ptr&& m)
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
	T* get()
	{
		return m_ptr;
	}
	const T* get() const
	{
		return m_ptr;
	}
	const T& operator*() const
	{
		assert(m_ptr);
		return *m_ptr;
	}
	void reset()
	{
		for (auto ptr : m_refs)
			ptr->abandon();

		delete m_ptr;
		m_ptr = nullptr;
	}

	void swap(owner_ptr& o)
	{
		std::swap(m_ptr, o.m_ptr);
		std::swap(m_refs, o.m_refs);
		// update pointers in m_refs
		refresh();
		o.refresh();
	}

	ref_ptr<T> getRef()
	{
		ref_ptr<T> ref = ref_ptr<T>(this);
		return ref;
	}
	operator bool() const
	{
		return m_ptr != nullptr;
	}
private:
	void reg(ref_ptr<T>* p)
	{
		m_refs.push_back(p);
	}
	void unreg(ref_ptr<T>* p)
	{
		for(auto e = m_refs.begin(), end = m_refs.end(); e != end; ++e)
		{
			if(*e == p)
			{
				m_refs.erase(e);
				return;
			}
		}
	}
	void refresh()
	{
		for(ref_ptr<T>* p : m_refs)
		{
			p->m_owner = this;
		}
	}
private:
	T* m_ptr = nullptr;
	std::vector<ref_ptr<T>*> m_refs;
};

template <class T>
ref_ptr<T>::ref_ptr(owner_ptr<T>* o)
	: m_owner(o)
{
	if (m_owner)
		m_owner->reg(this);
}

template <class T>
ref_ptr<T>::ref_ptr()
	: m_owner(nullptr)
{}

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
	return m_owner->operator*();
}

template <class T>
const T& ref_ptr<T>::operator*() const
{
	assert(m_owner);
	return m_owner->operator*();
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
void ref_ptr<T>::swap(ref_ptr& o)
{
	// unregister
	if(o.m_owner)
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
	if(m_owner)
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
	return *m_owner;
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
void ref_ptr<T>::abandon()
{
	if(m_owner)
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
