//{future header message}
#ifndef __FvSingleton_H__
#define __FvSingleton_H__

#include <assert.h>

#define FRIEND_SINGLETON(className)	\
public:														\
	friend class FvSingleton<className>;	\
	~className();										\
protected:													\
	className();											\

template <class T>
class FvSingleton
{
protected:
	static T *ms_pkInstance;
	FvSingleton()
	{
		assert(ms_pkInstance == nullptr);
		ms_pkInstance = static_cast< T * >( this );
	}

public:
	~FvSingleton()
	{
		ms_pkInstance = 0;
	}

	static T &Instance()
	{
		if (ms_pkInstance == nullptr)
		{
			new T;
		}
		return *ms_pkInstance;
	}

	static T *pInstance()
	{
		if (ms_pkInstance == nullptr)
		{
			new T;
		}
		return ms_pkInstance;
	}
};

#define FV_SINGLETON_STORAGE( TYPE )						\
template <>													\
TYPE * FvSingleton< TYPE >::ms_pkInstance = 0;			\

#endif // __FvSingleton_H__
